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

#include "EmailDetails.h"

class EmailDetails;
class QxtSmtp;

class Emailer : public QObject
{
    Q_OBJECT

public:
    Emailer(const EmailDetails &emailDetails, QObject *parent = NULL);
    ~Emailer();

    // Returns whether an email is waiting to be sent or is in the process of being sent
    bool pending() const;

    // Returns whether the email was sent successfully (if the email has been sent yet)
    bool sentSuccessfully() const;

    // Returns the details of the email that is being sent
    const EmailDetails & emailDetails() const;

    // Tells the Emailer that it is okay for it to delete itself after it has finished sending the email
    void kill();

public slots:
    // Sends the email using the email details given in the constructor
    void send();

    // Slots indicating to the Emailer whether the connection was successful
    void connected();
    void connectionFailed(const QByteArray &message);

    // Slots indicating to the Emailer whether authentication was successful
    void authenticated();
    void authenticationFailed(const QByteArray &message);

    // Slots indicating to the Emailer whether the email was sent successfully
    void mailSent(int mailId);
    void mailFailed(int mailId, int errorCode, const QByteArray &message);
    void senderRejected(int mailId, const QString &address, const QByteArray &message);

    // A slot that is called when the email has finished sending
    void finished();

    // A slot that is called when the Emailer socket has disconnected from the host
    void disconnected();

signals:
    // Signals that are emitted when an email has either been sent or has failed to be sent
    void mailSent();
    void mailFailed();

private:
    QxtSmtp *socket;
    EmailDetails emailDetails_;
    QxtMailMessage message;
    bool completed, killNow;

    enum // an enumerated type indicating the state of connection, authentication, etc.
    {
        NONE,
        PENDING,
        SUCCESS,
        FAILURE
    } connectionState, authenticationState, mailSendState;

    // Disconnects from the host if the Emailer socket is connected to one
    void closeConnection();
};

#endif // EMAILER_H
