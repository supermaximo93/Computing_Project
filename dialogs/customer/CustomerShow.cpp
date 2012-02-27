/*
 * CustomerShow.cpp
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#include "CustomerShow.h"
#include "ui_CustomerShow.h"

#include "Customer.h"
#include "Job.h"
#include "CustomerController.h"
#include "JobController.h"
#include "Utils.h"

CustomerShow::CustomerShow(Customer &customer, Database<Job>::recordList &jobs, QWidget *parent)
    : QDialog(parent), ui(new Ui::CustomerShow), customer(customer), jobs(jobs)
{
    ui->setupUi(this);

    updateView();
}

CustomerShow::~CustomerShow()
{
    delete ui;
}

void CustomerShow::updateView()
{
    QString customerName(createFullName(customer.getForename(), customer.getSurname()));
    ui->label_customerNameE->setText(customerName);
    ui->label_customerNameE2->setText(customerName);

    ui->label_addressLine1E->setText(customer.getAddressLine1());
    ui->label_addressLine2E->setText(customer.getAddressLine2());
    ui->label_townE->setText(customer.getTown());
    ui->label_postcodeE->setText(customer.getPostcode());
    ui->label_homePhoneE->setText(customer.getHomePhoneNumber());
    ui->label_mobilePhoneE->setText(customer.getMobilePhoneNumber());
    ui->label_emailAddressE->setText(customer.getEmailAddress());

    ui->listWidget_jobs->clear();
    for (unsigned i = 0; i < jobs.size(); ++i) ui->listWidget_jobs->addItem(Date(jobs[i].getDate()));
}

void CustomerShow::on_pushButton_ok_released()
{
    done(Accepted);
}

void CustomerShow::on_pushButton_edit_released()
{
    CustomerController::Edit(customer, this);
    updateView();
}

void CustomerShow::on_pushButton_delete_released()
{
    if (!showYesNoDialog("Are you sure you want to delete this customer?")) return;

    if (CustomerController::Destroy(customer, this))
    {
        showInfoDialog("The customer was deleted successfully");
        done(Accepted);
    }
}

void CustomerShow::on_listView_jobs_doubleClicked(const QModelIndex &index)
{
    Job & job = jobs[index.row()];
    JobController::Show(job, this);
    if (job.null()) jobs.erase(jobs.begin() + index.row());
    updateView();
}

void CustomerShow::on_pushButton_addJob_released()
{
    Job job = JobController::New(this);
    if (!job.null() && (job.getCustomerId() == customer.getId()))
    {
        jobs.push_back(job);
        updateView();
    }
}
