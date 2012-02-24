/*
 * TaskForm.h
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifndef TASKFORM_H
#define TASKFORM_H

#include <QDialog>

namespace Ui
{
    class TaskForm;
}

class Task;
class QDateTime;

class TaskForm : public QDialog
{
    Q_OBJECT

public:
    explicit TaskForm(Task &task, QWidget *parent = NULL);
    ~TaskForm();

    void updateView();

private slots:
    void on_dateTimeEdit_date_dateTimeChanged(const QDateTime &date);

    void on_plainTextEdit_description_textChanged();

    void on_pushButton_submit_released();

    void on_pushButton_cancel_released();

private:
    enum
    {
        NEW,
        EDIT
    } const formType;

    Ui::TaskForm *ui;
    Task &task;

    bool setRecordData();
};

#endif // TASKFORM_H
