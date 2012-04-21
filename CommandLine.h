/*
 * CommandLine.h
 *
 *  Created on: 20 April 2012
 *      Author: Max Foster
 */

#ifndef COMMANDLINE_H
#define COMMANDLINE_H

class Customer;
class Job;
class Part;
class Task;
class Expense;
class VatRate;
class Setting;
class Value;

namespace CommandLine
{

void run();

void setRecordValues(Customer &, const Value &);
void setRecordValues(Job &, const Value &);
void setRecordValues(Part &, const Value &);
void setRecordValues(Task &, const Value &);
void setRecordValues(Expense &, const Value &);
void setRecordValues(VatRate &, const Value &);
void setRecordValues(Setting &, const Value &);

}

#endif // COMMANDLINE_H
