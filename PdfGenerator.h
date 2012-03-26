/*
 * PdfGenerator.h
 *
 *  Created on: 29 Feb 2012
 *      Author: Max Foster
 */

#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

class Job;
class Date;

namespace PdfGenerator
{

bool generateInvoice(const char *fileName, const Job &job);
bool generateReceipt(const char *fileName, const Job &job);

bool generateReport(const char *fileName, int month, int year);
bool generateReport(const char *fileName, const Date &startDate, const Date &endDate);

}

#endif // PDFGENERATOR_H
