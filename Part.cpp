/*
 * Part.cpp
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
#include <stdexcept>
#include <cmath>
#include <cstring>
using namespace std;

#include "Globals.h"
#include "Part.h"
#include "Utils.h"

const string Part::databaseFilename = "parts.dat";
const int
Part::minNameLength   = 3, Part::maxNameLength   = 24,
Part::minNumberLength = 0, Part::maxNumberLength = 32;

int Part::size()
{
    return Record::size() + (sizeof(int) * 2) + maxNameLength + maxNumberLength + 2 + (sizeof(double) * 2);
}

Part::Part(const int jobId, const char *newName, const char *newNumber, const double price, const double vatRate,
           const int quantity)
    : pending(false), jobId(jobId), quantity(quantity), price(price),
      vatRate(vatRate < 0.0 ? Globals::vatRate(time(NULL)) : vatRate)
{
    name = new char[maxNameLength + 1];
    number = new char[maxNumberLength + 1];
    strcpy(name, newName);
    strcpy(number, newNumber);
}

Part::Part(const Part &part)
{
    name = new char[maxNameLength + 1];
    number = new char[maxNumberLength + 1];

    (*this) = part;
}

Part::~Part()
{
    delete[] name;
    delete[] number;
}

void Part::operator =(const Part &part)
{
    pending = part.pending;
    *((Record*)this) = (Record)part;
    jobId = part.jobId;
    strcpy(name, part.name);
    strcpy(number, part.number);
    price = part.price;
    vatRate = part.vatRate;
    quantity = part.quantity;
}

void Part::writeToFile(fstream &file) const
{
    Record::writeToFile(file);
    file.write(reinterpret_cast<const char *>(&jobId), sizeof(jobId));
    file.write(name, maxNameLength + 1);
    file.write(number, maxNumberLength + 1);
    file.write(reinterpret_cast<const char *>(&price), sizeof(price));
    file.write(reinterpret_cast<const char *>(&vatRate), sizeof(vatRate));
    file.write(reinterpret_cast<const char *>(&quantity), sizeof(quantity));
}

void Part::readFromFile(fstream &file)
{
    Record::readFromFile(file);
    file.read(reinterpret_cast<char *>(&jobId), sizeof(jobId));
    file.read(name, maxNameLength + 1);
    file.read(number, maxNumberLength + 1);
    file.read(reinterpret_cast<char *>(&price), sizeof(price));
    file.read(reinterpret_cast<char *>(&vatRate), sizeof(vatRate));
    file.read(reinterpret_cast<char *>(&quantity), sizeof(quantity));
}

bool Part::hasMatchingField(const string &fieldName, const int searchTerm) const
{
    if (fieldName == "jobid") return (jobId == searchTerm);
    else if (fieldName == "quantity") return (quantity == searchTerm);
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Part::hasMatchingField(const string &fieldName, const char *searchTerm) const
{
    if (fieldName == "name") return (strcmp(name, searchTerm) == 0);
    else if (fieldName == "number") return (strcmp(number, searchTerm) == 0);
    return false;
}

bool Part::hasMatchingField(const string &fieldName, const double searchTerm) const
{
    if (fieldName == "price") return (price == searchTerm);
    else if (fieldName == "vatRate") return (vatRate == searchTerm);
    return false;
}

bool Part::fieldCompare(const Part &rhs) const
{
    if (jobId != rhs.jobId) return false;
    if (strcmp(name, rhs.name) != 0) return false;
    if (strcmp(number, rhs.number) != 0) return false;
    if (fabs(price - rhs.price) > 0.00001) return false;
    if (fabs(vatRate - rhs.vatRate) > 0.00001) return false;
    if (quantity != rhs.quantity) return false;
    return true;
}

bool Part::completeCompare(const Part &rhs) const
{
    if (getId() != rhs.getId()) return false;
    return fieldCompare(rhs);
}

int Part::getJobId() const
{
    return jobId;
}

void Part::setJobId(const int newJobId)
{
    string errorMessage;
    if (isValidJobId(newJobId, errorMessage)) jobId = newJobId;
    else throw std::runtime_error(errorMessage);
}

bool Part::isValidJobId(const int value, std::string &errorMessage)
{
    if (value >= 0) return true;

    errorMessage = "Job ID must be at least 0";
    return false;
}

const char * Part::getName() const
{
    return name;
}

void Part::setName(const char *newName)
{
    string errorMessage;
    if (isValidName(newName, errorMessage)) strcpy(name, newName);
    else throw std::runtime_error(errorMessage);
}

bool Part::isValidName(const char *value, std::string &errorMessage)
{
    return validateLengthOf(value, minNameLength, maxNameLength, "Part name", errorMessage);
}

const char * Part::getNumber() const
{
    return number;
}

void Part::setNumber(const char *newNumber)
{
    string errorMessage;
    if (isValidNumber(newNumber, errorMessage)) strcpy(number, newNumber);
    else throw std::runtime_error(errorMessage);
}

bool Part::isValidNumber(const char *value, std::string &errorMessage)
{
    return validateLengthOf(value, maxNumberLength, "Part number", errorMessage);
}

double Part::getPrice() const
{
    return price;
}

void Part::setPrice(const double newPrice)
{
    string errorMessage;
    if (isValidPrice(newPrice, errorMessage)) price = newPrice;
    else throw std::runtime_error(errorMessage);
}

bool Part::isValidPrice(const double value, std::string &errorMessage)
{
    if (value >= 0.01) return true;

    errorMessage = "Price must be greater than 0.00";
    return false;
}

double Part::getVatRate() const
{
    return vatRate;
}

void Part::setVatRate(const double newVatRate)
{
    string errorMessage;
    if (isValidVatRate(newVatRate, errorMessage)) vatRate = newVatRate;
    else throw std::runtime_error(errorMessage);
}

bool Part::isValidVatRate(const double value, std::string &errorMessage)
{
    if (value >= 0.0) return true;

    errorMessage = "VAT rate must be at least 0.0";
    return false;
}

int Part::getQuantity() const
{
    return quantity;
}

void Part::setQuantity(const int newQuantity)
{
    string errorMessage;
    if (isValidQuantity(newQuantity, errorMessage)) quantity = newQuantity;
    else throw std::runtime_error(errorMessage);
}

bool Part::isValidQuantity(const int value, std::string &errorMessage)
{
    if (value > 0) return true;

    errorMessage = "Quantity must be greater than 0";
    return false;
}

void Part::validate() const
{
    string errorMessage;
    if (!isValidJobId(jobId, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidName(name, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidNumber(number, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidPrice(price, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidVatRate(vatRate, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidQuantity(quantity, errorMessage)) throw std::runtime_error(errorMessage);
}
