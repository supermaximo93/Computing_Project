/*
 * ProcessDialog.cpp
 *
 *  Created on: 14 March 2012
 *      Author: Max Foster
 */

#include "ProcessDialog.h"
#include "ui_ProcessDialog.h"

#include <QMovie>
#include <QCloseEvent>
#include <QTimer>

const int ProcessDialog::processCompleteCheckTimePeriod = 500;

ProcessDialog::ProcessDialog(const char *message, bool (*processCompleteCheckFunction)(void), ProcessDialog **pointer,
                             QWidget *parent)
    : QDialog(parent), ui(new Ui::ProcessDialog), processCompleteCheckFunction(processCompleteCheckFunction),
      pointer(pointer)
{
    ui->setupUi(this);
    setWindowTitle(message);
    ui->label_messageE->setText(message);

    loadingAnimation = new QMovie(":/gifs/loading.gif");
    ui->label_image->setMovie(loadingAnimation);
    loadingAnimation->start();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkIfProcessComplete()));
    timer->start(processCompleteCheckTimePeriod);
    connect(this, SIGNAL(accepted()), this, SLOT(complete()));
}

ProcessDialog::~ProcessDialog()
{
    if (pointer != NULL) *pointer = NULL;
    timer->stop();
    delete timer;
    delete ui;
    delete loadingAnimation;
}

void ProcessDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void ProcessDialog::checkIfProcessComplete()
{
    if ((processCompleteCheckFunction != NULL) && processCompleteCheckFunction()) done(Accepted);
}

void ProcessDialog::complete()
{
    delete this;
}
