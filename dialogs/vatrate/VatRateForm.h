/*
 * VatRateForm.h
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#ifndef VATRATEFORM_H
#define VATRATEFORM_H

#include <QDialog>

namespace Ui
{
    class VatRateForm;
}

class VatRate;
class QDate;

class VatRateForm : public QDialog
{
    Q_OBJECT

public:
    explicit VatRateForm(VatRate &vatRate, QWidget *parent = NULL);
    ~VatRateForm();

    void updateView();

private slots:
    void on_doubleSpinBox_vatRate_valueChanged(double value);

    void on_dateEdit_startDate_dateChanged(const QDate &date);

    void on_pushButton_submit_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_datePicker_clicked();

private:
    enum
    {
        NEW,
        EDIT
    } const formType;

    Ui::VatRateForm *ui;
    VatRate &vatRate;

    bool setRecordData();
};

#endif // VATRATEFORM_H
