/*
 * VatRateIndex.cpp
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#include "VatRateIndex.h"
#include "ui_VatRateIndex.h"

#include "VatRate.h"
#include "VatRateController.h"

VatRateIndex::VatRateIndex(Database<VatRate>::recordList &vatRates, QWidget *parent)
    : QDialog(parent), ui(new Ui::VatRateIndex), vatRates(vatRates)
{
    ui->setupUi(this);
    updateView();
}

VatRateIndex::~VatRateIndex()
{
    delete ui;
}

void VatRateIndex::updateView()
{
    VatRateController::sortVatRatesByStartDate(vatRates);

    ui->tableWidget->clear();

    QStringList headings;
    headings.append("Vat rate");
    headings.append("Start date");
    ui->tableWidget->setHorizontalHeaderLabels(headings);

    ui->tableWidget->setRowCount(vatRates.size());
    for (unsigned i = 0; i < vatRates.size(); ++i)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem((toString(vatRates[i].getValue()) + '%').c_str()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(Date(vatRates[i].getStartDate()).toQStringWithoutTime()));
    }
}

void VatRateIndex::on_pushButton_ok_clicked()
{
    done(Accepted);
}

void VatRateIndex::on_pushButton_newVatRate_clicked()
{
    VatRate vatRate = VatRateController::New(this);
    if (!vatRate.null())
    {
        vatRates.push_back(vatRate);
        updateView();
    }
}

void VatRateIndex::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    VatRate &vatRate = vatRates[index.row()];
    VatRateController::Show(vatRate, this);
    if (vatRate.null()) vatRates.erase(vatRates.begin() + index.row());
    updateView();
}
