/*
 * Record.cpp
 *
 *  Created on: 26 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
using namespace std;

#include "Record.h"

bool Record::operator ==(const Record &record) const
{
    return (id == record.id);
}

bool Record::operator ==(const int recordId) const
{
    return (id == recordId);
}

void Record::operator = (const Record &record)
{
    id = record.id;
}

int Record::getId() const
{
    return id;
}

bool Record::null() const
{
    return (id < 0);
}

bool Record::hasMatchingField(const std::string &fieldName, const int searchTerm) const
{
    // If fieldName matches the name of a field, return whether the value of that field matches searchTerm
    if (fieldName == "id") return (id == searchTerm);
    return false;
}

int Record::size()
{
    // Only data member is an integer, so just return the size of an integer
    return sizeof(int);
}

Record::Record() : id(-1) {}

void Record::writeToFile(fstream &file) const
{
    // Convert the ID field into a writable form (i.e. an array of characters) before writing
    file.write(reinterpret_cast<const char *>(&id), sizeof(id));
}

void Record::readFromFile(fstream &file)
{
    // Convert the array of characters read in into an integer while reading
    file.read(reinterpret_cast<char *>(&id), sizeof(id));
}
