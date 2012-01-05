/*
 * Job.cpp
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
using namespace std;

#include "Globals.h"
#include "Job.h"

const string Job::databaseFilename = "jobs.dat";

Job::Job(const int customerId, const time_t date, const float labourCharge, const int completionState,
         const int paymentMethod) :
    customerId(customerId), completionState(completionState), paymentMethod(paymentMethod), date(date),
    labourCharge(labourCharge), vat(labourCharge*Globals::vatRate) {}

void Job::writeToFile(fstream & file) const {
    Record::writeToFile(file);
    file.write(reinterpret_cast<const char *>(&customerId), sizeof(customerId));
    file.write(reinterpret_cast<const char *>(&date), sizeof(date));
    file.write(reinterpret_cast<const char *>(&labourCharge), sizeof(labourCharge));
    file.write(reinterpret_cast<const char *>(&vat), sizeof(vat));
    file.write(reinterpret_cast<const char *>(&completionState), sizeof(completionState));
    file.write(reinterpret_cast<const char *>(&paymentMethod), sizeof(paymentMethod));
}

void Job::readFromFile(fstream & file) {
    Record::readFromFile(file);
    file.read(reinterpret_cast<char *>(&customerId), sizeof(customerId));
    file.read(reinterpret_cast<char *>(&date), sizeof(date));
    file.read(reinterpret_cast<char *>(&labourCharge), sizeof(labourCharge));
    file.read(reinterpret_cast<char *>(&vat), sizeof(vat));
    file.read(reinterpret_cast<char *>(&completionState), sizeof(completionState));
    file.read(reinterpret_cast<char *>(&paymentMethod), sizeof(paymentMethod));
}

int Job::size() {
    return Record::size()+(sizeof(int)*3)+sizeof(time_t)+(sizeof(float)*2);
}

bool Job::hasMatchingField(const string & fieldName, const int searchTerm) const {
    if (fieldName == "customerid") return (customerId == searchTerm);
    else if (fieldName == "completionstate") return (completionState == searchTerm);
    else if (fieldName == "paymentmethod") return (paymentMethod == searchTerm);
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Job::hasMatchingField(const string & fieldName, const time_t searchTerm) const {
    if (fieldName == "date") return (date == searchTerm);
    return false;
}

bool Job::hasMatchingField(const string & fieldName, const float searchTerm) const {
    if (fieldName == "labourcharge") return (labourCharge == searchTerm);
    else if (fieldName == "vat") return (vat == searchTerm);
    return false;
}

int Job::getCustomerId() const {
    return customerId;
}

void Job::setCustomerId(const int newCustomerId) {
    customerId = newCustomerId;
}

time_t Job::getDate() const {
    return date;
}

void Job::setDate(const time_t newDate) {
    date = newDate;
}

float Job::getLabourCharge() const {
    return labourCharge;
}

void Job::setLabourCharge(const float newLabourCharge) {
    labourCharge = newLabourCharge;
    vat = labourCharge*Globals::vatRate;
}

float Job::getVat() const {
    return vat;
}

int Job::getCompletionState() const {
    return completionState;
}

void Job::setCompletionState(const int newCompletionState) {
    completionState = newCompletionState;
}

int Job::getPaymentMethod() const {
    return paymentMethod;
}

void Job::setPaymentMethod(const int newPaymentMethod) {
    paymentMethod = newPaymentMethod;
}
