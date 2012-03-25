/*
 * VatRateShow.cpp
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#include "VatRateShow.h"
#include "ui_VatRateShow.h"

#include "VatRate.h"
#include "VatRateController.h"

#include "Utils.h"

VatRateShow::VatRateShow(VatRate &vatRate, QWidget *parent)
    : QDialog(parent), ui(new Ui::VatRateShow), vatRate(vatRate)
{
    ui->setupUi(this);
    updateView();
}

VatRateShow::~VatRateShow()
{
    delete ui;
}

void VatRateShow::updateView()
{
    ui->label_vatRateE->setText((toString(vatRate.getValue()) + '%').c_str());
    ui->label_startDateE->setText(Date(vatRate.getStartDate()).toQStringWithoutTime());
}

void VatRateShow::on_pushButton_ok_clicked()
{
    done(Accepted);
}

void VatRateShow::on_pushButton_edit_clicked()
{
    VatRateController::Edit(vatRate, this);
    updateView();
}

void VatRateShow::on_pushButton_delete_clicked()
{
    if (!showYesNoDialog("Are you sure you want to delete this VAT rate?")) return;

    if (VatRateController::Destroy(vatRate, this))
    {
        showInfoDialog("The VAT rate was deleted successfully");
        done(Accepted);
    }
}
