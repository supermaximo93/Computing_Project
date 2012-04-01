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

/*
 * The Customer record class, containing the details of a customer that the end user will do a job for.
 * The emailAddress data member is used when sending email invoices/receipts to customers
 */

class Customer : public Record
{
public:
    static const std::string databaseFilename;
    static const int
    minNameLength,  maxNameLength,
    minAddressLineLength,  maxAddressLineLength,
    minTownLength,  maxTownLength         ,
    minPostcodeLength,  maxPostcodeLength,
    minPhoneNumberLength, maxPhoneNumberLength,
    minEmailAddressLength,  maxEmailAddressLength;

    static int size();

    Customer(const char *forename = "", const char *surname = "", const char *addressLine1 = "",
             const char *addressLine2 = "", const char *town = "", const char *postcode = "",
             const char *homePhoneNumber = "", const char *mobilePhoneNumber = "", const char *emailAddress = "");
    Customer(const Customer &customer);
    ~Customer();

    void operator =(const Customer &customer);

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

    // Returns whether the Customer has a field of the given fieldName with the value matching searchTerm
    bool hasMatchingField(const std::string &fieldName, int searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const char *searchTerm) const;

    // Returns whether the fields of the passed record, other than ID, match. Needed because the Record equality
    // operator only compares IDs (for speed).
    bool fieldCompare(const Customer &rhs) const;
    // Returns whether the fields of the passed record, including ID, match
    bool completeCompare(const Customer &rhs) const;


    /*
     * All setters throw an exception with an error message if an invalid input is given. The error message
     * could be used in the UI to help the user understand what is wrong with the input
     */

    const char * getForename() const;
    void setForename(const char *newForename);
    // Returns whether the string given is a valid forename, setting the errorMessage std::string if the forename is
    // invalid (all other isValidField methods have the same behaviour)
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

    // Validates the fields of the Customer. Throws an exception if any fields are invalid
    void validate() const;

private:
    // All fields are strings to be dynamically allocated. Dynamic allocation is used so huge (relatively speaking)
    // objects aren't allocated on the stack. Each field is a pointer to an array of characters, so each field is only
    // 8 bytes. If the fields were statically allocated, then a single Customer would take up over 200B on the stack
    char *forename, *surname, *addressLine1, *addressLine2, *town, *postcode, *homePhoneNumber, *mobilePhoneNumber,
    *emailAddress;
};

#endif /* CUSTOMER_H_ */
