/*
 * VatRate.h
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#ifndef VATRATE_H
#define VATRATE_H

#include <fstream>

#include "Record.h"

class VatRate : public Record
{
public:
    static const std::string databaseFilename;

    static int size();

    VatRate(double value = 20.0, time_t startDate = 0);
    VatRate(const VatRate &vatRate);
    ~VatRate();

    void operator =(const VatRate &vatrate);

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

    bool hasMatchingField(const std::string &fieldName, const int searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const double searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const time_t searchTerm) const;

    bool fieldCompare(const VatRate &rhs) const;
    bool completeCompare(const VatRate &rhs) const;

    double getValue() const;
    void setValue(const double newValue);
    static bool isValidValue(const double value, std::string &errorMessage);

    time_t getStartDate() const;
    void setStartDate(const time_t newStartDate);
    static bool isValidStartDate(const time_t value, std::string &errorMessage);

    void validate() const;

private:
    double value;
    time_t startDate;
};

#endif // VATRATE_H
