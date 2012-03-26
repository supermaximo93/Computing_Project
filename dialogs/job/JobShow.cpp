/*
 * JobShow.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

using namespace std;

#include "JobShow.h"
#include "ui_JobShow.h"

#include <QTimer>
#include <QDir>

#include "PaymentMethodDialog.h"

#include "Job.h"
#include "Part.h"
#include "Task.h"
#include "Customer.h"

#include "JobController.h"
#include "CustomerController.h"
#include "PartController.h"
#include "TaskController.h"

#include "PdfGenerator.h"
#include "Emailer.h"
#include "EmailerThread.h"

#include "Setting.h"
#include "SettingController.h"
#include "dialogs/setting/SettingForm.h"

#include "Utils.h"
#include "Globals.h"

JobShow::JobShow(Job &job, Database<Part>::recordList &parts, Database<Task>::recordList &tasks, QWidget *parent)
    : QDialog(parent), ui(new Ui::JobShow), job(job), parts(parts), tasks(tasks), timer(new QTimer(this))
{
    ui->setupUi(this);
    updateView();

    connect(timer, SIGNAL(timeout()), this, SLOT(checkInternetConnection()));
    timer->start(EmailerThread::queueCheckTimePeriod);
}

JobShow::~JobShow()
{
    timer->stop();
    delete timer;
    delete ui;
}

void JobShow::updateView()
{
    ui->label_dateE->setText(Date(job.getDate()));

    Customer customer = CustomerController::getCustomer(job.getCustomerId());
    ui->label_customerNameE->setText(createFullName(customer.getForename(), customer.getSurname()));

    ui->plainTextEdit_descriptionE->setPlainText(job.getDescription());

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

    updateCharges();

    QString str;
    switch (job.getCompletionState())
    {
    case Job::NOT_DONE:    str = "Not done"; break;
    case Job::DONE_UNPAID: str = "Done - not paid"; break;
    case Job::DONE_PAID:   str = "Done - paid"; break;
    }
    ui->label_completionStateE->setText(str);

    ui->verticalWidget_sendInvoiceMarkAsPaid->setHidden(job.getCompletionState() != Job::DONE_UNPAID);
    ui->pushButton_markAsDone->setHidden(job.getCompletionState() != Job::NOT_DONE);
    ui->horizontalWidget_paidBy->setHidden(job.getCompletionState() != Job::DONE_PAID);

    switch (job.getPaymentMethod())
    {
    case Job::NA:            str = "N/A"; break;
    case Job::CASH:          str = "Cash"; break;
    case Job::CHEQUE:        str = "Cheque"; break;
    case Job::CREDIT:        str = "Credit"; break;
    case Job::DEBIT:         str = "Debit"; break;
    case Job::BANK_TRANSFER: str = "Bank transfer"; break;
    }
    ui->label_paymentTypeE->setText(str);
}

void JobShow::updateCharges()
{
    ui->label_labourChargeE->setText(QString(toString(job.getLabourCharge()).c_str()).prepend(L'£'));
    ui->label_vatE->setText((toString(Globals::vatRate(Date(job.getDate()))) + "%").c_str());
    ui->label_totalChargeExclVatE->setText(QString(toString(getTotalChargeExclVat()).c_str()).prepend(L'£'));
    ui->label_totalChargeInclVatE->setText(QString(toString(getTotalChargeInclVat()).c_str()).prepend(L'£'));
}

double JobShow::getTotalChargeExclVat()
{
    double totalCharge = job.getLabourCharge();
    for (unsigned i = 0; i < parts.size(); ++i) totalCharge += parts[i].getPrice() * parts[i].getQuantity();
    return totalCharge;
}

double JobShow::getTotalChargeInclVat()
{
    double totalCharge = job.getLabourCharge() * (1.0 + (Globals::vatRate(Date(job.getDate())) / 100.0));

    for (unsigned i = 0; i < parts.size(); ++i)
        totalCharge += parts[i].getPrice() * parts[i].getQuantity() * (1.0 + (parts[i].getVatRate() / 100.0));

    return totalCharge;
}

void JobShow::checkInternetConnection()
{
    const bool connectionAvailable = EmailerThread::connectionAvailable();
    ui->pushButton_sendInvoice->setEnabled(connectionAvailable);
    ui->pushButton_sendReceipt->setEnabled(connectionAvailable);
}

void JobShow::on_pushButton_ok_released()
{
    done(Accepted);
}

void JobShow::on_pushButton_edit_released()
{
    JobController::Edit(job, this);
    parts = *JobController::getJobParts(job.getId());
    tasks = *JobController::getJobTasks(job.getId());
    updateView();
}

void JobShow::on_pushButton_delete_released()
{
    if (!showYesNoDialog("Are you sure you want to delete this job?\n"
                         "All parts and tasks associated with this job will also be deleted")) return;

    if (JobController::Destroy(job, this))
    {
        showInfoDialog("The job was deleted successfully");
        done(Accepted);
    }
}

void JobShow::on_pushButton_viewCustomerDetails_released()
{
    Customer customer = CustomerController::getCustomer(job.getCustomerId());
    if (customer.null()) // Make sure the customer actually exists
    {
        showErrorDialog("The job is not associated with a customer");
        return;
    }

    CustomerController::Show(customer, this);

    if (customer.null()) // If the customer got deleted, then nullify the job and quit to the parent window
    {
        job = Job();
        done(Accepted);
    }
    else updateView();
}

void JobShow::on_listWidget_partsE_doubleClicked(const QModelIndex &index)
{
    Part &part = parts[index.row()];
    PartController::Show(part, this);
    if (part.null()) parts.erase(parts.begin() + index.row());
    updateView();
}

void JobShow::on_listWidget_tasksE_doubleClicked(const QModelIndex &index)
{
    Task &task = tasks[index.row()];
    TaskController::Show(task, this);
    if (task.null()) tasks.erase(tasks.begin() + index.row());
    updateView();
}

void JobShow::on_pushButton_markAsDone_released()
{
    setNewJobCompletionState(Job::DONE_UNPAID);
}

void JobShow::on_pushButton_sendInvoice_released()
{
    Date jobDate(job.getDate());
    QString saveFolder
            = QDir::currentPath() + "/invoices/" + QDate::longMonthName(Date(job.getDate()).month) + '_'
            + toString(jobDate.year).c_str();

    if (!QDir(saveFolder).exists()) QDir().mkpath(saveFolder);

    Customer customer = CustomerController::getCustomer(job.getCustomerId());
    string invoiceFileName;
    invoiceFileName.reserve(256);
    invoiceFileName += "invoice_";
    invoiceFileName += customer.getForename();
    invoiceFileName += customer.getSurname();
    invoiceFileName += "_";
    invoiceFileName += toString(job.getId());
    invoiceFileName += "_";
    invoiceFileName += (string)jobDate;
    invoiceFileName += ".pdf";
    replaceChars(invoiceFileName, ' ', '_');
    replaceChars(invoiceFileName, '/', '-');
    replaceChars(invoiceFileName, ':', '-');

    invoiceFileName = saveFolder.toStdString() + '/' + invoiceFileName;

    PdfGenerator::generateInvoice(invoiceFileName.c_str(), job);

    EmailDetails emailDetails(customer.getEmailAddress(),
                              SettingController::getSetting(SettingForm::keyInvoiceSubject).getValue(),
                              SettingController::getSetting(SettingForm::keyInvoiceBody).getValue(),
                              invoiceFileName.c_str());

    EmailerThread::enqueueEmail(emailDetails);
}

void JobShow::on_pushButton_markAsPaid_released()
{
    int paymentMethod;
    PaymentMethodDialog dialog(paymentMethod);
    if (dialog.exec() == Accepted)
    {
        try { job.setPaymentMethod(paymentMethod); }
        catch (const std::exception &e) { showErrorDialog(e.what()); return; }

        if (!setNewJobCompletionState(Job::DONE_PAID)) // This'll update the record in the database automatically
        {
            try { job.setPaymentMethod(Job::NA); }
            catch (const std::exception &e) { showErrorDialog(e.what()); }
        }
    }
}

void JobShow::on_pushButton_sendReceipt_released()
{
    Date jobDate(job.getDate());
    QString saveFolder
            = QDir::currentPath() + "/receipts/" + QDate::longMonthName(Date(job.getDate()).month) + '_'
            + toString(jobDate.year).c_str();

    if (!QDir(saveFolder).exists()) QDir().mkpath(saveFolder);

    Customer customer = CustomerController::getCustomer(job.getCustomerId());
    string receiptFileName;
    receiptFileName.reserve(256);
    receiptFileName += "receipt_";
    receiptFileName += customer.getForename();
    receiptFileName += customer.getSurname();
    receiptFileName += "_";
    receiptFileName += toString(job.getId());
    receiptFileName += "_";
    receiptFileName += (string)Date(job.getDate());
    receiptFileName += ".pdf";
    replaceChars(receiptFileName, ' ', '_');
    replaceChars(receiptFileName, '/', '-');
    replaceChars(receiptFileName, ':', '-');

    receiptFileName = saveFolder.toStdString() + '/' + receiptFileName;

    PdfGenerator::generateReceipt(receiptFileName.c_str(), job);

    EmailDetails emailDetails(customer.getEmailAddress(),
                              SettingController::getSetting(SettingForm::keyReceiptSubject).getValue(),
                              SettingController::getSetting(SettingForm::keyReceiptBody).getValue(),
                              receiptFileName.c_str());

    EmailerThread::enqueueEmail(emailDetails);
}

bool JobShow::setNewJobCompletionState(const int state)
{
    const int previousCompletionState = job.getCompletionState();

    try { job.setCompletionState(state); }
    catch (const std::exception &e) { showErrorDialog(e.what()); return false; }

    if (JobController::Update(job, this)) updateView();
    else
    {
        try { job.setCompletionState(previousCompletionState); }
        catch (const std::exception &e) { showErrorDialog(e.what()); }
        return false;
    }

    return true;
}
