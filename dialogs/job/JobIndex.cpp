/*
 * JobIndex.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include <sstream>
using namespace std;

#include "JobIndex.h"
#include "ui_JobIndex.h"

#include "Job.h"
#include "Customer.h"
#include "JobController.h"
#include "CustomerController.h"
#include "Utils.h"

JobIndex::JobIndex(Database<Job>::recordList &jobs, QWidget *parent)
    : QDialog(parent), ui(new Ui::JobIndex), jobs(jobs)
{
    ui->setupUi(this);
    updateView();
}

JobIndex::~JobIndex()
{
    delete ui;
}

void JobIndex::updateView()
{
    JobController::sortJobsByDate(jobs, false);

    ui->tableWidget->clear();

    QStringList headings;
    headings.append("Customer Name");
    headings.append("Date");
    ui->tableWidget->setHorizontalHeaderLabels(headings);

    Customer customer;
    QString name, date;
    ui->tableWidget->setRowCount(jobs.size());
    for (unsigned i = 0; i < jobs.size(); ++i)
    {
        customer = CustomerController::getCustomer(jobs[i].getCustomerId());
        name = createFullName(customer.getForename(), customer.getSurname());
        date = (QString)Date(jobs[i].getDate());
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(name));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(date));
    }
}

void JobIndex::on_pushButton_ok_released()
{
    done(Accepted);
}

void JobIndex::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    Job &job = jobs[index.row()];
    JobController::Show(job, this);
    if (job.null()) jobs.erase(jobs.begin() + index.row());
    updateView();
}

void JobIndex::on_pushButton_addNewJob_released()
{
    Job job = JobController::New(this);
    if (!job.null())
    {
        jobs.push_back(job);
        updateView();
    }
}
