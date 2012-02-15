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

namespace Databases
{

#ifdef COMPILE_TESTS
void init(bool testing = false);
#else
void init();
#endif

void finalise();

Database<Customer> & customers();
Database<Expense> & expenses();
Database<Job> & jobs();
Database<Part> & parts();
Database<Task> & tasks();

}

#endif // DATABASES_H
