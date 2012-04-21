/*
 * Job.cpp
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
#include <stdexcept>
#include <cmath>
#include <ctime>
#include <cstring>
using namespace std;

#include "Globals.h"
#include "Job.h"
#include "Utils.h"

const string Job::databaseFilename = "jobs.dat";
const int Job::minDescriptionLength = 0, Job::maxDescriptionLength = 256;

int Job::size() {
    return Record::size() + (sizeof(int) * 3) + (sizeof(unsigned) * 2) + maxDescriptionLength + 1 + (sizeof(double)* 2);
}

Job::Job(const int customerId, const time_t date, const char *newDescription, const double labourCharge,
         const int completionState, const int paymentMethod, const time_t paymentDate) :
    customerId(customerId), completionState(completionState), paymentMethod(paymentMethod),
    date(date == 0 ? time(NULL) : date), paymentDate(paymentDate == 0 ? this->date : paymentDate),
    labourCharge(labourCharge), vat(doubleTo2Dp(labourCharge * (Globals::vatRate(this->date) / 100.0)))
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
    paymentDate = job.paymentDate;
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
    file.write(reinterpret_cast<const char *>(&paymentDate), sizeof(paymentDate));
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
    file.read(reinterpret_cast<char *>(&paymentDate), sizeof(paymentDate));
}

bool Job::hasMatchingField(const string &fieldName, const int searchTerm) const
{
    if (fieldName == "customerid") return (customerId == searchTerm);
    if (fieldName == "completionstate") return (completionState == searchTerm);
    if (fieldName == "paymentmethod") return (paymentMethod == searchTerm);
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Job::hasMatchingField(const string &fieldName, const time_t searchTerm) const
{
    if (fieldName == "date") return (date == searchTerm);
    if (fieldName == "paymentdate") return (paymentDate == searchTerm);
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
    if (fieldName == "vat") return (vat == searchTerm);
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
    if (paymentDate != rhs.paymentDate) return false;
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
    string errorMessage;
    if (isValidCustomerId(newCustomerId, errorMessage)) customerId = newCustomerId;
    else throw std::runtime_error(errorMessage);
}

bool Job::isValidCustomerId(const int value, std::string &errorMessage)
{
    if (value >= 0) return true;

    errorMessage = "Customer ID must be at least 0";
    return false;
}

time_t Job::getDate() const
{
    return date;
}

void Job::setDate(const time_t newDate)
{
    string errorMessage;
    if (isValidDate(newDate, errorMessage)) date = newDate;
    else throw std::runtime_error(errorMessage);
}

bool Job::isValidDate(const time_t, std::string &)
{
    return true;
}

const char * Job::getDescription() const
{
    return description;
}

void Job::setDescription(const char *newDescription)
{
    string errorMessage;
    if (isValidDescription(newDescription, errorMessage)) strcpy(description, newDescription);
    else throw std::runtime_error(errorMessage);
}

bool Job::isValidDescription(const char *value, std::string &errorMessage)
{
    if ((value == NULL) || (value[0] == '\0'))
    {
        errorMessage = "Description must be present";
        return false;
    }

    return validateLengthOf(value, maxDescriptionLength, "Description", errorMessage);
}

double Job::getLabourCharge() const
{
    return labourCharge;
}

void Job::setLabourCharge(const double newLabourCharge)
{
    string errorMessage;
    if (isValidLabourCharge(newLabourCharge, errorMessage)) labourCharge = newLabourCharge;
    else throw std::runtime_error(errorMessage);
    vat = doubleTo2Dp(labourCharge * (Globals::vatRate(date) / 100.0));
}

bool Job::isValidLabourCharge(const double value, std::string &errorMessage)
{
    if (value >= 0.0) return true;

    errorMessage = "Labour charge must be at least 0.00";
    return false;
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
    string errorMessage;
    if (isValidCompletionState(newCompletionState, errorMessage)) completionState = newCompletionState;
    else throw std::runtime_error(errorMessage);
}

bool Job::isValidCompletionState(const int value, std::string &errorMessage)
{
    if (value >= 0) return true;

    errorMessage = "Completion state must be at least 0";
    return false;
}

int Job::getPaymentMethod() const
{
    return paymentMethod;
}

string Job::getPaymentMethodString() const
{
    switch (paymentMethod)
    {
    case CASH: return "Cash";
    case CHEQUE: return "Cheque";
    case CREDIT: return "Credit";
    case DEBIT: return "Debit";
    case BANK_TRANSFER: return "Bank Transfer";
    default: return "N/A";
    }
}

void Job::setPaymentMethod(const int newPaymentMethod)
{
    string errorMessage;
    if (isValidPaymentMethod(newPaymentMethod, errorMessage)) paymentMethod = newPaymentMethod;
    else throw std::runtime_error(errorMessage);
}

bool Job::isValidPaymentMethod(const int value, std::string &errorMessage)
{
    if (value >= 0) return true;

    errorMessage = "Payment method must be at least 0";
    return false;
}

time_t Job::getPaymentDate() const
{
    return paymentDate;
}

void Job::setPaymentDate(const time_t newPaymentDate)
{
    string errorMessage;
    if (isValidPaymentDate(newPaymentDate, errorMessage))
    {
        if ((completionState == DONE_PAID) && (newPaymentDate < date))
            throw std::runtime_error("Payment time/date must be after the job time/date");
        paymentDate = newPaymentDate;
    }
    else throw std::runtime_error(errorMessage);
}

bool Job::isValidPaymentDate(const time_t, std::string &)
{
    return true;
}

void Job::validate() const
{
    string errorMessage;
    if (!isValidCustomerId(customerId, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidDate(date, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidDescription(description, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidLabourCharge(labourCharge, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidCompletionState(completionState, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidPaymentMethod(paymentMethod, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidPaymentDate(paymentDate, errorMessage)) throw std::runtime_error(errorMessage);
}
