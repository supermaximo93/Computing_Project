/*
 * JobEdit.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "JobEdit.h"
#include "ui_JobEdit.h"

#include "JobController.h"

JobEdit::JobEdit(Job & job, QWidget * parent)
    : QDialog(parent), ui(new Ui::JobEdit), job(job), jobCopy(job)
{
    ui->setupUi(this);
    updateView();
}

JobEdit::~JobEdit()
{
    delete ui;
}

void JobEdit::updateView()
{

}

void JobEdit::on_pushButton_cancel_released()
{
    done(Rejected);
}

void JobEdit::on_pushButton_submit_released()
{
    if (JobController::Update(jobCopy, this))
    {
        job = jobCopy;
        done(Accepted);
    }
}
