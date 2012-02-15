/*
 * Databases.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "Databases.h"
#include "Database.h"
#include "Customer.h"
#include "Expense.h"
#include "Job.h"
#include "Part.h"
#include "Task.h"
#include "AssignOncePointer.h"

static Database<Customer> * customerDatabasePtr = NULL;
static Database<Expense> * expenseDatabasePtr = NULL;
static Database<Job> * jobDatabasePtr = NULL;
static Database<Part> * partDatabasePtr = NULL;
static Database<Task> * taskDatabasePtr = NULL;

static bool initialised = false;

#ifdef COMPILE_TESTS
void Databases::init(bool testing)
{
    if (initialised) return;
    customerDatabasePtr = new Database<Customer>(testing);
    expenseDatabasePtr = new Database<Expense>(testing);
    jobDatabasePtr = new Database<Job>(testing);
    partDatabasePtr = new Database<Part>(testing);
    taskDatabasePtr = new Database<Task>(testing);
    initialised = true;
}
#else
void Databases::init()
{
    if (initialised) return;
    customerDatabasePtr = new Database<Customer>;
    expenseDatabasePtr = new Database<Expense>;
    jobDatabasePtr = new Database<Job>;
    partDatabasePtr = new Database<Part>;
    taskDatabasePtr = new Database<Task>;
    initialised = true;
}
#endif

void Databases::finalise()
{
    if (!initialised) return;
    delete customerDatabasePtr;
    delete expenseDatabasePtr;
    delete jobDatabasePtr;
    delete partDatabasePtr;
    delete taskDatabasePtr;
    customerDatabasePtr = NULL;
    expenseDatabasePtr = NULL;
    jobDatabasePtr = NULL;
    partDatabasePtr = NULL;
    taskDatabasePtr = NULL;
    initialised = false;
}

Database<Customer> & Databases::customers()
{
    if (customerDatabasePtr == NULL) throw(std::runtime_error("Customer database not initialised"));
    return *customerDatabasePtr;
}

Database<Expense> & Databases::expenses()
{
    if (expenseDatabasePtr == NULL) throw(std::runtime_error("Expense database not initialised"));
    return *expenseDatabasePtr;
}

Database<Job> & Databases::jobs()
{
    if (jobDatabasePtr == NULL) throw(std::runtime_error("Job database not initialised"));
    return *jobDatabasePtr;
}

Database<Part> & Databases::parts()
{
    if (partDatabasePtr == NULL) throw(std::runtime_error("Part database not initialised"));
    return *partDatabasePtr;
}

Database<Task> & Databases::tasks()
{
    if (taskDatabasePtr == NULL) throw(std::runtime_error("Task database not initialised"));
    return *taskDatabasePtr;
}
