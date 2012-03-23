/*
 * SettingController.h
 *
 *  Created on: 23 March 2012
 *      Author: Max Foster
 */

#ifndef SETTINGCONTROLLER_H
#define SETTINGCONTROLLER_H

#include "Database.h"

class QWidget;
class Setting;

namespace SettingController
{

bool Create(Setting &settingAttributes, QWidget *caller);

bool Update(const Setting &setting, QWidget *caller);

bool Destroy(int settingId, QWidget *caller);
bool Destroy(Setting &setting, QWidget *caller);

Setting getSetting(int settingId);
Setting getSetting(const char *key);
Database<Setting>::recordListPtr getAllSettings();

}

#endif // SETTINGCONTROLLER_H
