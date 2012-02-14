/*
 * JobNew.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef JOBNEW_H
#define JOBNEW_H

#include <QDialog>

namespace Ui
{
    class JobNew;
}

class Job;

class JobNew : public QDialog
{
    Q_OBJECT

public:
    explicit JobNew(Job & job, QWidget * parent = NULL);
    ~JobNew();

    void updateView();

private slots:
    void on_pushButton_cancel_released();

    void on_pushButton_submit_released();

private:
    Ui::JobNew * ui;
    Job & job;
};

#endif // JOBNEW_H
