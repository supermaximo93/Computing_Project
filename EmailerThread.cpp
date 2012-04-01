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
#include <QMessageBox>

#include "EmailerThread.h"
#include "EmailDetails.h"
#include "Emailer.h"
#include "Utils.h"

#include "dialogs/utils/ProcessDialog.h"

const int EmailerThread::mutexLockTimeout = 1000, EmailerThread::queueCheckTimePeriod = 5000;
EmailerThread *EmailerThread::emailerThread = NULL;
queue<EmailDetails> *EmailerThread::emailQueue = NULL;
QMutex *EmailerThread::emailQueueMutex = NULL, *EmailerThread::emailsInQueueMutex = NULL;
QMessageBox *EmailerThread::messageBox = NULL;
ProcessDialog *EmailerThread::emailProcessDialog = NULL;
bool EmailerThread::connectionAvailable_ = false;

void EmailerThread::init(QObject *parent)
{
    if (emailerThread == NULL) emailerThread = new EmailerThread(parent);
    if (emailQueue == NULL) emailQueue = new queue<EmailDetails>;
    if (emailQueueMutex == NULL) emailQueueMutex = new QMutex;
    if (emailsInQueueMutex == NULL) emailsInQueueMutex = new QMutex;
    if (messageBox == NULL) messageBox = new QMessageBox;

    emailerThread->start(LowestPriority);
}

static int emailCountBeforeClose = 0; // Used to calculate a percentage in EmailerThread::checkEmailQueuePercentDone()

bool EmailerThread::finalise()
{
    if (emailerThread == NULL) return true; // Emailer thread already deleted, so finish

    // Check if there are any emails in the queue by attempting to lock the emailsInQueueMutex. If it can be locked,
    // unlock it immediately so there is no risk of mutex deadlocks
    bool canLockEmailsInQueueMutex = false;
    if (emailsInQueueMutex->tryLock())
    {
        emailsInQueueMutex->unlock();
        canLockEmailsInQueueMutex = true;
    }

    if (!canLockEmailsInQueueMutex || !checkEmailQueueEmpty()) // If the queue isn't empty
    {
        // Ask the user if they want to cancel the emails (and double check)
        bool cancelEmails = showYesNoDialog("There are still emails waiting to be sent.\n"
                                            "Would you like to cancel the sending of these emails?");
        if (cancelEmails)
        {
            cancelEmails = showYesNoDialog("Are you sure you want to cancel the sending of these emails?\n"
                                           "They will not be able to be recovered after cancellation");
            if (cancelEmails)
            { // The user is completely sure they want to cancel, so empty the queue
                while (!emailQueueMutex->tryLock(mutexLockTimeout));
                while (emailQueue->size() > 0) emailQueue->pop();
                emailQueueMutex->unlock();
                showInfoDialog("Emails canceled");
            }
            else return false; // The user doesn't want to cancel the emails, so end the function
        }
        else
        {
            // Ask the user if they want the program to close itself after all of the emails have been sent, and if
            // they do, then show a pending dialog until the emails have been sent, then continue with the rest of the
            // function as normal
            bool closeAutomatically =
                    showYesNoDialog("Would you like the program to close after all the emails have been sent?");
            if (closeAutomatically)
            {
                while (!emailQueueMutex->tryLock(mutexLockTimeout));
                emailCountBeforeClose = emailQueue->size();
                emailQueueMutex->unlock();

                showPendingDialog("Waiting for emails to be sent", checkEmailQueuePercentDone);
            }
            else return false; // The user doesn't want the program to close, so end the function
        }
    }

    // Get the emailerThread to stop and delete it, along with the queue, mutexes, etc.
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

    if (messageBox != NULL)
    {
        delete messageBox;
        messageBox = NULL;
    }

    return true;
}

