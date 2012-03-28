/*
 * PartShow.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "PartShow.h"
#include "ui_PartShow.h"

#include "Part.h"
#include "PartController.h"
#include "Utils.h"

PartShow::PartShow(Part &part, QWidget *parent)
    : QDialog(parent), ui(new Ui::PartShow), part(part)
{
    ui->setupUi(this);
    updateView();
}

PartShow::~PartShow()
{
    delete ui;
}

void PartShow::updateView()
{
    setWindowTitle(part.getName());
    ui->label_partNameE->setText(part.getName());
    ui->label_partNumberE->setText(part.getNumber());
    ui->label_partPriceE->setText(to2Dp(toString(part.getPrice()).c_str()).prepend(L'£'));
    ui->label_vatRateE->setText((toString(part.getVatRate()) + '%').c_str());
    ui->label_quantityE->setText(toString(part.getQuantity()).c_str());
}

void PartShow::on_pushButton_ok_released()
{
    done(Accepted);
}

void PartShow::on_pushButton_edit_released()
{
    PartController::Edit(part, this);
    updateView();
}

void PartShow::on_pushButton_delete_released()
{
    if (!showYesNoDialog("Are you sure you want to delete this part?")) return;

    if (part.pending)
    {
        part = Part();
        done(Accepted);
    }
    else if (PartController::Destroy(part, this))
    {
        showInfoDialog("The part was deleted successfully");
        done(Accepted);
    }
}
