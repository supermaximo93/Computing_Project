/*
 * Emailer.cpp
 *
 *  Created on: 7 March 2012
 *      Author: Max Foster
 */

#include <iostream>
using namespace std;

#include <QSslSocket>
#include <QFile>

#include <Qxt/QxtSmtp>
#include <Qxt/QxtMailMessage>
#include <Qxt/QxtMailAttachment>

#include "Emailer.h"

const int Emailer::waitTimeInMilliseconds = 5000;

Emailer::Emailer(const char *recipient, const char *subject, const char *body, const char *attachmentFileName,
                 QObject *parent)
    : recipient(recipient), subject(subject), body(body), attachmentFileName(attachmentFileName),
      connectionState(NONE), authenticationState(NONE), mailSendState(NONE)
{
    socket = new QxtSmtp(parent);
    socket->setUsername("ianfosterservices");
    socket->setPassword("temppassword");

    connect(socket, SIGNAL(connected()), this, SLOT(conncted()));
    connect(socket, SIGNAL(connectionFailed(QByteArray)), this, SLOT(connectionFailed(QByteArray)));
    connect(socket, SIGNAL(authenticated()), this, SLOT(authenticated()));
    connect(socket, SIGNAL(authenticationFailed(QByteArray)), this, SLOT(authenticationFailed(QByteArray)));
    connect(socket, SIGNAL(mailSent(int)), this, SLOT(mailSent(int)));
    connect(socket, SIGNAL(mailFailed(int,int,QByteArray)), this, SLOT(mailFailed(int,int,QByteArray)));
    connect(socket, SIGNAL(senderRejected(int,QString,QByteArray)), this, SLOT(senderRejected(int,QString,QByteArray)));
    connect(socket, SIGNAL(finished()), this, SLOT(finished()));

    connectionState = PENDING;
    socket->connectToSecureHost("smtp.gmail.com");
    socket->sslSocket()->waitForConnected(waitTimeInMilliseconds);
}

Emailer::~Emailer()
{
    if (connectionState != NONE) closeConnection();
    delete socket;
}

bool Emailer::mailSent()
{
    return (mailSendState == SUCCESS);
}

void Emailer::conncted()
{
    connectionState = SUCCESS;
    cout << "Connection successful" << endl;

    mailSendState = PENDING;
    QxtMailMessage message;
    message.addRecipient(recipient.c_str());
    message.setSubject(subject.c_str());
    message.setBody(body.c_str());

    if (attachmentFileName.size() > 0)
    {
        QFile attachment(attachmentFileName.c_str());
        attachment.open(QFile::ReadOnly);
        message.addAttachment(attachmentFileName.c_str(), QxtMailAttachment(attachment.readAll()));
        attachment.close();
    }

    socket->send(message);
}

void Emailer::connectionFailed(const QByteArray &message)
{
    connectionState = FAILURE;
    cout << "Connection failed" << endl
         << QString(message).toStdString() << endl;
    closeConnection();
}

void Emailer::authenticated()
{
    authenticationState = SUCCESS;
    cout << "Authentication successful" << endl;
}

void Emailer::authenticationFailed(const QByteArray &message)
{
    authenticationState = FAILURE;
    cout << "Authentication failed" << endl
         << QString(message).toStdString() << endl;
    closeConnection();
}

void Emailer::mailSent(int)
{
    cout << "Mail sent successfully" << endl;
    closeConnection();
}

void Emailer::mailFailed(int, int, const QByteArray &message)
{
    cout << "Mail could not be sent" << endl
         << QString(message).toStdString() << endl;
    closeConnection();
}


void Emailer::senderRejected(int, const QString &address, const QByteArray &message)
{
    cout << "Sender rejected" << endl
         << address.toStdString() << endl
         << QString(message).toStdString() << endl;
    closeConnection();
}

void Emailer::finished()
{
    cout << "Finished" << endl;
}

void Emailer::closeConnection()
{
    socket->disconnectFromHost();
    connectionState = NONE;
    authenticationState = NONE;
    cout << "Connection closed" << endl;
}
