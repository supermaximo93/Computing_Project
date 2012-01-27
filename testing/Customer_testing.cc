/*
 * Customer_testing.cc
 *
 *  Created on: 5 Jan 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include <stdexcept>

#include "Database.h"
#include "Customer.h"

class CustomerTest : public ::testing::Test
{
protected:
    Database<Customer> * database;
    const Customer exampleCustomer;

    CustomerTest()
        : exampleCustomer("John", "Doe", "123 Example Lane", "Example Village", "Exampleville", "AB12 3CD",
              "01234567890", "07012345678", "john.doe@example.com") {}

    virtual void SetUp()
    {
        database = new Database<Customer>(true);
    }

    virtual void TearDown()
    {
        delete database;
    }
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

// Is Customer With Empty Name Rejected From Database
// The database should not allow records to be entered when they have invalid data
TEST_F(CustomerTest, IsCustomerWithEmptyNameRejected)
{
    // Create a new customer, setting its data (other than the forename, which is empty by default) to example data.
    // It shouldn't throw validation exceptions
    Customer customer;
    try
    {
        customer.setSurname(exampleCustomer.getSurname());
        customer.setAddressLine1(exampleCustomer.getAddressLine1());
        customer.setAddressLine2(exampleCustomer.getAddressLine2());
        customer.setTown(exampleCustomer.getTown());
        customer.setPostcode(exampleCustomer.getPostcode());
        customer.setHomePhoneNumber(exampleCustomer.getHomePhoneNumber());
        customer.setMobilePhoneNumber(exampleCustomer.getMobilePhoneNumber());
        customer.setEmailAddress(exampleCustomer.getEmailAddress());
    }
    catch (std::runtime_error & error)
    {
        FAIL() << "An exception was thrown when assigning valid example data to customers: " << error.what();
    }

    unsigned recordCountBefore = database->recordCount();

    // An exception should be thrown by the database
    EXPECT_THROW(database->addRecord(customer), std::runtime_error) << "An exception should have been thrown";

    // The database should have the same number of records as before
    EXPECT_EQ(recordCountBefore, database->recordCount()) << "Customer with empty name was accepted into the database";
}

#endif
