/*
 * Utils.cpp
 *
 *  Created on: 27 Jan 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <algorithm>
using namespace std;

#include "Utils.h"

string lowerCase(const string & str)
{
    string returnStr = str;
    transform(returnStr.begin(), returnStr.end(), returnStr.begin(), ::tolower);
    return returnStr;
}
