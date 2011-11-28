/*
 * Part.h
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#ifndef PART_H_
#define PART_H_

#include <fstream>

#include "Record.h"

class Part : public Record {
	static const int nameLength = 24, numberLength = 32;

	int jobId;
	char * name, * number;
	float price, vatRate;

public:
	static const std::string databaseFilename;

	Part(const int newJobId = 0, const char * newName = "", const char * newNumber = "", const float newPrice = 0.0f,
			const float newVatRate = -1.0f);
	Part(const Part & part);
	~Part();

	void operator =(const Part & part);

	void writeToFile(std::fstream & file) const;
	void readFromFile(std::fstream & file);

	static int size();

	bool hasMatchingField(const std::string & fieldName, const int searchTerm) const;
	bool hasMatchingField(const std::string & fieldName, const char * searchTerm) const;
	bool hasMatchingField(const std::string & fieldName, const float searchTerm) const;

	int getJobId() const;
	void setJobId(const int newJobId);

	const char * getName() const;
	void setName(const char * newName);

	const char * getNumber() const;
	void setNumber(const char * newNumber);

	float getPrice() const;
	void setPrice(const float newPrice);

	float getVatRate() const;
	void setVatRate(const float newVatRate);
};

#endif /* PART_H_ */
