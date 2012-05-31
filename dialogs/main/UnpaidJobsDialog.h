/*
 * UnpaidJobsDialog.h
 *
 *  Created on: 31 May 2012
 *      Author: Max Foster
 */

#ifndef UNPAIDJOBSDIALOG_H
#define UNPAIDJOBSDIALOG_H

#include <QDialog>

#include "Database.h"

namespace Ui
{
    class UnpaidJobsDialog;
}

class QModelIndex;

class UnpaidJobsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnpaidJobsDialog(Database<Job>::recordList &jobs, QWidget *parent = 0);
    ~UnpaidJobsDialog();

    void updateView();

private slots:
    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_ok_released();

private:
    Ui::UnpaidJobsDialog *ui;
    Database<Job>::recordList &jobs;
};

#endif // UNPAIDJOBSDIALOG_H
