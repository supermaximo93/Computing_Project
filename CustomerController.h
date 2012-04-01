/*
 * CustomerController.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef CUSTOMERCONTROLLER_H
#define CUSTOMERCONTROLLER_H

#include "Database.h"

class QWidget;
class Customer;
class Job;

namespace CustomerController
{

// Displays a GUI showing a list of all the Customer records in the database
void Index(QWidget *caller);

// Displays a GUI giving the data of a particular Customer record in the database
void Show(int customerId, QWidget *caller);
void Show(Customer &customer, QWidget *caller);

// Displays a GUI which allows the user to input details about a Customer record to be added to the database
Customer New(QWidget *caller);

// Displays a GUI which allows the user to change the details of a Customer record in the database
void Edit(int customerId, QWidget *caller);
void Edit(Customer &customer, QWidget *caller);

// Adds a new Customer record to the database. Returns true on success
bool Create(Customer &customerAttributes, QWidget *caller);

// Updates a Customer record in the database. The passed Customer's ID must correspond to the ID of an existing customer
// in the database. Returns true on success
bool Update(const Customer &customer, QWidget *caller);

// Removes the Customer record whose ID matches customerId from the database
bool Destroy(int customerId, QWidget *caller);
// Removes the Customer record whose ID matches the passed Customer's ID. The passed Customer will be assigned to a
// blank customer and true will be returned on success
bool Destroy(Customer &customer, QWidget *caller);

// Returns the Customer whose ID matches customerId. If the Customer could not be found in the database, a blank
// Customer will be returned
Customer getCustomer(int customerId);

// Returns a memory managed pointer to a list of all of the Customer records in the database
Database<Customer>::recordListPtr getAllCustomers();

// Returns a memory managed pointer to a list of all of the Job records associated with the Customer of ID customerID
Database<Job>::recordListPtr getCustomerJobs(int customerId);

// Sorts the given list of Customer records by their surname field
void sortCustomersBySurname(Database<Customer>::recordList &customers, bool ascending = true);

}

#endif // CUSTOMERCONTROLLER_H
