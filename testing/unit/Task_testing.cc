/*
 * Task_testing.cc
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

#include "Task.h"

class TaskUnitTest : public ::testing::Test
{
protected:
    const Task exampleTask;

    TaskUnitTest()
        : exampleTask(0, time(NULL) + 100000, "Example task") {}
};

// Is Task ID Minus One
TEST_F(TaskUnitTest, IsTaskIdMinusOne)
{
    Task task;
    EXPECT_EQ(-1, task.getId()) << "Task record that has not been added to the database must have an ID of -1";
}

// Does Task Reject Job ID Less Than Zero
TEST_F(TaskUnitTest, DoesTaskRejectJobIdLessThanZero)
{
    Task task(exampleTask);
    EXPECT_THROW(task.setJobId(-1), std::runtime_error)
            << "Exception was not thrown when the job ID was set to a value less than 0";
}

// Does Task Reject Date Before Now
TEST_F(TaskUnitTest, DoesTaskRejectDateBeforeNow)
{
    Task task(exampleTask);
    EXPECT_THROW(task.setDate(time(NULL) - 100000), std::runtime_error)
            << "Exception was not thrown when the date was set to a time before now";
}

// Does Task Reject Empty Description
TEST_F(TaskUnitTest, DoesTaskRejectEmptyDescription)
{
    Task task(exampleTask);
    EXPECT_THROW(task.setDescription(""), std::runtime_error)
            << "Exception was not thrown when the description was set to an empty string";
}

// Does Task Reject Description That Is Too Long
TEST_F(TaskUnitTest, DoesTaskRejectDescriptionThatIsTooLong)
{
    Task task(exampleTask);

    string testDescription;
    testDescription.reserve(Task::maxDescriptionLength + 1);
    while (testDescription.size() < Task::maxDescriptionLength + 1) testDescription += 'a';

    EXPECT_THROW(task.setDescription(testDescription.c_str()), std::runtime_error)
            << "Exception was not thrown when the description was set to a string longer than the maximum length";
}

// Does Task FieldCompare Member Function Work Correctly
TEST_F(TaskUnitTest, DoesTaskFieldCompareMemberFunctionWorkCorrectly)
{
    Task lhs(exampleTask), rhs(exampleTask);
    EXPECT_TRUE(lhs.fieldCompare(rhs));
}

// Does Task Read And Write To File Correctly
TEST_F(TaskUnitTest, DoesTaskReadAndWriteToFileCorrectly)
{
    Task task(exampleTask);
    const char *fileName = "DoesTaskReadAndWriteToFileCorrectly.dat.test";

    { // Write the task to a new file
        fstream outFile;
        outFile.open(fileName, ios::out | ios::binary);
        if (outFile.is_open())
        {
            task.writeToFile(outFile);
            outFile.close();
        }
        else FAIL() << "File to write test task to could not be created";
    }

    { // Read the task back in and check if the task matches the original
        fstream inFile;
        inFile.open(fileName);
        if (inFile.is_open(), ios::in | ios::binary)
        {
            Task tempTask;
            tempTask.readFromFile(inFile);
            EXPECT_TRUE(tempTask.fieldCompare(task));
            inFile.close();
        }
        else ADD_FAILURE() << "File to write test task to could not be opened";
        remove(fileName);
    }
}

#endif // COMPILE_TESTS
