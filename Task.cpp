/*
 * Task.cpp
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
#include <stdexcept>
#include <ctime>
#include <cstring>
using namespace std;

#include "Task.h"
#include "Utils.h"

const std::string Task::databaseFilename = "tasks.dat";
const int Task::minDescriptionLength = 0, Task::maxDescriptionLength = 256;

int Task::size()
{
    return Record::size() + sizeof(int) + sizeof(time_t) + maxDescriptionLength + 1;
}

Task::Task(const int jobId, const time_t date, const char *newDescription)
    : pending(false), jobId(jobId), date(date == 0 ? time(NULL) : date)
{
    description = new char[maxDescriptionLength + 1];
    strcpy(description, newDescription);
}

Task::Task(const Task &task)
{
    description = new char[maxDescriptionLength + 1];
    (*this) = task;
}

Task::~Task()
{
    delete[] description;
}

void Task::operator =(const Task &task)
{
    pending = task.pending;
    *((Record*)this) = (Record)task;
    jobId = task.jobId;
    date = task.date;
    strcpy(description, task.description);
}

void Task::writeToFile(fstream &file) const
{
    Record::writeToFile(file);
    file.write(reinterpret_cast<const char *>(&jobId), sizeof(jobId));
    file.write(reinterpret_cast<const char *>(&date), sizeof(date));
    file.write(description, maxDescriptionLength + 1);
}

void Task::readFromFile(fstream &file)
{
    Record::readFromFile(file);
    file.read(reinterpret_cast<char *>(&jobId), sizeof(jobId));
    file.read(reinterpret_cast<char *>(&date), sizeof(date));
    file.read(description, maxDescriptionLength + 1);
}

bool Task::hasMatchingField(const string &fieldName, const int searchTerm) const
{
    if (fieldName == "jobid") return (jobId == searchTerm);
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Task::hasMatchingField(const string &fieldName, const time_t searchTerm) const
{
    if (fieldName == "date") return (date == searchTerm);
    return false;
}

bool Task::hasMatchingField(const string &fieldName, const char *searchTerm) const
{
    if (fieldName == "description") return (strcmp(description, searchTerm) == 0);
    return false;
}

bool Task::fieldCompare(const Task &rhs) const
{
    if (jobId != rhs.jobId) return false;
    if (date != rhs.date) return false;
    if (strcmp(description, rhs.description) != 0) return false;
    return true;
}

bool Task::completeCompare(const Task &rhs) const
{
    if (getId() != rhs.getId()) return false;
    return fieldCompare(rhs);
}

int Task::getJobId() const
{
    return jobId;
}

void Task::setJobId(const int newJobId)
{
    string errorMessage;
    if (isValidJobId(newJobId, errorMessage)) jobId = newJobId;
    else throw std::runtime_error(errorMessage);
}

bool Task::isValidJobId(const int value, std::string &errorMessage)
{
    if (value >= 0) return true;

    errorMessage = "Job ID must be at least 0";
    return false;
}

time_t Task::getDate() const
{
    return date;
}

void Task::setDate(const time_t newDate)
{
    string errorMessage;
    if (isValidDate(newDate, errorMessage)) date = newDate;
    else throw std::runtime_error(errorMessage);
}

bool Task::isValidDate(const time_t, std::string &)
{
    return true;
}

const char * Task::getDescription() const
{
    return description;
}

void Task::setDescription(const char *newDescription)
{
    string errorMessage;
    if (isValidDescription(newDescription, errorMessage)) strcpy(description, newDescription);
    else throw std::runtime_error(errorMessage);
}

bool Task::isValidDescription(const char *value, std::string &errorMessage)
{
    if ((value == NULL) || (value[0] == '\0'))
    {
        errorMessage = "Description must be present";
        return false;
    }

    return validateLengthOf(value, maxDescriptionLength, "Description", errorMessage);
}

void Task::validate() const
{
    string errorMessage;
    if (!isValidJobId(jobId, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidDate(date, errorMessage)) throw std::runtime_error(errorMessage);
    if (!isValidDescription(description, errorMessage)) throw std::runtime_error(errorMessage);
}
