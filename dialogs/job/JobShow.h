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
class QModelIndex;
class QTimer;

class JobShow : public QDialog
{
    Q_OBJECT

public:
    explicit JobShow(Job &job, Database<Part>::recordList &parts, Database<Task>::recordList &tasks,
                     QWidget *parent = NULL);
    ~JobShow();

    void updateView();

private slots:
    void checkInternetConnection();

    void on_pushButton_ok_released();

    void on_pushButton_edit_released();

    void on_pushButton_delete_released();

    void on_pushButton_viewCustomerDetails_released();

    void on_listWidget_partsE_doubleClicked(const QModelIndex &index);

    void on_listWidget_tasksE_doubleClicked(const QModelIndex &index);

    void on_pushButton_markAsDone_released();

    void on_pushButton_sendInvoice_released();

    void on_pushButton_markAsPaid_released();

    void on_pushButton_sendReciept_released();

private:
    Ui::JobShow *ui;
    Job &job;
    Database<Part>::recordList &parts;
    Database<Task>::recordList &tasks;
    QTimer *timer;

    void updateCharges();
    double getTotalChargeExclVat();
    double getTotalChargeInclVat();

    bool setNewJobCompletionState(int state);
};

#endif // JOBSHOW_H
