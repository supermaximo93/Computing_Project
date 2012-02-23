/*
 * TaskForm.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "TaskForm.h"
#include "ui_TaskForm.h"

#include "Task.h"
#include "TaskController.h"
#include "Utils.h"

TaskForm::TaskForm(Task & task, QWidget * parent)
    : QDialog(parent), formType((task.null() && !task.pending) ? NEW : EDIT), ui(new Ui::TaskForm), task(task)
{
    ui->setupUi(this);

    ui->label_taskFormTitle->setText(formType == NEW ? "New Task" : "Editing Task");
    updateView();
}

TaskForm::~TaskForm()
{
    delete ui;
}

void TaskForm::updateView()
{
    ui->dateTimeEdit_date->setDateTime(Date(task.getDate()));
    ui->plainTextEdit_description->setPlainText(task.getDescription());
}

void TaskForm::on_dateTimeEdit_date_dateTimeChanged(const QDateTime & date)
{
    bool success = true;
    try
    {
        task.setDate(Date(date.time().minute(), date.time().hour(), date.date().day(), date.date().month(),
                          date.date().year()));
    }
    catch (const std::exception & e)
    {
        success = false;
        ui->dateTimeEdit_date->setToolTip(e.what());
        ui->dateTimeEdit_date->setStyleSheet("QDateTimeEdit { background-color: red; }");
    }
    if (success)
    {
        ui->dateTimeEdit_date->setStyleSheet("");
        ui->dateTimeEdit_date->setToolTip("");
    }
}

void TaskForm::on_plainTextEdit_description_textChanged()
{
    bool success = true;
    try { task.setDescription(ui->plainTextEdit_description->toPlainText().toStdString().c_str()); }
    catch (const std::exception & e)
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

void TaskForm::on_pushButton_submit_released()
{
    if (!setRecordData()) return;
    if ((formType == EDIT) && (!task.null()))
    {
        if (!TaskController::Update(task, this)) return;
    }
    else task.pending = true;
    done(Accepted);
}

void TaskForm::on_pushButton_cancel_released()
{
    done(Rejected);
}

bool TaskForm::setRecordData()
{
    bool success = true;

    on_dateTimeEdit_date_dateTimeChanged(ui->dateTimeEdit_date->dateTime());
    if (ui->dateTimeEdit_date->styleSheet() != "") success = false;

    on_plainTextEdit_description_textChanged();
    if (ui->plainTextEdit_description->styleSheet() != "") success = false;

    return success;
}
