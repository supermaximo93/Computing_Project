/*
 * TaskController.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "TaskController.h"
#include "Databases.h"
#include "Task.h"

void TaskController::Index(QWidget * caller)
{

}

void TaskController::Show(int taskId, QWidget * caller)
{

}

void TaskController::Show(Task & task, QWidget * caller)
{

}

Task TaskController::New(QWidget * caller)
{
    return Task();
}

void TaskController::Edit(int taskId, QWidget * caller)
{

}

void TaskController::Edit(Task & task, QWidget * caller)
{

}

bool TaskController::Create(Task & taskAttributes, QWidget * caller)
{

}

bool TaskController::Update(const Task & task, QWidget * caller)
{

}

bool TaskController::Destroy(int taskId, QWidget * caller)
{

}

bool TaskController::Destroy(Task & task, QWidget * caller)
{

}

Task TaskController::getTask(int taskId)
{
    Task task;
    try { task = Databases::tasks().findRecord("id", taskId); }
    catch (const std::exception & e)
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
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return Database<Task>::recordListPtr(new Database<Task>::recordList);
    }
    return tasks;
}
