/*
 * SettingController.cpp
 *
 *  Created on: 23 March 2012
 *      Author: Max Foster
 */

#include <fstream>
using namespace std;

#include <QFile>

#include "SettingController.h"
#include "Databases.h"
#include "Setting.h"
#include "Encrypter.h"
#include "dialogs/setting/SettingForm.h"

static const char
*databaseDirectoryStoreFilename = "databaseDirSav.dat",
*backupDirectoryStoreFilename = "backupDirSav.dat",
*pdfDirectoryStoreFilename = "pdfDirSav.dat";

bool SettingController::Create(Setting &settingAttributes, QWidget *)
{
    const bool saveDatabaseDirectory = (strcmp(settingAttributes.getKey(), SettingForm::keyDatabaseDirectory) == 0);
    const bool saveBackupDirectory = (strcmp(settingAttributes.getKey(), SettingForm::keyBackupDirectory) == 0);
    const bool savePdfDirectory = (strcmp(settingAttributes.getKey(), SettingForm::keyPdfDirectory) == 0);
    if (saveDatabaseDirectory || saveBackupDirectory || savePdfDirectory)
    {
        const char *filename;
        if (saveDatabaseDirectory) filename = databaseDirectoryStoreFilename;
        else if (saveBackupDirectory) filename = backupDirectoryStoreFilename;
        else filename = pdfDirectoryStoreFilename;

        if (QFile::exists(filename)) remove(filename);

        {
            ofstream file;
            file.open(filename, ios::binary);
            file.close();
        }

        fstream file;
        file.open(filename, ios::in | ios::out | ios::binary);
        if (file.is_open())
        {
            settingAttributes.writeToFile(file);
            file.close();
        }

        Encrypter::encryptFile(filename, false);

        return true;
    }

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
    const bool saveDatabaseDirectory = (strcmp(setting.getKey(), SettingForm::keyDatabaseDirectory) == 0);
    const bool saveBackupDirectory = (strcmp(setting.getKey(), SettingForm::keyBackupDirectory) == 0);
    const bool savePdfDirectory = (strcmp(setting.getKey(), SettingForm::keyPdfDirectory) == 0);
    if (saveDatabaseDirectory || saveBackupDirectory || savePdfDirectory)
    {
        const char *filename;
        if (saveDatabaseDirectory) filename = databaseDirectoryStoreFilename;
        else if (saveBackupDirectory) filename = backupDirectoryStoreFilename;
        else filename = pdfDirectoryStoreFilename;

        Encrypter::decryptFile(filename, false);

        fstream file;
        file.open(filename, ios::in | ios::out | ios::binary);
        if (file.is_open())
        {
            setting.writeToFile(file);
            file.close();
        }

        Encrypter::encryptFile(filename, false);
        return true;
    }

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
    const bool saveDatabaseDirectory = (strcmp(setting.getKey(), SettingForm::keyDatabaseDirectory) == 0);
    const bool saveBackupDirectory = (strcmp(setting.getKey(), SettingForm::keyBackupDirectory) == 0);
    const bool savePdfDirectory = (strcmp(setting.getKey(), SettingForm::keyPdfDirectory) == 0);
    if (saveDatabaseDirectory || saveBackupDirectory || savePdfDirectory)
    {
        const char *filename;
        if (saveDatabaseDirectory) filename = databaseDirectoryStoreFilename;
        else if (saveBackupDirectory) filename = backupDirectoryStoreFilename;
        else filename = pdfDirectoryStoreFilename;

        if (QFile::exists(filename))
        {
            remove(filename);
            setting = Setting();
            return true;
        }
        return false;
    }

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

    const bool saveDatabaseDirectory = (strcmp(key, SettingForm::keyDatabaseDirectory) == 0);
    const bool saveBackupDirectory = (strcmp(key, SettingForm::keyBackupDirectory) == 0);
    const bool savePdfDirectory = (strcmp(key, SettingForm::keyPdfDirectory) == 0);
    if (saveDatabaseDirectory || saveBackupDirectory || savePdfDirectory)
    {
        const char *filename;
        if (saveDatabaseDirectory) filename = databaseDirectoryStoreFilename;
        else if (saveBackupDirectory) filename = backupDirectoryStoreFilename;
        else filename = pdfDirectoryStoreFilename;

        Encrypter::decryptFile(filename, false);

        fstream file;
        file.open(filename, ios::in | ios::binary);
        if (file.is_open())
        {
            setting.readFromFile(file);
            file.close();
        }
        else
        {
            try { setting.setKey(key); } catch (const std::exception &e) {}
            try { setting.setValue(QDir::currentPath().toStdString().c_str()); } catch (const std::exception &e) {}
        }

        Encrypter::encryptFile(filename, false);

        return setting;
    }

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

    if (QFile::exists(databaseDirectoryStoreFilename))
        settings->push_back(getSetting(SettingForm::keyDatabaseDirectory));
    if (QFile::exists(backupDirectoryStoreFilename))
        settings->push_back(getSetting(SettingForm::keyBackupDirectory));
    if (QFile::exists(pdfDirectoryStoreFilename))
        settings->push_back(getSetting(SettingForm::keyPdfDirectory));

    return settings;
}
