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

#include "Job.h"
#include "Part.h"
#include "Customer.h"
#include "JobController.h"
#include "CustomerController.h"

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

    attributes["poundsign"] = L'£';
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

bool PdfGenerator::generateReport(const char *fileName_)
{
    QString fileName = toValidFileName(fileName_), templateFileName = "report_template.html";

    GET_WEBVIEW_AND_HTML();
    PRINT_TO_PDF();

    return true;
}
