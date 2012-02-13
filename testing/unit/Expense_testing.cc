/*
 * Expense_testing.cc
 *
 *  Created on: 2 Feb 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include <stdexcept>
#include <fstream>
#include <time.h>
using namespace std;

#include "Expense.h"

class ExpenseTest : public ::testing::Test
{
protected:
    const Expense exampleExpense;

    ExpenseTest()
        : exampleExpense(time(NULL) + 100000, "Petrol bill", 30.0, 6.0, 0) {}
};

// Is Expense ID Minus One
// A new expense record must have an ID of -1 (a expense record ID will be set when it is added to the database)
TEST_F(ExpenseTest, IsExpenseIdMinusOne)
{
    Expense expense;
    EXPECT_EQ(-1, expense.getId()) << "Expense record that has not been added to the database must have an ID of -1";
}

// Does Expense Reject Date Before Now
// Records should automatically validate data passed to a setter method, and throw an exception if the data is invalid
TEST_F(ExpenseTest, DoesExpenseRejectDateBeforeNow)
{
    // Create a expense based on the example data, and then attempt to set the date to a time before now.
    // An exception should be thrown
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setDate(time(NULL) - 100000), std::runtime_error)
            << "Exception was not thrown when the date was set to a time before now";
}

// Does Expense Reject Empty Description
// Records should automatically validate data passed to a setter method, and throw an exception if the data is invalid
TEST_F(ExpenseTest, DoesExpenseRejectEmptyDescription)
{
    // Create a expense based on the example data, and then attempt to set the description to an empty string.
    // An exception should be thrown
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setDescription(""), std::runtime_error)
            << "Exception was not thrown when the description was set to an empty string";
}

// Does Expense Reject Description That Is Too Long
// Records should automatically validate data passed to a setter method, and throw an exception if the data is invalid
TEST_F(ExpenseTest, DoesExpenseRejectDescriptionThatIsTooLong)
{
    // Create a expense based on the example data, and then attempt to set the forename to a string that is longer
    // than the maximum length allowed. An exception should be thrown
    Expense expense(exampleExpense);

    std::string testDescription;
    testDescription.reserve(Expense::maxDescriptionLength + 1);
    while (testDescription.size() < Expense::maxDescriptionLength + 1) testDescription += 'a';

    EXPECT_THROW(expense.setDescription(testDescription.c_str()), std::runtime_error)
            << "Exception was not thrown when the description was set to a string that is longer than the maximum";
}

// Does Expense Reject Price Less Than Zero
// Records should automatically validate data passed to a setter method, and throw an exception if the data is invalid
TEST_F(ExpenseTest, DoesExpenseRejectPriceLessThanZero)
{
    // Create a expense based on the example data, and then attempt to set the price to a value less than 0.
    // An exception should be thrown
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setPrice(-10.0), std::runtime_error)
            << "Exception was not thrown when the price was set to a value less than 0";
}

// Does Expense Reject VAT Less Than Zero
// Records should automatically validate data passed to a setter method, and throw an exception if the data is invalid
TEST_F(ExpenseTest, DoesExpenseRejectVATLessThanZero)
{
    // Create a expense based on the example data, and then attempt to set the price to a value less than 0.
    // An exception should be thrown
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setVat(-10.0), std::runtime_error)
            << "Exception was not thrown when the VAT was set to a value less than 0";
}

// Does Expense Reject Type Less Than Zero
// Records should automatically validate data passed to a setter method, and throw an exception if the data is invalid
TEST_F(ExpenseTest, DoesExpenseRejectTypeLessThanZero)
{
    // Create a expense based on the example data, and then attempt to set the price to a value less than 0.
    // An exception should be thrown
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setType(-1), std::runtime_error)
            << "Exception was not thrown when the type was set to a value less than 0";
}

// Does Expense FieldCompare Member Function Work Correctly
TEST_F(ExpenseTest, DoesExpenseFieldCompareMemberFunctionWorkCorrectly)
{
    Expense lhs(exampleExpense), rhs(exampleExpense);
    EXPECT_TRUE(lhs.fieldCompare(rhs));
}

// Does Expense Read And Write To File Correctly
TEST_F(ExpenseTest, DoesExpenseReadAndWriteToFileCorrectly)
{
    Expense expense(exampleExpense);
    const char * fileName = "DoesExpenseReadAndWriteToFileCorrectly.dat.test";

    { // Write the expense to a new file
        fstream outFile;
        outFile.open(fileName, ios::out | ios::binary);
        if (outFile.is_open())
        {
            expense.writeToFile(outFile);
            outFile.close();
        }
        else FAIL() << "File to write test expense to could not be created";
    }

    { // Read the expense back in and check if the customer matches the original
        fstream inFile;
        inFile.open(fileName);
        if (inFile.is_open(), ios::in | ios::binary)
        {
            Expense tempExpense;
            tempExpense.readFromFile(inFile);
            EXPECT_TRUE(tempExpense.fieldCompare(expense));
            inFile.close();
        }
        else ADD_FAILURE() << "File to write test expense to could not be opened";
        remove(fileName);
    }
}

#endif // COMPILE_TESTS
