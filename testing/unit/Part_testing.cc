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
#include <time.h>
using namespace std;

#include "Part.h"
#include "Globals.h"

class PartTest : public ::testing::Test
{
protected:
    const Part examplePart;

    PartTest()
        : examplePart(0, "Tap", "12345", 20.0, 20.0) {}
};

// Is Part ID Minus One
TEST_F(PartTest, IsPartIdMinusOne)
{
    Part part;
    EXPECT_EQ(-1, part.getId()) << "Part record that has not been added to the database must have an ID of -1";
}

// Does Part Reject Customer ID Less Than Zero
TEST_F(PartTest, DoesPartRejectCustomerIdLessThanZero)
{
    Part part(examplePart);
    EXPECT_THROW(part.setJobId(-1), std::runtime_error)
            << "Exception was not thrown when the job ID was set to a value less than 0";
}

// Does Part Reject Empty Name
TEST_F(PartTest, DoesPartRejectEmptyName)
{
    Part part(examplePart);
    EXPECT_THROW(part.setName(""), std::runtime_error)
            << "Exception was not thrown when the name was set to an empty string";
}

// Does Part Reject Name That Is Too Short
TEST_F(PartTest, DoesPartRejectNameThatIsTooShort)
{
    Part part(examplePart);

    string testName;
    testName.reserve(Part::minNameLength - 1);
    while (testName.size() < Part::minNameLength - 1) testName += 'a';

    EXPECT_THROW(part.setName(testName.c_str()), std::runtime_error)
            << "Exception was not thrown when the name was set to a string that is too short";
}

// Does Part Reject Name That Is Too Long
TEST_F(PartTest, DoesPartRejectNameThatIsTooLong)
{
    Part part(examplePart);

    string testName;
    testName.reserve(Part::maxNameLength + 1);
    while (testName.size() < Part::maxNameLength + 1) testName += 'a';

    EXPECT_THROW(part.setName(testName.c_str()), std::runtime_error)
            << "Exception was not thrown when the name was set to a string that is too long";
}

// Does Part Accept Empty Part Number
// The part number field is optional
TEST_F(PartTest, DoesPartAcceptEmptyPartNumber)
{
    Part part(examplePart);
    EXPECT_NO_THROW(part.setNumber(""))
            << "Exception was thrown when the name was set to an empty string";
}

// Does Part Reject Part Number That Is Too Long
TEST_F(PartTest, DoesPartRejectPartNumberThatIsTooLong)
{
    Part part(examplePart);

    string testName;
    testName.reserve(Part::maxNumberLength + 1);
    while (testName.size() < Part::maxNumberLength + 1) testName += '0';

    EXPECT_THROW(part.setNumber(testName.c_str()), std::runtime_error)
            << "Exception was not thrown when the part number was set to a string that is too long";
}

// Does Part Reject Price Less Than Zero
TEST_F(PartTest, DoesPartRejectPriceLessThanZero)
{
    Part part(examplePart);
    EXPECT_THROW(part.setPrice(-10.0), std::runtime_error)
            << "Exception was not thrown when the price was set to a value less than 0";
}

// Does Part Reject VAT Rate Less Than Zero
TEST_F(PartTest, DoesPartRejectVatRateLessThanZero)
{
    Part part(examplePart);
    EXPECT_THROW(part.setVatRate(-10.0), std::runtime_error)
            << "Exception was not thrown when the VAT rate was set to a value less than 0";
}

// Does Part Automatically Set VAT Rate To The Default In Constructor
// If no VAT rate is given to the Part record's constructor, then the VAT rate should be set to the default value
TEST_F(PartTest, DoesPartAutomaticallySetVatRateToTheDefaultInConstructor)
{
    Part part(0, "Tap", "12345", 20.0);
    EXPECT_EQ(Globals::vatRate, part.getVatRate())
            << "VAT rate was not set to the default in the Part constructor";
}

#endif // COMPILE_TESTS
