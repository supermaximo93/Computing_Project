/*
 * EmailerThread.cpp
 *
 *  Created on: 10 March 2012
 *      Author: Max Foster
 */

using namespace std;

#include <QMutex>
#include <QTimer>
#include <QTcpSocket>

#include "EmailerThread.h"
#include "EmailDetails.h"
#include "Emailer.h"
#include "Utils.h"

#include "dialogs/utils/ProcessDialog.h"

const int EmailerThread::mutexLockTimeout = 1000, EmailerThread::queueCheckTimePeriod = 5000;
EmailerThread *EmailerThread::emailerThread = NULL;
queue<EmailDetails> *EmailerThread::emailQueue = NULL;
QMutex *EmailerThread::emailQueueMutex = NULL, *EmailerThread::emailsInQueueMutex = NULL;
ProcessDialog *EmailerThread::emailProcessDialog = NULL;
bool EmailerThread::connectionAvailable_ = false;

void EmailerThread::init(QObject *parent)
{
    if (emailerThread == NULL) emailerThread = new EmailerThread(parent);
    if (emailQueue == NULL) emailQueue = new queue<EmailDetails>;
    if (emailQueueMutex == NULL) emailQueueMutex = new QMutex;
    if (emailsInQueueMutex == NULL) emailsInQueueMutex = new QMutex;
    emailerThread->start(LowestPriority);
}

static int emailCountBeforeClose = 0;

bool EmailerThread::finalise()
{
    if (emailerThread == NULL) return true;

    if (!emailsInQueueMutex->tryLock())
    {
        bool cancelEmails = showYesNoDialog("There are still emails waiting to be sent.\n"
                                            "Would you like to cancel the sending of these emails?");
        if (cancelEmails)
        {
            cancelEmails = showYesNoDialog("Are you sure you want to cancel the sending of these emails?\n"
                                           "They will not be able to be recovered after cancellation");
            if (cancelEmails)
            {
                while (!emailQueueMutex->tryLock(mutexLockTimeout));
                while (emailQueue->size() > 0) emailQueue->pop();
                emailQueueMutex->unlock();
                showInfoDialog("Emails canceled");
            }
            else return false;
        }
        else
        {
            bool closeAutomatically =
                    showYesNoDialog("Would you like the program to close after all the emails have been sent?");
            if (closeAutomatically)
            {
                while (!emailQueueMutex->tryLock(mutexLockTimeout));
                emailCountBeforeClose = emailQueue->size();
                emailQueueMutex->unlock();

                showPendingDialog("Finishing sending of emails", checkEmailQueuePercentDone);
            }
            else return false;
        }
    }

    emailsInQueueMutex->unlock();
    emailerThread->exit();
    while (emailerThread->isRunning()) emailerThread->wait();

    delete emailerThread;
    emailerThread = NULL;

    if (emailProcessDialog != NULL) emailProcessDialog->accept();

    if (emailQueue != NULL)
    {
        delete emailQueue;
        emailQueue = NULL;
    }

    if (emailQueueMutex != NULL)
    {
        delete emailQueueMutex;
        emailQueueMutex = NULL;
    }

    if (emailsInQueueMutex != NULL)
    {
        delete emailsInQueueMutex;
        emailsInQueueMutex = NULL;
    }

    return true;
}

int EmailerThread::checkEmailQueuePercentDone()
{
    while (!emailQueueMutex->tryLock(mutexLockTimeout));
    const int percentage = ((emailCountBeforeClose - emailQueue->size()) * 100) / emailCountBeforeClose;
    emailQueueMutex->unlock();
    return percentage;
}

bool EmailerThread::checkEmailQueueEmpty()
{
    while (!emailQueueMutex->tryLock(mutexLockTimeout));
    const bool emailQueueEmpty = emailQueue->empty();
    emailQueueMutex->unlock();
    return emailQueueEmpty && (emailerThread->emailer == NULL);
}

void EmailerThread::enqueueEmail(const EmailDetails &email)
{
    while (!emailQueueMutex->tryLock(mutexLockTimeout));
    if (emailQueue != NULL) emailQueue->push(email);
    emailQueueMutex->unlock();
}

bool EmailerThread::connectionAvailable()
{
    return connectionAvailable_;
}

void EmailerThread::checkEmailQueue()
{
    QTcpSocket *connectionChecker = new QTcpSocket;
    connectionChecker->connectToHost("www.google.co.uk", 80);
    connectionAvailable_ = connectionChecker->waitForConnected(1000);
    connectionChecker->disconnectFromHost();
    delete connectionChecker;
    if (!connectionAvailable_)
    {
        return;
    }

    if (emailer != NULL) return;
    if (!emailQueueMutex->tryLock()) return;

    if ((emailQueue == NULL) || emailQueue->empty())
    {
        emailQueueMutex->unlock();
        if (emailsInQueue)
        {
            emailsInQueueMutex->unlock();
            emailsInQueue = false;
        }

        emit emailQueueEmpty();
        return;
    }

    if (!emailsInQueue)
    {
        while (!emailsInQueueMutex->tryLock(mutexLockTimeout));
        emailsInQueue = true;

        if (emailProcessDialog == NULL)
        {
            emailProcessDialog = new ProcessDialog("Sending emails", checkEmailQueueEmpty, &emailProcessDialog);
            emailProcessDialog->show();
        }
    }
    emit emailQueueNotEmpty();

    emailer = new Emailer(emailQueue->front(), this);
    emailQueue->pop();
    emailQueueMutex->unlock();

    connect(this, SIGNAL(sendEmail()), emailer, SLOT(send()), Qt::QueuedConnection);
    connect(emailer, SIGNAL(mailSent()), this, SLOT(mailSent()), Qt::QueuedConnection);
    connect(emailer, SIGNAL(mailFailed()), this, SLOT(mailFailed()), Qt::QueuedConnection);
    emit sendEmail();
}

void EmailerThread::mailSent()
{
    if (emailer == NULL) return;

    emailer->kill();
    emailer = NULL;
}

void EmailerThread::mailFailed()
{
    if (emailer == NULL) return;

    while (!emailQueueMutex->tryLock(mutexLockTimeout));

    if (emailer->emailDetails().tries < EmailDetails::maxEmailTries) emailQueue->push(emailer->emailDetails());
    else cout << "Email to " << emailer->emailDetails().recipient << " could not be sent "
                 "(attempted to send " << emailer->emailDetails().tries << " times)" << endl;

    emailer->kill();
    emailer = NULL;
    emailQueueMutex->unlock();
}

EmailerThread::EmailerThread(QObject *parent) : QThread(parent), emailer(NULL), timer(NULL), emailsInQueue(false) {}

EmailerThread::~EmailerThread()
{
    if (emailer != NULL) delete emailer;
    if (timer != NULL) delete timer;
}

void EmailerThread::run()
{
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(checkEmailQueue()), Qt::QueuedConnection);
    timer->start(queueCheckTimePeriod);
    exec();
    timer->stop();
}
