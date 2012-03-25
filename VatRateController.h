/*
 * VatRateController.h
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#ifndef VATRATECONTROLLER_H
#define VATRATECONTROLLER_H

#include "Database.h"

class QWidget;
class VatRate;

namespace VatRateController
{

void Index(QWidget *caller);

void Show(int vatRateId, QWidget *caller);
void Show(VatRate &vatRate, QWidget *caller);

VatRate New(QWidget *caller);

void Edit(int vatRateId, QWidget *caller);
void Edit(VatRate &vatRate, QWidget *caller);

bool Create(VatRate &vatRateAttributes, QWidget *caller);

bool Update(const VatRate &vatRate, QWidget *caller);

bool Destroy(int vatRateId, QWidget *caller);
bool Destroy(VatRate &vatRate, QWidget *caller);

VatRate getVatRate(int vatRateId);
Database<VatRate>::recordListPtr getAllVatRates();

}

#endif // VATRATECONTROLLER_H
