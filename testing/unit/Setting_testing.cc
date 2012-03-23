/*
 * Setting_testing.cc
 *
 *  Created on: 23 March 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include <stdexcept>
#include <fstream>
#include <time.h>
using namespace std;

#include "testing/TestingHelpers.hpp"

#include "Setting.h"

class SettingUnitTest : public ::testing::Test
{
protected:
    const Setting exampleSetting;

    SettingUnitTest()
        : exampleSetting("example setting", "value") {}
};

// Is Setting ID Minus One
// A new setting record must have an ID of -1 (a setting record ID will be set when it is added to the database)
TEST_F(SettingUnitTest, IsSettingIdMinusOne)
{
    Setting setting;
    EXPECT_EQ(-1, setting.getId()) << "Setting record that has not been added to the database must have an ID of -1";
}

// Does Setting Reject Empty Key
TEST_F(SettingUnitTest, DoesSettingRejectEmptyKey)
{
    Setting setting(exampleSetting);
    EXPECT_THROW(setting.setKey(""), std::runtime_error)
            << "Exception was not thrown when the key was set to an empty string";
}

// Does Setting Reject Key That Is Too Short
TEST_F(SettingUnitTest, DoesSettingRejectKeyThatIsTooShort)
{
    Setting setting(exampleSetting);

    std::string testKey = createTestStringOfSize(Setting::minKeyLength - 1);
    EXPECT_THROW(setting.setKey(testKey.c_str()), std::runtime_error)
            << "Exception was not thrown when the key was set to a string that is shorter than the minimum";
}

// Does Setting Reject Key That Is Too Long
TEST_F(SettingUnitTest, DoesSettingRejectKeyThatIsTooLong)
{
    Setting setting(exampleSetting);

    std::string testKey = createTestStringOfSize(Setting::maxKeyLength + 1);
    EXPECT_THROW(setting.setKey(testKey.c_str()), std::runtime_error)
            << "Exception was not thrown when the key was set to a string that is longer than the maximum";
}

// Does Setting Accept Valid Key
TEST_F(SettingUnitTest, DoesSettingAcceptValidKey)
{
    Setting setting(exampleSetting);

    string testKey = createTestStringOfSize(Setting::maxKeyLength / 2);
    EXPECT_NO_THROW(setting.setKey(testKey.c_str()))
            << "Exception was thrown when the key was set to a valid key string";
}

// Does Setting Accept Extreme Valid Key (Lower Bound)
TEST_F(SettingUnitTest, DoesSettingAcceptExtremeValidKeyLowerBound)
{
    Setting setting(exampleSetting);

    string testKey = createTestStringOfSize(Setting::minKeyLength);
    EXPECT_NO_THROW(setting.setKey(testKey.c_str()))
            << "Exception was thrown when the key was set to a valid key string";
}

// Does Setting Accept Extreme Valid Key (Upper Bound)
TEST_F(SettingUnitTest, DoesSettingAcceptExtremeValidKeyUpperBound)
{
    Setting setting(exampleSetting);

    string testKey = createTestStringOfSize(Setting::maxKeyLength);
    EXPECT_NO_THROW(setting.setKey(testKey.c_str()))
            << "Exception was thrown when the key was set to a valid key string";
}

// Does Setting Accept Empty Value
TEST_F(SettingUnitTest, DoesSettingAcceptEmptyValue)
{
    Setting setting(exampleSetting);
    EXPECT_NO_THROW(setting.setValue(""))
            << "Exception was thrown when the value was set to an empty string";
}

// Does Setting Reject Value That Is Too Long
TEST_F(SettingUnitTest, DoesSettingRejectValueThatIsTooLong)
{
    Setting setting(exampleSetting);

    std::string testValue = createTestStringOfSize(Setting::maxValueLength + 1);
    EXPECT_THROW(setting.setValue(testValue.c_str()), std::runtime_error)
            << "Exception was not thrown when the value was set to a string that is longer than the maximum";
}

// Does Setting Accept Valid Value
TEST_F(SettingUnitTest, DoesSettingAcceptValidValue)
{
    Setting setting(exampleSetting);

    string testValue = createTestStringOfSize(Setting::maxValueLength / 2);
    EXPECT_NO_THROW(setting.setValue(testValue.c_str()))
            << "Exception was thrown when the value was set to a valid value string";
}

// Does Setting Accept Extreme Valid Value (Upper Bound)
TEST_F(SettingUnitTest, DoesSettingAcceptExtremeValidValueUpperBound)
{
    Setting setting(exampleSetting);

    string testValue = createTestStringOfSize(Setting::maxValueLength);
    EXPECT_NO_THROW(setting.setValue(testValue.c_str()))
            << "Exception was thrown when the value was set to a valid value string";
}

// Does Setting FieldCompare Member Function Work Correctly
TEST_F(SettingUnitTest, DoesSettingFieldCompareMemberFunctionWorkCorrectly)
{
    Setting lhs(exampleSetting), rhs(exampleSetting);
    EXPECT_TRUE(lhs.fieldCompare(rhs));
}

// Does Setting Read And Write To File Correctly
TEST_F(SettingUnitTest, DoesSettingReadAndWriteToFileCorrectly)
{
    Setting setting(exampleSetting);
    const char *fileName = "DoesSettingReadAndWriteToFileCorrectly.dat.test";

    { // Write the setting to a new file
        fstream outFile;
        outFile.open(fileName, ios::out | ios::binary);
        if (outFile.is_open())
        {
            setting.writeToFile(outFile);
            outFile.close();
        }
        else FAIL() << "File to write test setting to could not be created";
    }

    { // Read the setting back in and check if the setting matches the original
        fstream inFile;
        inFile.open(fileName);
        if (inFile.is_open(), ios::in | ios::binary)
        {
            Setting tempSetting;
            tempSetting.readFromFile(inFile);
            EXPECT_TRUE(tempSetting.fieldCompare(setting));
            inFile.close();
        }
        else ADD_FAILURE() << "File to write test setting to could not be opened";
        remove(fileName);
    }
}

#endif // COMPILE_TESTS

