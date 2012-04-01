/*
 * Databases.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "Databases.h"
#include "Database.h"
#include "Setting.h"
#include "Customer.h"
#include "Job.h"
#include "Part.h"
#include "Task.h"
#include "Expense.h"
#include "VatRate.h"

Database<Setting> *settingDatabasePtr = NULL; // Not static so it can be accessed by Database with extern
static Database<Customer> *customerDatabasePtr = NULL;
static Database<Job> *jobDatabasePtr = NULL;
static Database<Part> *partDatabasePtr = NULL;
static Database<Task> *taskDatabasePtr = NULL;
static Database<Expense> *expenseDatabasePtr = NULL;
static Database<VatRate> *vatRateDatabasePtr = NULL;

static bool initialised = false;

#ifdef COMPILE_TESTS
static bool testing;

void Databases::init(bool testing)
{
    // If the instances of Database have not already been created, create them
    if (initialised) return;
    settingDatabasePtr = new Database<Setting>(testing);
    customerDatabasePtr = new Database<Customer>(testing);
    jobDatabasePtr = new Database<Job>(testing);
    partDatabasePtr = new Database<Part>(testing);
    taskDatabasePtr = new Database<Task>(testing);
    expenseDatabasePtr = new Database<Expense>(testing);
    vatRateDatabasePtr = new Database<VatRate>(testing);
    initialised = true;
    ::testing = testing;
}
#else
void Databases::init()
{
    // If the instances of Database have not already been created, create them
    if (initialised) return;
    settingDatabasePtr = new Database<Setting>;
    customerDatabasePtr = new Database<Customer>;
    jobDatabasePtr = new Database<Job>;
    partDatabasePtr = new Database<Part>;
    taskDatabasePtr = new Database<Task>;
    expenseDatabasePtr = new Database<Expense>;
    vatRateDatabasePtr = new Database<VatRate>;
    initialised = true;
}
#endif

void Databases::finalise()
{
    // If the databases have been initialised, delete the instances of the databases
    if (!initialised) return;
    delete customerDatabasePtr;
    delete jobDatabasePtr;
    delete partDatabasePtr;
    delete taskDatabasePtr;
    delete expenseDatabasePtr;
    delete vatRateDatabasePtr;
    delete settingDatabasePtr;
    customerDatabasePtr = NULL;
    jobDatabasePtr = NULL;
    partDatabasePtr = NULL;
    taskDatabasePtr = NULL;
    expenseDatabasePtr = NULL;
    vatRateDatabasePtr = NULL;
    settingDatabasePtr = NULL;
    initialised = false;
}

Database<Setting> & Databases::settings()
{
    // If the database pointer is null, then throw an exception, otherwise return a reference to the database
    if (settingDatabasePtr == NULL) throw(std::runtime_error("Setting database not initialised"));
    return *settingDatabasePtr;
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

Database<VatRate> & Databases::vatRates()
{
    if (vatRateDatabasePtr == NULL) throw(std::runtime_error("VAT rate database not initialised"));
    return *vatRateDatabasePtr;
}

void Databases::reloadDatabaseFilenames()
{
    // If the databases haven't been initialised, throw an exception, otherwise force the databases to reload the
    // database file names and move the files if necessary
    if (!initialised) throw(std::runtime_error("Databases not initialised"));

#ifdef COMPILE_TESTS
    settingDatabasePtr->reloadFilename(true, testing);
    customerDatabasePtr->reloadFilename(true, testing);
    jobDatabasePtr->reloadFilename(true, testing);
    partDatabasePtr->reloadFilename(true, testing);
    taskDatabasePtr->reloadFilename(true, testing);
    expenseDatabasePtr->reloadFilename(true, testing);
    vatRateDatabasePtr->reloadFilename(true, testing);
#else
    settingDatabasePtr->reloadFilename(true);
    customerDatabasePtr->reloadFilename(true);
    jobDatabasePtr->reloadFilename(true);
    partDatabasePtr->reloadFilename(true);
    taskDatabasePtr->reloadFilename(true);
    expenseDatabasePtr->reloadFilename(true);
    vatRateDatabasePtr->reloadFilename(true);
#endif
}
