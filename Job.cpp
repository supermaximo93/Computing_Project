/*
 * Job.cpp
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
#include <cmath>
#include <ctime>
#include <cstring>
using namespace std;

#include "Globals.h"
#include "Job.h"

int Job::size() {
    return Record::size() + (sizeof(int) * 3) + sizeof(time_t) + maxDescriptionLength + 1 + (sizeof(double) * 2);
}

const string Job::databaseFilename = "jobs.dat";

Job::Job(const int customerId, const time_t date, const char *newDescription, const double labourCharge,
         const int completionState, const int paymentMethod) :
    customerId(customerId), completionState(completionState), paymentMethod(paymentMethod),
    date(date == 0 ? time(NULL) : date), labourCharge(labourCharge), vat(labourCharge * Globals::vatRate(this->date))
{
    description = new char[maxDescriptionLength + 1];
    strcpy(description, newDescription);
}

Job::Job(const Job &job)
{
    description = new char[maxDescriptionLength + 1];
    (*this) = job;
}

Job::~Job()
{
    delete[] description;
}

void Job::operator =(const Job &job)
{
    *((Record*)this) = (Record)job;

    customerId = job.customerId;
    strcpy(description, job.description);
    completionState = job.completionState;
    paymentMethod = job.paymentMethod;
    date = job.date;
    labourCharge = job.labourCharge;
    vat = job.vat;
}

void Job::writeToFile(fstream &file) const
{
    Record::writeToFile(file);
    file.write(reinterpret_cast<const char *>(&customerId), sizeof(customerId));
    file.write(reinterpret_cast<const char *>(&date), sizeof(date));
    file.write(description, maxDescriptionLength + 1);
    file.write(reinterpret_cast<const char *>(&labourCharge), sizeof(labourCharge));
    file.write(reinterpret_cast<const char *>(&vat), sizeof(vat));
    file.write(reinterpret_cast<const char *>(&completionState), sizeof(completionState));
    file.write(reinterpret_cast<const char *>(&paymentMethod), sizeof(paymentMethod));
}

void Job::readFromFile(fstream &file)
{
    Record::readFromFile(file);
    file.read(reinterpret_cast<char *>(&customerId), sizeof(customerId));
    file.read(reinterpret_cast<char *>(&date), sizeof(date));
    file.read(description, maxDescriptionLength + 1);
    file.read(reinterpret_cast<char *>(&labourCharge), sizeof(labourCharge));
    file.read(reinterpret_cast<char *>(&vat), sizeof(vat));
    file.read(reinterpret_cast<char *>(&completionState), sizeof(completionState));
    file.read(reinterpret_cast<char *>(&paymentMethod), sizeof(paymentMethod));
}

bool Job::hasMatchingField(const string &fieldName, const int searchTerm) const
{
    if (fieldName == "customerid") return (customerId == searchTerm);
    else if (fieldName == "completionstate") return (completionState == searchTerm);
    else if (fieldName == "paymentmethod") return (paymentMethod == searchTerm);
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Job::hasMatchingField(const string &fieldName, const time_t searchTerm) const
{
    if (fieldName == "date") return (date == searchTerm);
    return false;
}

bool Job::hasMatchingField(const string &fieldName, const char * searchTerm) const
{
    if (fieldName == "description") return (strcmp(description, searchTerm) == 0);
    return false;
}

bool Job::hasMatchingField(const string &fieldName, const double searchTerm) const
{
    if (fieldName == "labourcharge") return (labourCharge == searchTerm);
    else if (fieldName == "vat") return (vat == searchTerm);
    return false;
}

bool Job::fieldCompare(const Job &rhs) const
{
    if (customerId != rhs.customerId) return false;
    if (date != rhs.date) return false;
    if (fabs(labourCharge - rhs.labourCharge) > 0.00001) return false;
    if (fabs(vat - rhs.vat) > 0.00001) return false;
    if (completionState != rhs.completionState) return false;
    if (paymentMethod != rhs.paymentMethod) return false;
    if (strcmp(description, rhs.description) != 0) return false;
    return true;
}

bool Job::completeCompare(const Job &rhs) const
{
    if (getId() != rhs.getId()) return false;
    return fieldCompare(rhs);
}

int Job::getCustomerId() const
{
    return customerId;
}

void Job::setCustomerId(const int newCustomerId)
{
    customerId = newCustomerId;
}

time_t Job::getDate() const
{
    return date;
}

void Job::setDate(const time_t newDate)
{
    date = newDate;
}

const char * Job::getDescription() const
{
    return description;
}

void Job::setDescription(const char *newDescription)
{
    strcpy(description, newDescription);
}

double Job::getLabourCharge() const
{
    return labourCharge;
}

void Job::setLabourCharge(const double newLabourCharge)
{
    labourCharge = newLabourCharge;
    vat = labourCharge * (Globals::vatRate(date) / 100.0);
}

double Job::getVat() const
{
    return vat;
}

int Job::getCompletionState() const
{
    return completionState;
}

void Job::setCompletionState(const int newCompletionState)
{
    completionState = newCompletionState;
}

int Job::getPaymentMethod() const
{
    return paymentMethod;
}

void Job::setPaymentMethod(const int newPaymentMethod)
{
    paymentMethod = newPaymentMethod;
}
