/*
 * CustomerController.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef CUSTOMERCONTROLLER_H
#define CUSTOMERCONTROLLER_H

#include "Database.h"

class QWidget;
class Customer;

namespace CustomerController
{

void Index(QWidget * caller);

void Show(int customerId, QWidget * caller);
void Show(Customer & customer, QWidget * caller);

Customer New(QWidget * caller);

void Edit(int customerId, QWidget * caller);
void Edit(Customer & customer, QWidget * caller);

bool Create(Customer & customerAttributes, QWidget * caller);

bool Update(const Customer & customer, QWidget * caller);

bool Destroy(int customerId, QWidget * caller);
bool Destroy(Customer & customer, QWidget * caller);

Customer getCustomer(int customerId);
Database<Customer>::recordListPtr getAllCustomers();

}

#endif // CUSTOMERCONTROLLER_H
