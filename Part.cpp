/*
 * Part.cpp
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
#include <cmath>
#include <string.h>
using namespace std;

#include "Globals.h"
#include "Part.h"

int Part::size()
{
    return Record::size() + sizeof(int) + maxNameLength + maxNumberLength + (sizeof(float) * 2);
}

const string Part::databaseFilename = "parts.dat";

Part::Part(const int jobId, const char * newName, const char * newNumber, const float price, const float vatRate)
    : jobId(jobId), price(price), vatRate(vatRate < 0.0f ? Globals::vatRate : vatRate)
{
    name = new char[maxNameLength + 1];
    number = new char[maxNumberLength + 1];
    strcpy(name, newName);
    strcpy(number, newNumber);
}

Part::Part(const Part & part)
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

void Part::operator =(const Part & part)
{
    *((Record*)this) = (Record)part;
    jobId = part.jobId;
    strcpy(name, part.name);
    strcpy(number, part.number);
    price = part.price;
    vatRate = part.vatRate;
}

void Part::writeToFile(fstream & file) const
{
    Record::writeToFile(file);
    file.write(reinterpret_cast<const char *>(&jobId), sizeof(jobId));
    file.write(name, maxNameLength + 1);
    file.write(number, maxNumberLength + 1);
    file.write(reinterpret_cast<const char *>(&price), sizeof(price));
    file.write(reinterpret_cast<const char *>(&vatRate), sizeof(vatRate));
}

void Part::readFromFile(fstream & file)
{
    Record::readFromFile(file);
    file.read(reinterpret_cast<char *>(&jobId), sizeof(jobId));
    file.read(name, maxNameLength + 1);
    file.read(number, maxNumberLength + 1);
    file.read(reinterpret_cast<char *>(&price), sizeof(price));
    file.read(reinterpret_cast<char *>(&vatRate), sizeof(vatRate));
}

bool Part::hasMatchingField(const string & fieldName, const int searchTerm) const
{
    if (fieldName == "jobid") return (jobId == searchTerm);
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Part::hasMatchingField(const string & fieldName, const char * searchTerm) const
{
    if (fieldName == "name") return (strcmp(name, searchTerm) == 0);
    else if (fieldName == "number") return (strcmp(number, searchTerm) == 0);
    return false;
}

bool Part::hasMatchingField(const string & fieldName, const float searchTerm) const
{
    if (fieldName == "price") return (price == searchTerm);
    else if (fieldName == "vatRate") return (vatRate == searchTerm);
    return false;
}

bool Part::fieldCompare(const Part & rhs) const
{
    if (jobId != rhs.jobId) return false;
    if (strcmp(name, rhs.name) != 0) return false;
    if (strcmp(number, rhs.number) != 0) return false;
    if (fabs(price - rhs.price) > 0.001) return false;
    if (fabs(vatRate - rhs.vatRate) > 0.001) return false;
    return true;
}

bool Part::completeCompare(const Part & rhs) const
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
    jobId = newJobId;
}

const char * Part::getName() const
{
    return name;
}

void Part::setName(const char * newName)
{
    strcpy(name, newName);
}

const char * Part::getNumber() const
{
    return number;
}

void Part::setNumber(const char * newNumber)
{
    strcpy(number, newNumber);
}

float Part::getPrice() const
{
    return price;
}

void Part::setPrice(const float newPrice)
{
    price = newPrice;
}

float Part::getVatRate() const
{
    return vatRate;
}

void Part::setVatRate(const float newVatRate)
{
    vatRate = newVatRate;
}
