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

    bool pending; // Used when the part has been created successfully, but hasn't been added to the database yet

    Part(const int jobId = -1, const char *name = "", const char *number = "", const double price = 0.0,
         const double vatRate = -1.0, const int quantity = 1);
    Part(const Part &part);
    ~Part();

    void operator =(const Part &part);

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

    bool hasMatchingField(const std::string &fieldName, const int searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const char *searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const double searchTerm) const;

    bool fieldCompare(const Part &rhs) const;
    bool completeCompare(const Part &rhs) const;

    int getJobId() const;
    void setJobId(const int newJobId);
    static bool isValidJobId(const int value, std::string &errorMessage);

    const char * getName() const;
    void setName(const char *newName);
    static bool isValidName(const char *value, std::string &errorMessage);

    const char * getNumber() const;
    void setNumber(const char *newNumber);
    static bool isValidNumber(const char *value, std::string &errorMessage);

    double getPrice() const;
    void setPrice(const double newPrice);
    static bool isValidPrice(const double value, std::string &errorMessage);

    double getVatRate() const;
    void setVatRate(const double newVatRate);
    static bool isValidVatRate(const double value, std::string &errorMessage);

    int getQuantity() const;
    void setQuantity(const int newQuantity);
    static bool isValidQuantity(const int value, std::string &errorMessage);

    void validate();

private:
    int jobId, quantity;
    char *name, *number;
    double price, vatRate;
};

#endif /* PART_H_ */
