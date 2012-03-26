/*
 * PdfGenerator.cpp
 *
 *  Created on: 29 Feb 2012
 *      Author: Max Foster
 */

#include <map>
using namespace std;

#include <QWebView>
#include <QUrl>
#include <QPrinter>
#include <QFile>

#include "PdfGenerator.h"
#include "Globals.h"
#include "Utils.h"

#include "Databases.h"
#include "Job.h"
#include "Part.h"
#include "Customer.h"
#include "Expense.h"
#include "JobController.h"
#include "CustomerController.h"
#include "ExpenseController.h"

typedef map<QString, QString> Attributes;

QString toValidFileName(const char *fileName_)
{
    QString fileName(fileName_);
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) fileName += ".pdf";
    return fileName;
}

QString getHtml(const QString &filename)
{
    QString returnString;
    QFile file(filename);
    if (file.exists())
    {
        file.open(QFile::ReadOnly);
        if (file.isOpen())
        {
            returnString = file.readAll();
            file.close();
        }
    }
    return returnString;
}

void parseHtml(QString &html, Attributes &attributes)
{
    static const QString startTag = "<$", endTag = "$>";

    int startIndex = 0;
    QString key;
    while ((startIndex = html.indexOf(startTag, startIndex)) >= 0)
    {
        int substringStart = startIndex + startTag.length();

        int endIndex = html.indexOf(endTag, substringStart);
        if (endIndex < 0) break;
        int substringEnd = endIndex - endTag.length();

        while (html[substringStart] == ' ') ++substringStart;
        while (html[substringEnd] == ' ') --substringEnd;

        key = html.mid(substringStart, substringEnd - (substringStart - 1));

        html.replace(startIndex, endIndex + endTag.length() - startIndex, attributes[key]);
    }
}

QString to2Dp(const QString &str)
{
    const int pointIndex = str.lastIndexOf('.');
    if (pointIndex < 0) return str + ".00";

    switch (str.length() - pointIndex)
    {
    case 1: return str + "00";
    case 2: return str + "0";
    default: return str;
    }
}

#define GET_WEBVIEW_AND_HTML() \
QWebView view;\
QString html = getHtml(templateFileName);\
if (html.isEmpty())\
{\
    showErrorDialog(("Could not open file " + templateFileName).toStdString().c_str());\
    return false;\
}

#define PRINT_TO_PDF() \
QPrinter printer;\
printer.setOutputFormat(QPrinter::PdfFormat);\
printer.setOutputFileName(fileName);\
view.print(&printer)

