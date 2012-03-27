/*
 * PaymentMethodDialog.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "PaymentMethodDialog.h"
#include "ui_PaymentMethodDialog.h"

#include "Utils.h"

PaymentMethodDialog::PaymentMethodDialog(int &paymentMethodToSet, QDateTime &dateTimeToSet,
                                         const QDateTime &minimumDateTime, QWidget *parent)
    : QDialog(parent), ui(new Ui::PaymentMethodDialog), paymentMethodToSet(paymentMethodToSet),
      dateTimeToSet(dateTimeToSet), minimumDateTime(minimumDateTime)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(minimumDateTime);
    // Set a fixed date range so we don't need to do validation ourselves
    ui->dateTimeEdit->setDateTimeRange(minimumDateTime, QDateTime(QDate(minimumDateTime.date().year() + 100, 1, 1)));
}

PaymentMethodDialog::~PaymentMethodDialog()
{
    delete ui;
}

void PaymentMethodDialog::on_buttonBox_accepted()
{
    dateTimeToSet = ui->dateTimeEdit->dateTime();
    paymentMethodToSet = ui->comboBox->currentIndex() + 1; // +1 to account for N/A, which is not in the combo box
    done(Accepted);
}

void PaymentMethodDialog::on_buttonBox_rejected()
{
    done(Rejected);
}

void PaymentMethodDialog::on_pushButton_datePicker_clicked()
{
    QDate date;
    if (showDatePickerDialog(date))
        ui->dateTimeEdit->setDate(QDateTime(date) >= minimumDateTime ? date : minimumDateTime.date());
}
