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

class CustomerForm : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerForm(QWidget *parent = NULL);
    ~CustomerForm();

private:
    Ui::CustomerForm *ui;
};

#endif // CUSTOMERFORM_H
