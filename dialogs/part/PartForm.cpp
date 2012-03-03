/*
 * PartForm.cpp
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#include "PartForm.h"
#include "ui_PartForm.h"

#include "Part.h"
#include "PartController.h"

PartForm::PartForm(Part &part, QWidget *parent)
    : QDialog(parent), formType((part.null() && !part.pending) ? NEW : EDIT), ui(new Ui::PartForm), part(part)
{
    ui->setupUi(this);

    ui->label_partFormTitleE->setText(formType == NEW ? "New part" : "Editing part");
    updateView();
}

PartForm::~PartForm()
{
    delete ui;
}

void PartForm::updateView()
{
    ui->lineEdit_partName->setText(part.getName());
    ui->lineEdit_partNumber->setText(part.getNumber());
    ui->doubleSpinBox_price->setValue(part.getPrice());
    ui->doubleSpinBox_vatRate->setValue(part.getVatRate());
    ui->spinBox_quantity->setValue(part.getQuantity());
}

void PartForm::on_lineEdit_partName_textEdited(const QString &value)
{
    bool success = true;
    try { part.setName(value.toStdString().c_str()); }
    catch (const std::exception &e)
    {
        success = false;
        ui->lineEdit_partName->setToolTip(e.what());
        ui->lineEdit_partName->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_partName->setStyleSheet("");
        ui->lineEdit_partName->setToolTip("");
    }
}

void PartForm::on_lineEdit_partNumber_textEdited(const QString &value)
{
    bool success = true;
    try { part.setNumber(value.toStdString().c_str()); }
    catch (const std::exception &e)
    {
        success = false;
        ui->lineEdit_partNumber->setToolTip(e.what());
        ui->lineEdit_partNumber->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_partNumber->setStyleSheet("");
        ui->lineEdit_partNumber->setToolTip("");
    }
}

void PartForm::on_doubleSpinBox_price_valueChanged(double value)
{
    bool success = true;
    try { part.setPrice(value); }
    catch (const std::exception &e)
    {
        success = false;
        ui->doubleSpinBox_price->setToolTip(e.what());
        ui->doubleSpinBox_price->setStyleSheet("QDoubleSpinBox { background-color: red; }");
    }
    if (success)
    {
        ui->doubleSpinBox_price->setStyleSheet("");
        ui->doubleSpinBox_price->setToolTip("");
    }
}

void PartForm::on_doubleSpinBox_vatRate_valueChanged(double value)
{
    bool success = true;
    try { part.setVatRate(value); }
    catch (const std::exception &e)
    {
        success = false;
        ui->doubleSpinBox_vatRate->setToolTip(e.what());
        ui->doubleSpinBox_vatRate->setStyleSheet("QDoubleSpinBox { background-color: red; }");
    }
    if (success)
    {
        ui->doubleSpinBox_vatRate->setStyleSheet("");
        ui->doubleSpinBox_vatRate->setToolTip("");
    }
}

void PartForm::on_spinBox_quantity_valueChanged(int value)
{
    bool success = true;
    try { part.setQuantity(value); }
    catch (const std::exception &e)
    {
        success = false;
        ui->doubleSpinBox_vatRate->setToolTip(e.what());
        ui->doubleSpinBox_vatRate->setStyleSheet("QSpinBox { background-color: red; }");
    }
    if (success)
    {
        ui->doubleSpinBox_vatRate->setStyleSheet("");
        ui->doubleSpinBox_vatRate->setToolTip("");
    }
}

void PartForm::on_pushButton_submit_released()
{
    if (!setRecordData()) return;
    if ((formType == EDIT) && (!part.null()))
    {
        if (!PartController::Update(part, this)) return;
    }
    else part.pending = true;
    done(Accepted);
}

void PartForm::on_pushButton_cancel_released()
{
    done(Rejected);
}

bool PartForm::setRecordData()
{
    bool success = true;

    on_lineEdit_partName_textEdited(ui->lineEdit_partName->text());
    if (ui->lineEdit_partName->styleSheet() != "") success = false;

    on_lineEdit_partNumber_textEdited(ui->lineEdit_partNumber->text());
    if (ui->lineEdit_partNumber->styleSheet() != "") success = false;

    on_doubleSpinBox_price_valueChanged(ui->doubleSpinBox_price->value());
    if (ui->doubleSpinBox_price->styleSheet() != "") success = false;

    on_doubleSpinBox_vatRate_valueChanged(ui->doubleSpinBox_vatRate->value());
    if (ui->doubleSpinBox_vatRate->styleSheet() != "") success = false;

    on_spinBox_quantity_valueChanged(ui->spinBox_quantity->value());
    if (ui->spinBox_quantity->styleSheet() != "") success = false;

    return success;
}
