/*
 * SettingController.cpp
 *
 *  Created on: 23 March 2012
 *      Author: Max Foster
 */

#include "SettingController.h"
#include "Databases.h"
#include "Setting.h"

bool SettingController::Create(Setting &settingAttributes, QWidget *)
{
    try { Databases::settings().addRecord(settingAttributes); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (settingAttributes.null())
    {
        showErrorDialog("There was an error with adding the setting to the database");
        return false;
    }

    return true;
}

bool SettingController::Update(const Setting &setting, QWidget *)
{
    bool success = false;
    try { success = Databases::settings().updateRecord(setting); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with updating the setting in the database");
    return success;
}

bool SettingController::Destroy(int settingId, QWidget *)
{
    bool success = false;
    try { success = Databases::settings().deleteRecord(settingId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return false;
    }

    if (!success) showErrorDialog("There was an error with removing the setting from the database");
    return success;
}

bool SettingController::Destroy(Setting &setting, QWidget *caller)
{
    if (Destroy(setting.getId(), caller))
    {
        setting = Setting();
        return true;
    }

    return false;
}

Setting SettingController::getSetting(int settingId)
{
    Setting setting;
    try { setting = Databases::settings().findRecord("id", settingId); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Setting();
    }
    return setting;
}

Setting SettingController::getSetting(const char *key)
{
    Setting setting;
    try { setting = Databases::settings().findRecord("key", key); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Setting();
    }
    return setting;
}

Database<Setting>::recordListPtr SettingController::getAllSettings()
{
    Database<Setting>::recordListPtr settings;
    try { settings = Databases::settings().allRecords(); }
    catch (const std::exception &e)
    {
        showErrorDialog(e.what());
        return Database<Setting>::recordListPtr(new Database<Setting>::recordList);
    }
    return settings;
}
