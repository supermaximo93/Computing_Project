/*
 * ExpenseForm.cpp
 *
 *  Created on: 22 Feb 2012
 *      Author: Max Foster
 */

#include "ExpenseForm.h"
#include "ui_ExpenseForm.h"

#include "Expense.h"
#include "ExpenseController.h"
#include "Utils.h"

ExpenseForm::ExpenseForm(Expense &expense, QWidget *parent)
    : QDialog(parent), formType(expense.null() ? NEW : EDIT), ui(new Ui::ExpenseForm), expense(expense)
{
    ui->setupUi(this);
    ui->label_businessExpenseFormTitle->setText(formType == NEW ? "New Expense" : "Editing Expense");

    updateView();
}

ExpenseForm::~ExpenseForm()
{
    delete ui;
}

void ExpenseForm::updateView()
{
    ui->dateTimeEdit_date->setDateTime(Date(expense.getDate()));

    ui->plainTextEdit_description->blockSignals(true);
    ui->plainTextEdit_description->setPlainText(expense.getDescription());
    ui->plainTextEdit_description->blockSignals(false);

    ui->doubleSpinBox_priceExclVat->setValue(expense.getPrice());
    ui->doubleSpinBox_vat->setValue(expense.getVat());

    double totalPrice = expense.getPrice() + expense.getVat();
    ui->label_totalPriceE->setText(QString(toString(totalPrice).c_str()).prepend(L'£'));

    ui->comboBox_typeHeading->setCurrentIndex(expense.getType());
}

void ExpenseForm::on_pushButton_submit_released()
{
    if (!setRecordData()) return;

    if (formType == EDIT)
    {
        if (ExpenseController::Update(expense, this)) done(Accepted);
    }
    else
    {
        if (ExpenseController::Create(expense, this)) done(Accepted);
    }
}

void ExpenseForm::on_pushButton_cancel_released()
{
    done(Rejected);
}

void ExpenseForm::on_dateTimeEdit_date_dateTimeChanged(const QDateTime &date)
{
    bool success = true;
    try { expense.setDate(Date(date)); }
    catch (const std::exception &e)
    {
        success = false;
        ui->dateTimeEdit_date->setToolTip(e.what());
        ui->dateTimeEdit_date->setStyleSheet("QDateTime { background-color: red; }");
    }
    if (success)
    {
        ui->dateTimeEdit_date->setStyleSheet("");
        ui->dateTimeEdit_date->setToolTip("");
    }
}

void ExpenseForm::on_plainTextEdit_description_textChanged()
{
    bool success = true;
    try { expense.setDescription(ui->plainTextEdit_description->toPlainText().toStdString().c_str()); }
    catch (const std::exception &e)
    {
        success = false;
        ui->plainTextEdit_description->setToolTip(e.what());
        ui->plainTextEdit_description->setStyleSheet("QPlainTextEdit { background-color: red; }");
    }
    if (success)
    {
        ui->plainTextEdit_description->setStyleSheet("");
        ui->plainTextEdit_description->setToolTip("");
    }
}

void ExpenseForm::on_doubleSpinBox_priceExclVat_valueChanged(const double value)
{
    bool success = true;
    try { expense.setPrice(value); }
    catch (const std::exception &e)
    {
        success = false;
        ui->doubleSpinBox_priceExclVat->setToolTip(e.what());
        ui->doubleSpinBox_priceExclVat->setStyleSheet("QDoubleSpinBox { background-color: red; }");
    }
    if (success)
    {
        ui->doubleSpinBox_priceExclVat->setStyleSheet("");
        ui->doubleSpinBox_priceExclVat->setToolTip("");
    }

    updateView();
}

void ExpenseForm::on_doubleSpinBox_vat_valueChanged(const double value)
{
    bool success = true;
    try { expense.setVat(value); }
    catch (const std::exception &e)
    {
        success = false;
        ui->doubleSpinBox_vat->setToolTip(e.what());
        ui->doubleSpinBox_vat->setStyleSheet("QDoubleSpinBox { background-color: red; }");
    }
    if (success)
    {
        ui->doubleSpinBox_vat->setStyleSheet("");
        ui->doubleSpinBox_vat->setToolTip("");
    }

    updateView();
}

void ExpenseForm::on_comboBox_typeHeading_currentIndexChanged(const int index)
{
    bool success = true;
    try { expense.setType(index); }
    catch (const std::exception &e)
    {
        success = false;
        ui->comboBox_typeHeading->setToolTip(e.what());
        ui->comboBox_typeHeading->setStyleSheet("QComboBox { background-color: red; }");
    }
    if (success)
    {
        ui->comboBox_typeHeading->setStyleSheet("");
        ui->comboBox_typeHeading->setToolTip("");
    }
}

bool ExpenseForm::setRecordData()
{
    bool success = true;

    on_dateTimeEdit_date_dateTimeChanged(ui->dateTimeEdit_date->dateTime());
    if (ui->dateTimeEdit_date->styleSheet() != "") success = false;

    on_plainTextEdit_description_textChanged();
    if (ui->plainTextEdit_description->styleSheet() != "") success = false;

    on_doubleSpinBox_priceExclVat_valueChanged(ui->doubleSpinBox_priceExclVat->value());
    if (ui->doubleSpinBox_priceExclVat->styleSheet() != "") success = false;

    on_doubleSpinBox_vat_valueChanged(ui->doubleSpinBox_vat->value());
    if (ui->doubleSpinBox_vat->styleSheet() != "") success = false;

    on_comboBox_typeHeading_currentIndexChanged(ui->comboBox_typeHeading->currentIndex());
    if (ui->comboBox_typeHeading->styleSheet() != "") success = false;

    return success;
}

void ExpenseForm::on_pushButton_datePicker_clicked()
{
    QDate date;
    if (showDatePickerDialog(date)) ui->dateTimeEdit_date->setDate(date);
}
