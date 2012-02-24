/*
 * Globals.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "Globals.h"
#include "Utils.h"

double Globals::vatRate(const Date &time) // TODO: make it get VAT rate from different periods of time
{
    return 20.0;
}

double Globals::vatRate(const time_t time)
{
    return vatRate(Date(time));
}
