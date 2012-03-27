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
    CustomerController::sortCustomersBySurname(customers);

    ui->listWidget->clear();

    char firstPart[64] = { 0 };
    for (unsigned i = 0; i < customers.size(); ++i)
    {
        // Display customer names with the format <surname>, <forename>
        strcpy(firstPart, customers[i].getSurname());
        strcat(firstPart, ", ");
        ui->listWidget->addItem(createFullName(firstPart, customers[i].getForename()));
    }
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