int EmailerThread::checkEmailQueuePercentDone()
{
    // Lock the email queue mutex before calculating the percentage of completion
    while (!emailQueueMutex->tryLock(mutexLockTimeout));

    int percentage;
    // There might be an email still being sent even though the queue is empty, so check by attempting to lock
    // emailsInQueueMutex
    if (emailCountBeforeClose == 0)
    {
        bool canLock = emailsInQueueMutex->tryLock();
        percentage = canLock ? 100 : 0;
        if (canLock) emailsInQueueMutex->unlock();
    }
    else percentage = ((emailCountBeforeClose - emailQueue->size()) * 100) / emailCountBeforeClose;

    emailQueueMutex->unlock();
    return percentage;
}

bool EmailerThread::checkEmailQueueEmpty()
{
    // If emailsInQueueMutex can be locked, then the queue is empty
    bool canLock = emailsInQueueMutex->tryLock();
    if (canLock) emailsInQueueMutex->unlock();
    return canLock;
}

void EmailerThread::enqueueEmail(const EmailDetails &email)
{
    // Lock the emailQueueMutex before safely putting the email details onto the end of the queue
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
    // Check for an internet connection by creating a socket and connecting to a reliable website (i.e. Google).
    // If socket could not connect then end the function (no point sending emails if there is no internet connection)
    QTcpSocket *connectionChecker = new QTcpSocket;
    connectionChecker->connectToHost("www.google.co.uk", 80);
    connectionAvailable_ = connectionChecker->waitForConnected(1000);
    connectionChecker->disconnectFromHost();
    delete connectionChecker;
    if (!connectionAvailable_) return;

    // If there is already an Emailer sending an email or the email queue is being used then end the function because
    // the rest of the function uses the email queue as well as creating a new Emailer object
    if (emailer != NULL) return;
    if (!emailQueueMutex->tryLock()) return;

    // The email queue is empty and there are no emails being sent, so unlock the emailsInQueue mutex if there were
    // previously emails in the queue to indicate that email sending has finished
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

    // There are emails in the queue, so if there were no emails in the queue before, lock the emailsInQueueMutex and
    // create a dialog indicating to the user that there are emails being sent
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

    // Create a new Emailer to send the email at the front of the queue, and tell it to send the email
    emailer = new Emailer(emailQueue->front(), this);
    emailQueue->pop();
    emailQueueMutex->unlock(); // We're done with the email queue for now, so unlock it

    connect(this, SIGNAL(sendEmail()), emailer, SLOT(send()), Qt::QueuedConnection);
    connect(emailer, SIGNAL(mailSent()), this, SLOT(mailSent()), Qt::QueuedConnection);
    connect(emailer, SIGNAL(mailFailed()), this, SLOT(mailFailed()), Qt::QueuedConnection);
    emit sendEmail();
}

void EmailerThread::mailSent()
{
    if (emailer == NULL) return; // The emailer has already been destroyed, that's fine, end

    // Tell the emailer that it can delete itself when it has done whatever cleanup it needs to do. We can't just
    // delete it straight away here because the Emailer's socket may still be connected and if we delete it, it may
    // cause a crash!
    emailer->kill();
    emailer = NULL;
}

void EmailerThread::mailFailed()
{
    if (emailer == NULL) return;

    while (!emailQueueMutex->tryLock(mutexLockTimeout));

    // If the email has not exceeded the maximum number of tries, then put it back onto the queue, else display an error
    if (emailer->emailDetails().tries < EmailDetails::maxEmailTries) emailQueue->push(emailer->emailDetails());
    else
    {
        QString message
                = QString("Email to ") + emailer->emailDetails().recipient.c_str() + " could not be sent\n"
                + "(attempted to send " + toString(emailer->emailDetails().tries).c_str() + " times)";

        messageBox->setIcon(QMessageBox::Warning);
        messageBox->setWindowTitle("Error");
        messageBox->setText(message);
        messageBox->show();

        cout << message.toStdString() << endl; // Also put the message into the logfile
    }

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
    // Start the timer to make the EmailerThread check the email queue every so often
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(checkEmailQueue()), Qt::QueuedConnection);
    timer->start(queueCheckTimePeriod);
    exec();
    timer->stop();
}
