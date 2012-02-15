/*
 * ExpenseController.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "ExpenseController.h"
#include "Databases.h"
#include "Expense.h"

void ExpenseController::Index(QWidget *)
{

}

void ExpenseController::Show(int expenseId, QWidget * caller)
{

}

void ExpenseController::Show(Expense & expense, QWidget * caller)
{

}

Expense ExpenseController::New(QWidget * caller)
{
    return Expense();
}

void ExpenseController::Edit(int expenseId, QWidget * caller)
{

}

void ExpenseController::Edit(Expense & expense, QWidget * caller)
{

}

bool ExpenseController::Create(Expense & expenseAttributes, QWidget *)
{

}

bool ExpenseController::Update(const Expense & expense, QWidget *)
{

}

bool ExpenseController::Destroy(int expenseId, QWidget *)
{

}

bool ExpenseController::Destroy(Expense & expense, QWidget * caller)
{
    if (Destroy(expense.getId(), caller))
    {
        expense = Expense();
        return true;
    }

    return false;
}

Expense ExpenseController::getExpense(int expenseId)
{
    Expense expense;
    try { expense = Databases::expenses().findRecord("id", expenseId); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return Expense();
    }
    return expense;
}

Database<Expense>::recordListPtr ExpenseController::getAllExpenses()
{
    Database<Expense>::recordListPtr expenses;
    try { expenses = Databases::expenses().allRecords(); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return Database<Expense>::recordListPtr(new Database<Expense>::recordList);
    }
    return expenses;
}

