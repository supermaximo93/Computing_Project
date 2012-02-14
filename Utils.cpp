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
#include <QMessageBox>

#include "Utils.h"

Date::Date(const time_t seconds)
{
    tm * time;
    time = localtime(&seconds);
    minute = time->tm_min;
    hour = time->tm_hour;
    day = time->tm_mday;
    month = time->tm_mon + 1;
    year = time->tm_year + 1900;
}

Date::operator QString()
{
    stringstream stream;
    stream << *this;
    return QString(stream.str().c_str());
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
