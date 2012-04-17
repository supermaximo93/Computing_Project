/*
 * JobNew.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

using namespace std;

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

JobForm::JobForm(Job &job, Database<Customer>::recordList &customers, Database<Part>::recordList &parts,
                 Database<Task>::recordList &tasks, QWidget *parent)
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
    TaskController::sortTasksByDate(tasks);

    ui->dateTimeEdit_date->setDateTime(Date(job.getDate()));

    ui->comboBox_customer->blockSignals(true); // Need to block signals as when addItem is called, a callback is called
    ui->comboBox_customer->clear();
    unsigned customerIndex = 0;
    for (unsigned i = 0; i < customers.size(); ++i)
    {
        const Customer &customer = customers[i];
        const int customerId = customer.getId();
        if (customerId == job.getCustomerId()) customerIndex = i;

        ui->comboBox_customer->addItem(createFullName(customer.getForename(), customer.getSurname()), customerId);
    }
    ui->comboBox_customer->setCurrentIndex(customerIndex);
    ui->pushButton_viewCustomer->setVisible(ui->comboBox_customer->count() > 0);
    ui->comboBox_customer->blockSignals(false);

    ui->plainTextEdit_description->blockSignals(true);
    ui->plainTextEdit_description->setPlainText(job.getDescription());
    ui->plainTextEdit_description->blockSignals(false);

    ui->listWidget_partsE->clear();
    char partString[64];
    for (unsigned i = 0; i < parts.size(); ++i)
    {
        strcpy(partString, toString(parts[i].getQuantity()).c_str());
        strcat(partString, "x ");
        strcat(partString, parts[i].getName());
        ui->listWidget_partsE->addItem(partString);
    }

    ui->listWidget_tasksE->clear();
    for (unsigned i = 0; i < tasks.size(); ++i)
        ui->listWidget_tasksE->addItem(limitLength(tasks[i].getDescription(), 40));

    updateCharges(true);

    ui->comboBox_completionState->setCurrentIndex(job.getCompletionState());
    ui->comboBox_paidBy->setCurrentIndex(job.getPaymentMethod());
    ui->dateTimeEdit_paymentDate->setDateTime(Date(job.getPaymentDate()));
}

void JobForm::on_pushButton_cancel_released()
{
    done(Rejected);
}

template<typename T>
void addRecordsToDatabase(std::vector<T> &records, const int jobId, bool(*createFunction)(T&, QWidget*),
                          vector<string> &errorList)
{
    for (unsigned i = 0; i < records.size(); ++i)
    {
        if (records[i].getId() >= 0) continue;
        if (records[i].getJobId() != jobId)
        {
            try { records[i].setJobId(jobId); }
            catch (const std::exception &e)
            {
                addError(errorList, e.what());
                continue;
            }
        }
        createFunction(records[i], NULL);
        records[i].pending = false;
    }
}

void JobForm::addPartsAndTasksToDatabase()
{
    vector<string> errors;
    addRecordsToDatabase(parts, job.getId(), PartController::Create, errors);
    addRecordsToDatabase(tasks, job.getId(), TaskController::Create, errors);
    if (errors.size() > 0) showErrorDialog(errors);
}

void JobForm::on_pushButton_submit_released()
{
    if (!setRecordData()) return;
    if (formType == NEW)
    {
        if (JobController::Create(job, this))
        {
            addPartsAndTasksToDatabase();
            done(Accepted);
        }
    }
    else
    {
        if (JobController::Update(job, this))
        {
            addPartsAndTasksToDatabase();
            done(Accepted);
        }
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

    on_plainTextEdit_description_textChanged();
    if (ui->plainTextEdit_description->styleSheet() != "") success = false;

    on_doubleSpinBox_labourCharge_valueChanged(ui->doubleSpinBox_labourCharge->value());
    if (ui->doubleSpinBox_labourCharge->styleSheet() != "") success = false;

    on_comboBox_completionState_currentIndexChanged(ui->comboBox_completionState->currentIndex());
    if (ui->comboBox_completionState->styleSheet() != "") success = false;

    on_comboBox_paidBy_currentIndexChanged(ui->comboBox_paidBy->currentIndex());
    if (ui->comboBox_paidBy->styleSheet() != "") success = false;

    on_dateTimeEdit_paymentDate_dateTimeChanged(ui->dateTimeEdit_paymentDate->dateTime());
    if (ui->dateTimeEdit_paymentDate->styleSheet() != "") success = false;

    return success;
}

void JobForm::updateCharges(const bool updateFields)
{
    if (updateFields) ui->doubleSpinBox_labourCharge->setValue(job.getLabourCharge());
    ui->label_vatE->setText((toString(Globals::vatRate(Date(job.getDate()))) + "%").c_str());
    ui->label_totalChargeExclVatE->setText(to2Dp(toString(getTotalChargeExclVat()).c_str()).prepend(L'£'));
    ui->label_totalChargeInclVatE->setText(to2Dp(toString(getTotalChargeInclVat()).c_str()).prepend(L'£'));
}

double JobForm::getTotalChargeExclVat()
{
    double totalCharge = ui->doubleSpinBox_labourCharge->value();
    for (unsigned i = 0; i < parts.size(); ++i) totalCharge += parts[i].getPrice() * parts[i].getQuantity();
    return totalCharge;
}

double JobForm::getTotalChargeInclVat()
{
    double totalCharge = doubleTo2Dp(ui->doubleSpinBox_labourCharge->value()
                                     * (1.0 + (Globals::vatRate(Date(job.getDate())) / 100.0)));

    for (unsigned i = 0; i < parts.size(); ++i)
        totalCharge
                += doubleTo2Dp((parts[i].getPrice()*parts[i].getQuantity()) * (1.0 + (parts[i].getVatRate() / 100.0)));

    return totalCharge;
}

void JobForm::on_dateTimeEdit_date_dateTimeChanged(const QDateTime &date)
{
    bool success = true;
    try { job.setDate(Date(date)); }
    catch (const std::exception &e)
    {
        success = false;
        ui->dateTimeEdit_date->setToolTip(e.what());
        ui->dateTimeEdit_date->setStyleSheet("QDateTimeEdit { background-color: red; }");
    }
    if (success)
    {
        ui->dateTimeEdit_date->setStyleSheet("");
        ui->dateTimeEdit_date->setToolTip("");
        updateCharges();
    }
}

void JobForm::on_comboBox_customer_currentIndexChanged(int index)
{
    bool success = true;
    try { job.setCustomerId(ui->comboBox_customer->itemData(index).toInt()); }
    catch (const std::exception &e)
    {
        success = false;
        ui->comboBox_customer->setToolTip(e.what());
        ui->comboBox_customer->setStyleSheet("QComboBox { background-color: red; }");
    }
    if (success)
    {
        ui->comboBox_customer->setStyleSheet("");
        ui->comboBox_customer->setToolTip("");
    }
}

void JobForm::on_plainTextEdit_description_textChanged()
{
    bool success = true;
    try { job.setDescription(ui->plainTextEdit_description->toPlainText().toStdString().c_str()); }
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

void JobForm::on_doubleSpinBox_labourCharge_valueChanged(double value)
{
    bool success = true;
    try { job.setLabourCharge(value); }
    catch (const std::exception &e)
    {
        success = false;
        ui->doubleSpinBox_labourCharge->setToolTip(e.what());
        ui->doubleSpinBox_labourCharge->setStyleSheet("QDoubleSpinBox { background-color: red; }");
    }
    if (success)
    {
        ui->doubleSpinBox_labourCharge->setStyleSheet("");
        ui->doubleSpinBox_labourCharge->setToolTip("");
    }

    updateCharges();
}

void JobForm::on_comboBox_completionState_currentIndexChanged(int index)
{
    bool success = true;
    try { job.setCompletionState(index); }
    catch (const std::exception &e)
    {
        ui->comboBox_completionState->setToolTip(e.what());
        ui->comboBox_completionState->setStyleSheet("QComboBox { background-color: red; }");
    }
    if (success)
    {
        ui->comboBox_completionState->setStyleSheet("");
        ui->comboBox_completionState->setToolTip("");
    }
}

void JobForm::on_comboBox_paidBy_currentIndexChanged(int index)
{
    bool success = true;
    try { job.setPaymentMethod(index); }
    catch (const std::exception &e)
    {
        ui->comboBox_completionState->setToolTip(e.what());
        ui->comboBox_completionState->setStyleSheet("QComboBox { background-color: red; }");
    }
    if (success)
    {
        ui->comboBox_completionState->setStyleSheet("");
        ui->comboBox_completionState->setToolTip("");
    }
}

void JobForm::on_pushButton_addNewCustomer_released()
{
    Customer customer = CustomerController::New(this);
    if (!customer.null())
    {
        customers.push_back(customer);
        try { job.setCustomerId(customer.getId()); }
        catch (const std::exception &e) { showErrorDialog(e.what()); }
        updateView();
    }
}

void JobForm::on_pushButton_viewCustomer_released()
{
    Customer & customer = customers[ui->comboBox_customer->currentIndex()];
    CustomerController::Show(customer, this);
    if (customer.null()) customers.erase(customers.begin() + ui->comboBox_customer->currentIndex());
    updateView();
}

void JobForm::on_listWidget_partsE_doubleClicked(const QModelIndex &index)
{
    Part &part = parts[index.row()];
    PartController::Show(part, this);
    if (part.null() && !part.pending) parts.erase(parts.begin() + index.row());
    updateView();
}

void JobForm::on_listWidget_tasksE_doubleClicked(const QModelIndex &index)
{
    Task &task = tasks[index.row()];
    TaskController::Show(task, this);
    if (task.null() && !task.pending) tasks.erase(tasks.begin() + index.row());
    updateView();
}

void JobForm::on_pushButton_addPart_released()
{
    Part part = PartController::New(this);
    if (part.pending)
    {
        parts.push_back(part);
        updateView();
    }
}

void JobForm::on_pushButton_addTask_released()
{
    Task task = TaskController::New(this);
    if (task.pending)
    {
        tasks.push_back(task);
        updateView();
    }
}

void JobForm::on_pushButton_datePickerDate_clicked()
{
    QDate date;
    if (showDatePickerDialog(date)) ui->dateTimeEdit_date->setDate(date);
}

void JobForm::on_dateTimeEdit_paymentDate_dateTimeChanged(const QDateTime &date)
{
    bool success = true;
    try { job.setPaymentDate(Date(date)); }
    catch (const std::exception &e)
    {
        success = false;
        ui->dateTimeEdit_paymentDate->setToolTip(e.what());
        ui->dateTimeEdit_paymentDate->setStyleSheet("QDateTimeEdit { background-color: red; }");
    }
    if (success)
    {
        ui->dateTimeEdit_paymentDate->setStyleSheet("");
        ui->dateTimeEdit_paymentDate->setToolTip("");
    }
}

void JobForm::on_pushButton_datePickerPaymentDate_clicked()
{
    QDate date;
    if (showDatePickerDialog(date)) ui->dateTimeEdit_paymentDate->setDate(date);
}
