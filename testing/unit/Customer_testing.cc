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

#include "testing/TestingHelpers.hpp"

#include "Customer.h"

class CustomerUnitTest : public ::testing::Test
{
protected:
    const Customer exampleCustomer;

    CustomerUnitTest()
        : exampleCustomer("John", "Doe", "123 Example Lane", "Example Village", "Exampleville", "AB12 3CD",
              "01234567890", "07012345678", "john.doe@example.com") {}
};

// Is Customer ID Minus One
// A new customer record must have an ID of -1 (a customer record ID will be set when it is added to the database)
TEST_F(CustomerUnitTest, IsCustomerIdMinusOne)
{
    Customer customer;
    EXPECT_EQ(-1, customer.getId()) << "Customer record that has not been added to the database must have an ID of -1";
}


// Does Customer Reject Empty Forename
// Records should automatically validate data passed to a setter method, and throw an exception if the data is invalid
TEST_F(CustomerUnitTest, DoesCustomerRejectEmptyForename)
{
    // Create a customer based on the example data, and then attempt to set the forename to an empty string.
    // An exception should be thrown
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setForename(""), std::runtime_error)
            << "Exception was not thrown when the forename was set to an empty string";
}

// Does Customer Reject Forename That Is Too Short
TEST_F(CustomerUnitTest, DoesCustomerRejectForenameThatIsTooShort)
{
    // Create a customer based on the example data, and then attempt to set the forename to a string shorter than the
    // minimum length. An exception should be thrown
    Customer customer(exampleCustomer);

    string testForename = createTestStringOfSize(Customer::minNameLength - 1);
    EXPECT_THROW(customer.setForename(testForename.c_str()), std::runtime_error)
            << "Exception was not thrown when the forename was set to a string that is shorter than the minimum";
}

// Does Customer Reject Forename That Is Too Long
TEST_F(CustomerUnitTest, DoesCustomerRejectForenameThatIsTooLong)
{
    // Create a customer based on the example data, and then attempt to set the forename to a string that is longer
    // than the maximum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    string testForename = createTestStringOfSize(Customer::maxNameLength + 1);
    EXPECT_THROW(customer.setForename(testForename.c_str()), std::runtime_error)
            << "Exception was not thrown when the forename was set to a string that is longer than the maximum";
}

// Does Customer Accept Valid Forename
TEST_F(CustomerUnitTest, DoesCustomerAcceptValidForename)
{
    Customer customer(exampleCustomer);

    string testForename = createTestStringOfSize(Customer::maxNameLength / 2);
    EXPECT_NO_THROW(customer.setForename(testForename.c_str()))
            << "Exception was thrown when the forename was set to a valid forename string";
}

//Does Customer Accept Extreme Valid Forename (Lower Bound)
TEST_F(CustomerUnitTest, DoesCustomerAcceptExtremeValidForenameLowerBound)
{
    Customer customer(exampleCustomer);

    string testForename = createTestStringOfSize(Customer::minNameLength);
    EXPECT_NO_THROW(customer.setForename(testForename.c_str()))
            << "Exception was thrown when the forename was set to a valid forename string";
}

//Does Customer Accept Extreme Valid Forename (Upper Bound)
TEST_F(CustomerUnitTest, DoesCustomerAcceptExtremeValidForenameUpperBound)
{
    Customer customer(exampleCustomer);

    string testForename = createTestStringOfSize(Customer::maxNameLength);
    EXPECT_NO_THROW(customer.setForename(testForename.c_str()))
            << "Exception was thrown when the forename was set to a valid forename string";
}


// Does Customer Reject Empty Surname
TEST_F(CustomerUnitTest, DoesCustomerRejectEmptySurname)
{
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setSurname(""), std::runtime_error)
            << "Exception was not thrown when the surname was set to an empty string";
}

// Does Customer Reject Surname That Is Too Short
TEST_F(CustomerUnitTest, DoesCustomerRejectSurnameThatIsTooShort)
{
    Customer customer(exampleCustomer);

    string testSurname = createTestStringOfSize(Customer::minNameLength - 1);
    EXPECT_THROW(customer.setSurname(testSurname.c_str()), std::runtime_error)
            << "Exception was not thrown when the surname was set to a string that is shorter than the minimum";
}

// Does Customer Reject Surname That Is Too Long
TEST_F(CustomerUnitTest, DoesCustomerRejectSurnameThatIsTooLong)
{
    Customer customer(exampleCustomer);

    string testSurname = createTestStringOfSize(Customer::maxNameLength + 1);
    EXPECT_THROW(customer.setSurname(testSurname.c_str()), std::runtime_error)
            << "Exception was not thrown when the surname was set to a string that is longer than the maximum";
}

