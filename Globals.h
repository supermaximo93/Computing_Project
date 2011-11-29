/*
 * Globals.h
 *
 *  Created on: 27 Nov 2011
 *      Author: Max Foster
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <iostream>
#include <queue>

struct Message {
    std::string message;
    int data;

    Message() {};
    Message(const std::string & newMessage, const int newData) {
        message = newMessage;
        data = newData;
    }
};

namespace Globals {

const float vatRate = 20.0f;

}

#endif /* GLOBALS_H_ */
