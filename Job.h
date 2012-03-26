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

    static const std::string databaseFilename;
    static const int minDescriptionLength, maxDescriptionLength;

    static int size();

    Job(const int customerId = -1, const time_t date = 0, const char *description = "", const double labourCharge = 0.0,
        const int completionState = 0, const int paymentMethod = 0);

    Job(const Job &job);
    ~Job();

    void operator =(const Job &job);

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

    bool hasMatchingField(const std::string &fieldName, const int searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const time_t searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const char * searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const double searchTerm) const;

    bool fieldCompare(const Job &rhs) const;
    bool completeCompare(const Job &rhs) const;

    int getCustomerId() const;
    void setCustomerId(const int newCustomerId);
    static bool isValidCustomerId(const int value, std::string &errorMessage);

    time_t getDate() const;
    void setDate(const time_t newDate);
    static bool isValidDate(const time_t value, std::string &errorMessage);

    const char * getDescription() const;
    void setDescription(const char *newDescription);
    static bool isValidDescription(const char *value, std::string &errorMessage);

    double getLabourCharge() const;
    void setLabourCharge(const double newLabourCharge); // Recalculates VAT
    static bool isValidLabourCharge(const double value, std::string &errorMessage);

    double getVat() const; // No setter because VAT is automatically calculated

    int getCompletionState() const;
    void setCompletionState(const int newCompletionState);
    static bool isValidCompletionState(const int value, std::string &errorMessage);

    int getPaymentMethod() const;
    std::string getPaymentMethodString() const;
    void setPaymentMethod(const int newPaymentMethod);
    static bool isValidPaymentMethod(const int value, std::string &errorMessage);

    void validate() const;

private:
    int customerId, completionState, paymentMethod;
    time_t date;
    char *description;
    double labourCharge, vat;
};

#endif /* JOB_H_ */
