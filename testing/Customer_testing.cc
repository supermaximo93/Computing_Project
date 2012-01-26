/*
 * Customer_testing.cc
 *
 *  Created on: 5 Jan 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

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

// Is Customer With Empty Name Rejected From Database
TEST_F(CustomerTest, IsCustomerWithEmptyNameRejected)
{
    // Create a new customer, setting its data to example data. Then empty the forename field
    Customer customer(exampleCustomer);
    customer.setForename("");

    unsigned recordCountBefore = database->recordCount();

    // An exception should be thrown by the database
    EXPECT_THROW(database->addRecord(customer), const char *) << "An exception should have been thrown";

    // The database should have the same number of records as before
    EXPECT_EQ(recordCountBefore, database->recordCount()) << "Customer with empty name was accepted into the database";
}

#endif
