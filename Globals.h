/*
 * Globals.h
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <ctime>

class Date;

namespace Globals
{

double vatRate(const Date & time); // Get the VAT rate at a particular time in history
double vatRate(time_t time);

}

#endif /* GLOBALS_H_ */
