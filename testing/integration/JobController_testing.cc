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
#include "Part.h"
#include "Task.h"

class JobControllerIntegrationTest : public ::testing::Test
{
protected:
    const Job exampleJob;
    const Part examplePart;
    const Task exampleTask;

    JobControllerIntegrationTest()
        : exampleJob(0, time(NULL) + 100000, 50.0, Job::DONE_UNPAID, Job::NA),
          examplePart(0, "Tap", "12345", 20.0, 20.0),
          exampleTask(0, time(NULL) + 100000, "Example task") {}

    virtual void SetUp()
    {
        Databases::init(true);

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
        std::string jobsFilename = Databases::jobs().filename(),
                partsFilename = Databases::parts().filename(),
                tasksFilename = Databases::tasks().filename();

        Databases::finalise();
        remove(jobsFilename.c_str());
        remove(partsFilename.c_str());
        remove(tasksFilename.c_str());
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
    try { recordCountAfter = Databases::jobs().recordCount(); } catch (const std::exception & e) { FAIL() << e.what(); }

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
    try { tempJob = Databases::jobs().recordAt(0); } catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_TRUE(job.completeCompare(tempJob))
            << "The job was not updated in the database correctly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::jobs().recordCount(); } catch (const std::exception & e) { FAIL() << e.what(); }

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
    try { job = Databases::jobs().recordAt(0); } catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(JobController::Destroy(job, NULL))
            << "The Job Controller did not catch an exception";

    EXPECT_TRUE(job.null())
            << "The job was not removed from the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::jobs().recordCount(); } catch (const std::exception & e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore - 1, recordCountAfter)
            << "The job was not removed from the database properly";
}

// Does getJobParts Work
TEST_F(JobControllerIntegrationTest, DoesGetJobPartsWork)
{
    // Get a job and create a part to work with
    Job job;
    try { job = Databases::jobs().recordAt(0); } catch (const std::exception & e) { FAIL() << e.what(); }

    Part part(examplePart);
    try // Set the part's jobId to the job's ID, and add it to the database
    {
        part.setJobId(job.getId());
        Databases::parts().addRecord(part);
    }
    catch (const std::exception & e) { FAIL() << e.what(); }

    Database<Part>::recordListPtr parts;
    EXPECT_NO_THROW(parts = JobController::getJobParts(job.getId()))
            << "The Job Controller did not catch an exception";

    ASSERT_GT(parts->size(), 0) // Is an assert rather than expect because if the size of the list is actually 0 then
            << "The Job Controller couldn't find any associated parts in the database"; // the next test won't work

    bool success = false;
    for (unsigned i = 0; i < parts->size(); ++i)
    {
        if (parts->at(i).completeCompare(part))
        {
            success = true;
            break;
        }
    }
    if (!success) ADD_FAILURE() << "The test part could not be found in the job's parts list";
}

// Does getJobTasks Work
TEST_F(JobControllerIntegrationTest, DoesGetJobTasksWork)
{
    Job job;
    try { job = Databases::jobs().recordAt(0); } catch (const std::exception & e) { FAIL() << e.what(); }

    Task task(exampleTask);
    try
    {
        task.setJobId(job.getId());
        Databases::tasks().addRecord(task);
    }
    catch (const std::exception & e) { FAIL() << e.what(); }

    Database<Task>::recordListPtr tasks;
    EXPECT_NO_THROW(tasks = JobController::getJobTasks(job.getId()))
            << "The Job Controller did not catch an exception";

    ASSERT_GT(tasks->size(), 0)
            << "The Job Controller couldn't find any associated tasks in the database";

    bool success = false;
    for (unsigned i = 0; i < tasks->size(); ++i)
    {
        if (tasks->at(i).completeCompare(task))
        {
            success = true;
            break;
        }
    }
    if (!success) ADD_FAILURE() << "The test task could not be found in the job's tasks list";
}

#endif // COMPILE_TESTS
