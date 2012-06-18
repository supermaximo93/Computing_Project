/*
 * Databases.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <fstream>
using namespace std;

#include "Databases.h"
#include "Database.h"
#include "Setting.h"
#include "Customer.h"
#include "Job.h"
#include "Part.h"
#include "Task.h"
#include "Expense.h"
#include "VatRate.h"

#include "Globals.h"

#include "ExpenseController.h"
#include "CustomerController.h"
#include "JobController.h"

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

string sanitize(const string &str)
{
    string returnStr = str;

    size_t index = -1;
    while ((index = returnStr.find(L'£', index + 1)) != string::npos) returnStr.replace(index, 1, "&pound;");

    index = -1;
    while ((index = returnStr.find('&', index + 1)) != string::npos) returnStr.replace(index, 1, "&amp;");

    return returnStr;
}

void exportExpenseXml(ofstream &file)
{
    Database<Expense>::recordListPtr expenses = ExpenseController::getAllExpenses();
    string output = "<expenses>\n";
    file.write(output.c_str(), output.size());

    for (unsigned i = 0; i < expenses->size(); ++i)
    {
        Expense &expense = expenses->at(i);
        Date date(expense.getDate());

        output =
                "<expense>\n"
                "<day>" + toString(date.day) + "</day>\n"
                "<month>" + toString(date.month) + "</month>\n"
                "<year>" + toString(date.year) + "</year>\n"
                "<description>" + sanitize(expense.getDescription()) + "</description>\n"
                "<price>" + to2Dp(toString(expense.getPrice()).c_str()).toStdString() + "</price>\n"
                "<vat>" + to2Dp(toString(expense.getVat()).c_str()).toStdString() + "</vat>\n"
                "<category>" + lowerCase(expense.getTypeString()) + "</category>\n"
                "</expense>\n";
        file.write(output.c_str(), output.size());
    }

    output = "</expenses>\n";
    file.write(output.c_str(), output.size());
}

void exportPartsXml(ofstream &file, Job &job)
{
    Database<Part>::recordListPtr parts = JobController::getJobParts(job.getId());
    string output = "<materials>\n";
    file.write(output.c_str(), output.size());

    for (unsigned i = 0; i < parts->size(); ++i)
    {
        Part &part = parts->at(i);

        output =
                "<material>\n"
                "<name>" + sanitize(part.getName()) + "</name>\n"
                "<code>" + sanitize(part.getNumber()) + "</code>\n"
                "<price>" + to2Dp(toString(part.getPrice()).c_str()).toStdString() + "</price>\n"
                "<quantity>" + toString(part.getQuantity()) + "</quantity>\n"
                "</material>\n";
        file.write(output.c_str(), output.size());
    }

    output = "</materials>\n";
    file.write(output.c_str(), output.size());
}

void exportJobXml(ofstream &file, Customer &customer)
{
    Database<Job>::recordListPtr jobs = CustomerController::getCustomerJobs(customer.getId());
    string output = "<jobs>\n";
    file.write(output.c_str(), output.size());

    for (unsigned i = 0; i < jobs->size(); ++i)
    {
        Job &job = jobs->at(i);
        Date date(job.getDate());

        string completedString, invoiceIdCodeString;
        if ((job.getCompletionState() == Job::DONE_PAID) || (job.getCompletionState() == Job::DONE_UNPAID))
            completedString = "true";
        else completedString = "false";

        invoiceIdCodeString = toString(job.getId()) + toString(date.day) + toString(date.month) + toString(date.year);

        output =
                "<job>\n"
                "<day>" + toString(date.day) + "</day>\n"
                "<month>" + toString(date.month) + "</month>\n"
                "<year>" + toString(date.year) + "</year>\n"
                "<hour>" + toString(date.hour) + "</hour>\n"
                "<minute>" + toString(date.minute - (date.minute % 5)) + "</minute>\n"
                "<description>" + sanitize(job.getDescription()) + "</description>\n"
                "<labour_charge>" + to2Dp(toString(job.getLabourCharge()).c_str()).toStdString() + "</labour_charge>\n"
                "<completed>" + completedString + "</completed>\n"
                "<vat_rate>" + toString(Globals::vatRate(date)) + "</vat_rate>\n"
                "<invoice_id_code>" + invoiceIdCodeString + "</invoice_id_code>\n";
        file.write(output.c_str(), output.size());

        exportPartsXml(file, job);

        if (job.getCompletionState() == Job::DONE_PAID)
        {
            Date date(job.getPaymentDate());

            output =
                    "<payment>\n"
                    "<payment_method>" + sanitize(lowerCase(job.getPaymentMethodString())) + "</payment_method>\n"
                    "<day>" + toString(date.day)  + "</day>\n"
                    "<month>" + toString(date.month) + "</month>\n"
                    "<year>" + toString(date.year) + "</year>\n"
                    "</payment>\n";
            file.write(output.c_str(), output.size());
        }

        output = "</job>\n";
        file.write(output.c_str(), output.size());
    }

    output = "</jobs>\n";
    file.write(output.c_str(), output.size());
}

void exportCustomerXml(ofstream &file)
{
    Database<Customer>::recordListPtr customers = CustomerController::getAllCustomers();
    string output = "<customers>\n";
    file.write(output.c_str(), output.size());

    for (unsigned i = 0; i < customers->size(); ++i)
    {
        Customer &customer = customers->at(i);

        output =
                "<customer>\n"
                "<forename>" + sanitize(customer.getForename()) + "</forename>\n"
                "<surname>" + sanitize(customer.getSurname()) + "</surname>\n"
                "<address_line_1>" + sanitize(customer.getAddressLine1()) + "</address_line_1>\n"
                "<address_line_2>" + sanitize(customer.getAddressLine2()) + "</address_line_2>\n"
                "<town>" + sanitize(customer.getTown()) + "</town>\n"
                "<postcode>" + sanitize(customer.getPostcode()) + "</postcode>\n"
                "<home_phone_number>" + sanitize(customer.getHomePhoneNumber()) + "</home_phone_number>\n"
                "<mobile_phone_number>" + sanitize(customer.getMobilePhoneNumber()) + "</mobile_phone_number>\n"
                "<email_address>" + sanitize(customer.getEmailAddress()) + "</email_address>\n";
        file.write(output.c_str(), output.size());

        exportJobXml(file, customer);

        output = "</customer>\n";
        file.write(output.c_str(), output.size());
    }

    output = "</customers>\n";
    file.write(output.c_str(), output.size());
}

void exportBusinessXml(ofstream &file)
{
    string output =
            "<business>\n"
            "<name>Ian Foster Services</name>\n"
            "<currency>&amp;pound;</currency>\n"
            "<country>United Kingdom</country>\n"
            "<business_details></business_details>\n";
    file.write(output.c_str(), output.size());

    exportCustomerXml(file);
    exportExpenseXml(file);

    output = "</business>";
    file.write(output.c_str(), output.size());
}

void Databases::exportXml(const char *filename)
{
    if (!initialised) throw(std::runtime_error("Databases not initialised"));

    ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        cout << "File 'database.xml' could not be created" << endl;
        return;
    }

    string output = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file.write(output.c_str(), output.size());
    exportBusinessXml(file);
    file.close();
}
