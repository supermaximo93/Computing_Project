/*
 * JobController.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

using namespace std;

#include "JobController.h"
#include "Databases.h"
#include "Database.h"
#include "Job.h"
#include "Part.h"
#include "Task.h"

#include "dialogs/job/JobIndex.h"
#include "dialogs/job/JobShow.h"
#include "dialogs/job/JobForm.h"

void JobController::Index(QWidget * caller)
{
    Database<Job>::recordListPtr jobs;
    try { jobs = Databases::jobs().allRecords(); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return;
    }

    JobIndex view(*jobs, caller);
    view.setModal(true);
    view.exec();
}

void JobController::Show(const int jobId, QWidget * caller)
{
    Job job;
    try { job = Databases::jobs().findRecord("id", jobId); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (job.null())
    {
        showErrorDialog(("Job with ID " + toString(jobId) + " could not be found").c_str());
        return;
    }

    JobController::Show(job, caller);
}

void JobController::Show(Job & job, QWidget * caller)
{
    Database<Part>::recordListPtr parts;
    Database<Task>::recordListPtr tasks;
    try
    {
        parts = Databases::parts().findRecords("jobId", job.getId());
        tasks = Databases::tasks().findRecords("jobId", job.getId());
    }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return;
    }

    JobShow view(job, *parts, *tasks, caller);
    view.setModal(true);
    view.exec();
}

Job JobController::New(QWidget * caller)
{
    Job job;
    JobForm view(job, caller);
    view.setModal(true);
    int result = view.exec();
    return (result == QDialog::Rejected ? Job() : job);
}

void JobController::Edit(const int jobId, QWidget * caller)
{
    Job job;
    try { job = Databases::jobs().findRecord("id", jobId); }
    catch (const std::exception & e)
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

void JobController::Edit(Job & job, QWidget * caller)
{
    JobForm view(job, caller);
    view.setModal(true);
    view.exec();
}

bool JobController::Create(Job & jobAttributes, QWidget *)
{
    try { Databases::jobs().addRecord(jobAttributes); }
    catch (const std::exception & e)
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

bool JobController::Update(const Job & job, QWidget *)
{
    bool success = false;
    try { success = Databases::jobs().updateRecord(job); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with updating the job in the database");
    return success;
}

void addError(vector<string> & errors, const char * error)
{
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
    if (!alreadyAdded) errors.push_back(error + '\n');
}

bool JobController::Destroy(const int jobId, QWidget *)
{
    // Delete all associated parts and tasks first, recording any errors
    vector<string> errors;
    Database<Part>::recordListPtr parts = Databases::parts().findRecords("jobId", jobId);
    Database<Task>::recordListPtr tasks = Databases::tasks().findRecords("jobId", jobId);
    errors.reserve(parts->size() + tasks->size());
    for (unsigned i = 0; i < parts->size(); ++i)
    {
        try { Databases::parts().deleteRecord(parts->at(i).getId()); }
        catch (const std::exception & e) { addError(errors, e.what()); }
    }
    for (unsigned i = 0; i < tasks->size(); ++i)
    {
        try { Databases::tasks().deleteRecord(tasks->at(i).getId()); }
        catch (const std::exception & e) { addError(errors, e.what()); }
    }

    // If there were errors, report them
    if (errors.size() > 0)
    {
        string errorMessage;
        errorMessage.reserve(1024);
        for (unsigned i = 0; i < errors.size(); ++i) errorMessage += errors[i];
        showErrorDialog(errorMessage.c_str());
    }
    else // otherwise try and delete the job record
    {
        bool success = false;
        try { success = Databases::jobs().deleteRecord(jobId); }
        catch (const std::exception & e)
        {
            showErrorDialog(e.what());
            return false;
        }

        if (!success) showErrorDialog("There was an error with removing the job from the database");
        return success;
    }

    return false;
}

bool JobController::Destroy(Job & job, QWidget * caller)
{
    if (Destroy(job.getId(), caller))
    {
        job = Job();
        return true;
    }

    return false;
}

Database<Job>::recordListPtr getAllJobs()
{
    Database<Job>::recordListPtr jobs;
    try { jobs = Databases::jobs().allRecords(); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return Database<Job>::recordListPtr(new Database<Job>::recordList);
    }
    return jobs;
}
