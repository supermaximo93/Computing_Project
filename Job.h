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

class Job : public Record
{
public:
    enum CompletionState
    {
        NOT_DONE = 0,
        DONE_UNPAID,
        DONE_PAID
    };

    enum PaymentMethod
    {
        NA = 0,
        CASH,
        CHEQUE,
        CREDIT,
        DEBIT,
        BANK_TRANSFER
    };

    static int size();

    static const std::string databaseFilename;

    Job(const int customerId = 0, const time_t date = 0, const double labourCharge = 0.0,
        const int completionState = 0, const int paymentMethod = 0);

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

    bool hasMatchingField(const std::string &fieldName, const int searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const time_t searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const double searchTerm) const;

    bool fieldCompare(const Job &rhs) const;
    bool completeCompare(const Job &rhs) const;

    int getCustomerId() const;
    void setCustomerId(const int newCustomerId);

    time_t getDate() const;
    void setDate(const time_t newDate);

    double getLabourCharge() const;
    void setLabourCharge(const double newLabourCharge); // Recalculates VAT

    double getVat() const; // No setter because VAT is automatically calculated

    int getCompletionState() const;
    void setCompletionState(const int newCompletionState);

    int getPaymentMethod() const;
    void setPaymentMethod(const int newPaymentMethod);

private:
    int customerId, completionState, paymentMethod;
    time_t date;
    double labourCharge, vat;
};

#endif /* JOB_H_ */
