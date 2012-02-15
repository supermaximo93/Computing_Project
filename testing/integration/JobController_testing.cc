/*
 * JobController_testing.cc
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include "Databases.h"
#include "JobController.h"
#include "Job.h"
#include "Customer.h"

class JobControllerIntegrationTest : public ::testing::Test
{
protected:
    const Job exampleJob;
    const Customer exampleCustomer;

    JobControllerIntegrationTest()
        : exampleJob(0, time(NULL) + 100000, 50.0, Job::DONE_UNPAID, Job::NA),
          exampleCustomer("John", "Doe", "123 Example Lane", "Example Village", "Exampleville", "AB12 3CD",
              "01234567890", "07012345678", "john.doe@example.com") {}

    virtual void SetUp()
    {
        Databases::init(true);
        Customer customer(exampleCustomer);
        Databases::customers().addRecord(customer);

        Job job(exampleJob);
        for (unsigned i = 0; i < 20; ++i)
        {
            job.setDate(job.getDate() + 86400); // Add an extra day
            job.setLabourCharge(job.getLabourCharge() + 10.0);
            Databases::jobs().addRecord(job);
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
TEST_F(JobControllerIntegrationTest, DoesCreateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::jobs().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    Job job(exampleJob);
    EXPECT_NO_THROW(JobController::Create(job, NULL)) // The controllers should handle any exceptions
            << "The Job Controller did not catch an exception";

    EXPECT_FALSE(job.null())
            << "The job was not added to the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::jobs().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore + 1, recordCountAfter)
            << "The job was not added to the database properly";
}

// Does Update Work
TEST_F(JobControllerIntegrationTest, DoesUpdateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::jobs().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    Job job;
    try
    {
        job = Databases::jobs().recordAt(0);
        job.setDate(job.getDate() + 86400);
    }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(JobController::Update(job, NULL))
            << "The Job Controller did not catch an exception";

    Job tempJob;
    try { tempJob = Databases::jobs().recordAt(0); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_TRUE(job.completeCompare(tempJob))
            << "The job was not updated in the database correctly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::jobs().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore, recordCountAfter)
            << "A record was added to the database while updating an existing job";
}

// Does Destroy Work
TEST_F(JobControllerIntegrationTest, DoesDestroyWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::jobs().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    Job job;
    try { job = Databases::jobs().recordAt(0); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(JobController::Destroy(job, NULL))
            << "The Job Controller did not catch an exception";

    EXPECT_TRUE(job.null())
            << "The job was not removed from the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::jobs().recordCount(); }
    catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore - 1, recordCountAfter)
            << "The job was not removed from the database properly";
}

#endif // COMPILE_TESTS
