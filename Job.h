/*
 * Job.h
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#ifndef JOB_H_
#define JOB_H_

#include <fstream>
#include <ctime>

#include "Record.h"

class Job : public Record {
    int customerId, completionState, paymentMethod;
    time_t date;
    float labourCharge, vat;

public:
    static const std::string databaseFilename;

    Job(const int customerId = 0, const time_t date = 0, const float labourCharge = 0.0f,
        const int completionState = 0, const int paymentMethod = 0);

    void writeToFile(std::fstream & file) const;
    void readFromFile(std::fstream & file);

    static int size();

    bool hasMatchingField(const std::string & fieldName, const int searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const time_t searchTerm) const;
    bool hasMatchingField(const std::string & fieldName, const float searchTerm) const;

    int getCustomerId() const;
    void setCustomerId(const int newCustomerId);

    time_t getDate() const;
    void setDate(const time_t newDate);

    float getLabourCharge() const;
    void setLabourCharge(const float newLabourCharge); // Recalculates VAT

    float getVat() const; // No setter because VAT is automatically calculated

    int getCompletionState() const;
    void setCompletionState(const int newCompletionState);

    int getPaymentMethod() const;
    void setPaymentMethod(const int newPaymentMethod);
};

#endif /* JOB_H_ */
