/*
 * AssignOncePointer.h
 *
 *  Created on: 27 Jan 2012
 *      Author: Max Foster
 */

#ifndef ASSIGNONCEPOINTER_H
#define ASSIGNONCEPOINTER_H

#include <iostream>

// A class that makes sure only one instance of each database is created. Also handles database destruction
template<typename type>
class AssignOncePointer
{
public:
    AssignOncePointer<type>() : pointer(NULL) {}
    AssignOncePointer<type>(const AssignOncePointer & assignOncePointer) : pointer(assignOncePointer.pointer) {}
    AssignOncePointer<type>(type * const newPointer) : pointer(newPointer) {}

    ~AssignOncePointer()
    {
        delete pointer;
    }

    void operator =(type * const newPointer)
    {
        if (pointer == NULL) pointer = newPointer;
        else
        {
            std::cout << "Reassignment forbidden" << std::endl;
            delete newPointer;
        }
    }

    type & operator *()
    {
        return *pointer;
    }

    type * operator ->()
    {
        return pointer;
    }

    operator type*()
    {
        return pointer;
    }

private:
    type * pointer;
};

#endif // ASSIGNONCEPOINTER_H
