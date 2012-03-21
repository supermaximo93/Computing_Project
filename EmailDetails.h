/*
 * EmailDetails.h
 *
 *  Created on: 21 March 2012
 *      Author: Max Foster
 */

#ifndef EMAILDETAILS_H
#define EMAILDETAILS_H

#include <iostream>

struct EmailDetails
{
    static const int maxEmailTries = 3;

    std::string recipient, subject, body, attachmentFileName;
    int tries;

    EmailDetails(const char *recipient, const char *subject, const char *body, const char *attachmentFileName = "")
        : recipient(recipient), subject(subject), body(body), attachmentFileName(attachmentFileName), tries(0) {}
};

#endif // EMAILDETAILS_H
