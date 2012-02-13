/*
 * Part.h
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#ifndef PART_H_
#define PART_H_

#include <fstream>

#include "Record.h"

class Part : public Record {
public:
    static int size();

    static const std::string databaseFilename;
    static const int
    minNameLength   = 3, maxNameLength   = 24,
    minNumberLength = 0, maxNumberLength = 32;

    Part(const int jobId = 0, const char * name = "", const char * number = "", const double price = 0.0,
         const double vatRate = -1.0);
    Part(const Part & part);
    ~Part();

    void operator =(const Part & part);

    void writeToFile(std::fstream & file) const;
    void readFromFile(std::fstream & file);

    bool hasMatchingField(const std::string & fieldName, const int searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const char * searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const double searchTerm) const;

    bool fieldCompare(const Part & rhs) const;
    bool completeCompare(const Part & rhs) const;

    int getJobId() const;
    void setJobId(const int newJobId);

    const char * getName() const;
    void setName(const char * newName);

    const char * getNumber() const;
    void setNumber(const char * newNumber);

    double getPrice() const;
    void setPrice(const double newPrice);

    double getVatRate() const;
    void setVatRate(const double newVatRate);

private:
    int jobId;
    char * name, * number;
    double price, vatRate;
};

#endif /* PART_H_ */