void getInvoiceReceiptAttributes(Attributes &attributes, const Job &job)
{
    Customer customer = CustomerController::getCustomer(job.getCustomerId());
    Database<Part>::recordListPtr parts = JobController::getJobParts(job.getId());

    attributes["date"] = Date(job.getDate()).toQStringWithoutTime();
    attributes["customer-name"] = createFullName(customer.getForename(), customer.getSurname());
    attributes["customer-addressline1"] = customer.getAddressLine1();

    if (strlen(customer.getAddressLine2()) > 0)
    {
        attributes["customer-addressline2"] = customer.getAddressLine2();
        attributes["customer-town"] = customer.getTown();
    }
    else
    {
        attributes["customer-addressline2"] = customer.getTown();
        attributes["customer-town"] = "";
    }
    attributes["customer-postcode"] = customer.getPostcode();

    QString customerTelephone = customer.getHomePhoneNumber();
    if (strlen(customer.getMobilePhoneNumber()) > 0)
    {
        customerTelephone += " / ";
        customerTelephone += customer.getMobilePhoneNumber();
    }
    attributes["customer-telephone"] = customerTelephone;
    attributes["customer-emailaddress"] = customer.getEmailAddress();
    attributes["job-description"] = job.getDescription();

    double totalPartPriceExclVat = 0.0, totalPartPriceInclVat = 0.0;
    QString partHtml = "";
    for (unsigned i = 0; i < parts->size(); ++i)
    {
        Part &part = parts->at(i);

        partHtml += "<tr><td class='text-mid'>";
        partHtml += toString(part.getQuantity()).c_str();
        partHtml += "</td><td>";
        partHtml += part.getName();
        partHtml += "</td><td class='text-mid'>";
        partHtml += L'£';
        partHtml += to2Dp(toString(part.getPrice()).c_str());
        partHtml += "</td><td class='text-mid'>";
        partHtml += L'£';
        partHtml += to2Dp(toString(part.getPrice() * (1.0 + (part.getVatRate() / 100.0))).c_str());
        partHtml += "</td></tr>";

        totalPartPriceExclVat += part.getPrice();
        totalPartPriceInclVat += part.getPrice() * (1.0 + (part.getVatRate() / 100.0));
    }
    attributes["parts-rows"] = partHtml;
    attributes["parts-totalpriceexclvat"] = to2Dp(toString(totalPartPriceExclVat).c_str());
    attributes["parts-totalpriceinclvat"] = to2Dp(toString(totalPartPriceInclVat).c_str());
    attributes["job-labourcharge"] = to2Dp(toString(job.getLabourCharge()).c_str());
    attributes["vatrate"] = toString(Globals::vatRate(job.getDate())).c_str();
    attributes["job-vat"] = to2Dp(toString(job.getVat()).c_str());
    attributes["totalprice"] = to2Dp(toString(job.getLabourCharge() + job.getVat() + totalPartPriceInclVat).c_str());
}

bool PdfGenerator::generateInvoice(const char *fileName_, const Job &job)
{
    QString fileName = toValidFileName(fileName_), templateFileName = "invoice_receipt_template.html";

    Attributes attributes;
    getInvoiceReceiptAttributes(attributes, job);
    attributes["title"] = "INVOICE";

    GET_WEBVIEW_AND_HTML();
    parseHtml(html, attributes);
    view.setHtml(html);
    PRINT_TO_PDF();

    return true;
}

bool PdfGenerator::generateReceipt(const char *fileName_, const Job &job)
{
    QString fileName = toValidFileName(fileName_), templateFileName = "invoice_receipt_template.html";

    Attributes attributes;
    getInvoiceReceiptAttributes(attributes, job);
    attributes["title"] = "RECEIPT";
    attributes["date"] = Date(time(NULL)).toQStringWithoutTime();

    QString extraTotalRow = "<td class='tbl-l'><strong>Payment Method:</strong></td><td class='tbl-r'>";
    extraTotalRow += job.getPaymentMethodString().c_str();
    extraTotalRow += "</td>";
    attributes["extratotalrows"] = extraTotalRow;

    GET_WEBVIEW_AND_HTML();
    parseHtml(html, attributes);
    view.setHtml(html);
    PRINT_TO_PDF();

    return true;
}


namespace DateFunctions
{
    static time_t dateLowerBound, dateUpperBound;

    template<typename T>
    static bool isRecordDateWithinBounds(const T &record, void *)
    {
        return (record.getDate() >= dateLowerBound) && (record.getDate() <= dateUpperBound);
    }

    template<typename T>
    static int compareRecordDates(const T &record1, const T &record2)
    {
        const time_t date1 = record1.getDate(), date2 = record2.getDate();
        return date1 < date2 ? -1 : (date1 > date2 ? 1 : 0);
    }
}

