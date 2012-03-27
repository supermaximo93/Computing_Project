/*
 * JobController.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

using namespace std;

#include "JobController.h"
#include "PartController.h"
#include "TaskController.h"
#include "Databases.h"
#include "Database.h"
#include "Job.h"
#include "Customer.h"
#include "Part.h"
#include "Task.h"

#include "dialogs/job/JobIndex.h"
#include "dialogs/job/JobShow.h"
#include "dialogs/job/JobForm.h"

namespace DateFunctions
{
    static time_t dateLowerBound, dateUpperBound;

    static bool isJobDateWithinBounds(const Job &job, void *)
    {
        return (job.getDate() >= dateLowerBound) && (job.getDate() <= dateUpperBound);
    }
}

void JobController::Index(const QDate &date, QWidget *caller)
{
    Database<Job>::recordListPtr jobs;
    try { jobs = Databases::jobs().allRecords(); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    DateFunctions::dateLowerBound = Date(0, 0, 1, date.month(), date.year());
    DateFunctions::dateUpperBound = time_t(Date(0, 0, 1, date.month() + 1, date.year())) - 1;
    Databases::jobs().keepRecords(*jobs, DateFunctions::isJobDateWithinBounds, NULL);

    const QString title = "Jobs for " + QDate::longMonthName(date.month()) + ' ' + toString(date.year()).c_str();
    JobIndex(*jobs, title, caller).exec();
}

void JobController::Show(const int jobId, QWidget *caller)
{
    Job job;
    try { job = Databases::jobs().findRecord("id", jobId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (job.null())
    {
        showErrorDialog(("Job with ID " + toString(jobId) + " could not be found").c_str());
        return;
    }

    Show(job, caller);
}

void JobController::Show(Job &job, QWidget *caller)
{
    Database<Part>::recordListPtr parts;
    Database<Task>::recordListPtr tasks;
    try
    {
        parts = Databases::parts().findRecords("jobId", job.getId());
        tasks = Databases::tasks().findRecords("jobId", job.getId());
    }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    JobShow(job, *parts, *tasks, caller).exec();
}

Job JobController::New(QWidget *caller)
{
    Job job;
    Database<Part>::recordList parts;
    Database<Task>::recordList tasks;

    Database<Customer>::recordListPtr customers;
    try { customers = Databases::customers().allRecords(); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return job;
    }

    JobForm view(job, *customers, parts, tasks, caller);
    return (view.exec() == QDialog::Rejected ? Job() : job);
}

void JobController::Edit(const int jobId, QWidget *caller)
{
    Job job;
    try { job = Databases::jobs().findRecord("id", jobId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (job.null())
    {
        showErrorDialog(("Job with ID " + toString(jobId) + " could not be found").c_str());
        return;
    }

    Edit(job, caller);
}

void JobController::Edit(Job &job, QWidget *caller)
{
    Database<Customer>::recordListPtr customers;
    Database<Part>::recordListPtr parts;
    Database<Task>::recordListPtr tasks;
    try
    {
        customers = Databases::customers().allRecords();
        parts = Databases::parts().findRecords("jobId", job.getId());
        tasks = Databases::tasks().findRecords("jobId", job.getId());
    }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    Job tempJob = job;
    JobForm view(tempJob, *customers, *parts, *tasks, caller);
    if (view.exec() == JobForm::Accepted) job = tempJob;
}

bool JobController::Create(Job &jobAttributes, QWidget *)
{
    try { Databases::jobs().addRecord(jobAttributes); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (jobAttributes.null())
    {
        showErrorDialog("There was an error with adding the job to the database");
        return false;
    }

    return true;
}

bool JobController::Update(const Job &job, QWidget *)
{
    bool success = false;
    try { success = Databases::jobs().updateRecord(job); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with updating the job in the database");
    return success;
}

bool JobController::Destroy(const int jobId, QWidget * caller)
{
    // Delete all associated parts and tasks first, recording any errors
    vector<string> errors;
    Database<Part>::recordListPtr parts = Databases::parts().findRecords("jobId", jobId);
    Database<Task>::recordListPtr tasks = Databases::tasks().findRecords("jobId", jobId);
    errors.reserve(parts->size() + tasks->size());
    for (unsigned i = 0; i < parts->size(); ++i) PartController::Destroy(parts->at(i), caller);
    for (unsigned i = 0; i < tasks->size(); ++i) TaskController::Destroy(tasks->at(i), caller);

    // If there were errors, report them
    if (errors.size() > 0) showErrorDialog(errors);
    else // otherwise try and delete the job record
    {
        bool success = false;
        try { success = Databases::jobs().deleteRecord(jobId); }
        catch (const std::exception &e)
        {
            showErrorDialog(e.what());
            return false;
        }

        if (!success) showErrorDialog("There was an error with removing the job from the database");
        return success;
    }

    return false;
}

bool JobController::Destroy(Job &job, QWidget *caller)
{
    if (Destroy(job.getId(), caller))
    {
        job = Job();
        return true;
    }

    return false;
}

Job JobController::getJob(const int jobId)
{
    Job job;
    try { job = Databases::jobs().findRecord("id", jobId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Job();
    }
    return job;
}

Database<Job>::recordListPtr JobController::getAllJobs()
{
    Database<Job>::recordListPtr jobs;
    try { jobs = Databases::jobs().allRecords(); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<Job>::recordListPtr(new Database<Job>::recordList);
    }
    return jobs;
}

Database<Part>::recordListPtr JobController::getJobParts(const int jobId)
{
    Database<Part>::recordListPtr parts;
    try { parts = Databases::parts().findRecords("jobId", jobId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<Part>::recordListPtr(new Database<Part>::recordList);
    }
    return parts;
}

Database<Task>::recordListPtr JobController::getJobTasks(const int jobId)
{
    Database<Task>::recordListPtr tasks;
    try { tasks = Databases::tasks().findRecords("jobId", jobId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<Task>::recordListPtr(new Database<Task>::recordList);
    }
    return tasks;
}

void JobController::sortJobsByDate(Database<Job>::recordList &jobs, bool ascending)
{
    struct NestedFunctions
    {
        static int dateCompareAsc(const Job &job1, const Job &job2)
        {
            const time_t date1 = job1.getDate(), date2 = job2.getDate();
            if (date1 < date2) return -1;
            if (date1 > date2) return 1;
            return 0;
        }

        static int dateCompareDec(const Job &job1, const Job &job2)
        {
            const time_t date1 = job1.getDate(), date2 = job2.getDate();
            if (date1 < date2) return 1;
            if (date1 > date2) return -1;
            return 0;
        }
    };

    Databases::jobs().sortRecords(jobs, 0, jobs.size() - 1, ascending ?
                                      NestedFunctions::dateCompareAsc : NestedFunctions::dateCompareDec);
}
