/*
 * JobController.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "JobController.h"
#include "Databases.h"
#include "Database.h"
#include "Job.h"
#include "Part.h"
#include "Task.h"

#include "dialogs/job/JobIndex.h"
#include "dialogs/job/JobShow.h"
#include "dialogs/job/JobNew.h"
#include "dialogs/job/JobEdit.h"

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
    JobNew view(job, caller);
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
    JobEdit view(job, caller);
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

bool JobController::Destroy(const int jobId, QWidget *)
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

bool JobController::Destroy(Job & job, QWidget * caller)
{
    if (Destroy(job.getId(), caller))
    {
        job = Job();
        return true;
    }

    return false;
}
