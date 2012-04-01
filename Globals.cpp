/*
 * Globals.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "Globals.h"
#include "Utils.h"
#include "Databases.h"

#include "VatRate.h"
#include "VatRateController.h"

const double Globals::defaultVatRate = 20.0;

double Globals::vatRate(const Date &time)
{
    // Convert the date to a time_t to overload Globals::vatRate to keep the code DRY
    return vatRate((time_t)time);
}

double Globals::vatRate(const time_t time)
{
    // Make sure that VatRate database has been initialised. If not then return default VAT rate value
    try { Databases::vatRates(); } catch (const std::exception &e) { return defaultVatRate; }

    // Get all of the VAT rates and sort them by date
    Database<VatRate>::recordListPtr vatRates = VatRateController::getAllVatRates();
    VatRateController::sortVatRatesByStartDate(*vatRates);

    // Loop through the VAT rates until one is found that starts on or before the date
    VatRate foundVatRate;
    for (unsigned i = 0; i < vatRates->size(); ++i)
    {
        VatRate &vatRate = vatRates->at(i);

        if (vatRate.getStartDate() <= time) foundVatRate = vatRate;
        else break; // VAT rate date is too high, so we can assume that the rest are too high as well
                    // (as they are sorted by date ascending), so terminate the loop
    }

    return foundVatRate.getValue();
}
