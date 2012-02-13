/*
 * CustomerDatabase_testing.cc
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include <stdexcept>
using namespace std;

#include "Customer.h"
#include "Database.h"
#include "Utils.h"

class CustomerDatabaseIntegrationTest : public ::testing::Test
{
protected:
    Database<Customer> * const database;
    const Customer exampleCustomer;

    CustomerDatabaseIntegrationTest()
        : database(new Database<Customer>(true)),
          exampleCustomer("John", "Doe", "123 Example Lane", "Example Village", "Exampleville", "AB12 3CD",
              "01234567890", "07012345678", "john.doe@example.com") {}

    virtual void SetUp()
    {
        // Put some records in the database to make the tests a bit more realistic
        Customer customer(exampleCustomer);
        string customerForename(exampleCustomer.getForename());
        for (unsigned i = 0; i < 20; ++i)
        {
            customer.setForename((customerForename + toString(i)).c_str());
            database->addRecord(customer);
        }
    }

    virtual void TearDown()
    {
        std::string filename = database->filename();
        delete database;
        remove(filename.c_str());
    }
};

// Does Database Add Valid Customer Record
TEST_F(CustomerDatabaseIntegrationTest, DoesDatabaseAddValidCustomerRecord)
{
    Customer customer(exampleCustomer);

    unsigned recordCountBefore;
    try { recordCountBefore = database->recordCount(); } // Throws an exception if the database file couldn't be found
    catch (const std::exception & e) { FAIL() << e.what(); }

    // The customer should pass validation, so no exception should be thrown
    EXPECT_NO_THROW(database->addRecord(customer))
            << "An exception was thrown when a valid customer was added to the database";

    // The customer ID should be set to a value >= 0 when it is added to the database
    EXPECT_NE(-1, customer.getId())
            << "The customer record ID was not set when it was added to the database";

    // The number of records in the database should have increased by one
    unsigned recordCountAfter;
    try { recordCountAfter = database->recordCount(); } // Throws an exception if the database file couldn't be found
    catch (const std::exception & e) { FAIL() << e.what(); }
    EXPECT_EQ(recordCountBefore + 1, recordCountAfter)
            << "The record count did not increase by one when a valid customer record was added";
}

// Does Database Reject Invalid Customer Record
TEST_F(CustomerDatabaseIntegrationTest, DoesDatabaseRejectInvalidCustomerRecord)
{
    Customer customer;

    unsigned recordCountBefore;
    try { recordCountBefore = database->recordCount(); } // Throws an exception if the database file couldn't be found
    catch (const std::exception & e) { FAIL() << e.what(); }

    // Invalid records should fail validation and an exception should be thrown
    EXPECT_THROW(database->addRecord(customer), std::runtime_error)
            << "An exception was not thrown when an invalid customer was added to the database";

    // Since the customer was not added to the database (hopefully) then the record ID should still be -1
    EXPECT_EQ(-1, customer.getId())
            << "The invalid customer record ID was set to a value >= 0";


    // The number of records in the database should have stayed the same
    unsigned recordCountAfter;
    try { recordCountAfter = database->recordCount(); } catch (const std::exception & e) { FAIL() << e.what(); }
    EXPECT_EQ(recordCountBefore, recordCountAfter)
            << "The record count changed when attempting to add an invalid customer to the database";
}

// Does Database Update Valid Customer Record
TEST_F(CustomerDatabaseIntegrationTest, DoesDatabaseUpdateValidCustomerRecord)
{
    // Get a record from the database, Exceptions may be thrown if the database file can't be found.
    // Bear in mind that the value passed to Database::recordAt is NOT a record ID, but rather a position in the file
    Customer customer;
    try { customer = database->recordAt(0); } catch (const std::exception & e) { FAIL() << e.what(); }

    // Change a field in the customer
    try { customer.setForename(exampleCustomer.getForename()); } // Validation shouldn't throw, but just in case
    catch (const std::exception & e) { FAIL() << e.what(); }

    // Record should be updated without failing
    try { EXPECT_TRUE(database->updateRecord(customer)) << "The record was not updated"; }
    catch (const std::exception & e) { FAIL() << e.what(); }

    // Get the customer back from the same position in the database. The customer should match the original updated one
    Customer tempCustomer;
    try { tempCustomer = database->recordAt(0); } catch (const std::exception & e) { FAIL() << e.what(); }
    EXPECT_TRUE(customer.completeCompare(tempCustomer))
            << "The customer was not updated/saved/read from the database properly";
}

// Does Database Reject Update Of Invalid Customer Record
// Making a valid record taken from the database invalid is impossible due to the record validation.
// Instead, test if a valid record that has not been taken from the database is rejected (i.e. is invalid for updating)
TEST_F(CustomerDatabaseIntegrationTest, DoesDatabaseRejectUpdateOfInvalidCustomerRecord)
{
    Customer customer(exampleCustomer);
    EXPECT_THROW(database->updateRecord(customer), std::runtime_error)
            << "An exception was not thrown when a record with ID < 0 was attempting to be updated in the database";
}

// Does Database Find Single Records
TEST_F(CustomerDatabaseIntegrationTest, DoesDatabaseFindRecords)
{
    // Create a customer and add it to the database
    Customer customer(exampleCustomer);
    const char * forename = "Niklaus", * surname = "Wirth";
    try
    {
        customer.setForename(forename);
        customer.setSurname(surname);
        database->addRecord(customer);
    }
    catch (const std::exception & e) { FAIL() << e.what(); }

    // Try to find the record in different ways, and compare with the original
    try
    {
        Customer tempCustomer = database->findRecord("id", customer.getId());
        EXPECT_TRUE(customer.completeCompare(tempCustomer))
                << "A customer that had been added to the database was not found by its ID field";

        tempCustomer = database->findRecord("forename", forename);
        EXPECT_TRUE(customer.completeCompare(tempCustomer))
                << "A customer that had been added to the database was not found by its Forename field";

        tempCustomer = database->findRecord("surname", surname);
        EXPECT_TRUE(customer.completeCompare(tempCustomer))
                << "A customer that had been added to the database was not found by its Surname field";
    }
    catch (const std::exception & e) { FAIL() << e.what(); }
}

// A helper function for tests that removes matching records between two lists
void removeMatches(std::vector<Customer> & listA, std::vector<Customer> & listB)
{
    for (unsigned i = 0; i < listA.size(); ++i)
    {
        bool match = false;
        for (unsigned j = 0; j < listB.size(); ++j)
        {
            if (listA[i].completeCompare(listB[j]))
            {
                listB.erase(listB.begin() + j);
                match = true;
                break;
            }
        }
        if (match)
        {
            listA.erase(listA.begin() + i);
            --i; // decrement i to counter the increment before next iteration, because the list size has decreased
        }
    }
}

// Does Database Find Multiple Records
TEST_F(CustomerDatabaseIntegrationTest, DoesDatabaseFindMultipleRecords)
{
    // Create some customers that all live in the same town, put them in a std::vector (basically a smart array)
    // and add them to the database
    const unsigned customerCount = 10;
    std::vector<Customer> customerList;
    customerList.reserve(customerCount);

    Customer customer(exampleCustomer);
    const char * town = "Watford";
    try { customer.setTown(town); } catch (const std::exception & e) { FAIL() << e.what(); }
    for (unsigned i = 0; i < customerCount; ++i)
    {
        try
        {
            customer.setForename(("Bill" + toString(i)).c_str());
            database->addRecord(customer);
        }
        catch (const std::exception & e) { FAIL() << e.what(); }
        customerList.push_back(customer);
    }

    // Get all the records whose Town field matches
    Database<Customer>::recordListPtr recordList;
    try { recordList = database->findRecords("town", town); } catch (const std::exception & e) { FAIL() << e.what(); }

    // The record counts in each list should be equal
    EXPECT_EQ(customerList.size(), recordList->size())
            << "The wrong number of records were found in the database";

    // Loop through the lists, removing any matches. Each list should end up being empty
    removeMatches(customerList, *recordList);

    EXPECT_EQ(0, customerList.size())
            << "The records found in the database do not match the records added to the database";
    EXPECT_EQ(0, recordList->size())
            << "The records found in the database do not match the records added to the database";
}

// Do Database Record List Helpers Work
TEST_F(CustomerDatabaseIntegrationTest, DoDatabaseRecordListHelpersWork)
{
    // Add some customers from the same town to the database, like the previous test
    const unsigned customerCount = 10;
    std::vector<Customer> customerList;
    customerList.reserve(customerCount);

    Customer customer(exampleCustomer);
    const char * town = "Watford";
    try { customer.setTown(town); } catch (const std::exception & e) { FAIL() << e.what(); }
    for (unsigned i = 0; i < customerCount; ++i)
    {
        try
        {
            customer.setForename(("Bill" + toString(i)).c_str());
            database->addRecord(customer);
        }
        catch (const std::exception & e) { FAIL() << e.what(); }
        customerList.push_back(customer);
    }

    // Get a list of all the records in the database, and find all the records whose Town field matches.
    // Check if the resulting list matches the original by removing matches (same method as last test)
    try
    {
        Database<Customer>::recordListPtr recordList(database->allRecords());
        Database<Customer>::recordListPtr newRecordList(database->findRecords(*recordList, "town", town));

        std::vector<Customer> tempCustomerList(customerList); // Make a copy of the original list; we'll need it later
        removeMatches(tempCustomerList, *newRecordList);
        EXPECT_EQ(0, tempCustomerList.size())
                << "The records found in the database do not match the records added to the database";
        EXPECT_EQ(0, newRecordList->size())
                << "The records found in the database do not match the records added to the database";
    }
    catch (const std::exception & e) { FAIL() << e.what(); }

    // Get a list of all the records in the database, and keep all the records whose Town field matches.
    // Check if the resulting list matches the original by removing matches
    try
    {
        Database<Customer>::recordListPtr recordList(database->allRecords());
        database->keepRecords(*recordList, "town", town);

        std::vector<Customer> tempCustomerList(customerList);
        removeMatches(tempCustomerList, *recordList);
        EXPECT_EQ(0, tempCustomerList.size())
                << "The records found in the database do not match the records added to the database";
        EXPECT_EQ(0, recordList->size())
                << "The records found in the database do not match the records added to the database";
    }
    catch (const std::exception & e) { FAIL() << e.what(); }

    // Get a list of all the records in the database, and remove all the records whose Town field matches.
    // The resulting list should have no matches with the original
    try
    {
        Database<Customer>::recordListPtr recordList(database->allRecords());
        database->removeRecords(*recordList, "town", town);

        const unsigned customerListSizeBefore = customerList.size(), recordListSizeBefore = recordList->size();
        removeMatches(customerList, *recordList); // No need to use a copy, this is the last time it's needed
        EXPECT_EQ(customerListSizeBefore, customerList.size())
                << "The records found in the database do not match the records added to the database";
        EXPECT_EQ(recordListSizeBefore, recordList->size())
                << "The records found in the database do not match the records added to the database";
    }
    catch (const std::exception & e) { FAIL() << e.what(); }
}

// Does Database Delete Records
TEST_F(CustomerDatabaseIntegrationTest, DoesDatabaseDeleteRecordCorrectly)
{
    // Get a customer record
    Customer customer;
    try { customer = database->recordAt(0); } catch (const std::exception & e) { FAIL() << e.what(); }

    // Get a record count before record deletion
    unsigned recordCountBefore;
    try { recordCountBefore = database->recordCount(); } catch (const std::exception & e) { FAIL() << e.what(); }

    // Delete it from database
    try { EXPECT_TRUE(database->deleteRecord(customer.getId())) << "The customer record could not be deleted"; }
    catch (const std::exception & e) { FAIL() << e.what(); }

    // Get a record count after record deletion. The record count now should be one less than before
    unsigned recordCountAfter;
    try { recordCountAfter = database->recordCount(); } catch (const std::exception & e) { FAIL() << e.what(); }
    EXPECT_EQ(recordCountBefore - 1, recordCountAfter)
            << "The record count did not decrease by one when a customer record was deleted";

    // Try to find the record in the database. The record returned should have an ID less than 0
    Customer tempCustomer;
    try { tempCustomer = database->findRecord("id", customer.getId()); }
    catch (const std::exception & e) { FAIL() << e.what(); }
    EXPECT_EQ(-1, tempCustomer.getId())
            << "A record that had been deleted from the database was found somehow";

    // Now let's try to delete the record again. The Database::deleteRecord should return false
    try
    {
        EXPECT_FALSE(database->deleteRecord(customer.getId()))
                << "A record that had already been deleted from the database was deleted again somehow";
    }
    catch (const std::exception & e) { FAIL() << e.what(); }
}

// Does Database Fail Updating Record That No Longer Exists
TEST_F(CustomerDatabaseIntegrationTest, DoesDatabaseFailUpdatingRecordThatNoLongerExists)
{
    Customer customer;
    try { customer = database->recordAt(0); } catch (const std::exception & e) { FAIL() << e.what(); }

    // Database::deleteRecord takes an ID rather than file position, unlike Database::recordAt
    try { EXPECT_TRUE(database->deleteRecord(customer.getId())) << "The customer record could not be deleted"; }
    catch (const std::exception & e) { FAIL() << e.what(); }

    // Change a field in the customer
    try { customer.setForename(exampleCustomer.getForename()); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    // The database shouldn't throw an exception in this case, because not being able to find a record isn't an
    // exceptional circumstance. It will throw if the database file couldn't be found, however
    try
    {
        EXPECT_FALSE(database->updateRecord(customer))
                << "A record that had been deleted from the database was updated somehow";
    }
    catch (const std::exception & e) { FAIL() << e.what(); }
}

#endif // COMPILE_TESTS
