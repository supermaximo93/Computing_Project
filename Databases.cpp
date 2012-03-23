/*
 * Databases.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "Databases.h"
#include "Database.h"
#include "Customer.h"
#include "Job.h"
#include "Part.h"
#include "Task.h"
#include "Expense.h"
#include "Setting.h"

static Database<Customer> *customerDatabasePtr = NULL;
static Database<Job> *jobDatabasePtr = NULL;
static Database<Part> *partDatabasePtr = NULL;
static Database<Task> *taskDatabasePtr = NULL;
static Database<Expense> *expenseDatabasePtr = NULL;
static Database<Setting> *settingDatabasePtr = NULL;

static bool initialised = false;

#ifdef COMPILE_TESTS
void Databases::init(bool testing)
{
    if (initialised) return;
    customerDatabasePtr = new Database<Customer>(testing);
    jobDatabasePtr = new Database<Job>(testing);
    partDatabasePtr = new Database<Part>(testing);
    taskDatabasePtr = new Database<Task>(testing);
    expenseDatabasePtr = new Database<Expense>(testing);
    settingDatabasePtr = new Database<Setting>(testing);
    initialised = true;
}
#else
void Databases::init()
{
    if (initialised) return;
    customerDatabasePtr = new Database<Customer>;
    jobDatabasePtr = new Database<Job>;
    partDatabasePtr = new Database<Part>;
    taskDatabasePtr = new Database<Task>;
    expenseDatabasePtr = new Database<Expense>;
    settingDatabasePtr = new Database<Setting>
    initialised = true;
}
#endif

void Databases::finalise()
{
    if (!initialised) return;
    delete customerDatabasePtr;
    delete jobDatabasePtr;
    delete partDatabasePtr;
    delete taskDatabasePtr;
    delete expenseDatabasePtr;
    delete settingDatabasePtr;
    customerDatabasePtr = NULL;
    jobDatabasePtr = NULL;
    partDatabasePtr = NULL;
    taskDatabasePtr = NULL;
    expenseDatabasePtr = NULL;
    settingDatabasePtr = NULL;
    initialised = false;
}

Database<Customer> & Databases::customers()
{
    if (customerDatabasePtr == NULL) throw(std::runtime_error("Customer database not initialised"));
    return *customerDatabasePtr;
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

Database<Expense> & Databases::expenses()
{
    if (expenseDatabasePtr == NULL) throw(std::runtime_error("Expense database not initialised"));
    return *expenseDatabasePtr;
}

Database<Setting> & Databases::settings()
{
    if (settingDatabasePtr == NULL) throw(std::runtime_error("Setting database not initialised"));
    return *settingDatabasePtr;
}
