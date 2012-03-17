/*
 * Utils_testing.cc
 *
 *  Created on: 15 March 2012
 *      Author: Max Foster
 */

#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include <iostream>
#include <vector>
using namespace std;

#include "Utils.h"

TEST(UtilsUnitTest, LowerCase)
{
    // Create a test string with capital letters, and see if the lowerCase function converts the string correctly
    string testString = "This Is a TEST";
    EXPECT_EQ("this is a test", lowerCase(testString));
}

TEST(UtilsUnitTest, ReplaceChars)
{
    // Create a test string, and see if specific characters in the string are replaced by replaceChars correctly
    string testString = "This is a test";
    replaceChars(testString, ' ', '_');
    EXPECT_EQ("This_is_a_test", testString);
}

TEST(UtilsUnitTest, CreateFullName)
{
    // Create a test forename and surname, and see if the full name is created by createFullName properly
    const char *forename = "John", *surname = "Doe", *fullName = "John Doe";
    EXPECT_EQ(0, strcmp(fullName, createFullName(forename, surname)));
}

TEST(UtilsUnitTest, AddError)
{
    // Create an error list, and add some errors to it
    vector<string> errors;
    addError(errors, "Error 1");
    addError(errors, "Error 2");
    addError(errors, "Error 3");

    // Newlines should be added to the end of errors
    EXPECT_EQ(errors[0], string("Error 1\n"));
    EXPECT_EQ(errors[1], string("Error 2\n"));
    EXPECT_EQ(errors[2], string("Error 3\n"));

    // Make sure duplicates don't get added
    const unsigned errorCount = errors.size();

    addError(errors, "Error 1");
    addError(errors, "Error 2");
    addError(errors, "Error 3");

    EXPECT_EQ(errorCount, errors.size());

    // Make sure everything is the same as before
    EXPECT_EQ(errors[0], string("Error 1\n"));
    EXPECT_EQ(errors[1], string("Error 2\n"));
    EXPECT_EQ(errors[2], string("Error 3\n"));
}

TEST(UtilsUnitTest, ToString)
{
    // Pass various values to the toString function, checking if the output string is correct in each case
    EXPECT_EQ(string("2"), toString(2));
    EXPECT_EQ(string("3.14"), toString(3.14));
    EXPECT_EQ(string("c"), toString('c'));
    EXPECT_EQ(string("hello"), toString("hello"));
}

#endif // COMPILE_TESTS