bool PdfGenerator::generateReport(const char *fileName_, const int month, const int year)
{
    QString fileName = toValidFileName(fileName_), templateFileName = "report_template.html";

    DateFunctions::dateLowerBound = Date(0, 0, 0, month, year);
    DateFunctions::dateUpperBound = time_t(Date(0, 0, 0, month + 1, year)) - 1;

    Database<Job>::recordListPtr jobs = JobController::getAllJobs();
    Databases::jobs().keepRecords(*jobs, DateFunctions::isRecordDateWithinBounds, NULL);
    Databases::jobs().sortRecords(*jobs, 0, jobs->size() - 1, DateFunctions::compareRecordDates);

    Database<Expense>::recordListPtr expenses = ExpenseController::getAllExpenses();
    Databases::expenses().keepRecords(*expenses, DateFunctions::isRecordDateWithinBounds, NULL);
    Databases::expenses().sortRecords(*expenses, 0, expenses->size() - 1, DateFunctions::compareRecordDates);

    double income = 0.0, vat = 0.0;
    QString jobHtml = "";
    for (unsigned i = 0; i < jobs->size(); ++i)
    {
        Job &job = jobs->at(i);
        if (job.getCompletionState() == Job::DONE_PAID)
        {
            double jobChargeExclVat = job.getLabourCharge(), jobVat = job.getVat();

            Database<Part>::recordListPtr parts = JobController::getJobParts(job.getId());
            for (unsigned j = 0; j < parts->size(); ++j)
            {
                Part &part = parts->at(j);
                jobChargeExclVat += part.getPrice();
                jobVat += part.getPrice() * (part.getVatRate() / 100.0);
            }

            income += jobChargeExclVat;
            vat += jobVat;

            Customer customer = CustomerController::getCustomer(job.getCustomerId());

            jobHtml += "<tr><td class='text-mid'>";
            jobHtml += Date(job.getDate()).toQStringWithoutTime();
            jobHtml += "</td><td class='text-mid'>";
            jobHtml += createFullName(customer.getForename(), customer.getSurname());
            jobHtml += "</td><td class='text-mid'>";
            jobHtml += job.getPaymentMethodString().c_str();
            jobHtml += "</td><td class='text-mid'>";
            jobHtml += L'£';
            jobHtml += to2Dp(toString(jobChargeExclVat).c_str());
            jobHtml += "</td><td class='text-mid'>";
            jobHtml += L'£';
            jobHtml += to2Dp(toString(jobVat).c_str());
            jobHtml += "</td><td class='text-mid'>";
            jobHtml += L'£';
            jobHtml += to2Dp(toString(jobChargeExclVat + jobVat).c_str());
            jobHtml += "</td></tr>";
        }
    }

    double expenseTotal = 0.0;
    QString expenseHtml = "";
    for (unsigned i = 0; i < expenses->size(); ++i)
    {
        Expense &expense = expenses->at(i);
        expenseTotal += expense.getTotalPrice();

        expenseHtml += "<tr><td class='text-mid'>";
        expenseHtml += Date(expense.getDate()).toQStringWithoutTime();
        expenseHtml += "</td><td class='text-mid'>";
        expenseHtml += expense.getTypeString().c_str();
        expenseHtml += "</td><td class='text-mid'>";
        expenseHtml += L'£';
        expenseHtml += to2Dp(toString(expense.getPrice()).c_str());
        expenseHtml += "</td><td class='text-mid'>";
        expenseHtml += L'£';
        expenseHtml += to2Dp(toString(expense.getVat()).c_str());
        expenseHtml += "</td><td class='text-mid'>";
        expenseHtml += L'£';
        expenseHtml += to2Dp(toString(expense.getTotalPrice()).c_str());
        expenseHtml += "</td></tr>";
    }

    Attributes attributes;
    attributes["title"] = QString("Report for ") + QDate::longMonthName(month) + ' ' + toString(year).c_str();
    attributes["income"] = to2Dp(toString(income).c_str());
    attributes["vat"] = to2Dp(toString(vat).c_str());
    attributes["totalincome"] = to2Dp(toString(income + vat).c_str());
    attributes["expenses"] = to2Dp(toString(expenseTotal).c_str());
    attributes["grandtotal"] = to2Dp(toString(income + vat - expenseTotal).c_str());
    attributes["jobs-rows"] = jobHtml;
    attributes["expenses-rows"] = expenseHtml;

    GET_WEBVIEW_AND_HTML();
    parseHtml(html, attributes);
    view.setHtml(html);
    PRINT_TO_PDF();

    return true;
}
