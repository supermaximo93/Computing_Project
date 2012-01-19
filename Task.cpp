/*
 * Task.cpp
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
#include <string.h>
using namespace std;

#include "Task.h"

const std::string Task::databaseFilename = "tasks.dat";

Task::Task(const int jobId, const time_t date, const char * newDescription)
    : jobId(jobId), date(date)
{
    description = new char[descriptionLength + 1];
    strcpy(description, newDescription);
}

Task::Task(const Task & task)
{
    description = new char[descriptionLength + 1];
    (*this) = task;
}

Task::~Task()
{
    delete[] description;
}

void Task::operator =(const Task & task)
{
    *((Record*)this) = (Record)task;
    jobId = task.jobId;
    date = task.date;
    strcpy(description, task.description);
}

void Task::writeToFile(fstream & file) const
{
    Record::writeToFile(file);
    file.write(reinterpret_cast<const char *>(&jobId), sizeof(jobId));
    file.write(reinterpret_cast<const char *>(&date), sizeof(date));
    file.write(description, descriptionLength);
}

void Task::readFromFile(fstream & file)
{
    Record::readFromFile(file);
    file.read(reinterpret_cast<char *>(&jobId), sizeof(jobId));
    file.read(reinterpret_cast<char *>(&date), sizeof(date));
    file.read(description, descriptionLength);
}

int Task::size()
{
    return Record::size() + sizeof(int) + sizeof(time_t) + descriptionLength;
}

bool Task::hasMatchingField(const string & fieldName, const int searchTerm) const
{
    if (fieldName == "jobid") return (jobId == searchTerm);
    return Record::hasMatchingField(fieldName, searchTerm);
}

bool Task::hasMatchingField(const string & fieldName, const time_t searchTerm) const
{
    if (fieldName == "date") return (date == searchTerm);
    return false;
}

bool Task::hasMatchingField(const string & fieldName, const char * searchTerm) const
{
    if (fieldName == "description") return (strcmp(description, searchTerm) == 0);
    return false;
}

int Task::getJobId() const
{
    return jobId;
}

void Task::setJobId(const int newJobId)
{
    jobId = newJobId;
}

time_t Task::getDate() const
{
    return date;
}

void Task::setDate(const time_t newDate)
{
    date = newDate;
}

const char * Task::getDescription() const
{
    return description;
}

void Task::setDescription(const char * newDescription)
{
    strcpy(description, newDescription);
}
