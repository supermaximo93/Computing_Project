/*
 * Job_testing.cc
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include <stdexcept>
#include <fstream>
#include <time.h>
using namespace std;

#include "Job.h"

class JobUnitTest : public ::testing::Test
{
protected:
    const Job exampleJob;

    JobUnitTest()
        : exampleJob(0, time(NULL) + 100000, 50.0, Job::DONE_PAID, Job::CASH) {}
};

// Is Job ID Minus One
TEST_F(JobUnitTest, IsJobIdMinusOne)
{
    Job job;
    EXPECT_EQ(-1, job.getId()) << "Job record that has not been added to the database must have an ID of -1";
}

// Does Job Reject Customer ID Less Than Zero
TEST_F(JobUnitTest, DoesJobRejectCustomerIdLessThanZero)
{
    Job job(exampleJob);
    EXPECT_THROW(job.setCustomerId(-1), std::runtime_error)
            << "Exception was not thrown when the customer ID was set to a value less than 0";
}

// Does Job Reject Date Before Now
TEST_F(JobUnitTest, DoesJobRejectDateBeforeNow)
{
    Job job(exampleJob);
    EXPECT_THROW(job.setDate(time(NULL) - 100000), std::runtime_error)
            << "Exception was not thrown when the date was set to a time before now";
}

// Does Job Reject Labour Charge Less Than Zero
TEST_F(JobUnitTest, DoesExpenseRejectLabourChargeLessThanZero)
{
    Job job(exampleJob);
    EXPECT_THROW(job.setLabourCharge(-10.0), std::runtime_error)
            << "Exception was not thrown when the labour charge was set to a value less than 0";
}

// Does Job FieldCompare Member Function Work Correctly
TEST_F(JobUnitTest, DoesJobFieldCompareMemberFunctionWorkCorrectly)
{
    Job lhs(exampleJob), rhs(exampleJob);
    EXPECT_TRUE(lhs.fieldCompare(rhs));
}

// Does Job Read And Write To File Correctly
TEST_F(JobUnitTest, DoesJobReadAndWriteToFileCorrectly)
{
    Job job(exampleJob);
    const char *fileName = "DoesJobReadAndWriteToFileCorrectly.dat.test";

    { // Write the job to a new file
        fstream outFile;
        outFile.open(fileName, ios::out | ios::binary);
        if (outFile.is_open())
        {
            job.writeToFile(outFile);
            outFile.close();
        }
        else FAIL() << "File to write test job to could not be created";
    }

    { // Read the job back in and check if the job matches the original
        fstream inFile;
        inFile.open(fileName);
        if (inFile.is_open(), ios::in | ios::binary)
        {
            Job tempJob;
            tempJob.readFromFile(inFile);
            EXPECT_TRUE(tempJob.fieldCompare(job));
            inFile.close();
        }
        else ADD_FAILURE() << "File to write test job to could not be opened";
        remove(fileName);
    }
}

#endif // COMPILE_TESTS
