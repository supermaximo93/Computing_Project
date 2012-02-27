/*
 * CustomerIndex.h
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#ifndef CUSTOMERINDEX_H
#define CUSTOMERINDEX_H

#include <QDialog>

#include "Database.h"

namespace Ui
{
    class CustomerIndex;
}

class Customer;
class QModelIndex;

class CustomerIndex : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerIndex(Database<Customer>::recordList &customers, QWidget *parent = NULL);
    ~CustomerIndex();

    void updateView();

private slots:
    void on_pushButton_ok_released();

    void on_pushButton_addNewCustomer_released();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::CustomerIndex *ui;

    Database<Customer>::recordList &customers;
};

#endif // CUSTOMERINDEX_H
