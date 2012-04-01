/*
 * EmailerThread.h
 *
 *  Created on: 10 March 2012
 *      Author: Max Foster
 */

#ifndef EMAILERTHREAD_H
#define EMAILERTHREAD_H

#include <queue>

#include <QThread>

class EmailDetails;
class Emailer;
class QMutex;
class QTimer;
class QMessageBox;
class ProcessDialog;

class EmailerThread : public QThread
{
    Q_OBJECT

public:
    static const int
    mutexLockTimeout,      // the time the EmailerThread takes to wait for a mutex to become unlocked
    queueCheckTimePeriod;  // the period of time the EmailerThread waits before checking the email queue

    // Initialises the email thread and email queue
    static void init(QObject *parent = NULL);

    // Deletes the email thread and email queue. If the queue still has items in it, then a dialog box will pop up
    // asking if the user wants to wait for the emails to send or cancel them
    static bool finalise();

    // Returns the percentage of emails that have been sent (returns 100% when the queue is empty)
    static int checkEmailQueuePercentDone();

    // Returns whether the email queue is empty
    static bool checkEmailQueueEmpty();

    // Enqueues an email onto the email queue to be sent automatically by the email thread
    static void enqueueEmail(const EmailDetails &email);

    // Returns whether there is an internet connection available
    static bool connectionAvailable();

public slots:
    // A slot that makes the EmailerThread check the email queue for any emails
    void checkEmailQueue();

    // Slots to be called when either the email has been sent or the email sending failed
    void mailSent();
    void mailFailed();

signals:
    // A signal that, when emitted, requests an email to be sent
    void sendEmail();

    // Signals to be emitted when an email queue has an item enqueued after being empty or has been emptied
    void emailQueueNotEmpty();
    void emailQueueEmpty();

private:
    static EmailerThread *emailerThread;
    static QMutex *emailQueueMutex, // Locked when the email queue is being accessed
    *emailsInQueueMutex;            // If itemsInQueueMutex is locked then there are still emails being sent
    static std::queue<EmailDetails> *emailQueue;
    static ProcessDialog *emailProcessDialog;
    static QMessageBox *messageBox; // A message box used for alerting the user to any problems
    static bool connectionAvailable_;

    Emailer *emailer; // The object that will actually send the emails
    QTimer *timer;    // A timer to make the EmailerThread check the queue at set time intervals
    bool emailsInQueue;

    EmailerThread(QObject *parent);
    ~EmailerThread();

    void run(); // Starts the thread
};

#endif // EMAILERTHREAD_H
