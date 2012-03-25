/*
 * VatRate_testing.cc
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include <stdexcept>
#include <fstream>
#include <time.h>
using namespace std;

#include "testing/TestingHelpers.hpp"

#include "VatRate.h"

class VatRateUnitTest : public ::testing::Test
{
protected:
    const VatRate exampleVatRate;

    VatRateUnitTest()
        : exampleVatRate(20, time(NULL) - 86400) {}
};

// Is VatRate ID Minus One
TEST_F(VatRateUnitTest, IsVatRateIdMinusOne)
{
    VatRate vatRate;
    EXPECT_EQ(-1, vatRate.getId()) << "VatRate record that has not been added to the database must have an ID of -1";
}

//Does VatRate Accept Valid Value
TEST_F(VatRateUnitTest, DoesVatRateAcceptValidValue)
{
    VatRate vatRate(exampleVatRate);
    EXPECT_NO_THROW(vatRate.setValue(17.5))
            << "Exception was thrown when the value was set to a valid value";
}

//Does VatRate Accept Extreme Valid Value (Lower Bound)
TEST_F(VatRateUnitTest, DoesVatRateAcceptExtremeValidValueLowerBound)
{
    VatRate vatRate(exampleVatRate);
    EXPECT_NO_THROW(vatRate.setValue(0.0))
            << "Exception was thrown when the value was set to a valid value";
}

// Does VatRate Reject Negative Value
TEST_F(VatRateUnitTest, DoesVatRateRejectNegativeValue)
{
    VatRate vatRate(exampleVatRate);
    EXPECT_THROW(vatRate.setValue(-1.0);, std::runtime_error)
            << "Exception was not thrown when the value was set to a negative number";
}

//Does VatRate Accept Valid Start Date
TEST_F(VatRateUnitTest, DoesVatRateAcceptValidStartDate)
{
    VatRate vatRate(exampleVatRate);
    EXPECT_NO_THROW(vatRate.setStartDate(10))
            << "Exception was thrown when the start date was set to a valid value";
}

// Does VatRate FieldCompare Member Function Work Correctly
TEST_F(VatRateUnitTest, DoesVatRateFieldCompareMemberFunctionWorkCorrectly)
{
    VatRate lhs(exampleVatRate), rhs(exampleVatRate);
    EXPECT_TRUE(lhs.fieldCompare(rhs));
}

// Does VatRate Read And Write To File Correctly
TEST_F(VatRateUnitTest, DoesVatRateReadAndWriteToFileCorrectly)
{
    VatRate vatRate(exampleVatRate);
    const char *fileName = "DoesVatRateReadAndWriteToFileCorrectly.dat.test";

    { // Write the vatRate to a new file
        fstream outFile;
        outFile.open(fileName, ios::out | ios::binary);
        if (outFile.is_open())
        {
            vatRate.writeToFile(outFile);
            outFile.close();
        }
        else FAIL() << "File to write test vatRate to could not be created";
    }

    { // Read the vatRate back in and check if the vatRate matches the original
        fstream inFile;
        inFile.open(fileName);
        if (inFile.is_open(), ios::in | ios::binary)
        {
            VatRate tempVatRate;
            tempVatRate.readFromFile(inFile);
            EXPECT_TRUE(tempVatRate.fieldCompare(vatRate));
            inFile.close();
        }
        else ADD_FAILURE() << "File to write test vatRate to could not be opened";
        remove(fileName);
    }
}

#endif // COMPILE_TESTS

