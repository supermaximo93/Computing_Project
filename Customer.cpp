/*
 * Customer.cpp
 *
 *  Created on: 26 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
#include <stdexcept>
#include <cstring>
using namespace std;

#include <QRegExp>

#include "Customer.h"
#include "Utils.h"

const int
Customer::minNameLength         = 2,  Customer::maxNameLength         = 16,
Customer::minAddressLineLength  = 8,  Customer::maxAddressLineLength  = 32,
Customer::minTownLength         = 4,  Customer::maxTownLength         = 16,
Customer::minPostcodeLength     = 7,  Customer::maxPostcodeLength     = 8,
Customer::minPhoneNumberLength  = 11, Customer::maxPhoneNumberLength  = 11,
Customer::minEmailAddressLength = 6,  Customer::maxEmailAddressLength = 128;

int Customer::size()
{
    return Record::size() +
        ((maxNameLength + maxAddressLineLength + maxPhoneNumberLength) * 2) + maxTownLength + maxPostcodeLength +
            maxEmailAddressLength + 9;
}

const string Customer::databaseFilename = "customers.dat";

Customer::Customer(const char *newForename, const char *newSurname, const char *newAddressLine1,
                   const char *newAddressLine2, const char *newTown, const char *newPostcode,
                   const char *newHomePhoneNumber, const char *newMobilePhoneNumber, const char *newEmailAddress)
{

    forename = new char[maxNameLength + 1];
    surname = new char[maxNameLength + 1];
    addressLine1 = new char[maxAddressLineLength + 1];
    addressLine2 = new char[maxAddressLineLength + 1];
    town = new char[maxTownLength + 1];
    postcode = new char[maxPostcodeLength + 1];
    homePhoneNumber = new char[maxPhoneNumberLength + 1];
    mobilePhoneNumber = new char[maxPhoneNumberLength + 1];
    emailAddress = new char[maxEmailAddressLength + 1];

    strcpy(forename, newForename);
    strcpy(surname, newSurname);
    strcpy(addressLine1, newAddressLine1);
    strcpy(addressLine2, newAddressLine2);
    strcpy(town, newTown);
    strcpy(postcode, newPostcode);
    strcpy(homePhoneNumber, newHomePhoneNumber);
    strcpy(mobilePhoneNumber, newMobilePhoneNumber);
    strcpy(emailAddress, newEmailAddress);
}

Customer::Customer(const Customer &customer)
{
    forename = new char[maxNameLength + 1];
    surname = new char[maxNameLength + 1];
    addressLine1 = new char[maxAddressLineLength + 1];
    addressLine2 = new char[maxAddressLineLength + 1];
    town = new char[maxTownLength + 1];
    postcode = new char[maxPostcodeLength + 1];
    homePhoneNumber = new char[maxPhoneNumberLength + 1];
    mobilePhoneNumber = new char[maxPhoneNumberLength + 1];
    emailAddress = new char[maxEmailAddressLength + 1];

    (*this) = customer;
}

Customer::~Customer()
{
    delete[] forename;
    delete[] surname;
    delete[] addressLine1;
    delete[] addressLine2;
    delete[] town;
    delete[] postcode;
    delete[] homePhoneNumber;
    delete[] mobilePhoneNumber;
    delete[] emailAddress;
}

void Customer::operator =(const Customer &customer)
{
    *((Record*)this) = (Record)customer;
    strcpy(forename, customer.forename);
    strcpy(surname, customer.surname);
    strcpy(addressLine1, customer.addressLine1);
    strcpy(addressLine2, customer.addressLine2);
    strcpy(town, customer.town);
    strcpy(postcode, customer.postcode);
    strcpy(homePhoneNumber, customer.homePhoneNumber);
    strcpy(mobilePhoneNumber, customer.mobilePhoneNumber);
    strcpy(emailAddress, customer.emailAddress);
}

void Customer::writeToFile(fstream &file) const
{
    Record::writeToFile(file);
    file.write(forename, maxNameLength + 1);
    file.write(surname, maxNameLength + 1);
    file.write(addressLine1, maxAddressLineLength + 1);
    file.write(addressLine2, maxAddressLineLength + 1);
    file.write(town, maxTownLength + 1);
    file.write(postcode, maxPostcodeLength + 1);
    file.write(homePhoneNumber, maxPhoneNumberLength + 1);
    file.write(mobilePhoneNumber, maxPhoneNumberLength + 1);
    file.write(emailAddress, maxEmailAddressLength + 1);
}

void Customer::readFromFile(fstream &file)
{
    Record::readFromFile(file);
    file.read(forename, maxNameLength + 1);
    file.read(surname, maxNameLength + 1);
    file.read(addressLine1, maxAddressLineLength + 1);
    file.read(addressLine2, maxAddressLineLength + 1);
    file.read(town, maxTownLength + 1);
    file.read(postcode, maxPostcodeLength + 1);
    file.read(homePhoneNumber, maxPhoneNumberLength + 1);
    file.read(mobilePhoneNumber, maxPhoneNumberLength + 1);
    file.read(emailAddress, maxEmailAddressLength + 1);
}

bool Customer::hasMatchingField(const string &fieldName, const int searchTerm) const
{
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Customer::hasMatchingField(const string &fieldName, const char *searchTerm) const
{
    if (fieldName == "forename") return (strcmp(forename, searchTerm) == 0);
    else if (fieldName == "surname") return (strcmp(surname, searchTerm) == 0);
    else if (fieldName == "addressline1") return (strcmp(addressLine1, searchTerm) == 0);
    else if (fieldName == "addressline2") return (strcmp(addressLine2, searchTerm) == 0);
    else if (fieldName == "town") return (strcmp(town, searchTerm) == 0);
    else if (fieldName == "postcode") return (strcmp(postcode, searchTerm) == 0);
    else if (fieldName == "homephonenumber") return (strcmp(homePhoneNumber, searchTerm) == 0);
    else if (fieldName == "mobilephonenumber") return (strcmp(mobilePhoneNumber, searchTerm) == 0);
    else if (fieldName == "emailaddress") return (strcmp(emailAddress, searchTerm) == 0);

    return false;
}

bool Customer::fieldCompare(const Customer &rhs) const
{
    if (strcmp(forename, rhs.forename) != 0) return false;
    if (strcmp(surname, rhs.surname) != 0) return false;
    if (strcmp(addressLine1, rhs.addressLine1) != 0) return false;
    if (strcmp(addressLine2, rhs.addressLine2) != 0) return false;
    if (strcmp(town, rhs.town) != 0) return false;
    if (strcmp(postcode, rhs.postcode) != 0) return false;
    if (strcmp(homePhoneNumber, rhs.homePhoneNumber) != 0) return false;
    if (strcmp(mobilePhoneNumber, rhs.mobilePhoneNumber) != 0) return false;
    if (strcmp(emailAddress, rhs.emailAddress) != 0) return false;
    return true;
}

bool Customer::completeCompare(const Customer &rhs) const
{
    if (getId() != rhs.getId()) return false;
    return fieldCompare(rhs);
}

const char * Customer::getForename() const
{
    return forename;
}

void Customer::setForename(const char *newForename)
{
    string errorMessage;
    if (isValidForename(newForename, errorMessage)) strcpy(forename, newForename);
    else throw std::runtime_error(errorMessage);
}

bool Customer::isValidForename(const char *value, std::string &errorMessage)
{
    return validateLengthOf(value, minNameLength, maxNameLength, "Forename", errorMessage);;
}

const char * Customer::getSurname() const
{
    return surname;
}

void Customer::setSurname(const char *newSurname)
{
    string errorMessage;
    if (isValidSurname(newSurname, errorMessage)) strcpy(surname, newSurname);
    else throw std::runtime_error(errorMessage);
}

bool Customer::isValidSurname(const char *value, std::string &errorMessage)
{
    return validateLengthOf(value, minNameLength, maxNameLength, "Surname", errorMessage);
}

const char * Customer::getAddressLine1() const
{
    return addressLine1;
}

void Customer::setAddressLine1(const char *newAddressLine1)
{
    string errorMessage;
    if (isValidAddressLine1(newAddressLine1, errorMessage)) strcpy(addressLine1, newAddressLine1);
    else throw std::runtime_error(errorMessage);
}

bool Customer::isValidAddressLine1(const char *value, std::string &errorMessage)
{
    return validateLengthOf(value, minAddressLineLength, maxAddressLineLength, "Address line 1", errorMessage);
}

const char * Customer::getAddressLine2() const
{
    return addressLine2;
}

void Customer::setAddressLine2(const char *newAddressLine2)
{
    string errorMessage;
    if (isValidAddressLine2(newAddressLine2, errorMessage)) strcpy(addressLine2, newAddressLine2);
    else throw std::runtime_error(errorMessage);
}

bool Customer::isValidAddressLine2(const char *value, std::string &errorMessage)
{
    return validateLengthOf(value, maxAddressLineLength, "Address line 2", errorMessage);
}

const char * Customer::getTown() const
{
    return town;
}

void Customer::setTown(const char *newTown)
{
    string errorMessage;
    if (isValidTown(newTown, errorMessage)) strcpy(town, newTown);
    else throw std::runtime_error(errorMessage);
}

bool Customer::isValidTown(const char *value, std::string &errorMessage)
{
    return validateLengthOf(value, minTownLength, maxTownLength, "Town", errorMessage);
}

const char * Customer::getPostcode() const
{
    return postcode;
}

void Customer::setPostcode(const char *newPostcode)
{
    string errorMessage;
    if (isValidPostcode(newPostcode, errorMessage)) strcpy(postcode, newPostcode);
    else throw std::runtime_error(errorMessage);
}

bool Customer::isValidPostcode(const char *value, std::string &errorMessage)
{
    QRegExp regularExpression("^[a-zA-Z]{1,2}[0-9][0-9a-zA-Z]? [0-9][a-zA-Z]{2}$");
    const bool isValid = regularExpression.exactMatch(value);
    if (!isValid) errorMessage = "Postcode is not in the correct format";
    return isValid;
}

const char * Customer::getHomePhoneNumber() const
{
    return homePhoneNumber;
}

void Customer::setHomePhoneNumber(const char *newHomePhoneNumber)
{
    string errorMessage;
    if (isValidHomePhoneNumber(newHomePhoneNumber, errorMessage)) strcpy(homePhoneNumber, newHomePhoneNumber);
    else throw std::runtime_error(errorMessage);
}

bool Customer::isValidHomePhoneNumber(const char *value, std::string &errorMessage)
{
    const size_t length = strlen(value);
    if (length != 11)
    {
        errorMessage = "Home phone number must be 11 digits long";
        return false;
    }

    QRegExp regularExpression("^\\d+$");
    const bool isValid = regularExpression.exactMatch(value);
    if (!isValid) errorMessage = "Home phone number must only contain digits";
    return isValid;
}

const char * Customer::getMobilePhoneNumber() const
{
    return mobilePhoneNumber;
}

void Customer::setMobilePhoneNumber(const char *newMobilePhoneNumber)
{
    string errorMessage;
    if (isValidMobilePhoneNumber(newMobilePhoneNumber, errorMessage)) strcpy(mobilePhoneNumber, newMobilePhoneNumber);
    else throw std::runtime_error(errorMessage);
}

bool Customer::isValidMobilePhoneNumber(const char *value, std::string &errorMessage)
{
    const size_t length = strlen(value);
    if (length == 0) return true;
    if (length != 11)
    {
        errorMessage = "Mobile phone number must be 11 digits long";
        return false;
    }

    QRegExp regularExpression("^\\d+$");
    const bool isValid = regularExpression.exactMatch(value);
    if (!isValid) errorMessage = "Mobile phone number must only contain digits";
    return isValid;
}

const char * Customer::getEmailAddress() const
{
    return emailAddress;
}

void Customer::setEmailAddress(const char *newEmailAddress)
{
    string errorMessage;
    if (isValidEmailAddress(newEmailAddress, errorMessage)) strcpy(emailAddress, newEmailAddress);
    else throw std::runtime_error(errorMessage);
}

bool Customer::isValidEmailAddress(const char *value, std::string &errorMessage)
{
    if (!validateLengthOf(value, minEmailAddressLength, maxEmailAddressLength, "Email address", errorMessage))
        return false;

    QRegExp regularExpression("^([0-9a-z]|-|_|\\.)+@([0-9a-z]|-|_|\\.)+\\.[a-z]+$");
    const bool isValid = regularExpression.exactMatch(value);
    if (!isValid) errorMessage = "Email address is not a valid email address";
    return isValid;
}

void Customer::validate() const
{
    string errorMessage;
    if (!isValidForename(forename, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidSurname(surname, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidAddressLine1(addressLine1, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidAddressLine2(addressLine2, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidTown(town, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidPostcode(postcode, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidHomePhoneNumber(homePhoneNumber, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidMobilePhoneNumber(mobilePhoneNumber, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidEmailAddress(emailAddress, errorMessage)) throw std::runtime_error(errorMessage);
}
