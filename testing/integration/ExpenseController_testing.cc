/*
 * ExpenseController_testing.cc
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include "Databases.h"
#include "ExpenseController.h"
#include "Expense.h"

class ExpenseControllerIntegrationTest : public ::testing::Test
{
protected:
    const Expense exampleExpense;

    ExpenseControllerIntegrationTest()
        : exampleExpense(time(NULL) + 100000, "Petrol bill", 30.0, 6.0, 0) {}

    virtual void SetUp()
    {
        Databases::init(true);

        Expense expense(exampleExpense);
        for (unsigned i = 0; i < 20; ++i)
        {
            expense.setDate(expense.getDate() + 86400);
            expense.setPrice(expense.getPrice() + 10.0);
            Databases::expenses().addRecord(expense);
        }
    }

    virtual void TearDown()
    {
        std::string expensesFilename = Databases::expenses().filename();
        Databases::finalise();
        remove(expensesFilename.c_str());
    }
};

// Does Create Work
TEST_F(ExpenseControllerIntegrationTest, DoesCreateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::expenses().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Expense expense(exampleExpense);
    EXPECT_NO_THROW(ExpenseController::Create(expense, NULL))
            << "The Expense Controller did not catch an exception";

    EXPECT_FALSE(expense.null())
            << "The expense was not added to the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::expenses().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore + 1, recordCountAfter)
            << "The expense was not added to the database properly";
}

// Does Update Work
TEST_F(ExpenseControllerIntegrationTest, DoesUpdateWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::expenses().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Expense expense;
    try
    {
        expense = Databases::expenses().recordAt(0);
        expense.setPrice(expense.getPrice() + 10.0);
    }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(ExpenseController::Update(expense, NULL))
            << "The Expense Controller did not catch an exception";

    Expense tempExpense;
    try { tempExpense = Databases::expenses().recordAt(0); } catch (const std::exception &e) { FAIL() << e.what(); }
    EXPECT_TRUE(expense.completeCompare(tempExpense))
            << "The expense was not updated in the database correctly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::expenses().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore, recordCountAfter)
            << "A record was added to the database while updating an existing expense";
}

// Does Destroy Work
TEST_F(ExpenseControllerIntegrationTest, DoesDestroyWork)
{
    unsigned recordCountBefore;
    try { recordCountBefore = Databases::expenses().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    Expense expense;
    try { expense = Databases::expenses().recordAt(0); } catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_NO_THROW(ExpenseController::Destroy(expense, NULL))
            << "The Expense Controller did not catch an exception";

    EXPECT_TRUE(expense.null())
            << "The expense was not removed from the database properly";

    unsigned recordCountAfter;
    try { recordCountAfter = Databases::expenses().recordCount(); }
    catch (const std::exception &e) { FAIL() << e.what(); }

    EXPECT_EQ(recordCountBefore - 1, recordCountAfter)
            << "The expense was not removed from the database properly";
}

#endif // COMPILE_TESTS
