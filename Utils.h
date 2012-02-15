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
#include <vector>

struct Message
{
    std::string message;
    int data;

    Message() {}
    Message(const std::string & newMessage, const int newData) : message(newMessage), data(newData) {}
};

class QString;
class QDateTime;

struct Date
{
    unsigned seconds, minute, hour, day, month, year;

    Date(time_t seconds);
    Date(unsigned minute, unsigned hour, unsigned day, unsigned month, unsigned year);

    operator QString();
    operator QDateTime();
    operator time_t();
};

std::ostream & operator <<(std::ostream & stream, const Date & date);

std::string lowerCase(const std::string & str);

void showInfoDialog(const char * message);
void showErrorDialog(const char * message);
void showErrorDialog(const std::vector<std::string> & errors);
void showFatalDialog(const char * message);
bool showYesNoDialog(const char * question);

const char * createFullName(const char * forename, const char * surname);

void addError(std::vector<std::string> & errors, const char * error);

template <typename type>
std::string toString(const type & variable)
{
    std::stringstream stream;
    stream << variable;
    return stream.str();
}

#endif // UTILS_H
