/*
 * DatePickerDialog.cpp
 *
 *  Created on: 26 March 2012
 *      Author: Max Foster
 */

#include "DatePickerDialog.h"
#include "ui_DatePickerDialog.h"

#include <QDate>

DatePickerDialog::DatePickerDialog(QDate &date, QWidget *parent)
    : QDialog(parent), ui(new Ui::DatePickerDialog), date(date)
{
    ui->setupUi(this);
    setWindowTitle("Select a date");
}

DatePickerDialog::~DatePickerDialog()
{
    delete ui;
}

void DatePickerDialog::on_pushButton_ok_clicked()
{
    date = ui->calendarWidget->selectedDate();
    done(Accepted);
}

void DatePickerDialog::on_pushButton_cancel_clicked()
{
    done(Rejected);
}
