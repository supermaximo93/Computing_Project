/*
 * CustomerController.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

using namespace std;

#include "CustomerController.h"
#include "JobController.h"
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

    CustomerIndex(*customers, caller).exec();
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

    CustomerShow(customer, *jobs, caller).exec();
}

Customer CustomerController::New(QWidget *caller)
{
    Customer customer;
    CustomerForm view(customer, caller);
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

bool CustomerController::Destroy(const int customerId, QWidget * caller)
{
    // Delete all associated jobs first, recording any errors
    vector<string> errors;
    Database<Job>::recordListPtr jobs = Databases::jobs().findRecords("customerId", customerId);
    errors.reserve(jobs->size());
    for (unsigned i = 0; i < jobs->size(); ++i) JobController::Destroy(jobs->at(i), caller);

    // If there were errors, report them
    if (errors.size() > 0) showErrorDialog(errors);
    else // otherwise try and delete the customer record
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

    return false;
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

void CustomerController::sortCustomersBySurname(Database<Customer>::recordList &customers, bool ascending)
{
    struct NestedFunctions
    {
        // Not DRY but can't be done any other way
        static int surnameCompareAsc(const Customer &customer1, const Customer &customer2)
        {
            static char surname1[64] = { 0 }, surname2[64] = { 0 };
            static const int aTakeA = 'a' - 'A'; // for lowercase conversion

            strcpy(surname1, customer1.getSurname());
            strcpy(surname2, customer2.getSurname());
            const size_t length1 = strlen(surname1), length2 = strlen(surname2);

            for (unsigned i = 0; (i < length1) && (i < length2); ++i)
            {
                // convert to lowercase first
                if (surname1[i] >= 'a') surname1[i] -= aTakeA;
                if (surname2[i] >= 'a') surname2[i] -= aTakeA;

                if (surname1[i] < surname2[i]) return -1;
                if (surname1[i] > surname2[i]) return 1;
            }

            if (length1 < length2) return -1;
            if (length1 > length2) return 1;

            return forenameCompare(customer1, customer2, true);
        }

        static int surnameCompareDec(const Customer &customer1, const Customer &customer2)
        {
            static char surname1[64] = { 0 }, surname2[64] = { 0 };
            static const int aTakeA = 'a' - 'A';

            strcpy(surname1, customer1.getSurname());
            strcpy(surname2, customer2.getSurname());
            const size_t length1 = strlen(surname1), length2 = strlen(surname2);

            for (unsigned i = 0; (i < length1) && (i < length2); ++i)
            {
                if (surname1[i] >= 'a') surname1[i] -= aTakeA;
                if (surname2[i] >= 'a') surname2[i] -= aTakeA;

                if (surname1[i] < surname2[i]) return 1;
                if (surname1[i] > surname2[i]) return -1;
            }

            if (length1 < length2) return 1;
            if (length1 > length2) return -1;

            return forenameCompare(customer1, customer2, false);
        }

        static int forenameCompare(const Customer &customer1, const Customer &customer2, const bool ascending)
        {
            static char forename1[64] = { 0 }, forename2[64] = { 0 };
            static const int aTakeA = 'a' - 'A';

            strcpy(forename1, customer1.getForename());
            strcpy(forename2, customer2.getForename());
            const size_t length1 = strlen(forename1), length2 = strlen(forename2);

            for (unsigned i = 0; (i < length1) && (i < length2); ++i)
            {
                if (forename1[i] >= 'a') forename1[i] -= aTakeA;
                if (forename2[i] >= 'a') forename2[i] -= aTakeA;

                if (ascending)
                {
                    if (forename1[i] < forename2[i]) return -1;
                    if (forename1[i] > forename2[i]) return 1;
                }
                else
                {
                    if (forename1[i] < forename2[i]) return 1;
                    if (forename1[i] > forename2[i]) return -1;
                }
            }

            if (ascending)
            {
                if (length1 < length2) return -1;
                if (length1 > length2) return 1;
            }
            else
            {
                if (length1 < length2) return 1;
                if (length1 > length2) return -1;
            }
            return 0;
        }
    };

    Databases::customers().sortRecords(customers, 0, customers.size() - 1, ascending ?
                                           NestedFunctions::surnameCompareAsc : NestedFunctions::surnameCompareDec);
}
