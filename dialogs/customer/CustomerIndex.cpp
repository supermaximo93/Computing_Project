/*
 * CustomerIndex.cpp
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#include "CustomerIndex.h"
#include "ui_CustomerIndex.h"

CustomerIndex::CustomerIndex(QWidget * parent)
    : QDialog(parent), ui(new Ui::CustomerIndex)
{
    ui->setupUi(this);
}

CustomerIndex::~CustomerIndex()
{
    delete ui;
}
