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
    // Attempt to get a list of all the Customer records in the database.
    // If there is an error then show it in a dialog box and terminate the function
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
    // Attempt to get the Customer record with ID customerId. If there are any problems show a dialog box and finish
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

    // Pass the Customer to the overloaded Show function to keep code DRY
    Show(customer, caller);
}

void CustomerController::Show(Customer &customer, QWidget *caller)
{
    // Get the Customer records associated jobs (if any), showing a dialog if there is an error
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
    // Create a blank customer for the Customer Form to use. If the 'cancel' button is clicked then return a blank
    // Customer record, otherwise return the Customer record with its attributes
    Customer customer;
    CustomerForm view(customer, caller);
    return (view.exec() == QDialog::Rejected ? Customer() : customer);
}

void CustomerController::Edit(const int customerId, QWidget *caller)
{
    // Find the Customer record with ID customerId, showing a dialog if there are any errors
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
    // Pass a copy of the customer to the Customer Form just in case the user presses the 'cancel' button.
    // If the edit was successful, then copy the attributes of the updated Customer copy to the passed Customer
    // If not, then the passed customer will remain unchanged
    Customer tempCustomer = customer;
    CustomerForm view(tempCustomer, caller);
    if (view.exec() == CustomerForm::Accepted) customer = tempCustomer;
}

bool CustomerController::Create(Customer &customerAttributes, QWidget *)
{
    // Add a new record with the given attributes to the database. If there were any errors then show them in a dialog
    // and return false (i.e. unsuccessful)
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
    // Database::updateRecord returns a boolean value so keep track of this value and use it to see if the update
    // operation was successful. If there are any problems show a dialog
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
        // Database::deleteRecord returns a boolean value, so keep track of it and use it to see if the operation was
        // successful
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
    // Destroy the Customer record with the ID of the passed Customer. If the operation was successful then clear the
    // attributes of the passed Customer, which makes it 'null'
    if (Destroy(customer.getId(), caller))
    {
        customer = Customer();
        return true;
    }

    return false;
}

Customer CustomerController::getCustomer(const int customerId)
{
    // Get the customer with the ID customerId from the database, reporting any errors to the user. If there was an
    // error, then return a blank Customer record
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
    // Get a memory managed pointer to a list of all the customers in the database, reporting any errors to the user.
    // If there was an error, then return an empty list
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
    // Get all of the jobs whose customerId field matches the value of customerId. If there was an error, report it to
    // the user and return an empty list
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
            // Memory to store copies of the surnames of each customer. We need copies because the surnames need to be
            // converted into lowercase without affecting the original Customer records
            static char surname1[64] = { 0 }, surname2[64] = { 0 };
            static const int aTakeA = 'a' - 'A'; // for lowercase conversion

            // Copy over the surnames
            strcpy(surname1, customer1.getSurname());
            strcpy(surname2, customer2.getSurname());
            const size_t length1 = strlen(surname1), length2 = strlen(surname2);

            // Loop through the characters of each surname, converting them to lowercase if necessary, and comparing
            // them with each other. If any surname character not equal to the other, then return whether the surname
            // comes 'before' (-1) or 'after' (1)
            for (unsigned i = 0; (i < length1) && (i < length2); ++i)
            {
                if (surname1[i] >= 'a') surname1[i] -= aTakeA;
                if (surname2[i] >= 'a') surname2[i] -= aTakeA;

                if (surname1[i] < surname2[i]) return -1;
                if (surname1[i] > surname2[i]) return 1;
            }

            // No differences yet so compare the lengths. The shorter surname comes 'before'
            if (length1 < length2) return -1;
            if (length1 > length2) return 1;

            // Surnames are the same, so compare the forenames
            return forenameCompare(customer1, customer2, true);
        }

        static int surnameCompareDec(const Customer &customer1, const Customer &customer2)
        {
            // Same as above but surnames that have characters later in the alphabet or are longer come 'before'

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
            // Same as above but this time a boolean can be passed that decides whether the list is ascending or
            // decending, which decides what forename comes 'before' the other

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

    // Sort the passed list of customers, passing the relevant comparison function depending on whether the list is
    // to be sorted with surnames ascending or decending
    Databases::customers().sortRecords(customers, 0, customers.size() - 1, ascending ?
                                           NestedFunctions::surnameCompareAsc : NestedFunctions::surnameCompareDec);
}
