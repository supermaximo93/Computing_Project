/*
 * Databases.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef DATABASES_H
#define DATABASES_H

template<class recordType> class Database;

class Customer;
class Job;
class Part;
class Task;
class Expense;
class Setting;

namespace Databases
{

#ifdef COMPILE_TESTS
void init(bool testing = false);
#else
void init();
#endif

void finalise();

Database<Customer> & customers();
Database<Job> & jobs();
Database<Part> & parts();
Database<Task> & tasks();
Database<Expense> & expenses();
Database<Setting> & settings();

}

#endif // DATABASES_H
