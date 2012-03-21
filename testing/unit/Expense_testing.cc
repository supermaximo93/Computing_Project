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

#include "testing/TestingHelpers.hpp"

#include "Expense.h"

class ExpenseUnitTest : public ::testing::Test
{
protected:
    const Expense exampleExpense;

    ExpenseUnitTest()
        : exampleExpense(time(NULL) + 100000, "Petrol bill", 30.0, 6.0, 0) {}
};

// Is Expense ID Minus One
// A new expense record must have an ID of -1 (a expense record ID will be set when it is added to the database)
TEST_F(ExpenseUnitTest, IsExpenseIdMinusOne)
{
    Expense expense;
    EXPECT_EQ(-1, expense.getId()) << "Expense record that has not been added to the database must have an ID of -1";
}

//Does Expense Accept Valid Date
TEST_F(ExpenseUnitTest, DoesExpenseAcceptValidDate)
{
    Expense expense(exampleExpense);
    EXPECT_NO_THROW(expense.setDate(time(NULL) + 100000))
            << "Exception was thrown when the date was set to a valid date";
}

// Does Expense Reject Empty Description
TEST_F(ExpenseUnitTest, DoesExpenseRejectEmptyDescription)
{
    // Create a expense based on the example data, and then attempt to set the description to an empty string.
    // An exception should be thrown
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setDescription(""), std::runtime_error)
            << "Exception was not thrown when the description was set to an empty string";
}

// Does Expense Reject Description That Is Too Long
TEST_F(ExpenseUnitTest, DoesExpenseRejectDescriptionThatIsTooLong)
{
    // Create a expense based on the example data, and then attempt to set the forename to a string that is longer
    // than the maximum length allowed. An exception should be thrown
    Expense expense(exampleExpense);

    std::string testDescription = createTestStringOfSize(Expense::maxDescriptionLength + 1);
    EXPECT_THROW(expense.setDescription(testDescription.c_str()), std::runtime_error)
            << "Exception was not thrown when the description was set to a string that is longer than the maximum";
}

// Does Expense Accept Valid Description
TEST_F(ExpenseUnitTest, DoesExpenseAcceptValidDescription)
{
    Expense expense(exampleExpense);

    string testDescription = createTestStringOfSize(Expense::maxDescriptionLength / 2);
    EXPECT_NO_THROW(expense.setDescription(testDescription.c_str()))
            << "Exception was thrown when the description was set to a valid description string";
}

// Does Expense Accept Extreme Valid Description (Upper Bound)
TEST_F(ExpenseUnitTest, DoesExpenseAcceptExtremeValidDescriptionUpperBound)
{
    Expense expense(exampleExpense);

    string testDescription = createTestStringOfSize(Expense::maxDescriptionLength);
    EXPECT_NO_THROW(expense.setDescription(testDescription.c_str()))
            << "Exception was thrown when the description was set to a valid description string";
}

// Does Expense Reject Price Less Than Zero
TEST_F(ExpenseUnitTest, DoesExpenseRejectPriceLessThanZero)
{
    // Create a expense based on the example data, and then attempt to set the price to a value less than 0.
    // An exception should be thrown
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setPrice(-10.0), std::runtime_error)
            << "Exception was not thrown when the price was set to a value less than 0";
}

// Does Expense Reject Price Of Zero
TEST_F(ExpenseUnitTest, DoesExpenseRejectPriceOfZero)
{
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setPrice(0.0), std::runtime_error)
            << "Exception was not thrown when the price was set to 0";
}

// Does Expense Reject Price Above Zero But Below Zero Point Zero One
TEST_F(ExpenseUnitTest, DoesExpenseRejectPriceAboveZeroButBelowZeroPointZeroOne)
{
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setPrice(0.00999), std::runtime_error)
            << "Exception was not thrown when the price was set to 0.00999";
}

// Does Expense Accept Valid Price
TEST_F(ExpenseUnitTest, DoesExpenseAcceptValidPrice)
{
    Expense expense(exampleExpense);
    EXPECT_NO_THROW(expense.setPrice(50.0))
            << "Exeption was thrown when the price was set to a a valid value";
}

// Does Expense Accept Extreme Valid Price (Lower Bound)
TEST_F(ExpenseUnitTest, DoesExpenseAcceptExtremeValidPriceLowerBound)
{
    Expense expense(exampleExpense);
    EXPECT_NO_THROW(expense.setPrice(0.01))
            << "Exeption was thrown when the price was set to a a valid value";
}

// Does Expense Reject VAT Less Than Zero
TEST_F(ExpenseUnitTest, DoesExpenseRejectVATLessThanZero)
{
    // Create a expense based on the example data, and then attempt to set the price to a value less than 0.
    // An exception should be thrown
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setVat(-10.0), std::runtime_error)
            << "Exception was not thrown when the VAT was set to a value less than 0";
}

// Does Expense Accept Valid VAT
TEST_F(ExpenseUnitTest, DoesExpenseAcceptValidVAT)
{
    Expense expense(exampleExpense);
    EXPECT_NO_THROW(expense.setVat(20.0))
            << "Exception was thrown when the VAT was set to a valid value";
}

// Does Expense Accept Extreme Valid VAT (Lower Bound)
TEST_F(ExpenseUnitTest, DoesExpenseAcceptExtremeValidVATLowerBound)
{
    Expense expense(exampleExpense);
    EXPECT_NO_THROW(expense.setVat(0.0))
            << "Exception was thrown when the VAT was set to a valid value";
}

// Does Expense Reject Type Less Than Zero
TEST_F(ExpenseUnitTest, DoesExpenseRejectTypeLessThanZero)
{
    Expense expense(exampleExpense);
    EXPECT_THROW(expense.setType(-1), std::runtime_error)
            << "Exception was not thrown when the type was set to a value less than 0";
}

// Does Expense Accept Valid Type
TEST_F(ExpenseUnitTest, DoesExpenseAcceptValidType)
{
    Expense expense(exampleExpense);
    EXPECT_NO_THROW(expense.setType(1))
            << "Exception was thrown when the type was set to a valid value";
}

// Does Expense Accept Extreme Valid Type (Lower Bound)
TEST_F(ExpenseUnitTest, DoesExpenseAcceptExtremeValidTypeLowerBound)
{
    Expense expense(exampleExpense);
    EXPECT_NO_THROW(expense.setType(0))
            << "Exception was thrown when the type was set to a valid value";
}

// Does Expense FieldCompare Member Function Work Correctly
TEST_F(ExpenseUnitTest, DoesExpenseFieldCompareMemberFunctionWorkCorrectly)
{
    Expense lhs(exampleExpense), rhs(exampleExpense);
    EXPECT_TRUE(lhs.fieldCompare(rhs));
}

// Does Expense Read And Write To File Correctly
TEST_F(ExpenseUnitTest, DoesExpenseReadAndWriteToFileCorrectly)
{
    Expense expense(exampleExpense);
    const char *fileName = "DoesExpenseReadAndWriteToFileCorrectly.dat.test";

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

    { // Read the expense back in and check if the expense matches the original
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
