/*
 * Emailer.h
 *
 *  Created on: 7 March 2012
 *      Author: Max Foster
 */

#ifndef EMAILER_H
#define EMAILER_H

#include <QObject>

class QxtSmtp;

class Emailer : public QObject
{
    Q_OBJECT

public:
    static const int waitTimeInMilliseconds;

    Emailer(const char *recipient, const char *subject, const char *body, const char *attachmentFileName = "",
            QObject *parent = NULL);
    ~Emailer();

    bool mailSent();

public slots:
    void conncted();
    void connectionFailed(const QByteArray &message);

    void authenticated();
    void authenticationFailed(const QByteArray &message);

    void mailSent(int mailId);
    void mailFailed(int mailId, int errorCode, const QByteArray &message);
    void senderRejected(int mailId, const QString &address, const QByteArray &message);

    void finished();

private:
    QxtSmtp *socket;
    std::string recipient, subject, body, attachmentFileName;

    enum
    {
        NONE,
        PENDING,
        SUCCESS,
        FAILURE
    } connectionState, authenticationState, mailSendState;

    void closeConnection();
};

#endif // EMAILER_H
