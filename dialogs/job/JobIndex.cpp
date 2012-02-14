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
#include "JobController.h"
#include "Utils.h"

JobIndex::JobIndex(Database<Job>::recordList & jobs, QWidget * parent)
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
    ui->listWidget_jobsE->clear();
    for (unsigned i = 0; i < jobs.size(); ++i) ui->listWidget_jobsE->addItem(Date(jobs[i].getDate()));
}

void JobIndex::on_pushButton_ok_released()
{
    done(Accepted);
}

void JobIndex::on_listWidget_jobsE_doubleClicked(const QModelIndex & index)
{
    Job & job = jobs[index.row()];
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
