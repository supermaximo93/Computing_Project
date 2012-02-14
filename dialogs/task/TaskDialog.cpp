/*
 * TaskNew.cpp
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#include "TaskNew.h"
#include "ui_TaskNew.h"

TaskNew::TaskNew(QWidget * parent)
    : QDialog(parent), ui(new Ui::TaskNew)
{
    ui->setupUi(this);
}

TaskNew::~TaskNew()
{
    delete ui;
}
