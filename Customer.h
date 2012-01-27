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
    static const int nameLength = 16, addressLineLength = 32, townLength = 16, postcodeLength = 8,
    phoneNumberLength = 11, emailAddressLength = 128;

    Customer(const char * forename = "", const char * surname = "", const char * addressLine1 = "",
             const char * addressLine2 = "", const char * town = "", const char * postcode = "",
             const char * homePhoneNumber = "", const char * mobilePhoneNumber = "", const char * emailAddress = "");
    Customer(const Customer & customer);
    ~Customer();

    void operator =(const Customer & customer);

    void writeToFile(std::fstream & file) const;
    void readFromFile(std::fstream & file);

    bool hasMatchingField(const std::string & fieldName, int searchTerm) const; // Won't work without it
    bool hasMatchingField(const std::string & fieldName, const char * searchTerm) const;

    const char * getForename() const;
    void setForename(const char * newForename);

    const char * getSurname() const;
    void setSurname(const char * newSurname);

    const char * getAddressLine1() const;
    void setAddressLine1(const char * newAddressLine1);

    const char * getAddressLine2() const;
    void setAddressLine2(const char * newAddressLine2);

    const char * getTown() const;
    void setTown(const char * newTown);

    const char * getPostcode() const;
    void setPostcode(const char * newPostcode);

    const char * getHomePhoneNumber() const;
    void setHomePhoneNumber(const char * newHomePhoneNumber);

    const char * getMobilePhoneNumber() const;
    void setMobilePhoneNumber(const char * newMobilePhoneNumber);

    const char * getEmailAddress() const;
    void setEmailAddress(const char * newEmailAddress);

private:
    char * forename, * surname, * addressLine1, * addressLine2, * town, * postcode, * homePhoneNumber,
    * mobilePhoneNumber, * emailAddress;
};

#endif /* CUSTOMER_H_ */
