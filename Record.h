/*
 * Record.h
 *
 *  Created on: 26 Nov 2011
 *      Author: Max Foster
 */

#ifndef RECORD_H_
#define RECORD_H_

#include <fstream>

template<class recordType> class Database;

class Customer;
class Job;
class Part;
class Task;
class Expense;

class Record
{
public:
    // Only compares ID for speed. Use fieldCompare or completeCompare for comparing all fields of derivative classes
    bool operator ==(const Record &record) const;
    bool operator ==(const int recordId) const;
    void operator =(const Record &record);

    int getId() const;
    bool null() const;

    bool hasMatchingField(const std::string &fieldName, const int searchTerm) const;

    friend class Database<Record>;
    friend class Database<Customer>;
    friend class Database<Job>;
    friend class Database<Part>;
    friend class Database<Task>;
    friend class Database<Expense>;

protected:
    static int size();

    Record();

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

private:
    int id;
};

#endif /* RECORD_H_ */
