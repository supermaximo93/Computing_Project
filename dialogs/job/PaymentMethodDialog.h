/*
 * PaymentMethodDialog.h
 *
 *  Created on: 15 2 2012
 *      Author: Max Foster
 */

#ifndef PAYMENTMETHODDIALOG_H
#define PAYMENTMETHODDIALOG_H

#include <QDialog>

namespace Ui {
    class PaymentMethodDialog;
}

class PaymentMethodDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaymentMethodDialog(int &paymentMethodToSet, QWidget *parent = NULL);
    ~PaymentMethodDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PaymentMethodDialog *ui;
    int &paymentMethodToSet;
};

#endif // PAYMENTMETHODDIALOG_H
