/*
 * CustomerForm.h
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#ifndef CUSTOMERFORM_H
#define CUSTOMERFORM_H

#include <QDialog>

namespace Ui
{
    class CustomerForm;
}

class Customer;

class CustomerForm : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerForm(Customer & customer, QWidget *parent = NULL);
    ~CustomerForm();

    void updateView();

private slots:
    void on_pushButton_submit_released();

    void on_pushButton_cancel_released();

    void on_lineEdit_forename_textChanged(const QString &value);

    void on_lineEdit_surname_textEdited(const QString &value);

    void on_lineEdit_addressLine1_textChanged(const QString &value);

    void on_lineEdit_addressLine2_textChanged(const QString &value);

    void on_lineEdit_town_textChanged(const QString &value);

    void on_lineEdit_postcode_textChanged(const QString &value);

    void on_lineEdit_homePhone_textChanged(const QString &value);

    void on_lineEdit_mobilePhone_textChanged(const QString &value);

    void on_lineEdit_emailAddress_textChanged(const QString &value);

private:
    enum
    {
        NEW,
        EDIT
    } const formType;

    Ui::CustomerForm *ui;
    Customer & customer;

    bool setRecordData();
};

#endif // CUSTOMERFORM_H
