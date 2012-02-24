/*
 * TaskController.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "TaskController.h"
#include "Databases.h"
#include "Task.h"

#include "dialogs/task/TaskShow.h"
#include "dialogs/task/TaskForm.h"

void TaskController::Index(QWidget *) {}

void TaskController::Show(int taskId, QWidget *caller)
{
    Task task;
    try { Databases::tasks().findRecord("id", taskId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (task.null())
    {
        showErrorDialog(("Task with ID " + toString(taskId) + " could not be found").c_str());
        return;
    }

    Show(task, caller);
}

void TaskController::Show(Task &task, QWidget *caller)
{
    TaskShow view(task, caller);
    view.setModal(true);
    view.exec();
}

Task TaskController::New(QWidget *caller)
{
    Task task;
    TaskForm view(task, caller);
    view.setModal(true);
    return (view.exec() == QDialog::Rejected ? Task() : task);
}

void TaskController::Edit(int taskId, QWidget *caller)
{
    Task task;
    try { Databases::tasks().findRecord("id", taskId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (task.null())
    {
        showErrorDialog(("Task with ID " + toString(taskId) + " could not be found").c_str());
        return;
    }

    Edit(task, caller);
}

void TaskController::Edit(Task &task, QWidget *caller)
{
    TaskForm view(task, caller);
    view.setModal(true);
    view.exec();
}

bool TaskController::Create(Task &taskAttributes, QWidget *)
{
    try { Databases::tasks().addRecord(taskAttributes); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (taskAttributes.null())
    {
        showErrorDialog("There was an error with adding the task to the database");
        return false;
    }

    return true;
}

bool TaskController::Update(const Task &task, QWidget *)
{
    bool success = false;
    try { success = Databases::tasks().updateRecord(task); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with updating the task in the database");
    return success;
}

bool TaskController::Destroy(int taskId, QWidget *)
{
    bool success = false;
    try { success = Databases::tasks().deleteRecord(taskId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with removing the task from the database");
    return success;
}

bool TaskController::Destroy(Task &task, QWidget *caller)
{
    if (Destroy(task.getId(), caller))
    {
        task = Task();
        return true;
    }

    return false;
}

Task TaskController::getTask(int taskId)
{
    Task task;
    try { task = Databases::tasks().findRecord("id", taskId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Task();
    }
    return task;
}

Database<Task>::recordListPtr TaskController::getAllTasks()
{
    Database<Task>::recordListPtr tasks;
    try { tasks = Databases::tasks().allRecords(); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<Task>::recordListPtr(new Database<Task>::recordList);
    }
    return tasks;
}
