/*
 * ProcessDialog.h
 *
 *  Created on: 14 March 2012
 *      Author: Max Foster
 */

#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QDialog>

namespace Ui
{
    class ProcessDialog;
}

class QMovie;
class QTimer;

class ProcessDialog : public QDialog
{
    Q_OBJECT

public:
    static const int processCompleteCheckTimePeriod;

    explicit ProcessDialog(const char *message, bool (*processCompleteCheckFunction)(void), ProcessDialog **pointer,
                           QWidget *parent = NULL);
    ~ProcessDialog();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void checkIfProcessComplete();
    void complete();

private:
    Ui::ProcessDialog *ui;
    bool (*processCompleteCheckFunction)(void);
    ProcessDialog **pointer;
    QMovie *loadingAnimation;
    QTimer *timer;
};

#endif // PROCESSDIALOG_H
