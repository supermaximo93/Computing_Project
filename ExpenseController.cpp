/*
 * ExpenseController.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "ExpenseController.h"
#include "Databases.h"
#include "Expense.h"

#include "dialogs/expense/ExpenseIndex.h"
#include "dialogs/expense/ExpenseShow.h"
#include "dialogs/expense/ExpenseForm.h"

void ExpenseController::Index(QWidget *caller)
{
    Database<Expense>::recordListPtr expenses;
    try { expenses = Databases::expenses().allRecords(); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    ExpenseIndex view(*expenses, caller);
    view.setModal(true);
    view.exec();
}

void ExpenseController::Show(const int expenseId, QWidget *caller)
{
    Expense expense;
    try { expense = Databases::expenses().findRecord("id", expenseId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (expense.null())
    {
        showErrorDialog(("Expense with ID " + toString(expenseId) + " could not be found").c_str());
        return;
    }

    Show(expense, caller);
}

void ExpenseController::Show(Expense &expense, QWidget *caller)
{
    ExpenseShow view(expense, caller);
    view.setModal(true);
    view.exec();
}

Expense ExpenseController::New(QWidget *caller)
{
    Expense expense;
    ExpenseForm view(expense, caller);
    view.setModal(true);
    return (view.exec() == QDialog::Rejected ? Expense() : expense);
}

void ExpenseController::Edit(const int expenseId, QWidget *caller)
{
    Expense expense;
    try { expense = Databases::expenses().findRecord("id", expenseId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (expense.null())
    {
        showErrorDialog(("Expense with ID " + toString(expenseId) + " could not be found").c_str());
        return;
    }

    Edit(expense, caller);
}

void ExpenseController::Edit(Expense &expense, QWidget *caller)
{
    Expense tempExpense = expense;
    ExpenseForm view(tempExpense, caller);
    view.setModal(true);
    if (view.exec() == ExpenseForm::Accepted) expense = tempExpense;
}

bool ExpenseController::Create(Expense &expenseAttributes, QWidget *)
{
    try { Databases::expenses().addRecord(expenseAttributes); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (expenseAttributes.null())
    {
        showErrorDialog("There was an error with adding the expense to the database");
        return false;
    }

    return true;
}

bool ExpenseController::Update(const Expense &expense, QWidget *)
{
    bool success = false;
    try { success = Databases::expenses().updateRecord(expense); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with updating the expense in the database");
    return success;
}

bool ExpenseController::Destroy(const int expenseId, QWidget *)
{
    bool success = false;
    try { success = Databases::expenses().deleteRecord(expenseId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with removing the expense from the database");
    return success;
}

bool ExpenseController::Destroy(Expense &expense, QWidget *caller)
{
    if (Destroy(expense.getId(), caller))
    {
        expense = Expense();
        return true;
    }

    return false;
}

Expense ExpenseController::getExpense(const int expenseId)
{
    Expense expense;
    try { expense = Databases::expenses().findRecord("id", expenseId); }
    catch (const std::exception &e)
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
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<Expense>::recordListPtr(new Database<Expense>::recordList);
    }
    return expenses;
}

void ExpenseController::sortExpensesByDate(Database<Expense>::recordList &expenses, bool ascending)
{
    struct NestedFunctions
    {
        static int dateCompareAsc(const Expense &expense1, const Expense &expense2)
        {
            const time_t date1 = expense1.getDate(), date2 = expense2.getDate();
            if (date1 < date2) return -1;
            if (date1 > date2) return 1;
            return 0;
        }

        static int dateCompareDec(const Expense &expense1, const Expense &expense2)
        {
            const time_t date1 = expense1.getDate(), date2 = expense2.getDate();
            if (date1 < date2) return 1;
            if (date1 > date2) return -1;
            return 0;
        }
    };

    Databases::expenses().sortRecords(expenses, 0, expenses.size() - 1, ascending ?
                                          NestedFunctions::dateCompareAsc : NestedFunctions::dateCompareDec);
}