// Does Customer Accept Valid Surname
TEST_F(CustomerUnitTest, DoesCustomerAcceptValidSurname)
{
    Customer customer(exampleCustomer);

    string testSurname = createTestStringOfSize(Customer::maxNameLength / 2);
    EXPECT_NO_THROW(customer.setSurname(testSurname.c_str()))
            << "Exception was thrown when the surname was set to a valid surname string";
}

//Does Customer Accept Extreme Valid Surname (Lower Bound)
TEST_F(CustomerUnitTest, DoesCustomerAcceptExtremeValidSurnameLowerBound)
{
    Customer customer(exampleCustomer);

    string testSurname = createTestStringOfSize(Customer::minNameLength);
    EXPECT_NO_THROW(customer.setSurname(testSurname.c_str()))
            << "Exception was thrown when the surname was set to a valid surname string";
}

//Does Customer Accept Extreme Valid Surname (Upper Bound)
TEST_F(CustomerUnitTest, DoesCustomerAcceptExtremeValidSurnameUpperBound)
{
    Customer customer(exampleCustomer);

    string testSurname = createTestStringOfSize(Customer::maxNameLength);
    EXPECT_NO_THROW(customer.setSurname(testSurname.c_str()))
            << "Exception was thrown when the surname was set to a valid surname string";
}


// Does Customer Reject Empty Address Line 1
TEST_F(CustomerUnitTest, DoesCustomerRejectEmptyAddressLine1)
{
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setAddressLine1(""), std::runtime_error)
            << "Exception was not thrown when the address line 1 was set to an empty string";
}

// Does Customer Reject Address Line 1 That Is Too Short
TEST_F(CustomerUnitTest, DoesCustomerRejectAddressLine1ThatIsTooShort)
{
    Customer customer(exampleCustomer);

    string testAddressLine1 = createTestStringOfSize(Customer::minAddressLineLength - 1);
    EXPECT_THROW(customer.setAddressLine1(testAddressLine1.c_str()), std::runtime_error)
            << "Exception was not thrown when the address line 1 was set to a string that is shorter than the maximum";
}

// Does Customer Reject Address Line 1 That Is Too Long
TEST_F(CustomerUnitTest, DoesCustomerRejectAddressLine1ThatIsTooLong)
{
    Customer customer(exampleCustomer);

    string testAddressLine1 = createTestStringOfSize(Customer::maxAddressLineLength + 1);
    EXPECT_THROW(customer.setAddressLine1(testAddressLine1.c_str()), std::runtime_error)
            << "Exception was not thrown when the address line 1 was set to a string that is longer than the maximum";
}

// Does Customer Accept Valid Address Line 1
TEST_F(CustomerUnitTest, DoesCustomerAcceptValidAddressLine1)
{
    Customer customer(exampleCustomer);

    string testAddressLine1 = createTestStringOfSize(Customer::maxAddressLineLength / 2);
    EXPECT_NO_THROW(customer.setAddressLine1(testAddressLine1.c_str()))
            << "Exception was thrown when the address line 1 was set to a valid address line string";
}

//Does Customer Accept Extreme Valid Address Line 1 (Lower Bound)
TEST_F(CustomerUnitTest, DoesCustomerAcceptExtremeValidAddressLine1LowerBound)
{
    Customer customer(exampleCustomer);

    string testAddressLine1 = createTestStringOfSize(Customer::minAddressLineLength);
    EXPECT_NO_THROW(customer.setAddressLine1(testAddressLine1.c_str()))
            << "Exception was thrown when the address line 1 was set to a valid address line string";
}

//Does Customer Accept Extreme Valid Address Line 1 (Upper Bound)
TEST_F(CustomerUnitTest, DoesCustomerAcceptExtremeValidAddressLine1UpperBound)
{
    Customer customer(exampleCustomer);

    string testAddressLine1 = createTestStringOfSize(Customer::maxAddressLineLength);
    EXPECT_NO_THROW(customer.setAddressLine1(testAddressLine1.c_str()))
            << "Exception was thrown when the address line 1 was set to a valid address line string";
}

// Does Customer Accept Empty Address Line 2
// The address line 2 field is optional
TEST_F(CustomerUnitTest, DoesCustomerAcceptEmptyAddressLine2)
{
    Customer customer(exampleCustomer);
    EXPECT_NO_THROW(customer.setAddressLine2(""))
            << "Exception was thrown when the address line 2 was set to an empty string";
}

