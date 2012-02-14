/*
 * JobNew.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "JobForm.h"
#include "ui_JobForm.h"

#include "Globals.h"
#include "Job.h"
#include "Part.h"
#include "Task.h"
#include "Customer.h"
#include "JobController.h"
#include "CustomerController.h"

JobForm::JobForm(Job & job, QWidget * parent)
    : QDialog(parent), formType(job.null() ? NEW : EDIT), ui(new Ui::JobForm), job(job)
{
    ui->setupUi(this);

    ui->label_title->setText(formType == NEW ? "New Job" : "Editing Job");

    ui->comboBox_completionState->addItem("Not done", Job::NOT_DONE);
    ui->comboBox_completionState->addItem("Done - not paid", Job::DONE_UNPAID);
    ui->comboBox_completionState->addItem("Done - paid", Job::DONE_PAID);

    ui->comboBox_paidBy->addItem("N/A", Job::NA);
    ui->comboBox_paidBy->addItem("Cash", Job::CASH);
    ui->comboBox_paidBy->addItem("Cheque", Job::CHEQUE);
    ui->comboBox_paidBy->addItem("Credit", Job::CREDIT);
    ui->comboBox_paidBy->addItem("Debit", Job::DEBIT);
    ui->comboBox_paidBy->addItem("Bank transfer", Job::BANK_TRANSFER);

    updateView();
}

JobForm::~JobForm()
{
    delete ui;
}

void JobForm::updateView()
{
    Database<Customer>::recordListPtr customers = CustomerController::getAllCustomers();
    ui->comboBox_customer->clear();

    ui->dateTimeEdit_date->setDateTime(Date(job.getDate()));

    char fullName[(Customer::maxNameLength * 2) + 2];
    for (unsigned i = 0; i < customers->size(); ++i)
    {
        strcpy(fullName, customers->at(i).getForename());
        strcat(fullName, " ");
        strcat(fullName, customers->at(i).getSurname());
        ui->comboBox_customer->addItem(fullName, customers->at(i).getId());
    }

    for (unsigned i = 0; i < parts.size(); ++i) ui->listWidget_partsE->addItem(parts[i].getName());

    const unsigned descriptionPreviewLength = 63;
    char descriptionPreview[descriptionPreviewLength + 1];
    for (unsigned i = 0; i < tasks.size(); ++i)
    {
        strncpy(descriptionPreview, tasks[i].getDescription(), descriptionPreviewLength);
        ui->listWidget_tasksE->addItem(descriptionPreview);
    }

    updateCharges();

    ui->comboBox_completionState->setCurrentIndex(job.getCompletionState());
    ui->comboBox_paidBy->setCurrentIndex(job.getPaymentMethod());
}

void JobForm::on_pushButton_cancel_released()
{
    done(Rejected);
}

void JobForm::on_pushButton_submit_released()
{
    if (!setRecordData()) return;
    if (formType == NEW)
    {
        if (JobController::Create(job, this)) done(Accepted);
    }
    else
    {
        if (JobController::Update(job, this)) done(Accepted);
    }
}

bool JobForm::setRecordData()
{
    bool success = true;

    // Just use the event handlers to keep the code DRY.
    // If the validation was successful, then the stylesheets for each widget should be empty

    on_dateTimeEdit_date_dateTimeChanged(ui->dateTimeEdit_date->dateTime());
    if (ui->dateTimeEdit_date->styleSheet() != "") success = false;

    on_comboBox_customer_currentIndexChanged(ui->comboBox_customer->currentIndex());
    if (ui->comboBox_customer->styleSheet() != "") success = false;

    on_doubleSpinBox_labourCharge_valueChanged(ui->doubleSpinBox_labourCharge->value());
    if (ui->doubleSpinBox_labourCharge->styleSheet() != "") success = false;

    on_comboBox_completionState_currentIndexChanged(ui->comboBox_completionState->currentIndex());
    if (ui->comboBox_completionState->styleSheet() != "") success = false;

    on_comboBox_paidBy_currentIndexChanged(ui->comboBox_paidBy->currentIndex());
    if (ui->comboBox_paidBy->styleSheet() != "") success = false;

    return success;
}

void JobForm::updateCharges()
{
    ui->doubleSpinBox_labourCharge->setValue(job.getLabourCharge());
    ui->label_vatE->setText((toString(Globals::vatRate(Date(job.getDate()))) + "%").c_str());
    ui->label_totalChargeExclVatE->setText(QString(toString(getTotalChargeExclVat()).c_str()).prepend(L'£'));
    ui->label_totalChargeInclVatE->setText(QString(toString(getTotalChargeInclVat()).c_str()).prepend(L'£'));
}

double JobForm::getTotalChargeExclVat()
{
    double totalCharge = ui->doubleSpinBox_labourCharge->value();
    for (unsigned i = 0; i < parts.size(); ++i) totalCharge += parts[i].getPrice();
    return totalCharge;
}

double JobForm::getTotalChargeInclVat()
{
    double totalCharge =
            ui->doubleSpinBox_labourCharge->value() * (1.0 + (Globals::vatRate(Date(job.getDate())) / 100.0));

    for (unsigned i = 0; i < parts.size(); ++i)
        totalCharge += parts[i].getPrice() * (1.0 + (parts[i].getVatRate() / 100.0));

    return totalCharge;
}

void JobForm::on_dateTimeEdit_date_dateTimeChanged(const QDateTime & date)
{
    ui->dateTimeEdit_date->setStyleSheet("");
    try
    {
        job.setDate(Date(date.time().minute(), date.time().hour(), date.date().day(), date.date().month(),
                         date.date().year()));
    }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        ui->dateTimeEdit_date->setStyleSheet("QDateTimeEdit { background-color: red; }");
    }
}

void JobForm::on_comboBox_customer_currentIndexChanged(int index)
{
    ui->comboBox_customer->setStyleSheet("");
    try { job.setCustomerId(ui->comboBox_customer->itemData(index).toInt()); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        ui->comboBox_customer->setStyleSheet("QComboBox { background-color: red; }");
    }
}

void JobForm::on_doubleSpinBox_labourCharge_valueChanged(double value)
{
    ui->doubleSpinBox_labourCharge->setStyleSheet("");
    try { job.setLabourCharge(value); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        ui->doubleSpinBox_labourCharge->setValue(job.getLabourCharge());
        ui->doubleSpinBox_labourCharge->setStyleSheet("QDoubleSpinBox { background-color: red; }");
    }
    updateCharges();
}

void JobForm::on_comboBox_completionState_currentIndexChanged(int index)
{
    ui->comboBox_completionState->setStyleSheet("");
    try { job.setCompletionState(ui->comboBox_completionState->itemData(index).toInt()); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        ui->comboBox_completionState->setStyleSheet("QComboBox { background-color: red; }");
    }
}

void JobForm::on_comboBox_paidBy_currentIndexChanged(int index)
{
    ui->comboBox_completionState->setStyleSheet("");
    try { job.setPaymentMethod(ui->comboBox_completionState->itemData(index).toInt()); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        ui->comboBox_completionState->setStyleSheet("QComboBox { background-color: red; }");
    }
}
