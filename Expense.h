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

class Expense : public Record {
    static const int descriptionLength = 256;

    time_t date;
    char * description;
    float price, vat;
    int type;

public:
    static const std::string databaseFilename;

    Expense(const time_t newDate = 0, const char * newDescription = "", const float newPrice = 0.0f,
            const float newVat = 0.0f, const int newType = 0);
    Expense(const Expense & expense);
    ~Expense();

    void operator =(const Expense & expense);

    void writeToFile(std::fstream & file) const;
    void readFromFile(std::fstream & file);

    static int size();

    bool hasMatchingField(const std::string & fieldName, const time_t searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const char * searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const float searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const int searchTerm) const;

    time_t getDate() const;
    void setDate(const time_t newDate);

    const char * getDescription() const;
    void setDescription(const char * newDescription);

    float getPrice() const;
    void setPrice(const float newPrice);

    float getVat() const;
    void setVat(const float newVat);

    int getType() const;
    void setType(const int newType);
};

#endif /* EXPENSE_H_ */