// Does Customer Reject Address Line 2 That Is Too Long
TEST_F(CustomerUnitTest, DoesCustomerRejectAddressLine2ThatIsTooLong)
{
    Customer customer(exampleCustomer);

    string testAddressLine2 = createTestStringOfSize(Customer::maxAddressLineLength + 1);
    EXPECT_THROW(customer.setAddressLine2(testAddressLine2.c_str()), std::runtime_error)
            << "Exception was not thrown when the address line 2 was set to a string that is longer than the maximum";
}

// Does Customer Accept Valid Address Line 2
TEST_F(CustomerUnitTest, DoesCustomerAcceptValidAddressLine2)
{
    Customer customer(exampleCustomer);

    string testAddressLine2 = createTestStringOfSize(Customer::maxAddressLineLength / 2);
    EXPECT_NO_THROW(customer.setAddressLine2(testAddressLine2.c_str()))
            << "Exception was thrown when the address line 2 was set to a valid address line string";
}

//Does Customer Accept Extreme Valid Address Line 2 (Upper Bound)
TEST_F(CustomerUnitTest, DoesCustomerAcceptExtremeValidAddressLine2UpperBound)
{
    Customer customer(exampleCustomer);

    string testAddressLine2 = createTestStringOfSize(Customer::maxAddressLineLength);
    EXPECT_NO_THROW(customer.setAddressLine2(testAddressLine2.c_str()))
            << "Exception was thrown when the address line 2 was set to a valid address line string";
}

// Does Customer Reject Empty Town
TEST_F(CustomerUnitTest, DoesCustomerRejectEmptyTown)
{
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setSurname(""), std::runtime_error)
            << "Exception was not thrown when the town was set to an empty string";
}

// Does Customer Reject Town That Is Too Short
TEST_F(CustomerUnitTest, DoesCustomerRejectTownThatIsTooShort)
{
    Customer customer(exampleCustomer);

    string testTown = createTestStringOfSize(Customer::minTownLength - 1);
    EXPECT_THROW(customer.setTown(testTown.c_str()), std::runtime_error)
            << "Exception was not thrown when the town was set to a string that is shorter than the minimum";
}

// Does Customer Reject Town That Is Too Long
TEST_F(CustomerUnitTest, DoesCustomerRejectTownThatIsTooLong)
{
    Customer customer(exampleCustomer);

    string testTown = createTestStringOfSize(Customer::maxTownLength + 1);
    EXPECT_THROW(customer.setTown(testTown.c_str()), std::runtime_error)
            << "Exception was not thrown when the town was set to a string that is longer than the maximum";
}

// Does Customer Accept Valid Town
TEST_F(CustomerUnitTest, DoesCustomerAcceptValidTown)
{
    Customer customer(exampleCustomer);

    string testTown = createTestStringOfSize(Customer::maxTownLength / 2);
    EXPECT_NO_THROW(customer.setTown(testTown.c_str()))
            << "Exception was thrown when the town was set to a valid town string";
}

//Does Customer Accept Extreme Valid Town (Lower Bound)
TEST_F(CustomerUnitTest, DoesCustomerAcceptExtremeValidTownLowerBound)
{
    Customer customer(exampleCustomer);

    string testTown = createTestStringOfSize(Customer::minTownLength);
    EXPECT_NO_THROW(customer.setTown(testTown.c_str()))
            << "Exception was thrown when the town was set to a valid town string";
}

//Does Customer Accept Extreme Valid Town (Upper Bound)
TEST_F(CustomerUnitTest, DoesCustomerAcceptExtremeValidTownUpperBound)
{
    Customer customer(exampleCustomer);

    string testTown = createTestStringOfSize(Customer::maxTownLength);
    EXPECT_NO_THROW(customer.setTown(testTown.c_str()))
            << "Exception was thrown when the town was set to a valid town string";
}


// Does Customer Reject Empty Postcode
TEST_F(CustomerUnitTest, DoesCustomerRejectEmptyPostcode)
{
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setPostcode(""), std::runtime_error)
            << "Exception was not thrown when the postcode was set to an empty string";
}

