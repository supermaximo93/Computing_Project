/*
 * UnpaidJobsDialog.cpp
 *
 *  Created on: 31 May 2012
 *      Author: Max Foster
 */

#include "UnpaidJobsDialog.h"
#include "ui_UnpaidJobsDialog.h"

#include <QUrl>
#include <QDesktopServices>

#include "JobController.h"
#include "Job.h"
#include "CustomerController.h"
#include "Customer.h"
#include "SettingController.h"
#include "Setting.h"
#include "EmailerThread.h"
#include "EmailDetails.h"

UnpaidJobsDialog::UnpaidJobsDialog(Database<Job>::recordList &jobs, QWidget *parent)
    : QDialog(parent), ui(new Ui::UnpaidJobsDialog), jobs(jobs)
{
    ui->setupUi(this);
    setWindowTitle("Unpaid jobs");
    updateView();
}

UnpaidJobsDialog::~UnpaidJobsDialog()
{
    delete ui;
}

void UnpaidJobsDialog::updateView()
{
    JobController::sortJobsByDate(jobs, true);

    ui->tableWidget->clear();

    QStringList headings;
    headings.append("Customer Name");
    headings.append("Date");
    headings.append("Amount");
    ui->tableWidget->setHorizontalHeaderLabels(headings);

    Customer customer;
    QString name, date, amount;
    ui->tableWidget->setRowCount(jobs.size());
    for (unsigned i = 0; i < jobs.size(); ++i)
    {
        customer = CustomerController::getCustomer(jobs[i].getCustomerId());
        name = createFullName(customer.getForename(), customer.getSurname());
        date = (QString)Date(jobs[i].getDate());
        amount = QString(L'£') + toString(jobs[i].getTotalPrice()).c_str();
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(name));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(date));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(amount));
    }
}

void UnpaidJobsDialog::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    Job &job = jobs[index.row()];
    JobController::Show(job, this);
    if (job.null() || (job.getCompletionState() == Job::DONE_PAID)) jobs.erase(jobs.begin() + index.row());
    updateView();
}

void UnpaidJobsDialog::on_pushButton_ok_released()
{
    done(Accepted);
}

void UnpaidJobsDialog::on_pushButton_sendReminder_released()
{
    Setting subject = SettingController::getSetting(SettingForm::keyReminderSubject),
            body = SettingController::getSetting(SettingForm::keyReminderBody);

    if (strlen(SettingController::getSetting(SettingForm::keyEmailHost).getValue()) == 0)
    {
        for (int i = 0; i < ui->tableWidget->selectedItems().size(); ++i)
        {
            int index = ui->tableWidget->selectedItems().at(i)->row();
            Customer customer = CustomerController::getCustomer(jobs[index].getCustomerId());

            QString mailtoLink
                    = QString("mailto:") + customer.getEmailAddress()
                    + "?subject=" + subject.getValue()
                    + "&body=" + body.getValue();

            QDesktopServices::openUrl(QUrl(mailtoLink));
        }
    }
    else
    {
        for (int i = 0; i < ui->tableWidget->selectedItems().size(); ++i)
        {
            int index = ui->tableWidget->selectedItems().at(i)->row();
            Customer customer = CustomerController::getCustomer(jobs[index].getCustomerId());
            EmailerThread::enqueueEmail(EmailDetails(customer.getEmailAddress(), subject.getValue(), body.getValue()));
        }
    }
}
