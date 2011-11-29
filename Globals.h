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
    Message(const std::string & newMessage, const int newData) : message(newMessage), data(newData) {}
};

// A class that makes sure only one instance of each database is created. Also handles database destruction
template<typename type>
class AssignOncePointer {
    type * pointer;

public:
    AssignOncePointer<type>() : pointer(NULL) {}
    AssignOncePointer<type>(const AssignOncePointer & assignOncePointer) : pointer(assignOncePointer.pointer) {}
    AssignOncePointer<type>(type * const newPointer) : pointer(newPointer) {}

    ~AssignOncePointer() {
        delete pointer;
    }

    void operator =(type * const newPointer) {
        if (pointer == NULL) pointer = newPointer;
        else {
            std::cout << "Reassignment forbidden" << std::endl;
            delete newPointer;
        }
    }

    type & operator *() {
        return *pointer;
    }

    type * operator ->() {
        return pointer;
    }

    operator type*() {
        return pointer;
    }
};

namespace Globals {

const float vatRate = 20.0f;

}

#endif /* GLOBALS_H_ */
