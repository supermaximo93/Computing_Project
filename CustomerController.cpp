/*
 * CustomerController.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "CustomerController.h"
#include "Databases.h"
#include "Customer.h"
#include "Job.h"

void CustomerController::Index(QWidget * caller)
{

}

void CustomerController::Show(int customerId, QWidget * caller)
{

}

void CustomerController::Show(Customer & customer, QWidget * caller)
{

}

Customer CustomerController::New(QWidget * caller)
{
    return Customer();
}

void CustomerController::Edit(int customerId, QWidget * caller)
{

}

void CustomerController::Edit(Customer & customer, QWidget * caller)
{

}

bool CustomerController::Create(Customer & customerAttributes, QWidget * caller)
{

}

bool CustomerController::Update(const Customer & customer, QWidget * caller)
{

}

bool CustomerController::Destroy(int customerId, QWidget * caller)
{

}

bool CustomerController::Destroy(Customer & customer, QWidget * caller)
{

}

Customer CustomerController::getCustomer(int customerId)
{
    Customer customer;
    try { customer = Databases::customers().findRecord("id", customerId); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return Customer();
    }
    return customer;
}

Database<Customer>::recordListPtr CustomerController::getAllCustomers()
{
    Database<Customer>::recordListPtr customers;
    try { customers = Databases::customers().allRecords(); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return Database<Customer>::recordListPtr(new Database<Customer>::recordList);
    }
    return customers;
}

Database<Job>::recordListPtr CustomerController::getCustomerJobs(int customerId)
{
    return Database<Job>::recordListPtr(new Database<Job>::recordList);
}
