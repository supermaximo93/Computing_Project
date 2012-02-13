/*
 * PartDialog.cpp
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#include "PartDialog.h"
#include "ui_PartDialog.h"

PartDialog::PartDialog(QWidget * parent)
    : QDialog(parent), ui(new Ui::PartDialog)
{
    ui->setupUi(this);
}

PartDialog::~PartDialog()
{
    delete ui;
}
