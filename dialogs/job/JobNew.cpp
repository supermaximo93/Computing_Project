/*
 * JobNew.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "JobNew.h"
#include "ui_JobNew.h"

#include "JobController.h"

JobNew::JobNew(Job & job, QWidget * parent)
    : QDialog(parent), ui(new Ui::JobNew), job(job)
{
    ui->setupUi(this);
}

JobNew::~JobNew()
{
    delete ui;
}

void JobNew::on_pushButton_cancel_released()
{
    done(Rejected);
}

void JobNew::on_pushButton_submit_released()
{
    if (JobController::Create(job, this)) done(Accepted);
}
