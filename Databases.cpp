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

static AssignOncePointer< Database<Customer> > customerDatabasePtr;
static AssignOncePointer< Database<Expense> > expenseDatabasePtr;
static AssignOncePointer< Database<Job> > jobDatabasePtr;
static AssignOncePointer< Database<Part> > partDatabasePtr;
static AssignOncePointer< Database<Task> > taskDatabasePtr;

static bool initialised = false;

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

Database<Customer> & Databases::customers()
{
    return *customerDatabasePtr;
}

Database<Expense> & Databases::expenses()
{
    return *expenseDatabasePtr;
}

Database<Job> & Databases::jobs()
{
    return *jobDatabasePtr;
}

Database<Part> & Databases::parts()
{
    return *partDatabasePtr;
}

Database<Task> & Databases::tasks()
{
    return *taskDatabasePtr;
}
