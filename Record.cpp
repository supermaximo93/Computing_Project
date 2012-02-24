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
    if (fieldName == "id") return (id == searchTerm);
    return false;
}

int Record::size()
{
    return sizeof(int);
}

Record::Record()
{
    id = -1;
}

void Record::writeToFile(fstream &file) const
{
    file.write(reinterpret_cast<const char *>(&id), sizeof(id));
}

void Record::readFromFile(fstream &file)
{
    file.read(reinterpret_cast<char *>(&id), sizeof(id));
}
