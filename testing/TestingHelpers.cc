/*
 * TestingHelpers.cc
 *
 *  Created on: 17 March 2012
 *      Author: Max Foster
 */

#include "TestingHelpers.hpp"

std::string createTestStringOfSize(unsigned size)
{
    std::string returnString;
    returnString.reserve(size);
    while (returnString.size() < size) returnString += 'a';
    return returnString;
}
