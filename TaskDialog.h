/*
 * TaskDialog.h
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>

namespace Ui
{
    class TaskDialog;
}

class TaskDialog : public QDialog
{
public:
    explicit TaskDialog(QWidget * parent = NULL);
    ~TaskDialog();

private:
    Q_OBJECT
    Ui::TaskDialog * ui;
};

#endif // TASKDIALOG_H
