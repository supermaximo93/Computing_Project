/*
 * ExpenseController.h
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifndef EXPENSECONTROLLER_H
#define EXPENSECONTROLLER_H

#include "Database.h"

class QWidget;
class Expense;

namespace ExpenseController
{

void Index(QWidget *caller);

void Show(int expenseId, QWidget *caller);
void Show(Expense &expense, QWidget *caller);

Expense New(QWidget *caller);

void Edit(int expenseId, QWidget *caller);
void Edit(Expense &expense, QWidget *caller);

bool Create(Expense &expenseAttributes, QWidget *caller);

bool Update(const Expense &expense, QWidget *caller);

bool Destroy(int expenseId, QWidget *caller);
bool Destroy(Expense &expense, QWidget *caller);

Expense getExpense(int expenseId);
Database<Expense>::recordListPtr getAllExpenses();

}

#endif // EXPENSECONTROLLER_H
