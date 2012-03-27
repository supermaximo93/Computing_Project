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

#include "testing/TestingHelpers.hpp"

#include "Job.h"

class JobUnitTest : public ::testing::Test
{
protected:
    const Job exampleJob;

    JobUnitTest()
        : exampleJob(0, time(NULL) + 86400, "Replaced boiler", 50.0, Job::DONE_PAID, Job::CASH, time(NULL) + 96400) {}
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

// Does Job Accept Valid Customer ID
TEST_F(JobUnitTest, DoesJobAcceptValidCustomerId)
{
    Job job(exampleJob);
    for (int i = 1; i <= 10; ++i)
    {
        EXPECT_NO_THROW(job.setCustomerId(i))
                << "Exception was thrown when the customer ID was set to a valid value";
    }
}

// Does Job Accept Extreme Valid Customer ID (Lower Bound)
TEST_F(JobUnitTest, DoesJobAcceptExtremeValidCustomerIdLowerBound)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setCustomerId(0))
            << "Exception was thrown when the customer ID was set to a valid value";
}

// Does Job Accept Valid Date
TEST_F(JobUnitTest, DoesJobAcceptValidDate)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setDate(time(NULL) + 100000))
            << "Exception was thrown when the date was set to a valid date";
}

// Does Job Reject Empty Description
TEST_F(JobUnitTest, DoesJobRejectEmptyDescription)
{
    Job job(exampleJob);
    EXPECT_THROW(job.setDescription(""), std::runtime_error)
            << "Exception was not thrown when the description was set to an empty string";
}

// Does Job Reject Description That Is Too Long
TEST_F(JobUnitTest, DoesJobRejectDescriptionThatIsTooLong)
{
    Job job(exampleJob);

    std::string testDescription = createTestStringOfSize(Job::maxDescriptionLength + 1);
    EXPECT_THROW(job.setDescription(testDescription.c_str()), std::runtime_error)
            << "Exception was not thrown when the description was set to a string that is longer than the maximum";
}

// Does Job Accept Valid Description
TEST_F(JobUnitTest, DoesJobAcceptValidDescription)
{
    Job job(exampleJob);

    string testDescription = createTestStringOfSize(Job::maxDescriptionLength / 2);
    EXPECT_NO_THROW(job.setDescription(testDescription.c_str()))
            << "Exception was thrown when the description was set to a valid description string";
}

// Does Job Accept Extreme Valid Description (Upper Bound)
TEST_F(JobUnitTest, DoesJobAcceptExtremeValidDescriptionUpperBound)
{
    Job job(exampleJob);

    string testDescription = createTestStringOfSize(Job::maxDescriptionLength);
    EXPECT_NO_THROW(job.setDescription(testDescription.c_str()))
            << "Exception was thrown when the description was set to a valid description string";
}

// Does Job Reject Labour Charge Less Than Zero
TEST_F(JobUnitTest, DoesJobRejectLabourChargeLessThanZero)
{
    Job job(exampleJob);
    EXPECT_THROW(job.setLabourCharge(-10.0), std::runtime_error)
            << "Exception was not thrown when the labour charge was set to a value less than 0";
}

// Does Job Accept Valid Labour Charge
TEST_F(JobUnitTest, DoesJobAcceptValidLabourCharge)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setLabourCharge(50.0))
            << "Exception was thrown when the labour charge was set to a valid value";
}

// Does Job Accept Extreme Valid Labour Charge (Lower Bound)
TEST_F(JobUnitTest, DoesJobAcceptExtremeValidLabourChargeLowerBound)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setLabourCharge(0.0))
            << "Exception was thrown when the labour charge was set to a valid value";
}

// Does Job Reject Completion State Less Than Zero
TEST_F(JobUnitTest, DoesJobRejectCompletionStateLessThanZero)
{
    Job job(exampleJob);
    EXPECT_THROW(job.setCompletionState(-1), std::runtime_error)
            << "Exception was not thrown when the completion state was set to a value less than 0";
}

// Does Job Accept Valid Completion State
TEST_F(JobUnitTest, DoesJobAcceptValidCompletionState)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setCompletionState(1))
            << "Exception was thrown when the completion state was set to a valid value";
}

// Does Job Accept Extreme Valid Completion State (Lower Bound)
TEST_F(JobUnitTest, DoesJobAcceptExtremeValidCompletionStateLowerBound)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setCompletionState(0))
            << "Exception was thrown when the completion state was set to a valid value";
}

// Does Job Reject Payment Method Less Than Zero
TEST_F(JobUnitTest, DoesJobRejectPaymentMethodLessThanZero)
{
    Job job(exampleJob);
    EXPECT_THROW(job.setPaymentMethod(-1), std::runtime_error)
            << "Exception was not thrown when the payment method was set to a value less than 0";
}

// Does Job Accept Valid Payment Method
TEST_F(JobUnitTest, DoesJobAcceptValidPaymentMethod)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setPaymentMethod(1))
            << "Exception was thrown when the payment method was set to a valid value";
}

// Does Job Accept Extreme Valid Payment Method (Lower Bound)
TEST_F(JobUnitTest, DoesJobAcceptExtremeValidPaymentMethodLowerBound)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setPaymentMethod(0))
            << "Exception was thrown when the payment method was set to a valid value";
}

// Does Job Accept Valid Payment Date
TEST_F(JobUnitTest, DoesJobAcceptValidPaymentDate)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setPaymentDate(job.getDate() + 86400))
            << "Exception was thrown when the payment date was set to a valid value (i.e. value > job.date)";
}

// Does Job Accept Extreme Valid Payment Date (Lower Bound)
TEST_F(JobUnitTest, DoesJobAcceptExtremeValidPaymentDateLowerBound)
{
    Job job(exampleJob);
    EXPECT_NO_THROW(job.setPaymentDate(job.getDate()))
            << "Exception was thrown when the payment date was set to a valid value (i.e. value == job.date)";
}

// Does Job Reject Invalid Payment Date
TEST_F(JobUnitTest, DoesJobRejectInvalidPaymentDate)
{
    Job job(exampleJob);
    EXPECT_THROW(job.setPaymentDate(job.getDate() - 1), std::runtime_error)
            << "Exception was not thrown when the payment date was set to an invalid value (i.e. value < job.date)";
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
