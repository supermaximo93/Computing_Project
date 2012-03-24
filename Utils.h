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

class QString;
class QDateTime;

struct Date
{
    unsigned seconds, minute, hour, day, month, year;

    Date(time_t seconds);
    Date(unsigned minute, unsigned hour, unsigned day, unsigned month, unsigned year);

    operator std::string() const;
    operator QString() const;
    operator QDateTime() const;
    operator time_t() const;

    QString toQStringWithoutTime() const;
};

std::ostream & operator <<(std::ostream &stream, const Date &date);

std::string lowerCase(const std::string &str);
void replaceChars(std::string &str, char searchChar, char newChar);
const char * limitLength(const char *str, unsigned maxLength);

void showInfoDialog(const char *message);
void showErrorDialog(const char *message);
void showErrorDialog(const std::vector<std::string> &errors);
void showFatalDialog(const char *message);
bool showYesNoDialog(const char *question);
void showPendingDialog(const char *message, int (*percentCompleteCheckFunction)(void));

const char * createFullName(const char *forename, const char *surname);

void addError(std::vector<std::string> &errors, std::string error);

bool validateLengthOf(const char *value, int min, int max, const std::string &valueName, std::string &errorMessage);
bool validateLengthOf(const char *value, int max, const std::string &valueName, std::string &errorMessage);

void copyFile(const char *sourceFilename, const char *destinationFilename);
void moveDirectory(const char *sourceDirectoryPath, const char *destinationDirectoryPath);

template <typename type>
std::string toString(const type &variable)
{
    std::stringstream stream;
    stream << variable;
    return stream.str();
}

#endif // UTILS_H
