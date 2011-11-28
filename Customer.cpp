/*
 * Customer.cpp
 *
 *  Created on: 26 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
#include <string.h>
using namespace std;

#include "Customer.h"

const string Customer::databaseFilename = "customers.dat";

Customer::Customer(const char * newForename, const char * newSurname, const char * newAddressLine1,
                   const char * newAddressLine2, const char * newTown, const char * newPostcode,
                   const char * newHomePhoneNumber, const char * newMobilePhoneNumber, const char * newEmailAddress) {

    forename = new char[nameLength+1];
    surname = new char[nameLength+1];
    addressLine1 = new char[addressLineLength+1];
    addressLine2 = new char[addressLineLength+1];
    town = new char[townLength+1];
    postcode = new char[postcodeLength+1];
    homePhoneNumber = new char[phoneNumberLength+1];
    mobilePhoneNumber = new char[phoneNumberLength+1];
    emailAddress = new char[emailAddressLength+1];

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

Customer::Customer(const Customer & customer) {
    forename = new char[nameLength+1];
    surname = new char[nameLength+1];
    addressLine1 = new char[addressLineLength+1];
    addressLine2 = new char[addressLineLength+1];
    town = new char[townLength+1];
    postcode = new char[postcodeLength+1];
    homePhoneNumber = new char[phoneNumberLength+1];
    mobilePhoneNumber = new char[phoneNumberLength+1];
    emailAddress = new char[emailAddressLength+1];

    (*this) = customer;
}

Customer::~Customer() {
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

void Customer::operator =(const Customer & customer) {
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

void Customer::writeToFile(fstream & file) const {
    Record::writeToFile(file);
    file.write(forename, nameLength);
    file.write(surname, nameLength);
    file.write(addressLine1, addressLineLength);
    file.write(addressLine2, addressLineLength);
    file.write(town, townLength);
    file.write(postcode, postcodeLength);
    file.write(homePhoneNumber, phoneNumberLength);
    file.write(mobilePhoneNumber, phoneNumberLength);
    file.write(emailAddress, emailAddressLength);
}

void Customer::readFromFile(fstream & file) {
    Record::readFromFile(file);
    file.read(forename, nameLength);
    file.read(surname, nameLength);
    file.read(addressLine1, addressLineLength);
    file.read(addressLine2, addressLineLength);
    file.read(town, townLength);
    file.read(postcode, postcodeLength);
    file.read(homePhoneNumber, phoneNumberLength);
    file.read(mobilePhoneNumber, phoneNumberLength);
    file.read(emailAddress, emailAddressLength);
}

int Customer::size() {
    return Record::size()+
        ((nameLength+addressLineLength+phoneNumberLength)*2)+townLength+postcodeLength+emailAddressLength;
}

bool Customer::hasMatchingField(const string & fieldName, const int searchTerm) const {
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Customer::hasMatchingField(const string & fieldName, const char * searchTerm) const {
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

const char * Customer::getForename() const {
    return forename;
}

void Customer::setForename(const char * newForename) {
    strcpy(forename, newForename);
}

const char * Customer::getSurname() const {
    return surname;
}

void Customer::setSurname(const char * newSurname) {
    strcpy(surname, newSurname);
}

const char * Customer::getAddressLine1() const {
    return addressLine1;
}

void Customer::setAddressLine1(const char * newAddressLine1) {
    strcpy(addressLine1, newAddressLine1);
}

const char * Customer::getAddressLine2() const {
    return addressLine2;
}

void Customer::setAddressLine2(const char * newAddressLine2) {
    strcpy(addressLine2, newAddressLine2);
}

const char * Customer::getTown() const {
    return town    ;
}

void Customer::setTown(const char * newTown) {
    strcpy(town, newTown);
}

const char * Customer::getPostcode() const {
    return postcode;
}

void Customer::setPostcode(const char * newPostcode) {
    strcpy(postcode, newPostcode);
}

const char * Customer::getHomePhoneNumber() const {
    return homePhoneNumber;
}

void Customer::setHomePhoneNumber(const char * newHomePhoneNumber) {
    strcpy(homePhoneNumber, newHomePhoneNumber);
}

const char * Customer::getMobilePhoneNumber() const {
    return mobilePhoneNumber;
}

void Customer::setMobilePhoneNumber(const char * newMobilePhoneNumber) {
    strcpy(mobilePhoneNumber, newMobilePhoneNumber);
}

const char * Customer::getEmailAddress() const {
    return emailAddress;
}

void Customer::setEmailAddress(const char * newEmailAddress) {
    strcpy(emailAddress, newEmailAddress);
}

