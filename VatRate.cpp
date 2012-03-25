/*
 * VatRate.cpp
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#include <stdexcept>
#include <ctime>
using namespace std;

#include "VatRate.h"
#include "Globals.h"

const string VatRate::databaseFilename = "vatrates.dat";

int VatRate::size()
{
    return Record::size() + sizeof(double) + sizeof(time_t);
}

VatRate::VatRate(double value, time_t startDate)
    : value(value < 0.0 ? Globals::defaultVatRate : value), startDate(startDate == 0 ? time(NULL) : startDate) {}

VatRate::VatRate(const VatRate &vatRate)
{
    *this = vatRate;
}

VatRate::~VatRate() {}

void VatRate::operator =(const VatRate &vatRate)
{
    *((Record*)this) = (Record)vatRate;
    value = vatRate.value;
    startDate = vatRate.startDate;
}

void VatRate::writeToFile(std::fstream &file) const
{
    Record::writeToFile(file);
    file.write(reinterpret_cast<const char *>(&value), sizeof(value));
    file.write(reinterpret_cast<const char *>(&startDate), sizeof(startDate));
}

void VatRate::readFromFile(std::fstream &file)
{
    Record::readFromFile(file);
    file.read(reinterpret_cast<char *>(&value), sizeof(value));
    file.read(reinterpret_cast<char *>(&startDate), sizeof(startDate));
}

bool VatRate::hasMatchingField(const std::string &fieldName, const int searchTerm) const
{
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool VatRate::hasMatchingField(const std::string &fieldName, const double searchTerm) const
{
    if (fieldName == "value") return (value == searchTerm);
    return false;
}

bool VatRate::hasMatchingField(const std::string &fieldName, const time_t searchTerm) const
{
    if (fieldName == "startdate") return (startDate == searchTerm);
    return false;
}

bool VatRate::fieldCompare(const VatRate &rhs) const
{
    if (value != rhs.value) return false;
    if (startDate != rhs.startDate) return false;
    return true;
}

bool VatRate::completeCompare(const VatRate &rhs) const
{
    if (getId() != rhs.getId()) return false;
    return fieldCompare(rhs);
}

double VatRate::getValue() const
{
    return value;
}

void VatRate::setValue(const double newValue)
{
    string errorMessage;
    if (isValidValue(newValue, errorMessage)) value = newValue;
    else throw std::runtime_error(errorMessage);
}

bool VatRate::isValidValue(const double value, std::string &errorMessage)
{
    if (value >= 0.0) return true;

    errorMessage = "Value must be at least 0.00";
    return false;
}

time_t VatRate::getStartDate() const
{
    return startDate;
}

void VatRate::setStartDate(const time_t newStartDate)
{
    string errorMessage;
    if (isValidStartDate(newStartDate, errorMessage)) startDate = newStartDate;
    else throw std::runtime_error(errorMessage);
}

bool VatRate::isValidStartDate(const time_t, std::string &)
{
    return true;
}

void VatRate::validate() const
{
    string errorMessage;
    if (!isValidValue(value, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidStartDate(startDate, errorMessage)) throw std::runtime_error(errorMessage);
}
