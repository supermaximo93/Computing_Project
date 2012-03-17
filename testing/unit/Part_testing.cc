/*
 * Part_testing.cc
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include <stdexcept>
#include <fstream>
using namespace std;

#include "testing/TestingHelpers.hpp"

#include "Part.h"
#include "Globals.h"

class PartUnitTest : public ::testing::Test
{
protected:
    const Part examplePart;

    PartUnitTest()
        : examplePart(0, "Tap", "12345", 20.0, 20.0) {}
};

// Is Part ID Minus One
TEST_F(PartUnitTest, IsPartIdMinusOne)
{
    Part part;
    EXPECT_EQ(-1, part.getId()) << "Part record that has not been added to the database must have an ID of -1";
}

// Does Part Reject Job ID Less Than Zero
TEST_F(PartUnitTest, DoesPartRejectJobIdLessThanZero)
{
    Part part(examplePart);
    EXPECT_THROW(part.setJobId(-1), std::runtime_error)
            << "Exception was not thrown when the job ID was set to a value less than 0";
}

// Does Part Reject Empty Name
TEST_F(PartUnitTest, DoesPartRejectEmptyName)
{
    Part part(examplePart);
    EXPECT_THROW(part.setName(""), std::runtime_error)
            << "Exception was not thrown when the name was set to an empty string";
}

// Does Part Reject Name That Is Too Short
TEST_F(PartUnitTest, DoesPartRejectNameThatIsTooShort)
{
    Part part(examplePart);

    string testName = createTestStringOfSize(Part::minNameLength - 1);
    EXPECT_THROW(part.setName(testName.c_str()), std::runtime_error)
            << "Exception was not thrown when the name was set to a string that is too short";
}

// Does Part Reject Name That Is Too Long
TEST_F(PartUnitTest, DoesPartRejectNameThatIsTooLong)
{
    Part part(examplePart);

    string testName = createTestStringOfSize(Part::maxNameLength + 1);
    EXPECT_THROW(part.setName(testName.c_str()), std::runtime_error)
            << "Exception was not thrown when the name was set to a string that is too long";
}

// Does Part Accept Valid Name
TEST_F(PartUnitTest, DoesPartAcceptValidName)
{
    Part part(examplePart);

    string testPart = createTestStringOfSize(Part::maxNameLength / 2);
    EXPECT_NO_THROW(part.setName(testPart.c_str()))
            << "Exception was thrown when the name was set to a valid name string";
}

//Does Part Accept Extreme Valid Name (Lower Bound)
TEST_F(PartUnitTest, DoesPartAcceptExtremeValidNameLowerBound)
{
    Part part(examplePart);

    string testPart = createTestStringOfSize(Part::minNameLength);
    EXPECT_NO_THROW(part.setName(testPart.c_str()))
            << "Exception was thrown when the name was set to a valid name string";
}

//Does Part Accept Extreme Valid Name (Upper Bound)
TEST_F(PartUnitTest, DoesPartAcceptExtremeValidNameUpperBound)
{
    Part part(examplePart);

    string testPart = createTestStringOfSize(Part::maxNameLength);
    EXPECT_NO_THROW(part.setName(testPart.c_str()))
            << "Exception was thrown when the name was set to a valid name string";
}

// Does Part Accept Empty Part Number
// The part number field is optional
TEST_F(PartUnitTest, DoesPartAcceptEmptyPartNumber)
{
    Part part(examplePart);
    EXPECT_NO_THROW(part.setNumber(""))
            << "Exception was thrown when the name was set to an empty string";
}

// Does Part Reject Part Number That Is Too Long
TEST_F(PartUnitTest, DoesPartRejectPartNumberThatIsTooLong)
{
    Part part(examplePart);

    string testPartNumber = createTestStringOfSize(Part::maxNumberLength + 1);
    EXPECT_THROW(part.setNumber(testPartNumber.c_str()), std::runtime_error)
            << "Exception was not thrown when the part number was set to a string that is too long";
}

// Does Part Accept Valid Part Number
TEST_F(PartUnitTest, DoesPartAcceptValidPartNumber)
{
    Part part(examplePart);

    string testPartNumber = createTestStringOfSize(Part::maxNumberLength / 2);
    EXPECT_NO_THROW(part.setNumber(testPartNumber.c_str()))
            << "Exception was thrown when the part number was set to a valid part number string";
}

//Does Part Accept Extreme Valid Part Number (Upper Bound)
TEST_F(PartUnitTest, DoesPartAcceptExtremeValidPartNumberUpperBound)
{
    Part part(examplePart);

    string testPartNumber = createTestStringOfSize(Part::maxNumberLength);
    EXPECT_NO_THROW(part.setNumber(testPartNumber.c_str()))
            << "Exception was thrown when the part number was set to a valid part number string";
}

// Does Part Reject Price Less Than Zero
TEST_F(PartUnitTest, DoesPartRejectPriceLessThanZero)
{
    Part part(examplePart);
    EXPECT_THROW(part.setPrice(-10.0), std::runtime_error)
            << "Exception was not thrown when the price was set to a value less than 0";
}

// Does Part Reject Price Above Zero But Below Zero Point Zero One
TEST_F(PartUnitTest, DoesPartRejectPriceAboveZeroButBelowZeroPointZeroOne)
{
    Part part(examplePart);
    EXPECT_THROW(part.setPrice(0.00999), std::runtime_error)
            << "Exception was not thrown when the price was set to 0.00999";
}

// Does Part Accept Valid Price
TEST_F(PartUnitTest, DoesPartAcceptValidPrice)
{
    Part part(examplePart);
    EXPECT_NO_THROW(part.setPrice(50.0))
            << "Exeption was thrown when the price was set to a a valid value";
}

// Does Part Accept Extreme Valid Price (Lower Bound)
TEST_F(PartUnitTest, DoesPartAcceptExtremeValidPriceLowerBound)
{
    Part part(examplePart);
    EXPECT_NO_THROW(part.setPrice(0.01))
            << "Exeption was thrown when the price was set to a a valid value";
}

// Does Part Reject VAT Rate Less Than Zero
TEST_F(PartUnitTest, DoesPartRejectVatRateLessThanZero)
{
    Part part(examplePart);
    EXPECT_THROW(part.setVatRate(-10.0), std::runtime_error)
            << "Exception was not thrown when the VAT rate was set to a value less than 0";
}

// Does Part Accept Valid VAT Rate
TEST_F(PartUnitTest, DoesPartAcceptValidVatRate)
{
    Part part(examplePart);
    EXPECT_NO_THROW(part.setVatRate(20.0))
            << "Exception was thrown when the VAT rate was set to a valid value";
}

// Does Part Accept Extreme Valid VAT Rate (Lower Bound)
TEST_F(PartUnitTest, DoesPartAcceptExtremeValidVatRateLowerBound)
{
    Part part(examplePart);
    EXPECT_NO_THROW(part.setVatRate(0.0))
            << "Exception was thrown when the VAT rate was set to a valid value";
}

// Does Part Automatically Set VAT Rate To The Default In Constructor
// If no VAT rate is given to the Part record's constructor, then the VAT rate should be set to the default value
TEST_F(PartUnitTest, DoesPartAutomaticallySetVatRateToTheDefaultInConstructor)
{
    Part part(0, "Tap", "12345", 20.0);
    EXPECT_EQ(Globals::vatRate(time(NULL)), part.getVatRate())
            << "VAT rate was not set to the default in the Part constructor";
}

// Does Part Reject Quantity Equal To Zero
TEST_F(PartUnitTest, DoesPartRejectQuantityEqualToZero)
{
    Part part(examplePart);
    EXPECT_THROW(part.setQuantity(0), std::runtime_error)
            << "Exception was not thrown when the quantity was set to 0";
}

// Does Part Reject Quantity Less Than Zero
TEST_F(PartUnitTest, DoesPartRejectQuantityLessThanZero)
{
    Part part(examplePart);
    EXPECT_THROW(part.setQuantity(-1), std::runtime_error)
            << "Exception was not thrown when the quantity was set to a value less than 0";
}

// Does Part Accept Valid Quantity
TEST_F(PartUnitTest, DoesPartAcceptValidQuantity)
{
    Part part(examplePart);
    EXPECT_NO_THROW(part.setQuantity(10))
            << "Exception was thrown when the quantity was set to a valid value";
}

// Does Part Accept Extreme Valid Quantity (Lower Bound)
TEST_F(PartUnitTest, DoesPartAcceptExtremeValidQuantityLowerBound)
{
    Part part(examplePart);
    EXPECT_NO_THROW(part.setQuantity(1))
            << "Exception was thrown when the quantity was set to a valid value";
}

// Does Part FieldCompare Member Function Work Correctly
TEST_F(PartUnitTest, DoesPartFieldCompareMemberFunctionWorkCorrectly)
{
    Part lhs(examplePart), rhs(examplePart);
    EXPECT_TRUE(lhs.fieldCompare(rhs));
}

// Does Part Read And Write To File Correctly
TEST_F(PartUnitTest, DoesPartReadAndWriteToFileCorrectly)
{
    Part part(examplePart);
    const char *fileName = "DoesPartReadAndWriteToFileCorrectly.dat.test";

    { // Write the part to a new file
        fstream outFile;
        outFile.open(fileName, ios::out | ios::binary);
        if (outFile.is_open())
        {
            part.writeToFile(outFile);
            outFile.close();
        }
        else FAIL() << "File to write test part to could not be created";
    }

    { // Read the part back in and check if the part matches the original
        fstream inFile;
        inFile.open(fileName);
        if (inFile.is_open(), ios::in | ios::binary)
        {
            Part tempPart;
            tempPart.readFromFile(inFile);
            EXPECT_TRUE(tempPart.fieldCompare(part));
            inFile.close();
        }
        else ADD_FAILURE() << "File to write test part to could not be opened";
        remove(fileName);
    }
}

#endif // COMPILE_TESTS
