/*
 * Utils.h
 *
 *  Created on: 27 Jan 2012
 *      Author: Max Foster
 */

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>

struct Message
{
    std::string message;
    int data;

    Message() {}
    Message(const std::string & newMessage, const int newData) : message(newMessage), data(newData) {}
};

std::string lowerCase(const std::string & str);

template <typename type>
std::string toString(const type & variable)
{
    std::stringstream stream;
    stream << variable;
    return stream.str();
}

#endif // UTILS_H
