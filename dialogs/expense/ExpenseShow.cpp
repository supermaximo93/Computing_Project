/*
 * ExpenseShow.cpp
 *
 *  Created on: 22 Feb 2012
 *      Author: Max Foster
 */

#include "ExpenseShow.h"
#include "ui_ExpenseShow.h"

#include "Expense.h"
#include "ExpenseController.h"

ExpenseShow::ExpenseShow(Expense &expense, QWidget *parent)
    : QDialog(parent), ui(new Ui::ExpenseShow), expense(expense)
{
    ui->setupUi(this);

    updateView();
}

ExpenseShow::~ExpenseShow()
{
    delete ui;
}

void ExpenseShow::updateView()
{
    ui->label_dateE->setText(Date(expense.getDate()));
    ui->label_descriptionE->setText(expense.getDescription());
    ui->label_priceExclVatE->setText(to2Dp(toString(expense.getPrice()).c_str()).prepend(L'£'));
    ui->label_vatE->setText(to2Dp(toString(expense.getVat()).c_str()).prepend(L'£'));
    double totalPrice = expense.getPrice() + expense.getVat();
    ui->label_totalPriceE->setText(to2Dp(toString(totalPrice).c_str()).prepend(L'£'));
    ui->label_typeHeadingE->setText(expense.getTypeString().c_str());
}

void ExpenseShow::on_pushButton_ok_released()
{
    done(Accepted);
}

void ExpenseShow::on_pushButton_edit_released()
{
    ExpenseController::Edit(expense, this);
    updateView();
}

void ExpenseShow::on_pushButton_delete_released()
{
    if (!showYesNoDialog("Are you sure you want to delete this expense?")) return;

    if (ExpenseController::Destroy(expense, this))
    {
        showInfoDialog("The expense was deleted successfully");
        done(Accepted);
    }
}