// Does Customer Accept Postcode That Has Valid Format
TEST_F(CustomerUnitTest, DoesCustomerAcceptPostcodeThatHasValidFormat)
{
    Customer customer(exampleCustomer);

    // An array of valid postcodes to test
    const char *postcodes[] = {
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
TEST_F(CustomerUnitTest, DoesCustomerRejectPostcodeThatHasInvalidFormat)
{
    Customer customer(exampleCustomer);

    // An array of invalid postcodes to test
    const char *postcodes[] = {
        "SA659RL", "B439 4DJ", "976 232", "CV8 3", "S13 72G", "CO13 075", "2546783", "WS3 ", "WV7321",
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
TEST_F(CustomerUnitTest, DoesCustomerRejectEmptyHomePhoneNumber)
{
    // Create a customer based on the example data, and then attempt to set the home phone number to an empty string.
    // An exception should be thrown
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setHomePhoneNumber(""), std::runtime_error)
            << "Exception was not thrown when the home phone number was set to an empty string";
}

// Does Customer Reject Home Phone Number That Is Too Short
TEST_F(CustomerUnitTest, DoesCustomerRejectHomePhoneNumberThatIsTooShort)
{
    // Create a customer based on the example data, and then attempt to set the home phone number to a string that is
    // shorter than the minimum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    string testHomePhoneNumber;
    testHomePhoneNumber.reserve(Customer::minPhoneNumberLength - 1);
    while (testHomePhoneNumber.size() < Customer::minPhoneNumberLength - 1) testHomePhoneNumber += '0';

    EXPECT_THROW(customer.setHomePhoneNumber(testHomePhoneNumber.c_str()), std::runtime_error)
            << "Exception was not thrown when the home phone number was set to a string that is shorter than the "
               "minimum";
}

// Does Customer Reject Home Phone Number That Is Too Long
TEST_F(CustomerUnitTest, DoesCustomerRejectHomePhoneNumberThatIsTooLong)
{
    // Create a customer based on the example data, and then attempt to set the home phone number to a string that is
    // longer than the maximum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    string testHomePhoneNumber;
    testHomePhoneNumber.reserve(Customer::maxPhoneNumberLength + 1);
    while (testHomePhoneNumber.size() < Customer::maxPhoneNumberLength + 1) testHomePhoneNumber += '0';

    EXPECT_THROW(customer.setHomePhoneNumber(testHomePhoneNumber.c_str()), std::runtime_error)
            << "Exception was not thrown when the home phone number was set to a string that is longer than the "
               "maximum";
}

// Does Customer Accept Home Phone Number That Has Valid Format
TEST_F(CustomerUnitTest, DoesCustomerAcceptHomePhoneNumberThatHasValidFormat)
{
    Customer customer(exampleCustomer);

    // An array of valid phone numbers to test
    const char *phoneNumbers[] = {
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
TEST_F(CustomerUnitTest, DoesCustomerRejectHomePhoneNumberThatHasInvalidFormat)
{
    Customer customer(exampleCustomer);

    // An array of invalid phone numbers to test
    const char *phoneNumbers[] = {
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
TEST_F(CustomerUnitTest, DoesCustomerAcceptEmptyMobilePhoneNumber)
{
    // Create a customer based on the example data, and then attempt to set the mobile phone number to an empty string.
    // An exception should not be thrown because the mobile phone number field is optional
    Customer customer(exampleCustomer);
    EXPECT_NO_THROW(customer.setMobilePhoneNumber(""))
            << "Exception was thrown when the mobile phone number was set to an empty string";
}

// Does Customer Reject Mobile Phone Number That Is Too Short
TEST_F(CustomerUnitTest, DoesCustomerRejectMobilePhoneNumberThatIsTooShort)
{
    // Create a customer based on the example data, and then attempt to set the mobile phone number to a string that is
    // shorter than the minimum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    string testMobilePhoneNumber;
    testMobilePhoneNumber.reserve(Customer::minPhoneNumberLength - 1);
    while (testMobilePhoneNumber.size() < Customer::minPhoneNumberLength - 1) testMobilePhoneNumber += '0';

    EXPECT_THROW(customer.setMobilePhoneNumber(testMobilePhoneNumber.c_str()), std::runtime_error)
            << "Exception was not thrown when the mobile phone number was set to a string that is shorter than the "
               "minimum";
}

// Does Customer Reject Mobile Phone Number That Is Too Long
TEST_F(CustomerUnitTest, DoesCustomerRejectMobilePhoneNumberThatIsTooLong)
{
    // Create a customer based on the example data, and then attempt to set the mobile phone number to a string that is
    // longer than the maximum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    string testMobilePhoneNumber;
    testMobilePhoneNumber.reserve(Customer::maxPhoneNumberLength + 1);
    while (testMobilePhoneNumber.size() < Customer::maxPhoneNumberLength + 1) testMobilePhoneNumber += '0';

    EXPECT_THROW(customer.setMobilePhoneNumber(testMobilePhoneNumber.c_str()), std::runtime_error)
            << "Exception was not thrown when the mobile phone number was set to a string that is longer than the "
               "maximum";
}

// Does Customer Accept Mobile Phone Number That Has Valid Format
TEST_F(CustomerUnitTest, DoesCustomerAcceptMobilePhoneNumberThatHasValidFormat)
{
    Customer customer(exampleCustomer);

    // An array of valid phone numbers to test
    const char *phoneNumbers[] = {
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
TEST_F(CustomerUnitTest, DoesCustomerRejectMobilePhoneNumberThatHasInvalidFormat)
{
    Customer customer(exampleCustomer);

    // An array of invalid phone numbers to test
    const char *phoneNumbers[] = {
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
TEST_F(CustomerUnitTest, DoesCustomerRejectEmptyEmailAddress)
{
    // Create a customer based on the example data, and then attempt to set the email address to an empty string.
    // An exception should be thrown
    Customer customer(exampleCustomer);
    EXPECT_THROW(customer.setEmailAddress(""), std::runtime_error)
            << "Exception was not thrown when the email address was set to an empty string";
}

// Does Customer Reject Email Address That Is Too Short
TEST_F(CustomerUnitTest, DoesCustomerRejectEmailAddressThatIsTooShort)
{
    // Create a customer based on the example data, and then attempt to set the email address to a string that is
    // shorter than the minimum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    string testEmailAddress = "a@b.";
    testEmailAddress.reserve(Customer::minEmailAddressLength - 1);
    while (testEmailAddress.size() < Customer::minEmailAddressLength - 1) testEmailAddress += 'a';

    EXPECT_THROW(customer.setEmailAddress(testEmailAddress.c_str()), std::runtime_error)
            << "Exception was not thrown when the email address was set to a string that is shorter than the minimum";
}

// Does Customer Reject Email Address That Is Too Long
TEST_F(CustomerUnitTest, DoesCustomerRejectEmailAddressThatIsTooLong)
{
    // Create a customer based on the example data, and then attempt to set the email address to a string that is
    // longer than the maximum length allowed. An exception should be thrown
    Customer customer(exampleCustomer);

    string testEmailAddress;
    testEmailAddress.reserve(Customer::maxEmailAddressLength + 1);
    while (testEmailAddress.size() < Customer::maxEmailAddressLength / 2) testEmailAddress += 'a';
    testEmailAddress += '@';
    while (testEmailAddress.size() < Customer::maxEmailAddressLength - 3) testEmailAddress += 'a';
    testEmailAddress += ".com";

    EXPECT_THROW(customer.setEmailAddress(testEmailAddress.c_str()), std::runtime_error)
            << "Exception was not thrown when the email address was set to a string that is longer than the maximum";
}

// Does Customer Accept Email Address That Has Valid Format
TEST_F(CustomerUnitTest, DoesCustomerAcceptEmailAddressThatHasValidFormat)
{
    Customer customer(exampleCustomer);

    // An array of valid email addresses to test
    const char *emailAddresses[] = {
        "example@example.com", "example@example.uk", "example@example.co.uk", "example@example.net", "a@b.uk",
        "loremipsumdolorsitametconsecteturadipiscingelitposuere.radipiscin@" // one large 128 character email
        "loremipsumdolorsitametconsecteturadipiscingelp.radipiscina.com",    // (i.e. extreme data)
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
TEST_F(CustomerUnitTest, DoesCustomerRejectEmailAddressThatHasInvalidFormat)
{
    Customer customer(exampleCustomer);

    // An array of invalid email addresses to test
    const char *emailAddresses[] = {
        "Example@example.com", "example.com", "example@example", "example.example.com", "@example", "@example.co.uk",
        "example",
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
TEST_F(CustomerUnitTest, DoesCustomerFieldCompareMemberFunctionWorkCorrectly)
{
    Customer lhs(exampleCustomer), rhs(exampleCustomer);
    EXPECT_TRUE(lhs.fieldCompare(rhs));
}

// Does Customer Read And Write To File Correctly
TEST_F(CustomerUnitTest, DoesCustomerReadAndWriteToFileCorrectly)
{
    Customer customer(exampleCustomer);
    const char *fileName = "DoesCustomerReadAndWriteToFileCorrectly.dat.test";

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
