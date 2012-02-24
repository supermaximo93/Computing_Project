/*
 * TaskShow.h
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifndef TASKSHOW_H
#define TASKSHOW_H

#include <QDialog>

namespace Ui
{
    class TaskShow;
}

class Task;

class TaskShow : public QDialog
{
    Q_OBJECT

public:
    explicit TaskShow(Task &task, QWidget *parent = NULL);
    ~TaskShow();

    void updateView();

private slots:
    void on_pushButton_ok_released();

    void on_pushButton_edit_released();

    void on_pushButton_delete_released();

private:
    Ui::TaskShow *ui;
    Task &task;
};

#endif // TASKSHOW_H
