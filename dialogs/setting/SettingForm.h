/*
 * SettingForm.h
 *
 *  Created on: 23 March 2012
 *      Author: Max Foster
 */

#ifndef SETTINGFORM_H
#define SETTINGFORM_H

#include <QDialog>

namespace Ui
{
    class SettingForm;
}

class Setting;

class SettingForm : public QDialog
{
    Q_OBJECT

public:
    static const char
    *keyDatabaseDirectory,
    *keyBackupDirectory,
    *keyInvoiceSubject,
    *keyInvoiceBody,
    *keyReceiptSubject,
    *keyReceiptBody,
    *keyReminderSubject,
    *keyReminderBody,
    *keyEmailUsername,
    *keyEmailPassword,
    *keyEmailHost,
    *keyEmailPort,
    *keyProgramPassword;

    static Setting getDatabaseDirectory();

    explicit SettingForm(QWidget *parent = NULL);
    ~SettingForm();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_lineEdit_databaseDirectory_textEdited(const QString &value);

    void on_pushButton_browseDatabaseDirectory_clicked();

    void on_lineEdit_backupDirectory_textEdited(const QString &value);

    void on_pushButton_browseBackupDirectory_clicked();

    void on_pushButton_saveNewPassword_clicked();

private:
    Ui::SettingForm *ui;

    void populateInputs();
    bool inputsAreValid();

    bool updateSettings();
};

#endif // SETTINGFORM_H
