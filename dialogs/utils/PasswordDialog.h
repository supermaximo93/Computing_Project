/*
 * PasswordDialog.h
 *
 *  Created on: 24 March 2012
 *      Author: Max Foster
 */

#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui
{
    class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = NULL);
    ~PasswordDialog();

signals:
    void noPasswordSet();

private slots:
    void on_pushButton_submit_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::PasswordDialog *ui;
    int tries;
    std::string password;
};

#endif // PASSWORDDIALOG_H
