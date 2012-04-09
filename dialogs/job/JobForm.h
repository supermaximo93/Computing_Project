/*
 * JobNew.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef JOBNEW_H
#define JOBNEW_H

#include <QDialog>

namespace Ui
{
    class JobForm;
}

#include "Database.h"

class Job;
class Customer;
class Part;
class Task;
class QModelIndex;

class JobForm : public QDialog
{
    Q_OBJECT

public:
    explicit JobForm(Job &job, Database<Customer>::recordList &customers, Database<Part>::recordList &parts,
                     Database<Task>::recordList &tasks, QWidget *parent = NULL);
    ~JobForm();

    void updateView();

private slots:
    void on_pushButton_cancel_released();

    void on_pushButton_submit_released();

    void on_dateTimeEdit_date_dateTimeChanged(const QDateTime &date);

    void on_comboBox_customer_currentIndexChanged(int index);

    void on_plainTextEdit_description_textChanged();

    void on_doubleSpinBox_labourCharge_valueChanged(double value);

    void on_comboBox_completionState_currentIndexChanged(int index);

    void on_comboBox_paidBy_currentIndexChanged(int index);

    void on_pushButton_addNewCustomer_released();

    void on_pushButton_viewCustomer_released();

    void on_listWidget_partsE_doubleClicked(const QModelIndex &index);

    void on_listWidget_tasksE_doubleClicked(const QModelIndex &index);

    void on_pushButton_addPart_released();

    void on_pushButton_addTask_released();

    void on_pushButton_datePickerDate_clicked();

    void on_dateTimeEdit_paymentDate_dateTimeChanged(const QDateTime &date);

    void on_pushButton_datePickerPaymentDate_clicked();

private:
    enum
    {
        NEW,
        EDIT
    } const formType;

    Ui::JobForm *ui;
    Job &job;
    Database<Customer>::recordList &customers;
    Database<Part>::recordList &parts;
    Database<Task>::recordList &tasks;

    void addPartsAndTasksToDatabase();

    bool setRecordData();

    void updateCharges(bool updateFields = false);
    double getTotalChargeExclVat();
    double getTotalChargeInclVat();
};

#endif // JOBNEW_H
