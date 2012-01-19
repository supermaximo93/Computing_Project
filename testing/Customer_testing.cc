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
    Customer customer;

    virtual void SetUp()
    {
        database = new Database<Customer>;
    }

    virtual void TearDown()
    {
        delete database;
    }
};

TEST_F(CustomerTest, IsCustomerIdMinusOne)
{
    EXPECT_EQ(-1, customer.getId()) << "Customer record that has not been added to the database must have an ID of -1";
}
