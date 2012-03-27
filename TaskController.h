/*
 * TaskController.h
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

#include "Database.h"

class QWidget;
class Task;

namespace TaskController
{

void Index(QWidget *caller);

void Show(int taskId, QWidget *caller);
void Show(Task &task, QWidget *caller);

Task New(QWidget *caller);

void Edit(int taskId, QWidget *caller);
void Edit(Task &task, QWidget *caller);

bool Create(Task &taskAttributes, QWidget *caller);

bool Update(const Task &task, QWidget *caller);

bool Destroy(int taskId, QWidget *caller);
bool Destroy(Task &task, QWidget *caller);

Task getTask(int taskId);
Database<Task>::recordListPtr getAllTasks();

void sortTasksByDate(Database<Task>::recordList &tasks, bool ascending = true);

}

#endif // TASKCONTROLLER_H
