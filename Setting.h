/*
 * Setting.h
 *
 *  Created on: 22 March 2012
 *      Author: Max Foster
 */

#ifndef SETTING_H
#define SETTING_H

#include <fstream>

#include "Record.h"

class Setting : public Record
{
public:
    static const std::string databaseFilename;
    static const int
    minKeyLength, maxKeyLength,
    minValueLength, maxValueLength;

    static int size();

    Setting(const char *key = "", const char *value = "");
    Setting(const Setting &setting);
    ~Setting();

    void operator =(const Setting &setting);

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

    bool hasMatchingField(const std::string &fieldName, const int searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const char *searchTerm) const;

    bool fieldCompare(const Setting &rhs) const;
    bool completeCompare(const Setting &rhs) const;

    const char * getKey() const;
    void setKey(const char *newKey);
    static bool isValidKey(const char *value, std::string &errorMessage);

    const char * getValue() const;
    void setValue(const char *newValue);
    static bool isValidValue(const char *value, std::string &errorMessage);

    void validate() const;

private:
    char *key, *value;
};

#endif // SETTING_H
