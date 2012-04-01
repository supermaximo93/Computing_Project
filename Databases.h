/*
 * Databases.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef DATABASES_H
#define DATABASES_H

template<class recordType> class Database;

class Setting;
class Customer;
class Job;
class Part;
class Task;
class Expense;
class VatRate;

namespace Databases
{

// Loads up the databases ready for use
#ifdef COMPILE_TESTS
void init(bool testing = false);
#else
void init();
#endif

// Closes the databases
void finalise();

// Returns a reference to a particular database. An exception is thrown if the databases have not been initialised
Database<Setting> & settings();
Database<Customer> & customers();
Database<Job> & jobs();
Database<Part> & parts();
Database<Task> & tasks();
Database<Expense> & expenses();
Database<VatRate> & vatRates();

// Forces the databases to reload their database file names and move the files if necessary
void reloadDatabaseFilenames();

}

#endif // DATABASES_H
