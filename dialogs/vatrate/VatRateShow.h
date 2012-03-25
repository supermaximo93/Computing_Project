/*
 * VatRateShow.h
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#ifndef VATRATESHOW_H
#define VATRATESHOW_H

#include <QDialog>

namespace Ui
{
    class VatRateShow;
}

class VatRate;

class VatRateShow : public QDialog
{
    Q_OBJECT

public:
    explicit VatRateShow(VatRate &vatRate, QWidget *parent = NULL);
    ~VatRateShow();

    void updateView();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_delete_clicked();

private:
    Ui::VatRateShow *ui;
    VatRate &vatRate;
};

#endif // VATRATESHOW_H
