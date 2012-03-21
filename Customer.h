/*
 * Customer.h
 *
 *  Created on: 26 Nov 2011
 *      Author: Max Foster
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <fstream>

#include "Record.h"

class Customer : public Record
{
public:
    static int size();

    static const std::string databaseFilename;
    static const int
    minNameLength,  maxNameLength,
    minAddressLineLength,  maxAddressLineLength,
    minTownLength,  maxTownLength         ,
    minPostcodeLength,  maxPostcodeLength,
    minPhoneNumberLength, maxPhoneNumberLength,
    minEmailAddressLength,  maxEmailAddressLength;

    Customer(const char *forename = "", const char *surname = "", const char *addressLine1 = "",
             const char *addressLine2 = "", const char *town = "", const char *postcode = "",
             const char *homePhoneNumber = "", const char *mobilePhoneNumber = "", const char *emailAddress = "");
    Customer(const Customer &customer);
    ~Customer();

    void operator =(const Customer &customer);

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

    bool hasMatchingField(const std::string &fieldName, int searchTerm) const; // Won't work without it
    bool hasMatchingField(const std::string &fieldName, const char *searchTerm) const;

    // Returns whether the fields of the passed record, other than ID, match
    bool fieldCompare(const Customer &rhs) const;
    // Returns whether the fields of the passed record, including ID, match
    bool completeCompare(const Customer &rhs) const;

    const char * getForename() const;
    void setForename(const char *newForename);
    static bool isValidForename(const char *value, std::string &errorMessage);

    const char * getSurname() const;
    void setSurname(const char *newSurname);
    static bool isValidSurname(const char *value, std::string &errorMessage);

    const char * getAddressLine1() const;
    void setAddressLine1(const char *newAddressLine1);
    static bool isValidAddressLine1(const char *value, std::string &errorMessage);

    const char * getAddressLine2() const;
    void setAddressLine2(const char *newAddressLine2);
    static bool isValidAddressLine2(const char *value, std::string &errorMessage);

    const char * getTown() const;
    void setTown(const char *newTown);
    static bool isValidTown(const char *value, std::string &errorMessage);

    const char * getPostcode() const;
    void setPostcode(const char *newPostcode);
    static bool isValidPostcode(const char *value, std::string &errorMessage);

    const char * getHomePhoneNumber() const;
    void setHomePhoneNumber(const char *newHomePhoneNumber);
    static bool isValidHomePhoneNumber(const char *value, std::string &errorMessage);

    const char * getMobilePhoneNumber() const;
    void setMobilePhoneNumber(const char *newMobilePhoneNumber);
    static bool isValidMobilePhoneNumber(const char *value, std::string &errorMessage);

    const char * getEmailAddress() const;
    void setEmailAddress(const char *newEmailAddress);
    static bool isValidEmailAddress(const char *value, std::string &errorMessage);

    void validate();

private:
    char *forename, *surname, *addressLine1, *addressLine2, *town, *postcode, *homePhoneNumber, *mobilePhoneNumber,
    *emailAddress;
};

#endif /* CUSTOMER_H_ */
