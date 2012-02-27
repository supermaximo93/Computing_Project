/*
 * CustomerIndex.cpp
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#include "CustomerIndex.h"
#include "ui_CustomerIndex.h"

#include "Customer.h"
#include "CustomerController.h"
#include "Utils.h"

CustomerIndex::CustomerIndex(Database<Customer>::recordList &customers, QWidget *parent)
    : QDialog(parent), ui(new Ui::CustomerIndex), customers(customers)
{
    ui->setupUi(this);

    updateView();
}

CustomerIndex::~CustomerIndex()
{
    delete ui;
}

void CustomerIndex::updateView()
{
    ui->listWidget->clear();
    for (unsigned i = 0; i < customers.size(); ++i)
        ui->listWidget->addItem(createFullName(customers[i].getForename(), customers[i].getSurname()));
}

void CustomerIndex::on_pushButton_ok_released()
{
    done(Accepted);
}

void CustomerIndex::on_pushButton_addNewCustomer_released()
{
    Customer customer = CustomerController::New(this);
    if (!customer.null())
    {
        customers.push_back(customer);
        updateView();
    }
}

void CustomerIndex::on_listWidget_doubleClicked(const QModelIndex &index)
{
    Customer & customer = customers[index.row()];
    CustomerController::Show(customer, this);
    if (customer.null()) customers.erase(customers.begin() + index.row());
    updateView();
}
