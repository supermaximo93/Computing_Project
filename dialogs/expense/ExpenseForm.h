/*
 * ExpenseForm.h
 *
 *  Created on: 22 Feb 2012
 *      Author: Max Foster
 */

#ifndef EXPENSEFORM_H
#define EXPENSEFORM_H

#include <QDialog>

namespace Ui
{
    class ExpenseForm;
}

class Expense;
class QDateTime;

class ExpenseForm : public QDialog
{
    Q_OBJECT

public:
    explicit ExpenseForm(Expense & expense, QWidget * parent = NULL);
    ~ExpenseForm();

    void updateView();

private slots:
    void on_pushButton_submit_released();

    void on_pushButton_cancel_released();

    void on_dateTimeEdit_date_dateTimeChanged(const QDateTime & date);

    void on_plainTextEdit_description_textChanged();

    void on_doubleSpinBox_priceExclVat_valueChanged(double value);

    void on_doubleSpinBox_vat_valueChanged(double value);

    void on_comboBox_typeHeading_currentIndexChanged(int index);

private:
    enum
    {
        NEW,
        EDIT
    } const formType;

    Ui::ExpenseForm * ui;
    Expense & expense;

    bool setRecordData();
};

#endif // EXPENSEFORM_H
