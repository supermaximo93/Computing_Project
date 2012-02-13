/*
 * Customer_testing.cc
 *
 *  Created on: 5 Jan 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include <stdexcept>
#include <fstream>
using namespace std;

#include "Customer.h"

class CustomerTest : public ::testing::Test
{
protected:
    const Customer exampleCustomer;

    CustomerTest()
        : exampleCustomer("John", "Doe", "123 Example Lane", "Example Village", "Exampleville", "AB12 3CD",
              "01234567890", "07012345678", "john.doe@example.com") {}
};

// Is Customer ID Minus One
// A new customer record must have an ID of -1 (a customer record ID will be set when it is added to the database)
TEST_F(CustomerTest, IsCustomerIdMinusOne)
{
    Customer customer;
    EXPECT_EQ(-1, customer.getId()) << "Customer record that has not been added to the database must have an ID of -1";
}


// Does Customer Reject Empty Forename
// Records should automatically validate data passed to a setter method, and throw an exception if the data is invalid
TEST_F(CustomerTest, DoesCustomerRejectEmptyForename)
{
    // Create a customer based on the example data, and then attempt to set the forename to an empty string.
    // An exception should be thrown
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setForename(""), std::runtime_error)
            << "Exception was not thrown when the forename was set to an empty string";
}

// Does Customer Reject Forename That Is Too Short
TEST_F(CustomerTest, DoesCustomerRejectForenameThatIsTooShort)
{
    // Create a customer based on the example data, and then attempt to set the forename to a string shorter than the
    // minimum length. An exception should be thrown
    Customer customer(exampleCustomer);

    std::string testForename;
    testForename.reserve(Customer::minNameLength - 1);
    while (testForename.size() < Customer::minNameLength - 1) testForename += 'a';
    EXPECT_THROW(customer.setForename(""), std::runtime_error)
            << "Exception was not thrown when the forename was set to a string that is shorter than the minimum";
}

// Does Customer Reject Forename That Is Too Long
TEST_F(CustomerTest, DoesCustomerRejectForenameThatIsTooLong)
{
    // Create a customer based on the example data, and then attempt to set the forename to a string that is longer
    // than the maximum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    std::string testForename;
    testForename.reserve(Customer::maxNameLength + 1);
    while (testForename.size() < Customer::maxNameLength + 1) testForename += 'a';

    EXPECT_THROW(customer.setForename(testForename.c_str()), std::runtime_error)
            << "Exception was not thrown when the forename was set to a string that is longer than the maximum";
}


// Does Customer Reject Empty Surname
TEST_F(CustomerTest, DoesCustomerRejectEmptySurname)
{
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setSurname(""), std::runtime_error)
            << "Exception was not thrown when the surname was set to an empty string";
}

// Does Customer Reject Surname That Is Too Short
TEST_F(CustomerTest, DoesCustomerRejectSurnameThatIsTooShort)
{
    Customer customer(exampleCustomer);

    std::string testSurname;
    testSurname.reserve(Customer::minNameLength - 1);
    while (testSurname.size() < Customer::minNameLength - 1) testSurname += 'a';

    EXPECT_THROW(customer.setSurname(testSurname.c_str()), std::runtime_error)
            << "Exception was not thrown when the surname was set to a string that is shorter than the minimum";
}

// Does Customer Reject Surname That Is Too Long
TEST_F(CustomerTest, DoesCustomerRejectSurnameThatIsTooLong)
{
    Customer customer(exampleCustomer);

    std::string testSurname;
    testSurname.reserve(Customer::maxNameLength + 1);
    while (testSurname.size() < Customer::maxNameLength + 1) testSurname += 'a';

    EXPECT_THROW(customer.setSurname(testSurname.c_str()), std::runtime_error)
            << "Exception was not thrown when the surname was set to a string that is longer than the maximum";
}


// Does Customer Reject Empty Address Line 1
TEST_F(CustomerTest, DoesCustomerRejectEmptyAddressLine1)
{
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setAddressLine1(""), std::runtime_error)
            << "Exception was not thrown when the address line 1 was set to an empty string";
}

// Does Customer Reject Address Line 1 That Is Too Short
TEST_F(CustomerTest, DoesCustomerRejectAddressLine1ThatIsTooShort)
{
    Customer customer(exampleCustomer);

    std::string testAddressLine1;
    testAddressLine1.reserve(Customer::minAddressLineLength - 1);
    while (testAddressLine1.size() < Customer::minAddressLineLength - 1) testAddressLine1 += 'a';

    EXPECT_THROW(customer.setAddressLine1(testAddressLine1.c_str()), std::runtime_error)
            << "Exception was not thrown when the address line 1 was set to a string that is shorter than the maximum";
}

// Does Customer Reject Address Line 1 That Is Too Long
TEST_F(CustomerTest, DoesCustomerRejectAddressLine1ThatIsTooLong)
{
    Customer customer(exampleCustomer);

    std::string testAddressLine1;
    testAddressLine1.reserve(Customer::maxAddressLineLength + 1);
    while (testAddressLine1.size() < Customer::maxAddressLineLength + 1) testAddressLine1 += 'a';

    EXPECT_THROW(customer.setAddressLine1(testAddressLine1.c_str()), std::runtime_error)
            << "Exception was not thrown when the address line 1 was set to a string that is longer than the maximum";
}


// Does Customer Reject Empty Town
TEST_F(CustomerTest, DoesCustomerRejectEmptyTown)
{
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setSurname(""), std::runtime_error)
            << "Exception was not thrown when the town was set to an empty string";
}

// Does Customer Reject Town That Is Too Short
TEST_F(CustomerTest, DoesCustomerRejectTownThatIsTooShort)
{
    Customer customer(exampleCustomer);

    std::string testTown;
    testTown.reserve(Customer::minTownLength - 1);
    while (testTown.size() < Customer::minTownLength - 1) testTown += 'a';

    EXPECT_THROW(customer.setTown(testTown.c_str()), std::runtime_error)
            << "Exception was not thrown when the town was set to a string that is shorter than the minimum";
}

// Does Customer Reject Town That Is Too Long
TEST_F(CustomerTest, DoesCustomerRejectTownThatIsTooLong)
{
    Customer customer(exampleCustomer);

    std::string testTown;
    testTown.reserve(Customer::maxTownLength + 1);
    while (testTown.size() < Customer::maxTownLength + 1) testTown += 'a';

    EXPECT_THROW(customer.setTown(testTown.c_str()), std::runtime_error)
            << "Exception was not thrown when the town was set to a string that is longer than the maximum";
}


// Does Customer Reject Empty Postcode
TEST_F(CustomerTest, DoesCustomerRejectEmptyPostcode)
{
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setPostcode(""), std::runtime_error)
            << "Exception was not thrown when the postcode was set to an empty string";
}

// Does Customer Reject Postcode That Is Too Short
TEST_F(CustomerTest, DoesCustomerRejectPostcodeThatIsTooShort)
{
    Customer customer(exampleCustomer);

    std::string testPostcode;
    testPostcode.reserve(Customer::minPostcodeLength - 1);
    while (testPostcode.size() < Customer::minPostcodeLength - 1) testPostcode += 'a';

    EXPECT_THROW(customer.setPostcode(testPostcode.c_str()), std::runtime_error)
            << "Exception was not thrown when the postcode was set to a string that is shorter than the minimum";
}

// Does Customer Reject Postcode That Is Too Long
TEST_F(CustomerTest, DoesCustomerRejectPostcodeThatIsTooLong)
{
    Customer customer(exampleCustomer);

    std::string testPostcode;
    testPostcode.reserve(Customer::maxPostcodeLength + 1);
    while (testPostcode.size() < Customer::maxPostcodeLength + 1) testPostcode += 'a';

    EXPECT_THROW(customer.setPostcode(testPostcode.c_str()), std::runtime_error)
            << "Exception was not thrown when the postcode was set to a string that is longer than the maximum";
}

// Does Customer Accept Postcode That Has Valid Format
TEST_F(CustomerTest, DoesCustomerAcceptPostcodeThatHasValidFormat)
{
    Customer customer(exampleCustomer);

    // An array of valid postcodes to test
    const char * postcodes[] = {
        "SA65 9RL", "BS39 4DJ", "WD6 2FN", "TQ7 4LL", "CV8 3DD", "S13 7RG", "CO13 0QE", "UB8 9TG", "WS3 2JG", "WV7 3NF",
        "IV14 9BH", "CF99 1NA", "OL14 6PD", "LA21 8ER", "HP8 4PD", "PA68 6EL", "IV16 9YX", "WR9 8BH", "B66 1NS",
        "END"
    };

    // Loop through and check each postcode. An exception should not be thrown
    for (unsigned i = 0; strcmp(postcodes[i], "END") != 0; ++i)
    {
        EXPECT_NO_THROW(customer.setPostcode(postcodes[i]))
                << "Exception was thrown when the postcode was set to a string with a valid postcode format";
    }
}

// Does Customer Reject Postcode That Has Invalid Format
TEST_F(CustomerTest, DoesCustomerRejectPostcodeThatHasInvalidFormat)
{
    Customer customer(exampleCustomer);

    // An array of invalid postcodes to test
    const char * postcodes[] = {
        "SA659RL", "B439 4DJ", "976 232", "T17 4LL", "CV8 3", "S13 72G", "CO13 075", "2546783", "WS3 ", "WV7321",
        "IV14 9B3", "CF99-1NA", "0014 6PD", "3A21 8ER", "HP84PD", " 6EL", "IV16/9YX", "!R9 8BH", "B-6 1NS",
        "END"
    };

    // Loop through and check each postcode. An exception should be thrown
    for (unsigned i = 0; strcmp(postcodes[i], "END") != 0; ++i)
    {
        EXPECT_THROW(customer.setPostcode(postcodes[i]), std::runtime_error)
                << "Exception was not thrown when the postcode was set to a string with an invalid postcode format";
    }
}


// Does Customer Reject Empty Home Phone Number
TEST_F(CustomerTest, DoesCustomerRejectEmptyHomePhoneNumber)
{
    // Create a customer based on the example data, and then attempt to set the home phone number to an empty string.
    // An exception should be thrown
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setHomePhoneNumber(""), std::runtime_error)
            << "Exception was not thrown when the home phone number was set to an empty string";
}

// Does Customer Reject Home Phone Number That Is Too Short
TEST_F(CustomerTest, DoesCustomerRejectHomePhoneNumberThatIsTooShort)
{
    // Create a customer based on the example data, and then attempt to set the home phone number to a string that is
    // shorter than the minimum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    std::string testHomePhoneNumber;
    testHomePhoneNumber.reserve(Customer::minPhoneNumberLength - 1);
    while (testHomePhoneNumber.size() < Customer::minPhoneNumberLength - 1) testHomePhoneNumber += '0';

    EXPECT_THROW(customer.setHomePhoneNumber(testHomePhoneNumber.c_str()), std::runtime_error)
            << "Exception was not thrown when the home phone number was set to a string that is shorter than the "
               "minimum";
}

// Does Customer Reject Home Phone Number That Is Too Long
TEST_F(CustomerTest, DoesCustomerRejectHomePhoneNumberThatIsTooLong)
{
    // Create a customer based on the example data, and then attempt to set the home phone number to a string that is
    // longer than the maximum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    std::string testHomePhoneNumber;
    testHomePhoneNumber.reserve(Customer::maxPhoneNumberLength + 1);
    while (testHomePhoneNumber.size() < Customer::maxPhoneNumberLength + 1) testHomePhoneNumber += '0';

    EXPECT_THROW(customer.setHomePhoneNumber(testHomePhoneNumber.c_str()), std::runtime_error)
            << "Exception was not thrown when the home phone number was set to a string that is longer than the "
               "maximum";
}

// Does Customer Accept Home Phone Number That Has Valid Format
TEST_F(CustomerTest, DoesCustomerAcceptHomePhoneNumberThatHasValidFormat)
{
    Customer customer(exampleCustomer);

    // An array of valid phone numbers to test
    const char * phoneNumbers[] = {
        "01234567891", "95741358901", "95410654980", "01252496417", "98405456397", "02345287974", "01547786511",
        "01647954012", "97658412615",
        "END"
    };

    // Loop through and check each phone number. An exception should not be thrown
    for (unsigned i = 0; strcmp(phoneNumbers[i], "END") != 0; ++i)
    {
        EXPECT_NO_THROW(customer.setHomePhoneNumber(phoneNumbers[i]))
                << "Exception was thrown when the home phone number was set to a string with a valid phone number"
                   "format";
    }
}

// Does Customer Reject Home Phone Number That Has Invalid Format
TEST_F(CustomerTest, DoesCustomerRejectHomePhoneNumberThatHasInvalidFormat)
{
    Customer customer(exampleCustomer);

    // An array of invalid phone numbers to test
    const char * phoneNumbers[] = {
        "012345f7891", "9qw41358901", "9510654980", "012524964", "p8405456397", "02-45287974", "0/54778/511",
        "01e6479$$12", "phonenumber",
        "END"
    };

    // Loop through and check each phone number. An exception should not be thrown
    for (unsigned i = 0; strcmp(phoneNumbers[i], "END") != 0; ++i)
    {
        EXPECT_THROW(customer.setHomePhoneNumber(phoneNumbers[i]), std::runtime_error)
                << "Exception was not thrown when the home phone number was set to a string with an invalid phone "
                   "number format";
    }
}


// Does Customer Accept Empty Mobile Phone Number
TEST_F(CustomerTest, DoesCustomerAcceptEmptyMobilePhoneNumber)
{
    // Create a customer based on the example data, and then attempt to set the mobile phone number to an empty string.
    // An exception should not be thrown because the mobile phone number field is optional
    Customer customer(exampleCustomer);
    EXPECT_NO_THROW(customer.setMobilePhoneNumber(""))
            << "Exception was thrown when the mobile phone number was set to an empty string";
}

// Does Customer Reject Mobile Phone Number That Is Too Short
TEST_F(CustomerTest, DoesCustomerRejectMobilePhoneNumberThatIsTooShort)
{
    // Create a customer based on the example data, and then attempt to set the mobile phone number to a string that is
    // shorter than the minimum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    std::string testMobilePhoneNumber;
    testMobilePhoneNumber.reserve(Customer::minPhoneNumberLength - 1);
    while (testMobilePhoneNumber.size() < Customer::minPhoneNumberLength - 1) testMobilePhoneNumber += '0';

    EXPECT_THROW(customer.setMobilePhoneNumber(testMobilePhoneNumber.c_str()), std::runtime_error)
            << "Exception was not thrown when the mobile phone number was set to a string that is shorter than the "
               "minimum";
}

// Does Customer Reject Mobile Phone Number That Is Too Long
TEST_F(CustomerTest, DoesCustomerRejectMobilePhoneNumberThatIsTooLong)
{
    // Create a customer based on the example data, and then attempt to set the mobile phone number to a string that is
    // longer than the maximum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    std::string testMobilePhoneNumber;
    testMobilePhoneNumber.reserve(Customer::maxPhoneNumberLength + 1);
    while (testMobilePhoneNumber.size() < Customer::maxPhoneNumberLength + 1) testMobilePhoneNumber += '0';

    EXPECT_THROW(customer.setMobilePhoneNumber(testMobilePhoneNumber.c_str()), std::runtime_error)
            << "Exception was not thrown when the mobile phone number was set to a string that is longer than the "
               "maximum";
}

// Does Customer Accept Mobile Phone Number That Has Valid Format
TEST_F(CustomerTest, DoesCustomerAcceptMobilePhoneNumberThatHasValidFormat)
{
    Customer customer(exampleCustomer);

    // An array of valid phone numbers to test
    const char * phoneNumbers[] = {
        "01234567891", "95741358901", "95410654980", "01252496417", "98405456397", "02345287974", "01547786511",
        "01647954012", "97658412615",
        "END"
    };

    // Loop through and check each phone number. An exception should not be thrown
    for (unsigned i = 0; strcmp(phoneNumbers[i], "END") != 0; ++i)
    {
        EXPECT_NO_THROW(customer.setMobilePhoneNumber(phoneNumbers[i]))
                << "Exception was thrown when the mobile phone number was set to a string with a valid phone number"
                   "format";
    }
}

// Does Customer Reject Mobile Phone Number That Has Invalid Format
TEST_F(CustomerTest, DoesCustomerRejectMobilePhoneNumberThatHasInvalidFormat)
{
    Customer customer(exampleCustomer);

    // An array of invalid phone numbers to test
    const char * phoneNumbers[] = {
        "012345f7891", "9qw41358901", "9510654980", "012524964", "p8405456397", "02-45287974", "0/54778/511",
        "01e6479$$12", "phonenumber",
        "END"
    };

    // Loop through and check each phone number. An exception should not be thrown
    for (unsigned i = 0; strcmp(phoneNumbers[i], "END") != 0; ++i)
    {
        EXPECT_THROW(customer.setMobilePhoneNumber(phoneNumbers[i]), std::runtime_error)
                << "Exception was not thrown when the mobile phone number was set to a string with an invalid phone "
                   "number format";
    }
}


// Does Customer Reject Empty Email Address
TEST_F(CustomerTest, DoesCustomerRejectEmptyEmailAddress)
{
    // Create a customer based on the example data, and then attempt to set the email address to an empty string.
    // An exception should be thrown
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setEmailAddress(""), std::runtime_error)
            << "Exception was not thrown when the email address was set to an empty string";
}

// Does Customer Reject Email Address That Is Too Short
TEST_F(CustomerTest, DoesCustomerRejectEmailAddressThatIsTooShort)
{
    // Create a customer based on the example data, and then attempt to set the email address to a string that is
    // shorter than the minimum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    std::string testEmailAddress = "a@b.";
    testEmailAddress.reserve(Customer::minEmailAddressLength - 1);
    while (testEmailAddress.size() < Customer::minEmailAddressLength - 1) testEmailAddress += 'a';

    EXPECT_THROW(customer.setEmailAddress(testEmailAddress.c_str()), std::runtime_error)
            << "Exception was not thrown when the email address was set to a string that is shorter than the minimum";
}

// Does Customer Reject Email Address That Is Too Long
TEST_F(CustomerTest, DoesCustomerRejectEmailAddressThatIsTooLong)
{
    // Create a customer based on the example data, and then attempt to set the email address to a string that is
    // longer than the maximum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    std::string testEmailAddress;
    testEmailAddress.reserve(Customer::maxEmailAddressLength + 1);
    while (testEmailAddress.size() < Customer::maxEmailAddressLength / 2) testEmailAddress += 'a';
    testEmailAddress += '@';
    while (testEmailAddress.size() < Customer::maxEmailAddressLength - 3) testEmailAddress += 'a';
    testEmailAddress += ".com";

    EXPECT_THROW(customer.setEmailAddress(testEmailAddress.c_str()), std::runtime_error)
            << "Exception was not thrown when the email address was set to a string that is longer than the maximum";
}

// Does Customer Accept Email Address That Has Valid Format
TEST_F(CustomerTest, DoesCustomerAcceptEmailAddressThatHasValidFormat)
{
    Customer customer(exampleCustomer);

    // An array of valid email addresses to test
    const char * emailAddresses[] = {
        "example@example.com", "example@example.uk", "example@example.co.uk", "example@example.net", "a@b.uk",
        "abcdefghijklmnopqrstuvwxyz",
        "Loremipsumdolorsitametconsecteturadipiscingelitposuere.radipiscin@" // one large 128 character email
        "Loremipsumdolorsitametconsecteturadipiscingelp.radipiscina.com",    // (i.e. extreme data)
        "END"
    };

    // Loop through and check each phone number. An exception should not be thrown
    for (unsigned i = 0; strcmp(emailAddresses[i], "END") != 0; ++i)
    {
        EXPECT_NO_THROW(customer.setEmailAddress(emailAddresses[i]))
                << "Exception was thrown when the email address was set to a string with a valid email address format";
    }
}

// Does Customer Reject Email Address That Has Invalid Format
TEST_F(CustomerTest, DoesCustomerRejectEmailAddressThatHasInvalidFormat)
{
    Customer customer(exampleCustomer);

    // An array of invalid email addresses to test
    const char * emailAddresses[] = {
        "example.com", "example@example", "example.example.com", "@example", "@example.co.uk", "example",
        "END"
    };

    // Loop through and check each email address. An exception should not be thrown
    for (unsigned i = 0; strcmp(emailAddresses[i], "END") != 0; ++i)
    {
        EXPECT_THROW(customer.setEmailAddress(emailAddresses[i]), std::runtime_error)
                << "Exception was not thrown when the email address was set to a string with an invalid emai address"
                   "format";
    }
}

// Does Customer FieldCompare Member Function Work Correctly
TEST_F(CustomerTest, DoesCustomerFieldCompareMemberFunctionWorkCorrectly)
{
    Customer lhs(exampleCustomer), rhs(exampleCustomer);
    EXPECT_TRUE(lhs.fieldCompare(rhs));
}

// Does Customer Read And Write To File Correctly
TEST_F(CustomerTest, DoesCustomerReadAndWriteToFileCorrectly)
{
    Customer customer(exampleCustomer);
    const char * fileName = "DoesCustomerReadAndWriteToFileCorrectly.dat.test";

    { // Write the customer to a new file
        fstream outFile;
        outFile.open(fileName, ios::out | ios::binary);
        if (outFile.is_open())
        {
            customer.writeToFile(outFile);
            outFile.close();
        }
        else FAIL() << "File to write test customer to could not be created";
    }

    { // Read the customer back in and check if the customer matches the original
        fstream inFile;
        inFile.open(fileName);
        if (inFile.is_open(), ios::in | ios::binary)
        {
            Customer tempCustomer;
            tempCustomer.readFromFile(inFile);
            EXPECT_TRUE(tempCustomer.fieldCompare(customer));
            inFile.close();
        }
        else ADD_FAILURE() << "File to write test customer to could not be opened";
        remove(fileName);
    }
}

#endif // COMPILE_TESTS
