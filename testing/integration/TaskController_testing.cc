/*
 * TaskController_testing.cc
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include "Databases.h"
#include "TaskController.h"
#include "Task.h"

class TaskControllerIntegrationTest : public ::testing::Test
{
protected:
    const Task exampleTask;

    TaskControllerIntegrationTest()
        : exampleTask(0, time(NULL) + 100000, "Example task") {}

    virtual void SetUp()
    {
        Databases::init(true);

        Task task(exampleTask);
        for (unsigned i = 0; i < 20; ++i)
        {
            task.setDate(task.getDate() + 86400);
            task.setDescription((exampleTask.getDescription() + toString(i)).c_str());
            Databases::tasks().addRecord(task);
        }
    }

    virtual void TearDown()
    {
        std::string tasksFilename = Databases::tasks().filename();
        Databases::finalise();
        remove(tasksFilename.c_str());
    }
};

// Does Create Work
TEST_F(TaskControllerIntegrationTest, DoesCreateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::tasks().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Task task(exampleTask);
    EXPECT_NO_THROW(TaskController::Create(task, NULL))
            << "The Task Controller did not catch an exception";

    EXPECT_FALSE(task.null())
            << "The task was not added to the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::tasks().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore + 1, recordCountAfter)
            << "The task was not added to the database properly";
}

// Does Update Work
TEST_F(TaskControllerIntegrationTest, DoesUpdateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::tasks().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Task task;
    try
    {
        task = Databases::tasks().recordAt(0);
        task.setDate(task.getDate() + 86400);
    }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(TaskController::Update(task, NULL))
            << "The Task Controller did not catch an exception";

    Task tempTask;
    try { tempTask = Databases::tasks().recordAt(0); } catch (const std::exception &e) { FAIL() << e.what(); }
    EXPECT_TRUE(task.completeCompare(tempTask))
            << "The task was not updated in the database correctly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::tasks().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore, recordCountAfter)
            << "A record was added to the database while updating an existing task";
}

// Does Destroy Work
TEST_F(TaskControllerIntegrationTest, DoesDestroyWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::tasks().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Task task;
    try { task = Databases::tasks().recordAt(0); } catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(TaskController::Destroy(task, NULL))
            << "The Task Controller did not catch an exception";

    EXPECT_TRUE(task.null())
            << "The task was not removed from the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::tasks().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore - 1, recordCountAfter)
            << "The task was not removed from the database properly";
}

#endif // COMPILE_TESTS
