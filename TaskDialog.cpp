/*
 * TaskDialog.cpp
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#include "TaskDialog.h"
#include "ui_TaskDialog.h"

TaskDialog::TaskDialog(QWidget * parent)
    : QDialog(parent), ui(new Ui::TaskDialog)
{
    ui->setupUi(this);
}

TaskDialog::~TaskDialog()
{
    delete ui;
}
