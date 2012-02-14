/*
 * JobEdit.h
 *
 *  Created on: 14 Feb 2012
 *      Author: Max Foster
 */

#ifndef JOBEDIT_H
#define JOBEDIT_H

#include <QDialog>

#include "Job.h"

namespace Ui
{
    class JobEdit;
}

class JobEdit : public QDialog
{
    Q_OBJECT

public:
    explicit JobEdit(Job & job, QWidget * parent = NULL);
    ~JobEdit();

    void updateView();

private slots:
    void on_pushButton_cancel_released();

    void on_pushButton_submit_released();

private:
    Ui::JobEdit * ui;
    Job & job, jobCopy;
};

#endif // JOBEDIT_H
