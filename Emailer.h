/*
 * Emailer.h
 *
 *  Created on: 7 March 2012
 *      Author: Max Foster
 */

#ifndef EMAILER_H
#define EMAILER_H

#include <QObject>
#include <Qxt/QxtMailMessage>
class QxtSmtp;

struct EmailDetails
{
    static const int maxEmailTries = 3;

    std::string recipient, subject, body, attachmentFileName;
    int tries;

    EmailDetails(const char *recipient, const char *subject, const char *body, const char *attachmentFileName = "")
        : recipient(recipient), subject(subject), body(body), attachmentFileName(attachmentFileName), tries(0) {}
};

class Emailer : public QObject
{
    Q_OBJECT

public:
    static const int waitTimeInMilliseconds;

    Emailer(const EmailDetails &emailDetails, QObject *parent = NULL);
    ~Emailer();

    bool pending() const;
    bool sentSuccessfully() const;

    const EmailDetails & emailDetails() const;

    void kill();

public slots:
    void send();

    void connected();
    void connectionFailed(const QByteArray &message);

    void authenticated();
    void authenticationFailed(const QByteArray &message);

    void mailSent(int mailId);
    void mailFailed(int mailId, int errorCode, const QByteArray &message);
    void senderRejected(int mailId, const QString &address, const QByteArray &message);

    void finished();
    void disconnected();

signals:
    void mailSent();
    void mailFailed();

private:
    QxtSmtp *socket;
    EmailDetails emailDetails_;
    QxtMailMessage message;
    bool completed, killNow;

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
