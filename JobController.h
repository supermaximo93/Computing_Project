/*
 * JobController.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef JOBCONTROLLER_H
#define JOBCONTROLLER_H

#include "Database.h"

class QWidget;
class Job;
class Part;
class Task;

namespace JobController
{

void Index(QWidget *caller);

void Show(int jobId, QWidget *caller);
void Show(Job &job, QWidget *caller);

Job New(QWidget *caller);

void Edit(int jobId, QWidget *caller);
void Edit(Job &job, QWidget *caller);

bool Create(Job &jobAttributes, QWidget *caller);

bool Update(const Job &job, QWidget *caller);

bool Destroy(int jobId, QWidget *caller);
bool Destroy(Job &job, QWidget *caller);

Job getJob(int jobId);
Database<Job>::recordListPtr getAllJobs();
Database<Part>::recordListPtr getJobParts(int jobId);
Database<Task>::recordListPtr getJobTasks(int jobId);

void sortJobsByDate(Database<Job>::recordList &jobs, bool ascending = true);

}

#endif // JOBCONTROLLER_H
