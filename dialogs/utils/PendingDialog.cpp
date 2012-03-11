/*
 * PendingDialog.cpp
 *
 *  Created on: 10 March 2012
 *      Author: Max Foster
 */

#include "PendingDialog.h"
#include "ui_PendingDialog.h"

#include <QTimer>
#include <QCloseEvent>

const int PendingDialog::percentCheckTimePeriod = 500;

PendingDialog::PendingDialog(const QString & message, int (*percentCompleteCheckFunction)(void), QWidget *parent)
    : QDialog(parent), ui(new Ui::PendingDialog), percentCompleteCheckFunction(percentCompleteCheckFunction)
{
    ui->setupUi(this);
    ui->label_messageE->setText(message);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
    timer->start(percentCheckTimePeriod);
}

PendingDialog::~PendingDialog()
{
    timer->stop();
    delete timer;
    delete ui;
}

void PendingDialog::updateProgressBar()
{
    const int newPercentage = percentCompleteCheckFunction();
    ui->progressBar->setValue(newPercentage);
    if (newPercentage >= 100) done(Accepted);
}

void PendingDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
}
