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

namespace Databases
{

#ifdef COMPILE_TESTS
void init(bool testing = false);
#else
void init();
#endif

void finalise();

Database<Setting> & settings();
Database<Customer> & customers();
Database<Job> & jobs();
Database<Part> & parts();
Database<Task> & tasks();
Database<Expense> & expenses();

void reloadDatabaseFilenames();

}

#endif // DATABASES_H
