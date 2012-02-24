/*
 * PartController.cpp
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#include "PartController.h"
#include "Databases.h"
#include "Part.h"

#include "dialogs/part/PartShow.h"
#include "dialogs/part/PartForm.h"

void PartController::Index(QWidget *) {} // Parts are only accessible through a Job, so no need for an index screen

void PartController::Show(int partId, QWidget *caller)
{
    Part part;
    try { Databases::parts().findRecord("id", partId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (part.null())
    {
        showErrorDialog(("Part with ID " + toString(partId) + " could not be found").c_str());
        return;
    }

    Show(part, caller);
}

void PartController::Show(Part &part, QWidget *caller)
{
    PartShow view(part, caller);
    view.setModal(true);
    view.exec();
}

Part PartController::New(QWidget *caller)
{
    Part part;
    PartForm view(part, caller);
    view.setModal(true);
    return (view.exec() == QDialog::Rejected ? Part() : part);
}

void PartController::Edit(int partId, QWidget *caller)
{
    Part part;
    try { Databases::parts().findRecord("id", partId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (part.null())
    {
        showErrorDialog(("Part with ID " + toString(partId) + " could not be found").c_str());
        return;
    }

    Edit(part, caller);
}

void PartController::Edit(Part &part, QWidget *caller)
{
    PartForm view(part, caller);
    view.setModal(true);
    view.exec();
}

bool PartController::Create(Part &partAttributes, QWidget *)
{
    try { Databases::parts().addRecord(partAttributes); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (partAttributes.null())
    {
        showErrorDialog("There was an error with adding the part to the database");
        return false;
    }

    return true;
}

bool PartController::Update(const Part &part, QWidget *)
{
    bool success = false;
    try { success = Databases::parts().updateRecord(part); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with updating the part in the database");
    return success;
}

bool PartController::Destroy(int partId, QWidget *)
{
    bool success = false;
    try { success = Databases::parts().deleteRecord(partId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with removing the part from the database");
    return success;
}

bool PartController::Destroy(Part &part, QWidget *caller)
{
    if (Destroy(part.getId(), caller))
    {
        part = Part();
        return true;
    }

    return false;
}

Part PartController::getPart(int partId)
{
    Part part;
    try { part = Databases::parts().findRecord("id", partId); }
    catch (const std::exception &e)
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
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<Part>::recordListPtr(new Database<Part>::recordList);
    }
    return parts;
}

