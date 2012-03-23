/*
 * Setting.cpp
 *
 *  Created on: 23 3 2012
 *      Author: Max Foster
 */

#include <stdexcept>
#include <cstring>
using namespace std;

#include "Setting.h"
#include "Utils.h"

const string Setting::databaseFilename = "settings.dat";
const int
Setting::minKeyLength   = 2, Setting::maxKeyLength   =   32,
Setting::minValueLength = 0, Setting::maxValueLength = 2048;

int Setting::size()
{
    return Record::size() + maxKeyLength + maxValueLength + 2;
}

Setting::Setting(const char *newKey, const char *newValue)
{
    key = new char[maxKeyLength + 1];
    value = new char[maxValueLength + 1];

    strcpy(key, newKey);
    strcpy(value, newValue);
}

Setting::Setting(const Setting &setting)
{
    key = new char[maxKeyLength + 1];
    value = new char[maxValueLength + 1];

    (*this) = setting;
}

Setting::~Setting()
{
    delete[] key;
    delete[] value;
}

void Setting::operator =(const Setting &setting)
{
    *((Record*)this) = (Record)setting;
    strcpy(key, setting.key);
    strcpy(value, setting.value);
}

void Setting::writeToFile(fstream &file) const
{
    Record::writeToFile(file);
    file.write(key, maxKeyLength + 1);
    file.write(value, maxValueLength + 1);
}

void Setting::readFromFile(fstream &file)
{
    Record::readFromFile(file);
    file.read(key, maxKeyLength + 1);
    file.read(value, maxValueLength + 1);
}

bool Setting::hasMatchingField(const string &fieldName, const int searchTerm) const
{
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Setting::hasMatchingField(const string &fieldName, const char *searchTerm) const
{
    if (fieldName == "key") return (strcmp(key, searchTerm) == 0);
    if (fieldName == "value") return (strcmp(value, searchTerm) == 0);
    return false;
}

bool Setting::fieldCompare(const Setting &rhs) const
{
    if (strcmp(key, rhs.key) != 0) return false;
    if (strcmp(value, rhs.value) != 0) return false;
    return true;
}

bool Setting::completeCompare(const Setting &rhs) const
{
    if (getId() != rhs.getId()) return false;
    return fieldCompare(rhs);
}

const char * Setting::getKey() const
{
    return key;
}

void Setting::setKey(const char *newKey)
{
    string errorMessage;
    if (isValidKey(newKey, errorMessage)) strcpy(key, newKey);
    else throw std::runtime_error(errorMessage);
}

bool Setting::isValidKey(const char *value, string &errorMessage)
{
    return validateLengthOf(value, minKeyLength, maxKeyLength, "Key", errorMessage);
}

const char * Setting::getValue() const
{
    return value;
}

void Setting::setValue(const char *newValue)
{
    string errorMessage;
    if (isValidValue(newValue, errorMessage)) strcpy(value, newValue);
    else throw std::runtime_error(errorMessage);
}

bool Setting::isValidValue(const char *value, string &errorMessage)
{
    return validateLengthOf(value, minValueLength, maxValueLength, "Value", errorMessage);
}

void Setting::validate() const
{
    string errorMessage;
    if (!isValidKey(key, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidValue(value, errorMessage)) throw std::runtime_error(errorMessage);
}
