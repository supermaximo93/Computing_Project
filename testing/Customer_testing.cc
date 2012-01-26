/*
 * Customer_testing.cc
 *
 *  Created on: 5 Jan 2012
 *      Author: Max Foster
 */

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
    bool exceptionThrown = false;
    try
    {
        database->addRecord(customer);
    }
    catch(const char  * exception)
    {
        exceptionThrown = true;
        ASSERT_STRCASEEQ(exception, "Customer has an invalid forename (forename is empty)")
                << "The error message 'Customer has an invalid forename (forename is empty)' was not received";
    }

    ASSERT_TRUE(exceptionThrown) << "An exception should have been thrown";
    EXPECT_EQ(recordCountBefore, database->recordCount()) << "Customer with empty name was accepted into the database";
}
