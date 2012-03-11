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

Emailer::Emailer(const EmailDetails &emailDetails, QObject *parent)
    : QObject(parent), socket(NULL), emailDetails_(emailDetails), completed(false), killNow(false),
      connectionState(NONE), authenticationState(NONE), mailSendState(NONE) {}

Emailer::~Emailer()
{
    if ((connectionState != NONE) && (socket != NULL)) closeConnection();
    if (socket != NULL) delete socket;
}

void Emailer::send()
{
    mailSendState = PENDING;

    if (socket != NULL) delete socket;
    socket = new QxtSmtp(this);
    socket->setUsername("ianfosterservices");
    socket->setPassword("temppassword");

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(connectionFailed(QByteArray)), this, SLOT(connectionFailed(QByteArray)));
    connect(socket, SIGNAL(authenticated()), this, SLOT(authenticated()));
    connect(socket, SIGNAL(authenticationFailed(QByteArray)), this, SLOT(authenticationFailed(QByteArray)));
    connect(socket, SIGNAL(mailSent(int)), this, SLOT(mailSent(int)));
    connect(socket, SIGNAL(mailFailed(int,int,QByteArray)), this, SLOT(mailFailed(int,int,QByteArray)));
    connect(socket, SIGNAL(senderRejected(int,QString,QByteArray)), this, SLOT(senderRejected(int,QString,QByteArray)));
    connect(socket, SIGNAL(finished()), this, SLOT(finished()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    connectionState = PENDING;
    socket->connectToSecureHost("smtp.gmail.com");

    ++emailDetails_.tries;
}

bool Emailer::pending() const
{
    return (mailSendState == PENDING);
}

bool Emailer::sentSuccessfully() const
{
    return (mailSendState == SUCCESS);
}

const EmailDetails & Emailer::emailDetails() const
{
    return emailDetails_;
}

void Emailer::kill()
{
    killNow = true;
    if (completed) delete this;
}

void Emailer::connected()
{
    connectionState = SUCCESS;
    cout << "Connection successful" << endl;

    QxtMailMessage message;
    message.addRecipient(emailDetails_.recipient.c_str());
    message.setSubject(emailDetails_.subject.c_str());
    message.setBody(emailDetails_.body.c_str());

    if (emailDetails_.attachmentFileName.size() > 0)
    {
        QFile attachment(emailDetails_.attachmentFileName.c_str());
        attachment.open(QFile::ReadOnly);
        message.addAttachment(emailDetails_.attachmentFileName.c_str(), QxtMailAttachment(attachment.readAll()));
        attachment.close();
    }

    socket->send(message);
}

void Emailer::connectionFailed(const QByteArray &message)
{
    connectionState = mailSendState = FAILURE;
    cout << "Connection failed" << endl
         << QString(message).toStdString() << endl;
}

void Emailer::authenticated()
{
    authenticationState = SUCCESS;
    cout << "Authentication successful" << endl;
}

void Emailer::authenticationFailed(const QByteArray &message)
{
    authenticationState = mailSendState = FAILURE;
    cout << "Authentication failed" << endl
         << QString(message).toStdString() << endl;
}

void Emailer::mailSent(int)
{
    mailSendState = SUCCESS;
    cout << "Mail sent successfully" << endl;
}

void Emailer::mailFailed(int, int, const QByteArray &message)
{
    mailSendState = FAILURE;
    cout << "Mail could not be sent" << endl
         << QString(message).toStdString() << endl;
}


void Emailer::senderRejected(int, const QString &address, const QByteArray &message)
{
    mailSendState = FAILURE;
    cout << "Sender rejected" << endl
         << address.toStdString() << endl
         << QString(message).toStdString() << endl;
}

void Emailer::finished()
{
    closeConnection();
    cout << "Finished" << endl;
}

void Emailer::disconnected()
{
    connectionState = authenticationState = NONE;
    completed = true;
    cout << "Connection closed" << endl;

    if (mailSendState == FAILURE) emit mailFailed();
    else if (mailSendState == SUCCESS) emit mailSent();

    if (killNow) delete this;
}

void Emailer::closeConnection()
{
    if (socket != NULL) socket->disconnectFromHost();
}
