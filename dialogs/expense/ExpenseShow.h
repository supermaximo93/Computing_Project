/*
 * ExpenseShow.h
 *
 *  Created on: 22 Feb 2012
 *      Author: Max Foster
 */

#ifndef EXPENSESHOW_H
#define EXPENSESHOW_H

#include <QDialog>

namespace Ui
{
    class ExpenseShow;
}

class Expense;

class ExpenseShow : public QDialog
{
    Q_OBJECT

public:
    explicit ExpenseShow(Expense &expense, QWidget *parent = NULL);
    ~ExpenseShow();

    void updateView();

private slots:
    void on_pushButton_ok_released();

    void on_pushButton_edit_released();

    void on_pushButton_delete_released();

private:
    Ui::ExpenseShow *ui;

    Expense &expense;
};

#endif // EXPENSESHOW_H
