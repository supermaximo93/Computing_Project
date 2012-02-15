/*
 * PartController.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "PartController.h"
#include "Databases.h"
#include "Part.h"

void PartController::Index(QWidget * caller)
{

}

void PartController::Show(int partId, QWidget * caller)
{

}

void PartController::Show(Part & part, QWidget * caller)
{

}

Part PartController::New(QWidget * caller)
{
    return Part();
}

void PartController::Edit(int partId, QWidget * caller)
{

}

void PartController::Edit(Part & part, QWidget * caller)
{

}

bool PartController::Create(Part & partAttributes, QWidget * caller)
{

}

bool PartController::Update(const Part & part, QWidget * caller)
{

}

bool PartController::Destroy(int partId, QWidget * caller)
{

}

bool PartController::Destroy(Part & part, QWidget * caller)
{

}

Part PartController::getPart(int partId)
{
    Part part;
    try { part = Databases::parts().findRecord("id", partId); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return Part();
    }
    return part;
}

Database<Part>::recordListPtr PartController::getAllParts()
{
    Database<Part>::recordListPtr parts;
    try { parts = Databases::parts().allRecords(); }
    catch (const std::exception & e)
    {
        showErrorDialog(e.what());
        return Database<Part>::recordListPtr(new Database<Part>::recordList);
    }
    return parts;
}

