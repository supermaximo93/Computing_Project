/*
 * Task.h
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#ifndef TASK_H_
#define TASK_H_

#include <fstream>
#include <ctime>

#include "Record.h"

class Task : public Record
{
public:
    static const std::string databaseFilename;
    static const int minDescriptionLength, maxDescriptionLength;

    static int size();

    bool pending;

    Task(const int jobId = -1, const time_t date = 0, const char *description = "");
    Task(const Task &task);
    ~Task();

    void operator =(const Task &task);

    void writeToFile(std::fstream &file) const;
    void readFromFile(std::fstream &file);

    bool hasMatchingField(const std::string &fieldName, const int searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const time_t searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const char *searchTerm) const;
    bool hasMatchingField(const std::string &fieldName, const double searchTerm) const;

    bool fieldCompare(const Task &rhs) const;
    bool completeCompare(const Task &rhs) const;

    int getJobId() const;
    void setJobId(const int newJobId);
    static bool isValidJobId(const int value, std::string &errorMessage);

    time_t getDate() const;
    void setDate(const time_t newDate);
    static bool isValidDate(const time_t value, std::string &errorMessage);

    const char * getDescription() const;
    void setDescription(const char *newDescription);
    static bool isValidDescription(const char *value, std::string &errorMessage);

    void validate() const;

private:
    int jobId;
    time_t date;
    char *description;
};

std::ostream & operator <<(std::ostream &stream, const Task &task);

#endif /* TASK_H_ */
