/*
 * PendingDialog.h
 *
 *  Created on: 10 March 2012
 *      Author: Max Foster
 */

#ifndef PENDINGDIALOG_H
#define PENDINGDIALOG_H

#include <QDialog>

namespace Ui
{
    class PendingDialog;
}

class QTimer;

class PendingDialog : public QDialog
{
    Q_OBJECT

public:
    static const int percentCheckTimePeriod;

    explicit PendingDialog(const QString & message, int (*percentCompleteCheckFunction)(void), QWidget *parent = NULL);
    ~PendingDialog();

public slots:
    void updateProgressBar();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::PendingDialog *ui;
    int (*percentCompleteCheckFunction)(void);
    QTimer *timer;
};

#endif // PENDINGDIALOG_H
