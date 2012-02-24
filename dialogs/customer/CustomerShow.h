/*
 * CustomerShow.h
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#ifndef CUSTOMERSHOW_H
#define CUSTOMERSHOW_H

#include <QDialog>

namespace Ui
{
    class CustomerShow;
}

class Customer;
class QModelIndex;

class CustomerShow : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerShow(Customer &customer, QWidget *parent = NULL);
    ~CustomerShow();

private slots:
    void on_pushButton_ok_released();

    void on_pushButton_edit_released();

    void on_pushButton_delete_released();

    void on_listView_jobs_doubleClicked(const QModelIndex &index);

    void on_pushButton_addJob_released();

private:
    Ui::CustomerShow *ui;

    Customer &customer;
};

#endif // CUSTOMERSHOW_H
