/*
 * PaymentMethodDialog.h
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifndef PAYMENTMETHODDIALOG_H
#define PAYMENTMETHODDIALOG_H

#include <QDialog>

namespace Ui
{
    class PaymentMethodDialog;
}

class QDateTime;

class PaymentMethodDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaymentMethodDialog(int &paymentMethodToSet, QDateTime &dateTimeToSet, const QDateTime &minimumDateTime,
                                 QWidget *parent = NULL);
    ~PaymentMethodDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButton_datePicker_clicked();

private:
    Ui::PaymentMethodDialog *ui;
    int &paymentMethodToSet;
    QDateTime &dateTimeToSet;
    const QDateTime &minimumDateTime;
};

#endif // PAYMENTMETHODDIALOG_H
