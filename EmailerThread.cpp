/*
 * EmailerThread.cpp
 *
 *  Created on: 10 March 2012
 *      Author: Max Foster
 */
#include <iostream>
using namespace std;

#include <QMutex>
#include <QTimer>

#include "EmailerThread.h"
#include "Emailer.h"
#include "Utils.h"

const int EmailerThread::mutexLockTimeout = 1000, EmailerThread::queueCheckTimePeriod = 500;
EmailerThread *EmailerThread::emailerThread = NULL;
queue<EmailDetails> *EmailerThread::emailQueue = NULL;
QMutex *EmailerThread::emailQueueMutex = NULL, *EmailerThread::emailsInQueueMutex = NULL;

void EmailerThread::init()
{
    if (emailerThread == NULL) emailerThread = new EmailerThread;
    if (emailQueue == NULL) emailQueue = new queue<EmailDetails>;
    if (emailQueueMutex == NULL) emailQueueMutex = new QMutex;
    if (emailsInQueueMutex == NULL) emailsInQueueMutex = new QMutex;
    emailerThread->start(LowestPriority);
}

bool EmailerThread::finalise()
{
    if (emailerThread == NULL) return true;

    if (!emailsInQueueMutex->tryLock())
    {
        bool cancelEmails = showYesNoDialog("There are still emails waiting to be sent.\n"
                                            "Would you line to cancel the sending of these emails?");
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
                // show pending dialog
            }
            else return false;
        }
    }

    emailsInQueueMutex->unlock();
    emailerThread->exit();
    while (emailerThread->isRunning()) emailerThread->wait();

    delete emailerThread;
    emailerThread = NULL;

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

void EmailerThread::enqueueEmail(const EmailDetails &email)
{
    while (!emailQueueMutex->tryLock(mutexLockTimeout));
    if (emailQueue != NULL) emailQueue->push(email);
    emailQueueMutex->unlock();
}

const EmailerThread * EmailerThread::instance()
{
    return emailerThread;
}

void EmailerThread::checkEmailQueue()
{
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
    }
    emit emailQueueNotEmpty();

    emailer = new Emailer(emailQueue->front());
    emailQueue->pop();
    emailQueueMutex->unlock();

    connect(emailer, SIGNAL(mailSent()), this, SLOT(mailSent()));
    connect(emailer, SIGNAL(mailFailed()), this, SLOT(mailFailed()));
    emailer->send();
}

void EmailerThread::mailSent()
{
    if (emailer == NULL) return;

    delete emailer;
    emailer = NULL;
}

void EmailerThread::mailFailed()
{
    if (emailer == NULL) return;

    while (!emailQueueMutex->tryLock(mutexLockTimeout));
    emailQueue->push(emailer->emailDetails());
    delete emailer;
    emailer = NULL;
    emailQueueMutex->unlock();
}

void EmailerThread::waitForNextCheck()
{
    checkEmailQueue();
    QTimer::singleShot(queueCheckTimePeriod, this, SLOT(waitForNextCheck()));
}

EmailerThread::EmailerThread() : emailer(NULL), emailsInQueue(false) {}

EmailerThread::~EmailerThread()
{
    if (emailer != NULL) delete emailer;
}

void EmailerThread::run()
{
    QTimer::singleShot(queueCheckTimePeriod, this, SLOT(waitForNextCheck()));
    exec();
}
