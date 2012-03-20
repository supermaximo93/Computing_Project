/*
 * Utils.cpp
 *
 *  Created on: 27 Jan 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <algorithm>
#include <ctime>
using namespace std;

#include <QString>
#include <QDateTime>
#include <QMessageBox>

#include "Utils.h"

#include "dialogs/utils/PendingDialog.h"

Date::Date(const time_t seconds_)
{
    tm *time = localtime(&seconds_);
    seconds = time->tm_sec;
    minute = time->tm_min;
    hour = time->tm_hour;
    day = time->tm_mday;
    month = time->tm_mon + 1;
    year = time->tm_year + 1900;
}

Date::Date(unsigned minute, unsigned hour, unsigned day, unsigned month, unsigned year)
    : seconds(0), minute(minute), hour(hour), day(day), month(month), year(year) {}

Date::operator std::string() const
{
    stringstream stream;
    stream << *this;
    return stream.str();
}

Date::operator QString() const
{
    stringstream stream;
    stream << *this;
    return stream.str().c_str();
}

Date::operator QDateTime() const
{
    return QDateTime(QDate(year, month, day), QTime(hour, minute));
}

Date::operator time_t() const
{
    time_t t = 0;
    tm *time = localtime(&t);
    time->tm_sec = seconds;
    time->tm_min = minute;
    time->tm_hour = hour;
    time->tm_mday = day;
    time->tm_mon = month - 1;
    time->tm_year = year - 1900;
    return mktime(time);
}

QString Date::toQStringWithoutTime() const
{
    stringstream stream;
    stream << day << "/" << month << "/" << year;
    return stream.str().c_str();
}

ostream & operator <<(ostream &stream, const Date &date)
{
    stream << date.day << "/" << date.month << "/" << date.year << " - " << date.hour << ":" << date.minute;
    return stream;
}

string lowerCase(const string &str)
{
    string returnStr = str;
    transform(returnStr.begin(), returnStr.end(), returnStr.begin(), ::tolower);
    return returnStr;
}

void replaceChars(string &str, const char searchChar, const char newChar)
{
    size_t spacePos;
    while ((spacePos = str.find(searchChar)) != str.npos) str[spacePos] = newChar;
}

const char * limitLength(const char *str, unsigned maxLength)
{
    static const short stringSize = 1024, elipsisLength = 3;
    static char string[stringSize + 1];

    if (maxLength > stringSize) maxLength = stringSize;
    strncpy(string, str, maxLength - elipsisLength);
    if (strlen(str) > maxLength - elipsisLength) strcat(string, "...");
    string[maxLength] = '\0';
    return string;
}

void showInfoDialog(const char *message)
{
    QMessageBox messageBox(QMessageBox::Information, " ", message);
    messageBox.setModal(true);
    messageBox.exec();
}

void showErrorDialog(const char *message)
{
    QMessageBox messageBox(QMessageBox::Warning, "Error", message);
    messageBox.setModal(true);
    messageBox.exec();
}

void showErrorDialog(const vector<string> &errors)
{
    string errorMessage;
    errorMessage.reserve(1024);
    for (unsigned i = 0; i < errors.size(); ++i) errorMessage += errors[i];
    showErrorDialog(errorMessage.c_str());
}

void showFatalDialog(const char *message)
{
    QMessageBox messageBox(QMessageBox::Critical, "Fatal Error", message);
    messageBox.setModal(true);
    messageBox.exec();
}

bool showYesNoDialog(const char *question)
{
    QMessageBox messageBox(QMessageBox::Question, " ", question, QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    messageBox.setModal(true);
    switch (messageBox.exec())
    {
    case QMessageBox::Yes: return true;
    default: return false;
    }
}

void showPendingDialog(const char *message, int (*percentCompleteCheckFunction)(void))
{
    PendingDialog dialog(message, percentCompleteCheckFunction);
    dialog.setModal(true);
    dialog.exec();
}

const char * createFullName(const char *forename, const char *surname)
{
    static char name[1024];

    strcpy(name, forename);
    strcat(name, " ");
    strcat(name, surname);

    return name;
}

void addError(vector<string> &errors, string error)
{
    error += '\n';
    // Add the error to the list if it isn't already found in the list
    bool alreadyAdded = false;
    for (unsigned i = 0; i < errors.size(); ++i)
    {
        if (errors[i] == error)
        {
            alreadyAdded = true;
            break;
        }
    }
    if (!alreadyAdded) errors.push_back(error);
}
