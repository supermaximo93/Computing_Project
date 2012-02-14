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
class Part;
class Task;

class JobForm : public QDialog
{
    Q_OBJECT

public:
    explicit JobForm(Job & job, QWidget * parent = NULL);
    ~JobForm();

    void updateView();

private slots:
    void on_pushButton_cancel_released();

    void on_pushButton_submit_released();

    void on_comboBox_customer_currentIndexChanged(int index);

    void on_dateTimeEdit_date_dateTimeChanged(const QDateTime & date);

    void on_doubleSpinBox_labourCharge_valueChanged(double value);

    void on_comboBox_completionState_currentIndexChanged(int index);

    void on_comboBox_paidBy_currentIndexChanged(int index);

private:
    enum
    {
        NEW,
        EDIT
    } const formType;

    Ui::JobForm * ui;
    Job & job;
    Database<Part>::recordList parts;
    Database<Task>::recordList tasks;

    bool setRecordData();

    void updateCharges();
    double getTotalChargeExclVat();
    double getTotalChargeInclVat();
};

#endif // JOBNEW_H
