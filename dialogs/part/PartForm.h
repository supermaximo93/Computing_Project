/*
 * PartForm.h
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#ifndef PARTFORM_H
#define PARTFORM_H

#include <QDialog>

namespace Ui
{
    class PartForm;
}

class Part;

class PartForm : public QDialog
{
    Q_OBJECT

public:
    explicit PartForm(Part &part, QWidget *parent = NULL);
    ~PartForm();

    void updateView();

private slots:
    void on_lineEdit_partName_textEdited(const QString &value);

    void on_lineEdit_partNumber_textEdited(const QString &value);

    void on_doubleSpinBox_price_valueChanged(double value);

    void on_doubleSpinBox_vatRate_valueChanged(double value);

    void on_pushButton_submit_released();

    void on_pushButton_cancel_released();

private:
    enum
    {
        NEW,
        EDIT
    } const formType;

    Ui::PartForm *ui;
    Part &part;

    bool setRecordData();
};

#endif // PARTFORM_H
