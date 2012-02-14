/*
 * JobShow.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef JOBSHOW_H
#define JOBSHOW_H

#include <QDialog>

#include "Database.h"

namespace Ui
{
    class JobShow;
}

class Job;
class Part;
class Task;

class JobShow : public QDialog
{
    Q_OBJECT

public:
    explicit JobShow(Job & job, const Database<Part>::recordList & parts,
                     const Database<Task>::recordList & tasks, QWidget * parent = 0);
    ~JobShow();

    void updateView();

private slots:
    void on_pushButton_ok_released();

    void on_pushButton_edit_released();

    void on_pushButton_delete_released();

private:
    Ui::JobShow * ui;
    Job & job;
    const Database<Part>::recordList & parts;
    const Database<Task>::recordList & tasks;
};

#endif // JOBSHOW_H
