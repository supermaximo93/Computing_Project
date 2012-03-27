/*
 * ExpenseIndex.cpp
 *
 *  Created on: 22 Feb 2012
 *      Author: Max Foster
 */

#include "ExpenseIndex.h"
#include "ui_ExpenseIndex.h"

#include "Expense.h"
#include "ExpenseController.h"

ExpenseIndex::ExpenseIndex(Database<Expense>::recordList &expenses, QWidget * parent)
    : QDialog(parent), ui(new Ui::ExpenseIndex), expenses(expenses)
{
    ui->setupUi(this);

    updateView();
}

ExpenseIndex::~ExpenseIndex()
{
    delete ui;
}

void ExpenseIndex::updateView()
{
    ExpenseController::sortExpensesByDate(expenses, false);

    ui->tableWidget->clear();

    QStringList headings;
    headings.append("Expense Description");
    headings.append("Date");
    ui->tableWidget->setHorizontalHeaderLabels(headings);

    const unsigned descriptionLength = 30;
    char description[descriptionLength + 4];
    ui->tableWidget->setRowCount(expenses.size());
    for (unsigned i = 0; i < expenses.size(); ++i)
    {
        strncpy(description, expenses[i].getDescription(), descriptionLength);
        if (strlen(expenses[i].getDescription()) > descriptionLength) strcat(description, "...");
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString(description)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem((QString)Date(expenses[i].getDate())));
    }
}

void ExpenseIndex::on_pushButton_ok_released()
{
    done(Accepted);
}

void ExpenseIndex::on_pushButton_addNewExpense_released()
{
    Expense expense = ExpenseController::New(this);
    if (!expense.null())
    {
        expenses.push_back(expense);
        updateView();
    }
}

void ExpenseIndex::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    Expense &expense = expenses[index.row()];
    ExpenseController::Show(expense, this);
    if (expense.null()) expenses.erase(expenses.begin() + index.row());
    updateView();
}
