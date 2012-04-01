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

#include "Setting.h"
#include "SettingController.h"
#include "dialogs/setting/SettingForm.h"

Emailer::Emailer(const EmailDetails &emailDetails, QObject *parent)
    : QObject(parent), socket(NULL), emailDetails_(emailDetails), completed(false), killNow(false),
      connectionState(NONE), authenticationState(NONE), mailSendState(NONE) {}

Emailer::~Emailer()
{
    // If there is still a connection, close it
    if ((connectionState != NONE) && (socket != NULL)) closeConnection();
    if (socket != NULL) delete socket;
}

void Emailer::send()
{
    mailSendState = PENDING;

    // Create a new socket, and set the username and password to the values in the settings
    if (socket != NULL) delete socket;
    socket = new QxtSmtp(this);

    Setting usernameSetting = SettingController::getSetting(SettingForm::keyEmailUsername),
            passwordSetting = SettingController::getSetting(SettingForm::keyEmailPassword),
            hostSetting = SettingController::getSetting(SettingForm::keyEmailHost),
            portSetting = SettingController::getSetting(SettingForm::keyEmailPort);

    socket->setUsername(usernameSetting.getValue());
    socket->setPassword(passwordSetting.getValue());

    // Connect the Qt signals from the socket to the slots in the Emailer
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(connectionFailed(QByteArray)), this, SLOT(connectionFailed(QByteArray)));
    connect(socket, SIGNAL(authenticated()), this, SLOT(authenticated()));
    connect(socket, SIGNAL(authenticationFailed(QByteArray)), this, SLOT(authenticationFailed(QByteArray)));
    connect(socket, SIGNAL(mailSent(int)), this, SLOT(mailSent(int)));
    connect(socket, SIGNAL(mailFailed(int,int,QByteArray)), this, SLOT(mailFailed(int,int,QByteArray)));
    connect(socket, SIGNAL(senderRejected(int,QString,QByteArray)), this, SLOT(senderRejected(int,QString,QByteArray)));
    connect(socket, SIGNAL(finished()), this, SLOT(finished()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // Connect to the host with the host and port values from the settings
    connectionState = PENDING;
    socket->connectToSecureHost(hostSetting.getValue(), atoi(portSetting.getValue()));

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
    // Set killNow to true, indicating that when the Emailer is done, it can delete itself. If it has finished already,
    // then delete it now
    killNow = true;
    if (completed) delete this;
}

void Emailer::connected()
{
    connectionState = SUCCESS;
    cout << "Connection successful" << endl;

    // Connection successful, so create a new email from the email details
    QxtMailMessage message;
    message.addRecipient(emailDetails_.recipient.c_str());
    message.setSubject(emailDetails_.subject.c_str());
    message.setBody(emailDetails_.body.c_str());

    // If there is an attachment, open it and put the attachment data into the email
    if (emailDetails_.attachmentFileName.size() > 0)
    {
        QFile attachment(emailDetails_.attachmentFileName.c_str());
        attachment.open(QFile::ReadOnly);
        message.addAttachment(emailDetails_.attachmentFileName.c_str(), QxtMailAttachment(attachment.readAll()));
        attachment.close();
    }

    // Send the email
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

    // Send the relevant signal depending on whether the email sent successfully or not
    if (mailSendState == FAILURE) emit mailFailed();
    else if (mailSendState == SUCCESS) emit mailSent();

    if (killNow) delete this;
}

void Emailer::closeConnection()
{
    if (socket != NULL) socket->disconnectFromHost();
}
