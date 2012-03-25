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
#include "Job.h"
#include "Utils.h"

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

bool PdfGenerator::generateInvoice(const char *fileName_, const Job &job)
{
    QString fileName = toValidFileName(fileName_), templateFileName = "invoice_template.html";


    GET_WEBVIEW_AND_HTML();
    view.setHtml(html);
    PRINT_TO_PDF();

    return false;
}

bool PdfGenerator::generateReceipt(const char *fileName_, const Job &job)
{
    QString fileName = toValidFileName(fileName_), templateFileName = "receipt_template.html";

    GET_WEBVIEW_AND_HTML();
    PRINT_TO_PDF();

    return false;
}

bool PdfGenerator::generateReport(const char *fileName_)
{
    QString fileName = toValidFileName(fileName_), templateFileName = "report_template.html";

    GET_WEBVIEW_AND_HTML();
    PRINT_TO_PDF();

    return true;
}
