/*
 * VatRateForm.cpp
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#include "VatRateForm.h"
#include "ui_VatRateForm.h"

#include "VatRate.h"
#include "VatRateController.h"

VatRateForm::VatRateForm(VatRate &vatRate, QWidget *parent)
    : QDialog(parent), formType(vatRate.null() ? NEW : EDIT), ui(new Ui::VatRateForm), vatRate(vatRate)
{
    ui->setupUi(this);
    ui->label_vatRateFormTitle->setText(formType == NEW ? "New VAT rate" : "Editing VAT rate");
    updateView();
}

VatRateForm::~VatRateForm()
{
    delete ui;
}

void VatRateForm::updateView()
{
    ui->doubleSpinBox_vatRate->setValue(vatRate.getValue());
    ui->dateEdit_startDate->setDate(Date(vatRate.getStartDate()));
}

void VatRateForm::on_doubleSpinBox_vatRate_valueChanged(const double value)
{
    bool success = true;
    try { vatRate.setValue(value); }
    catch (const std::exception &e)
    {
        success = false;
        ui->doubleSpinBox_vatRate->setToolTip(e.what());
        ui->doubleSpinBox_vatRate->setStyleSheet("QDoubleSpinBox { background-color: red; }");
    }
    if (success)
    {
        ui->doubleSpinBox_vatRate->setToolTip("");
        ui->doubleSpinBox_vatRate->setStyleSheet("");
    }
}

void VatRateForm::on_dateEdit_startDate_dateChanged(const QDate &date)
{
    bool success = true;
    try { vatRate.setStartDate(Date(0, 0, date.day(), date.month(), date.year())); }
    catch (const std::exception &e)
    {
        success = false;
        ui->dateEdit_startDate->setToolTip(e.what());
        ui->dateEdit_startDate->setStyleSheet("QDateEdit { background-color: red; }");
    }
    if (success)
    {
        ui->dateEdit_startDate->setToolTip("");
        ui->dateEdit_startDate->setStyleSheet("");
    }
}

void VatRateForm::on_pushButton_submit_clicked()
{
    if (!setRecordData()) return;

    if (formType == EDIT)
    {
        if (VatRateController::Update(vatRate, this)) done(Accepted);
    }
    else
    {
        if (VatRateController::Create(vatRate, this)) done(Accepted);
    }
}

void VatRateForm::on_pushButton_cancel_clicked()
{
    done(Rejected);
}

bool VatRateForm::setRecordData()
{
    bool success = true;

    on_doubleSpinBox_vatRate_valueChanged(ui->doubleSpinBox_vatRate->value());
    if (ui->doubleSpinBox_vatRate->styleSheet() != "") success = false;

    on_dateEdit_startDate_dateChanged(ui->dateEdit_startDate->date());
    if (ui->dateEdit_startDate->styleSheet() != "") success = false;

    return success;
}
