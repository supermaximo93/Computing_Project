/*
 * PartNew.cpp
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#include "PartNew.h"
#include "ui_PartNew.h"

PartNew::PartNew(QWidget * parent)
    : QDialog(parent), ui(new Ui::PartNew)
{
    ui->setupUi(this);
}

PartNew::~PartNew()
{
    delete ui;
}
