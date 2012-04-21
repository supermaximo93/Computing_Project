/*
 * Record.h
 *
 *  Created on: 26 Nov 2011
 *      Author: Max Foster
 */

#ifndef RECORD_H_
#define RECORD_H_

#include <fstream>

#include "CommandLine.h"

template<class recordType> class Database;

class Customer;
class Job;
class Part;
class Task;
class Expense;
class Setting;
class VatRate;

class Record
{
public:
    // Only compares ID for speed. Use fieldCompare or completeCompare for comparing all fields of derivative classes
    bool operator ==(const Record &record) const;
    bool operator ==(const int recordId) const;

    // Assigns the record ID of the passed Record to this Record
    void operator =(const Record &record);

    int getId() const;

    // Returns whether the ID is less than  0. If the ID is less than 0, then the Record is considered to be 'null'
    // The record becomes non-null after it has been added to the database (where the ID field will be set)
    bool null() const;

    // Returns whether the Record has a field of the given fieldName with the value matching searchTerm
    bool hasMatchingField(const std::string &fieldName, const int searchTerm) const;

    // The following classes are friends because they need to access the private ID field directly
    friend class Database<Record>;
    friend class Database<Customer>;
    friend class Database<Job>;
    friend class Database<Part>;
    friend class Database<Task>;
    friend class Database<Expense>;
    friend class Database<Setting>;
    friend class Database<VatRate>;

    friend void CommandLine::setRecordValues(Customer &, const Value &);
    friend void CommandLine::setRecordValues(Job &, const Value &);
    friend void CommandLine::setRecordValues(Part &, const Value &);
    friend void CommandLine::setRecordValues(Task &, const Value &);
    friend void CommandLine::setRecordValues(Expense &, const Value &);
    friend void CommandLine::setRecordValues(VatRate &, const Value &);

protected:
    // Returns the size in bytes of the Record
    static int size();

    Record();

    // Member functions that read and write to a file that has already been opened
    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

private:
    int id, filePosition;
};

#endif /* RECORD_H_ */
