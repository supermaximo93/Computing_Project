/*
 * CustomerController.cpp
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#include "CustomerController.h"
#include "Databases.h"
#include "Customer.h"

Database<Customer>::recordListPtr CustomerController::getAllCustomers()
{
    Database<Customer>::recordListPtr customers;
    try { customers = Databases::customers().allRecords(); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return Database<Customer>::recordListPtr(new Database<Customer>::recordList);
    }
    return customers;
}
