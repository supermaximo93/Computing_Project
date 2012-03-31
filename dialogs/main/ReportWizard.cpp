/*
 * ReportWizard.cpp
 *
 *  Created on: 26 March 2012
 *      Author: Max Foster
 */

#include "ReportWizard.h"
#include "ui_ReportWizard.h"

#include <ctime>

#include <QDate>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

#include "Utils.h"
#include "PdfGenerator.h"

#include "dialogs/setting/SettingForm.h"

ReportWizard::ReportWizard(QWidget *parent)
    : QDialog(parent), ui(new Ui::ReportWizard)
{
    ui->setupUi(this);
    setWindowTitle("Report Wizard");

    // Default to the range of this month
    Date now(time(NULL));
    ui->dateEdit_startDate->setDate(QDate(now.year, now.month, 1));
    ui->dateEdit_endDate->setDate(QDate(now.year, now.month, QDate(now).daysInMonth()));
}

ReportWizard::~ReportWizard()
{
    delete ui;
}

void ReportWizard::on_pushButton_datePickerStartDate_clicked()
{
    QDate date;
    if (showDatePickerDialog(date)) ui->dateEdit_startDate->setDate(date);
}

void ReportWizard::on_pushButton_datePickerEndDate_clicked()
{
    QDate date;
    if (showDatePickerDialog(date)) ui->dateEdit_endDate->setDate(date);
}

void ReportWizard::on_pushButton_submit_clicked()
{
    QDate startDate = ui->dateEdit_startDate->date(), endDate = ui->dateEdit_endDate->date();
    if (endDate < startDate)
    {
        showErrorDialog("Start date must be before end date");
        return;
    }

    QString suggestedFilename
            = SettingForm::getPdfDirectoryWithoutSlash() + "/reports/report_" + toString(startDate.day()).c_str()
            + QDate::longMonthName(startDate.month()) + toString(startDate.year()).c_str() + "_to_"
            + toString(endDate.day()).c_str() + QDate::longMonthName(endDate.month()) + '_'
            + toString(endDate.year()).c_str() + ".pdf";

    QString filename = QFileDialog::getSaveFileName(this, "Save Report As", suggestedFilename, "PDF (*.pdf)");
    if (filename.isEmpty()) return;

    if (PdfGenerator::generateReport(filename.toStdString().c_str(), startDate, endDate))
    {
        showInfoDialog("Report generated successfully");
        QDesktopServices::openUrl(QUrl("file:///" + filename));
        done(Accepted);
    }
    else showErrorDialog("Report could not be generated");
}

void ReportWizard::on_pushButton_cancel_clicked()
{
    done(Rejected);
}
