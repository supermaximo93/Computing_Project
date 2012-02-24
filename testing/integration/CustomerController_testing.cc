/*
 * CustomerController_testing.cc
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include "Databases.h"
#include "CustomerController.h"
#include "Customer.h"
#include "Job.h"

class CustomerControllerIntegrationTest : public ::testing::Test
{
protected:
    const Customer exampleCustomer;
    const Job exampleJob;

    CustomerControllerIntegrationTest()
        : exampleCustomer("John", "Doe", "123 Example Lane", "Example Village", "Exampleville", "AB12 3CD",
              "01234567890", "07012345678", "john.doe@example.com"),
          exampleJob(0, time(NULL) + 100000, 50.0, Job::DONE_UNPAID, Job::NA) {}

    virtual void SetUp()
    {
        Databases::init(true);

        Customer customer(exampleCustomer);
        for (unsigned i = 0; i < 20; ++i)
        {
            customer.setForename((exampleCustomer.getForename() + toString(i)).c_str());
            customer.setSurname((exampleCustomer.getSurname() + toString(i)).c_str());
            Databases::customers().addRecord(customer);
        }
    }

    virtual void TearDown()
    {
        std::string customersFilename = Databases::customers().filename(),
                jobsFilename = Databases::jobs().filename();

        Databases::finalise();
        remove(customersFilename.c_str());
        remove(jobsFilename.c_str());
    }
};

// Does Create Work
TEST_F(CustomerControllerIntegrationTest, DoesCreateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::customers().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Customer customer(exampleCustomer);
    EXPECT_NO_THROW(CustomerController::Create(customer, NULL)) // The controllers should handle any exceptions
            << "The Customer Controller did not catch an exception";

    EXPECT_FALSE(customer.null())
            << "The customer was not added to the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::customers().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore + 1, recordCountAfter)
            << "The customer was not added to the database properly";
}

// Does Update Work
TEST_F(CustomerControllerIntegrationTest, DoesUpdateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::customers().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Customer customer;
    try
    {
        customer = Databases::customers().recordAt(0);
        customer.setForename("Jane");
        customer.setSurname("Doe");
    }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(CustomerController::Update(customer, NULL))
            << "The Customer Controller did not catch an exception";

    Customer tempCustomer;
    try { tempCustomer = Databases::customers().recordAt(0); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_TRUE(customer.completeCompare(tempCustomer))
            << "The customer was not updated in the database correctly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::customers().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore, recordCountAfter)
            << "A record was added to the database while updating an existing customer";
}

// Does Destroy Work
TEST_F(CustomerControllerIntegrationTest, DoesDestroyWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::customers().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Customer customer;
    try { customer = Databases::customers().recordAt(0); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(CustomerController::Destroy(customer, NULL))
            << "The Customer Controller did not catch an exception";

    EXPECT_TRUE(customer.null())
            << "The customer was not removed from the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::customers().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore - 1, recordCountAfter)
            << "The customer was not removed from the database properly";
}

// Does getCustomerJobs Work
TEST_F(CustomerControllerIntegrationTest, DoesGetCustomerJobsWork)
{
    // Get a customer and create a job to work with
    Customer customer;
    try { customer = Databases::customers().recordAt(0); } catch (const std::exception &e) { FAIL() << e.what(); }

    Job job(exampleJob);
    try // Set the job's customerId to the customer's ID, and add it to the database
    {
        job.setCustomerId(customer.getId());
        Databases::jobs().addRecord(job);
    }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Database<Job>::recordListPtr jobs;
    EXPECT_NO_THROW(jobs = CustomerController::getCustomerJobs(customer.getId()))
            << "The Customer Controller did not catch an exception";

    ASSERT_GT(jobs->size(), 0)
            << "The Customer Controller couldn't find any associated jobs in the database";

    bool success = false;
    for (unsigned i = 0; i < jobs->size(); ++i)
    {
        if (jobs->at(i).completeCompare(job))
        {
            success = true;
            break;
        }
    }
    if (!success) ADD_FAILURE() << "The test job could not be found in the customer's jobs list";
}

#endif // COMPILE_TESTS
