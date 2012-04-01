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

// Generates an invoice/receipt, saving to the given file name, using the data from the given job
bool generateInvoice(const char *fileName, const Job &job);
bool generateReceipt(const char *fileName, const Job &job);

// Generates a report for a single month
bool generateReport(const char *fileName, int month, int year);

// Generates a report for a time period within the given start and end dates
bool generateReport(const char *fileName, const Date &startDate, const Date &endDate);

}

#endif // PDFGENERATOR_H
