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

Date::Date(const time_t seconds_)
{
    tm * time = localtime(&seconds_);
    seconds = time->tm_sec;
    minute = time->tm_min;
    hour = time->tm_hour;
    day = time->tm_mday;
    month = time->tm_mon + 1;
    year = time->tm_year + 1900;
}

Date::Date(unsigned minute, unsigned hour, unsigned day, unsigned month, unsigned year)
    : seconds(0), minute(minute), hour(hour), day(day), month(month), year(year) {}

Date::operator QString()
{
    stringstream stream;
    stream << *this;
    return QString(stream.str().c_str());
}

Date::operator QDateTime()
{
    return QDateTime(QDate(year, month, day), QTime(hour, minute));
}

Date::operator time_t()
{
    time_t t = 0;
    tm * time = localtime(&t);
    time->tm_sec = seconds;
    time->tm_min = minute;
    time->tm_hour = hour;
    time->tm_mday = day;
    time->tm_mon = month - 1;
    time->tm_year = year - 1900;
    return mktime(time);
}

std::ostream & operator <<(std::ostream & stream, const Date & date)
{
    stream << date.day << "/" << date.month << "/" << date.year << " - " << date.hour << ":" << date.minute;
    return stream;
}

string lowerCase(const string & str)
{
    string returnStr = str;
    transform(returnStr.begin(), returnStr.end(), returnStr.begin(), ::tolower);
    return returnStr;
}

void showInfoDialog(const char * message)
{
    QMessageBox messageBox(QMessageBox::Information, " ", message);
    messageBox.exec();
}

void showErrorDialog(const char * message)
{
    QMessageBox messageBox(QMessageBox::Warning, "Error", message);
    messageBox.exec();
}

void showFatalDialog(const char * message)
{
    QMessageBox messageBox(QMessageBox::Critical, "Fatal Error", message);
    messageBox.exec();
}

bool showYesNoDialog(const char * question)
{
    QMessageBox messageBox(QMessageBox::Question, " ", question, QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    switch (messageBox.exec())
    {
    case QMessageBox::Yes: return true;
    default: return false;
    }
}

const char * createFullName(const char * forename, const char * surname)
{
    static char name[1024];

    strcpy(name, forename);
    strcat(name, " ");
    strcat(name, surname);

    return name;
}
