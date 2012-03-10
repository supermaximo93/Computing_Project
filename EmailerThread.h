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

class EmailerThread : public QThread
{
    Q_OBJECT

public:
    static const int mutexLockTimeout, queueCheckTimePeriod;

    static void init();
    static bool finalise();

    static void enqueueEmail(const EmailDetails &email);

    static const EmailerThread * instance();

    void checkEmailQueue();

public slots:
    void mailSent();
    void mailFailed();
    void waitForNextCheck();

signals:
    void emailQueueNotEmpty();
    void emailQueueEmpty();

private:
    static EmailerThread *emailerThread;
    static QMutex *emailQueueMutex, *emailsInQueueMutex; // if itemsInQueueMutex is locked there are still emails in q
    static std::queue<EmailDetails> *emailQueue;

    Emailer *emailer;
    bool emailsInQueue;

    EmailerThread();
    ~EmailerThread();

    void run();
};

#endif // EMAILERTHREAD_H
