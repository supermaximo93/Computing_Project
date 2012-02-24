/*
 * TaskShow.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "TaskShow.h"
#include "ui_TaskShow.h"

#include "Task.h"
#include "TaskController.h"
#include "Utils.h"

TaskShow::TaskShow(Task &task, QWidget *parent)
    : QDialog(parent), ui(new Ui::TaskShow), task(task)
{
    ui->setupUi(this);
    updateView();
}

TaskShow::~TaskShow()
{
    delete ui;
}

void TaskShow::updateView()
{
    ui->label_dateE->setText(Date(task.getDate()));
    ui->plainTextEdit_descriptionE->setPlainText(task.getDescription());
}

void TaskShow::on_pushButton_ok_released()
{
    done(Accepted);
}

void TaskShow::on_pushButton_edit_released()
{
    TaskController::Edit(task, this);
    updateView();
}

void TaskShow::on_pushButton_delete_released()
{
    if (!showYesNoDialog("Are you sure you want to delete this task?")) return;

    if (task.pending)
    {
        task = Task();
        done(Accepted);
    }
    else if (TaskController::Destroy(task, this))
    {
        showInfoDialog("The task was deleted successfully");
        done(Accepted);
    }
}
