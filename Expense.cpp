/*
 * Expense.cpp
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#include <fstream>
#include <string.h>
using namespace std;

#include "Expense.h"

const string Expense::databaseFilename = "expenses.dat";

Expense::Expense(const time_t newDate, const char * newDescription, const float newPrice, const float newVat,
		const int newType) {
	description = new char[descriptionLength+1];

	date = newDate;
	strcpy(description, newDescription);
	price = newPrice;
	vat = newVat;
	type = newType;
}

Expense::Expense(const Expense & expense) {
	description = new char[descriptionLength+1];
	(*this) = expense;
}

Expense::~Expense() {
	delete[] description;
}

void Expense::operator =(const Expense & expense) {
	*((Record*)this) = (Record)expense;
	date = expense.date;
	strcpy(description, expense.description);
	price = expense.price;
	vat = expense.vat;
	type = expense.type;
}

void Expense::writeToFile(fstream & file) const {
	Record::writeToFile(file);
	file.write(reinterpret_cast<const char *>(&date), sizeof(date));
	file.write(description, descriptionLength);
	file.write(reinterpret_cast<const char *>(&price), sizeof(price));
	file.write(reinterpret_cast<const char *>(&vat), sizeof(vat));
	file.write(reinterpret_cast<const char *>(&type), sizeof(type));
}

void Expense::readFromFile(fstream & file) {
	Record::readFromFile(file);
	file.read(reinterpret_cast<char *>(&date), sizeof(date));
	file.read(description, descriptionLength);
	file.read(reinterpret_cast<char *>(&price), sizeof(price));
	file.read(reinterpret_cast<char *>(&vat), sizeof(vat));
	file.read(reinterpret_cast<char *>(&type), sizeof(type));
}

int Expense::size() {
	return Record::size()+sizeof(time_t)+descriptionLength+(sizeof(float)*2)+sizeof(int);
}

bool Expense::hasMatchingField(const string & fieldName, const time_t searchTerm) const {
	if (fieldName == "date") return (date == searchTerm);
	return false;
}

bool Expense::hasMatchingField(const string & fieldName, const char * searchTerm) const {
	if (fieldName == "description") return (strcmp(description, searchTerm) == 0);
	return false;
}

bool Expense::hasMatchingField(const string & fieldName, const float searchTerm) const {
	if (fieldName == "price") return (price == searchTerm);
	else if (fieldName == "vat") return (vat == searchTerm);
	return false;
}

bool Expense::hasMatchingField(const string & fieldName, const int searchTerm) const {
	if (fieldName == "type") return (type == searchTerm);
	return Record::hasMatchingField(fieldName, searchTerm);
}

time_t Expense::getDate() const {
	return date;
}

void Expense::setDate(const time_t newDate) {
	date = newDate;
}

const char * Expense::getDescription() const {
	return description;
}

void Expense::setDescription(const char * newDescription) {
	strcpy(description, newDescription);
}

float Expense::getPrice() const {
	return price;
}

void Expense::setPrice(const float newPrice) {
	price = newPrice;
}

float Expense::getVat() const {
	return vat;
}

void Expense::setVat(const float newVat) {
	vat = newVat;
}

int Expense::getType() const {
	return type;
}

void Expense::setType(const int newType) {
	type = newType;
}
