/*
 * CustomerShow.cpp
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#include "CustomerShow.h"
#include "ui_CustomerShow.h"

CustomerShow::CustomerShow(QWidget * parent)
    : QDialog(parent), ui(new Ui::CustomerShow)
{
    ui->setupUi(this);
}

CustomerShow::~CustomerShow()
{
    delete ui;
}
