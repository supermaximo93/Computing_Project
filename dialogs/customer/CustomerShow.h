/*
 * CustomerShow.h
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#ifndef CUSTOMERSHOW_H
#define CUSTOMERSHOW_H

#include <QDialog>

#include "Database.h"

namespace Ui
{
    class CustomerShow;
}

class Customer;
class Job;
class QModelIndex;

class CustomerShow : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerShow(Customer &customer, Database<Job>::recordList &jobs, QWidget *parent = NULL);
    ~CustomerShow();

    void updateView();

private slots:
    void on_pushButton_ok_released();

    void on_pushButton_edit_released();

    void on_pushButton_delete_released();

    void on_listWidget_jobs_doubleClicked(const QModelIndex &index);

    void on_pushButton_addJob_released();

private:
    Ui::CustomerShow *ui;

    Customer &customer;
    Database<Job>::recordList &jobs;
};

#endif // CUSTOMERSHOW_H
