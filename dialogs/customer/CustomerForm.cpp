/*
 * CustomerForm.cpp
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#include "CustomerForm.h"
#include "ui_CustomerForm.h"

CustomerForm::CustomerForm(QWidget * parent)
    : QDialog(parent), ui(new Ui::CustomerForm)
{
    ui->setupUi(this);
}

CustomerForm::~CustomerForm()
{
    delete ui;
}
