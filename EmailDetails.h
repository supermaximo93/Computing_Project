/*
 * EmailDetails.h
 *
 *  Created on: 21 March 2012
 *      Author: Max Foster
 */

#ifndef EMAILDETAILS_H
#define EMAILDETAILS_H

#include <iostream>

// A struct containing the data needed to be able to construct an email used by the Emailer class
struct EmailDetails
{
    static const int maxEmailTries = 3;

    std::string recipient, subject, body, attachmentFileName;
    int tries; // The number of times the email has attempted to have been sent

    EmailDetails(const char *recipient, const char *subject, const char *body, const char *attachmentFileName = "")
        : recipient(recipient), subject(subject), body(body), attachmentFileName(attachmentFileName), tries(0) {}
};

#endif // EMAILDETAILS_H
