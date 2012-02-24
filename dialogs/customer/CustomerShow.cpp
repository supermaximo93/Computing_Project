/*
 * CustomerShow.cpp
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#include "CustomerShow.h"
#include "ui_CustomerShow.h"

#include "Customer.h"
#include "CustomerController.h"
#include "JobController.h"

CustomerShow::CustomerShow(Customer &customer, QWidget *parent)
    : QDialog(parent), ui(new Ui::CustomerShow), customer(customer)
{
    ui->setupUi(this);
}

CustomerShow::~CustomerShow()
{
    delete ui;
}

void CustomerShow::on_pushButton_ok_released()
{

}

void CustomerShow::on_pushButton_edit_released()
{

}

void CustomerShow::on_pushButton_delete_released()
{

}

void CustomerShow::on_listView_jobs_doubleClicked(const QModelIndex &index)
{

}

void CustomerShow::on_pushButton_addJob_released()
{

}
