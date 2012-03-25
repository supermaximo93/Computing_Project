/*
 * Globals.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "Globals.h"
#include "Utils.h"

#include "VatRate.h"
#include "VatRateController.h"

const double Globals::defaultVatRate = 20.0;

double Globals::vatRate(const Date &time)
{
    return vatRate((time_t)time);
}

double Globals::vatRate(const time_t time)
{
    Database<VatRate>::recordListPtr vatRates = VatRateController::getAllVatRates();
    VatRateController::sortVatRatesByStartDate(*vatRates);

    VatRate foundVatRate;
    for (unsigned i = 0; i < vatRates->size(); ++i)
    {
        VatRate &vatRate = vatRates->at(i);

        if (vatRate.getStartDate() <= time) foundVatRate = vatRate;
        else break;
    }

    return foundVatRate.getValue();
}
