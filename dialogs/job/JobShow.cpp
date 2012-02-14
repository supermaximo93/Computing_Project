/*
 * JobShow.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "JobShow.h"
#include "ui_JobShow.h"

#include "Job.h"
#include "Part.h"
#include "Task.h"
#include "JobController.h"
#include "Utils.h"

JobShow::JobShow(Job & job, const Database<Part>::recordList & parts,
                 const Database<Task>::recordList & tasks, QWidget * parent)
    : QDialog(parent), ui(new Ui::JobShow), job(job), parts(parts), tasks(tasks)
{
    ui->setupUi(this);
    updateView();
}

JobShow::~JobShow()
{
    delete ui;
}

void JobShow::updateView()
{
    ui->label_dateE->setText(Date(job.getDate()));
}

void JobShow::on_pushButton_ok_released()
{
    done(Accepted);
}

void JobShow::on_pushButton_edit_released()
{
    JobController::Edit(job, this);
    updateView();
}

void JobShow::on_pushButton_delete_released()
{
    if (!showYesNoDialog("Are you sure you want to delete this job?")) return;

    if (JobController::Destroy(job, this))
    {
        showInfoDialog("The job was deleted successfully");
        done(Accepted);
    }
}
