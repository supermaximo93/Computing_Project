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

#include "dialogs/customer/CustomerIndex.h"
#include "dialogs/customer/CustomerShow.h"
#include "dialogs/customer/CustomerForm.h"

void CustomerController::Index(QWidget *caller)
{
    Database<Customer>::recordListPtr customers;
    try { customers = Databases::customers().allRecords(); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    CustomerIndex view(*customers, caller);
    view.setModal(true);
    view.exec();
}

void CustomerController::Show(const int customerId, QWidget *caller)
{
    Customer customer;
    try { customer = Databases::customers().findRecord("id", customerId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (customer.null())
    {
        showErrorDialog(("Customer with ID " + toString(customerId) + " could not be found").c_str());
        return;
    }

    Show(customer, caller);
}

void CustomerController::Show(Customer &customer, QWidget *caller)
{
    Database<Job>::recordListPtr jobs;
    try { jobs = Databases::jobs().findRecords("customerId", customer.getId()); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    CustomerShow view(customer, *jobs, caller);
    view.setModal(true);
    view.exec();
}

Customer CustomerController::New(QWidget *caller)
{
    Customer customer;
    CustomerForm view(customer, caller);
    view.setModal(true);
    return (view.exec() == QDialog::Rejected ? Customer() : customer);
}

void CustomerController::Edit(const int customerId, QWidget *caller)
{
    Customer customer;
    try { customer = Databases::customers().findRecord("id", customerId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (customer.null())
    {
        showErrorDialog(("Customer with ID " + toString(customerId) + " could not be found").c_str());
        return;
    }

    Edit(customer, caller);
}

void CustomerController::Edit(Customer &customer, QWidget *caller)
{
    Customer tempCustomer = customer;
    CustomerForm view(tempCustomer, caller);
    view.setModal(true);
    if (view.exec() == CustomerForm::Accepted) customer = tempCustomer;
}

bool CustomerController::Create(Customer &customerAttributes, QWidget *)
{
    try { Databases::customers().addRecord(customerAttributes); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (customerAttributes.null())
    {
        showErrorDialog("There was an error with adding the customer to the database");
        return false;
    }

    return true;
}

bool CustomerController::Update(const Customer &customer, QWidget *)
{
    bool success = false;
    try { success = Databases::customers().updateRecord(customer); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with updating the customer in the database");
    return success;
}

bool CustomerController::Destroy(const int customerId, QWidget *)
{
    bool success = false;
    try { success = Databases::customers().deleteRecord(customerId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with removing the customer from the database");
    return success;
}

bool CustomerController::Destroy(Customer &customer, QWidget * caller)
{
    if (Destroy(customer.getId(), caller))
    {
        customer = Customer();
        return true;
    }

    return false;
}

Customer CustomerController::getCustomer(const int customerId)
{
    Customer customer;
    try { customer = Databases::customers().findRecord("id", customerId); }
    catch (const std::exception &e)
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
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<Customer>::recordListPtr(new Database<Customer>::recordList);
    }
    return customers;
}

Database<Job>::recordListPtr CustomerController::getCustomerJobs(const int customerId)
{
    Database<Job>::recordListPtr jobs;
    try { jobs = Databases::jobs().findRecords("customerId", customerId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<Job>::recordListPtr(new Database<Job>::recordList);
    }
    return jobs;
}
