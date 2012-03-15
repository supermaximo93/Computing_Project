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
class ProcessDialog;

class EmailerThread : public QThread
{
    Q_OBJECT

public:
    static const int mutexLockTimeout, queueCheckTimePeriod;

    static void init(QObject *parent = NULL);
    static bool finalise();

    static int checkEmailQueuePercentDone();
    static bool checkEmailQueueEmpty();

    static void enqueueEmail(const EmailDetails &email);

    static bool connectionAvailable();

public slots:
    void checkEmailQueue();
    void mailSent();
    void mailFailed();

signals:
    void sendEmail();
    void emailQueueNotEmpty();
    void emailQueueEmpty();

private:
    static EmailerThread *emailerThread;
    static QMutex *emailQueueMutex, *emailsInQueueMutex; // if itemsInQueueMutex is locked there are still emails in q
    static std::queue<EmailDetails> *emailQueue;
    static ProcessDialog *emailProcessDialog;
    static bool connectionAvailable_;

    Emailer *emailer;
    QTimer *timer;
    bool emailsInQueue;

    EmailerThread(QObject *parent);
    ~EmailerThread();

    void run();
};

#endif // EMAILERTHREAD_H
