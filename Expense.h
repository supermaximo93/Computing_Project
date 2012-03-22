/*
 * Expense.h
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#ifndef EXPENSE_H_
#define EXPENSE_H_

#include <fstream>
#include <ctime>

#include "Record.h"

class Expense : public Record
{
public:
    static const std::string databaseFilename;
    static const int minDescriptionLength, maxDescriptionLength;

    static int size();

    Expense(const time_t date = 0, const char *description = "", const double price = 0.0,
            const double vat = 0.0, const int type = 0);
    Expense(const Expense &expense);
    ~Expense();

    void operator =(const Expense &expense);

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

    bool hasMatchingField(const std::string & fieldName, const time_t searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const char *searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const double searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const int searchTerm) const;

    bool fieldCompare(const Expense &rhs) const;
    bool completeCompare(const Expense &rhs) const;

    time_t getDate() const;
    void setDate(const time_t newDate);
    static bool isValidDate(const time_t value, std::string &errorMessage);

    const char * getDescription() const;
    void setDescription(const char *newDescription);
    static bool isValidDescription(const char *value, std::string &errorMessage);

    double getPrice() const;
    void setPrice(const double newPrice);
    static bool isValidPrice(const double value, std::string &errorMessage);

    double getVat() const;
    void setVat(const double newVat);
    static bool isValidVat(const double value, std::string &errorMessage);

    int getType() const;
    void setType(const int newType);
    static bool isValidType(const double value, std::string &errorMessage);

    void validate() const;

    int getTotalPrice() const;

private:
    time_t date;
    char *description;
    double price, vat;
    int type;
};

#endif /* EXPENSE_H_ */
