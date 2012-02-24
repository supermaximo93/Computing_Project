/*
 * PaymentMethodDialog.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "PaymentMethodDialog.h"
#include "ui_PaymentMethodDialog.h"

PaymentMethodDialog::PaymentMethodDialog(int &paymentMethodToSet, QWidget *parent)
    : QDialog(parent), ui(new Ui::PaymentMethodDialog), paymentMethodToSet(paymentMethodToSet)
{
    ui->setupUi(this);
}

PaymentMethodDialog::~PaymentMethodDialog()
{
    delete ui;
}

void PaymentMethodDialog::on_buttonBox_accepted()
{
    paymentMethodToSet = ui->comboBox->currentIndex() + 1; // +1 to account for N/A, which is not in the combo box
    done(Accepted);
}

void PaymentMethodDialog::on_buttonBox_rejected()
{
    done(Rejected);
}
