/*
 * DatePickerDialog.h
 *
 *  Created on: 26 March 2012
 *      Author: Max Foster
 */

#ifndef DATEPICKERDIALOG_H
#define DATEPICKERDIALOG_H

#include <QDialog>

namespace Ui
{
    class DatePickerDialog;
}

class QDate;

class DatePickerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatePickerDialog(QDate &date, QWidget *parent = NULL);
    ~DatePickerDialog();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::DatePickerDialog *ui;
    QDate &date;
};

#endif // DATEPICKERDIALOG_H
