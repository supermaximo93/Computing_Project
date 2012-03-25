/*
 * VatRateIndex.h
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#ifndef VATRATEINDEX_H
#define VATRATEINDEX_H

#include <QDialog>

#include "Database.h"

namespace Ui
{
    class VatRateIndex;
}

class VatRate;
class QModelIndex;

class VatRateIndex : public QDialog
{
    Q_OBJECT

public:
    explicit VatRateIndex(Database<VatRate>::recordList &vatRates, QWidget *parent = NULL);
    ~VatRateIndex();

    void updateView();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_newVatRate_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::VatRateIndex *ui;
    Database<VatRate>::recordList &vatRates;
};

#endif // VATRATEINDEX_H
