/*
 * SettingController.cpp
 *
 *  Created on: 23 March 2012
 *      Author: Max Foster
 */

#include "SettingController.h"
#include "Databases.h"
#include "Setting.h"

bool SettingController::Create(Setting &settingAttributes, QWidget *caller)
{
    return false;
}

bool SettingController::Update(const Setting &setting, QWidget *caller)
{
    return false;
}

bool SettingController::Destroy(int settingId, QWidget *caller)
{
    return false;
}

bool SettingController::Destroy(Setting &setting, QWidget *caller)
{
    return false;
}

Setting SettingController::getSetting(int settingId)
{
    return Setting();
}

Database<Setting>::recordListPtr SettingController::getAllSettings()
{
    return Database<Setting>::recordListPtr(new Database<Setting>::recordList);
}
