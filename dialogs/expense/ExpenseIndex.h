/*
 * ExpenseIndex.h
 *
 *  Created on: 22 Feb 2012
 *      Author: Max Foster
 */

#ifndef EXPENSEINDEX_H
#define EXPENSEINDEX_H

#include <QDialog>

#include "Database.h"

namespace Ui
{
    class ExpenseIndex;
}

class Expense;
class QModelIndex;

class ExpenseIndex : public QDialog
{
    Q_OBJECT

public:
    explicit ExpenseIndex(Database<Expense>::recordList &expenses, const QString &title = "", QWidget *parent = NULL);
    ~ExpenseIndex();

    void updateView();

private slots:
    void on_pushButton_ok_released();

    void on_pushButton_addNewExpense_released();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::ExpenseIndex *ui;

    Database<Expense>::recordList &expenses;
};

#endif // EXPENSEINDEX_H
