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
#include "PartController.h"
#include "TaskController.h"

JobForm::JobForm(Job & job, Database<Customer>::recordList & customers, Database<Part>::recordList & parts,
                 Database<Task>::recordList & tasks, QWidget * parent)
    : QDialog(parent), formType(job.null() ? NEW : EDIT), ui(new Ui::JobForm), job(job), customers(customers),
      parts(parts), tasks(tasks)
{
    ui->setupUi(this);

    ui->label_title->setText(formType == NEW ? "New Job" : "Editing Job");

    updateView();
}

JobForm::~JobForm()
{
    delete ui;
}

void JobForm::updateView()
{
    ui->dateTimeEdit_date->setDateTime(Date(job.getDate()));

    ui->comboBox_customer->clear();
    unsigned customerIndex = 0;
    for (unsigned i = 0; i < customers.size(); ++i)
    {
        const Customer & customer = customers[i];
        const int customerId = customer.getId();
        if ((formType == EDIT) && (customerId == job.getCustomerId())) customerIndex = i;

        ui->comboBox_customer->addItem(createFullName(customer.getForename(), customer.getSurname()), customerId);
    }
    ui->comboBox_customer->setCurrentIndex(customerIndex);

    for (unsigned i = 0; i < parts.size(); ++i) ui->listWidget_partsE->addItem(parts[i].getName());

    const unsigned descriptionPreviewLength = 63;
    char descriptionPreview[descriptionPreviewLength + 4];
    for (unsigned i = 0; i < tasks.size(); ++i)
    {
        strncpy(descriptionPreview, tasks[i].getDescription(), descriptionPreviewLength);
        strcat(descriptionPreview, "...");
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
    try { job.setCompletionState(index); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        ui->comboBox_completionState->setStyleSheet("QComboBox { background-color: red; }");
    }
}

void JobForm::on_comboBox_paidBy_currentIndexChanged(int index)
{
    ui->comboBox_completionState->setStyleSheet("");
    try { job.setPaymentMethod(index); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        ui->comboBox_completionState->setStyleSheet("QComboBox { background-color: red; }");
    }
}

void JobForm::on_pushButton_addNewCustomer_released()
{
    Customer customer = CustomerController::New(this);
    if (!customer.null())
    {
        customers.push_back(customer);
        try { job.setCustomerId(customer.getId()); }
        catch (const std::exception & e) { showErrorDialog(e.what()); }
        updateView();
    }
}

void JobForm::on_listWidget_partsE_doubleClicked(const QModelIndex & index)
{
    Part & part = parts[index.row()];
    PartController::Show(part, this);
    if (part.null()) parts.erase(parts.begin() + index.row());
    updateView();
}

void JobForm::on_listWidget_tasksE_doubleClicked(const QModelIndex & index)
{
    Task & task = tasks[index.row()];
    TaskController::Show(task, this);
    if (task.null()) tasks.erase(tasks.begin() + index.row());
    updateView();
}

void JobForm::on_pushButton_addPart_released()
{
    Part part = PartController::New(this);
    if (!part.null())
    {
        parts.push_back(part);
        updateView();
    }
}

void JobForm::on_pushButton_addTask_released()
{
    Task task = TaskController::New(this);
    if (!task.null())
    {
        tasks.push_back(task);
        updateView();
    }
}
