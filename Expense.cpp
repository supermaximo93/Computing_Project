/*
 * Expense.cpp
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

#include "Expense.h"
#include "Utils.h"

int Expense::size()
{
    return Record::size() + sizeof(time_t) + maxDescriptionLength + 1 + (sizeof(double) * 2) + sizeof(int);
}

const string Expense::databaseFilename = "expenses.dat";

Expense::Expense(const time_t date, const char *newDescription, const double price, const double vat, const int type)
    : date(date == 0 ? time(NULL) : date), price(price), vat(vat), type(type)
{
    description = new char[maxDescriptionLength + 1];
    strcpy(description, newDescription);
}

Expense::Expense(const Expense &expense)
{
    description = new char[maxDescriptionLength + 1];
    (*this) = expense;
}

Expense::~Expense()
{
    delete[] description;
}

void Expense::operator =(const Expense &expense)
{
    *((Record*)this) = (Record)expense;
    date = expense.date;
    strcpy(description, expense.description);
    price = expense.price;
    vat = expense.vat;
    type = expense.type;
}

void Expense::writeToFile(fstream &file) const
{
    Record::writeToFile(file);
    file.write(reinterpret_cast<const char *>(&date), sizeof(date));
    file.write(description, maxDescriptionLength + 1);
    file.write(reinterpret_cast<const char *>(&price), sizeof(price));
    file.write(reinterpret_cast<const char *>(&vat), sizeof(vat));
    file.write(reinterpret_cast<const char *>(&type), sizeof(type));
}

void Expense::readFromFile(fstream &file)
{
    Record::readFromFile(file);
    file.read(reinterpret_cast<char *>(&date), sizeof(date));
    file.read(description, maxDescriptionLength + 1);
    file.read(reinterpret_cast<char *>(&price), sizeof(price));
    file.read(reinterpret_cast<char *>(&vat), sizeof(vat));
    file.read(reinterpret_cast<char *>(&type), sizeof(type));
}

bool Expense::hasMatchingField(const string &fieldName, const time_t searchTerm) const
{
    if (fieldName == "date") return (date == searchTerm);
    return false;
}

bool Expense::hasMatchingField(const string &fieldName, const char *searchTerm) const
{
    if (fieldName == "description") return (strcmp(description, searchTerm) == 0);
    return false;
}

bool Expense::hasMatchingField(const string &fieldName, const double searchTerm) const
{
    if (fieldName == "price") return (price == searchTerm);
    else if (fieldName == "vat") return (vat == searchTerm);
    return false;
}

bool Expense::hasMatchingField(const string &fieldName, const int searchTerm) const
{
    if (fieldName == "type") return (type == searchTerm);
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Expense::fieldCompare(const Expense &rhs) const
{
    if (date != rhs.date) return false;
    if (strcmp(description, rhs.description) != 0) return false;
    if (fabs(price - rhs.price) > 0.00001) return false; // Accouting for floating point error
    if (fabs(vat - rhs.vat) > 0.00001) return false;
    if (type != rhs.type) return false;
    return true;
}

bool Expense::completeCompare(const Expense &rhs) const
{
    if (getId() != rhs.getId()) return false;
    return fieldCompare(rhs);
}

time_t Expense::getDate() const
{
    return date;
}

void Expense::setDate(const time_t newDate)
{
    string errorMessage;
    if (isValidDate(newDate, errorMessage)) date = newDate;
    else throw std::runtime_error(errorMessage);
}

bool Expense::isValidDate(const time_t, std::string &)
{
    return true;
}

const char * Expense::getDescription() const
{
    return description;
}

void Expense::setDescription(const char *newDescription)
{
    string errorMessage;
    if (isValidDescription(newDescription, errorMessage)) strcpy(description, newDescription);
    else throw std::runtime_error(errorMessage);
}

bool Expense::isValidDescription(const char *value, std::string &errorMessage)
{
    if ((value == NULL) || (value[0] == '\0'))
    {
        errorMessage = "Description must be present";
        return false;
    }

    return validateLengthOf(value, maxDescriptionLength, "Description", errorMessage);
}

double Expense::getPrice() const
{
    return price;
}

void Expense::setPrice(const double newPrice)
{
    string errorMessage;
    if (isValidPrice(newPrice, errorMessage)) price = newPrice;
    else throw std::runtime_error(errorMessage);
}

bool Expense::isValidPrice(const double value, std::string &errorMessage)
{
    if (value >= 0.01) return true;

    errorMessage = "Price must be greater than 0.00";
    return false;
}

double Expense::getVat() const
{
    return vat;
}

void Expense::setVat(const double newVat)
{
    string errorMessage;
    if (isValidVat(newVat, errorMessage)) vat = newVat;
    else throw std::runtime_error(errorMessage);
}

bool Expense::isValidVat(const double value, std::string &errorMessage)
{
    if (value >= 0.0) return true;

    errorMessage = "VAT cost must be greater than 0.00";
    return false;
}

int Expense::getType() const
{
    return type;
}

void Expense::setType(const int newType)
{
    string errorMessage;
    if (isValidType(newType, errorMessage)) type = newType;
    else throw std::runtime_error(errorMessage);
}

bool Expense::isValidType(const double value, std::string &errorMessage)
{
    if (value >= 0) return true;

    errorMessage = "Type heading must be at least 0";
    return false;
}

int Expense::getTotalPrice() const
{
    return price + vat;
}

void Expense::validate()
{
    string errorMessage;
    if (!isValidDate(date, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidDescription(description, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidPrice(price, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidVat(vat, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidType(type, errorMessage)) throw std::runtime_error(errorMessage);
}
