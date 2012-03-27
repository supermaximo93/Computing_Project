/*
 * VatRateController.cpp
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#include "VatRateController.h"
#include "Databases.h"
#include "VatRate.h"

#include "dialogs/vatrate/VatRateIndex.h"
#include "dialogs/vatrate/VatRateShow.h"
#include "dialogs/vatrate/VatRateForm.h"

void VatRateController::Index(QWidget *caller)
{
    Database<VatRate>::recordListPtr vatRates;
    try { vatRates = Databases::vatRates().allRecords(); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    VatRateIndex(*vatRates, caller).exec();
}

void VatRateController::Show(const int vatRateId, QWidget *caller)
{
    VatRate vatRate;
    try { vatRate = Databases::vatRates().findRecord("id", vatRateId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (vatRate.null())
    {
        showErrorDialog(("VAT rate with ID " + toString(vatRateId) + " could not be found").c_str());
        return;
    }

    Show(vatRate, caller);
}

void VatRateController::Show(VatRate &vatRate, QWidget *caller)
{
    VatRateShow(vatRate, caller).exec();
}

VatRate VatRateController::New(QWidget *caller)
{
    VatRate vatRate;
    VatRateForm view(vatRate, caller);
    return (view.exec() == QDialog::Rejected ? VatRate() : vatRate);
}

void VatRateController::Edit(const int vatRateId, QWidget *caller)
{
    VatRate vatRate;
    try { vatRate = Databases::vatRates().findRecord("id", vatRateId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return;
    }

    if (vatRate.null())
    {
        showErrorDialog(("VAT rate with ID " + toString(vatRateId) + " could not be found").c_str());
        return;
    }

    Edit(vatRate, caller);
}

void VatRateController::Edit(VatRate &vatRate, QWidget *caller)
{
    VatRate tempVatRate = vatRate;
    VatRateForm view(tempVatRate, caller);
    if (view.exec() == VatRateForm::Accepted) vatRate = tempVatRate;
}

bool VatRateController::Create(VatRate &vatRateAttributes, QWidget *)
{
    try { Databases::vatRates().addRecord(vatRateAttributes); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (vatRateAttributes.null())
    {
        showErrorDialog("There was an error with adding the VAT rate to the database");
        return false;
    }

    return true;
}

bool VatRateController::Update(const VatRate &vatRate, QWidget *)
{
    bool success = false;
    try { success = Databases::vatRates().updateRecord(vatRate); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with updating the VAT rate in the database");
    return success;
}

bool VatRateController::Destroy(const int vatRateId, QWidget *)
{
    bool success = false;
    try { success = Databases::vatRates().deleteRecord(vatRateId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with removing the VAT rate from the database");
    return success;
}

bool VatRateController::Destroy(VatRate &vatRate, QWidget *caller)
{
    if (Destroy(vatRate.getId(), caller))
    {
        vatRate = VatRate();
        return true;
    }

    return false;
}

VatRate VatRateController::getVatRate(const int vatRateId)
{
    VatRate vatRate;
    try { vatRate = Databases::vatRates().findRecord("id", vatRateId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return VatRate();
    }
    return vatRate;
}

Database<VatRate>::recordListPtr VatRateController::getAllVatRates()
{
    Database<VatRate>::recordListPtr vatRates;
    try { vatRates = Databases::vatRates().allRecords(); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<VatRate>::recordListPtr(new Database<VatRate>::recordList);
    }
    return vatRates;
}

void VatRateController::sortVatRatesByStartDate(Database<VatRate>::recordList &vatRates, const bool ascending)
{
    struct NestedFunctions
    {
        static int startDateCompareAsc(const VatRate &vatRate1, const VatRate &vatRate2)
        {
            const time_t date1 = vatRate1.getStartDate(), date2 = vatRate2.getStartDate();
            if (date1 < date2) return -1;
            if (date1 > date2) return 1;
            return 0;
        }

        static int startDateCompareDec(const VatRate &vatRate1, const VatRate &vatRate2)
        {
            const time_t date1 = vatRate1.getStartDate(), date2 = vatRate2.getStartDate();
            if (date1 < date2) return 1;
            if (date1 > date2) return -1;
            return 0;
        }
    };

    Databases::vatRates().sortRecords(vatRates, 0, vatRates.size() - 1, ascending ?
                                          NestedFunctions::startDateCompareAsc : NestedFunctions::startDateCompareDec);
}
