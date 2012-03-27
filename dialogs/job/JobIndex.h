/*
 * JobIndex.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef JOBINDEX_H
#define JOBINDEX_H

#include <QDialog>

#include "Database.h"

namespace Ui
{
    class JobIndex;
}

class Job;
class QModelIndex;

class JobIndex : public QDialog
{
    Q_OBJECT

public:
    explicit JobIndex(Database<Job>::recordList &jobs, const QString &title = "", QWidget *parent = NULL);
    ~JobIndex();

    void updateView();

private slots:
    void on_pushButton_ok_released();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_addNewJob_released();

private:
    Ui::JobIndex *ui;
    Database<Job>::recordList &jobs;
};

#endif // JOBINDEX_H
