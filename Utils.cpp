/*
 * Utils.cpp
 *
 *  Created on: 27 Jan 2012
 *      Author: Max Foster
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <ctime>
using namespace std;

#include <QString>
#include <QDateTime>
#include <QMessageBox>
#include <QFile>
#include <QDir>

#include "Utils.h"

#include "dialogs/utils/PendingDialog.h"
#include "dialogs/utils/MoveFileExistsDialog.h"
#include "dialogs/utils/DatePickerDialog.h"

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

Date::Date(const QDateTime &qDateTime)
    : seconds(0), minute(qDateTime.time().minute()), hour(qDateTime.time().hour()),
      day(qDateTime.date().day()), month(qDateTime.date().month()), year(qDateTime.date().year()) {}

Date::Date(const QDate &qDate)
    : seconds(0), minute(0), hour(0), day(qDate.day()), month(qDate.month()), year(qDate.year()) {}

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

Date::operator QDate() const
{
    return QDate(year, month, day);
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
    static const unsigned stringSize = 1024, elipsisLength = 3;
    static char string[stringSize + 1];

    if (maxLength > stringSize) maxLength = stringSize;
    strncpy(string, str, maxLength - elipsisLength);
    if (strlen(str) > maxLength - elipsisLength) strcat(string, "...");
    string[maxLength] = '\0';
    return string;
}

void showInfoDialog(const char *message)
{
    QMessageBox(QMessageBox::Information, " ", message).exec();
}

void showErrorDialog(const char *message)
{
    QMessageBox(QMessageBox::Warning, "Error", message).exec();
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
    QMessageBox(QMessageBox::Critical, "Fatal Error", message).exec();
}

bool showYesNoDialog(const char *question)
{
    QMessageBox messageBox(QMessageBox::Question, " ", question, QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    switch (messageBox.exec())
    {
    case QMessageBox::Yes: return true;
    default: return false;
    }
}

void showPendingDialog(const char *message, int (*percentCompleteCheckFunction)(void))
{
    PendingDialog(message, percentCompleteCheckFunction).exec();
}

bool showDatePickerDialog(QDate &dateToModify)
{
    if (DatePickerDialog(dateToModify).exec() == DatePickerDialog::Accepted) return true;
    return false;
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

bool validateLengthOf(const char *value, const int min, const int max, const string &valueName, string &errorMessage)
{
    const int length = strlen(value);
    const bool isValid = (length >= min) && (length <= max);
    if (!isValid)
        errorMessage = valueName + " must be between " + toString(min) + " and " + toString(max) + " characters";

    return isValid;
}

bool validateLengthOf(const char *value, const int max, const string &valueName, string &errorMessage)
{
    const int length = strlen(value);
    const bool isValid = (length <= max);
    if (!isValid) errorMessage = valueName + " cannot be longer than " + toString(max) + " characters";
    return isValid;
}

void copyFile(const char *sourceFilename, const char *destinationFilename)
{
    if (QFile::exists(destinationFilename)) QFile::remove(destinationFilename);
    QFile::copy(sourceFilename, destinationFilename);
}

void moveFile(const char *sourceFilename, const char *destinationFilename)
{
    if (QFile::exists(destinationFilename))
    {
        switch (MoveFileExistsDialog(sourceFilename, destinationFilename).exec())
        {
        case MoveFileExistsDialog::MOVE_AND_REPLACE:
            QFile::remove(destinationFilename);
            QFile::copy(sourceFilename, destinationFilename);
            remove(sourceFilename);
            break;

        case MoveFileExistsDialog::DELETE_AND_KEEP:
            remove(sourceFilename);
            break;

        default: break;
        }
    }
    else
    {
        QFile::copy(sourceFilename, destinationFilename);
        remove(sourceFilename);
    }
}

void moveDirectory(const char *sourceDirectoryPath, const char *destinationDirectoryPath)
{
#ifdef _WIN32
    const char slashChar = '\\';
#else
    const char slashChar = '/';
#endif

    QDir dir, sourceDirectory(sourceDirectoryPath);
    if (!sourceDirectory.exists()) return;

    dir.mkpath(destinationDirectoryPath);

    QStringList entryList = sourceDirectory.entryList();
    for (int i = 0; i < entryList.size(); ++i)
    {
        QString &entry = entryList[i];
        if ((entry.length() == 0) || (entry[0] == QChar('.'))) continue;

        string source = string(sourceDirectoryPath) + slashChar + entry.toStdString(),
                destination = string(destinationDirectoryPath) + slashChar + entry.toStdString();

        if (QDir(source.c_str()).exists()) moveDirectory(source.c_str(), destination.c_str());
        else
        {
            copyFile(source.c_str(), destination.c_str());
            remove(source.c_str());
        }
    }

    dir.rmdir(sourceDirectoryPath);
}
