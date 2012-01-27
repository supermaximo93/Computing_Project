/*
 * AssignOncePointer.h
 *
 *  Created on: 27 Jan 2012
 *      Author: Max Foster
 */

#ifndef ASSIGNONCEPOINTER_H
#define ASSIGNONCEPOINTER_H

#include <iostream>
#include <stdexcept>

// A class that makes sure only one instance of each database is created. Also handles database destruction
template<typename type>
class AssignOncePointer
{
public:
    AssignOncePointer<type>() : pointer(NULL) {}
    AssignOncePointer<type>(type * const newPointer) : pointer(newPointer) {}
    AssignOncePointer<type>(const AssignOncePointer &) : pointer(NULL)
    {
        throw(std::runtime_error("AssignOncePointer cannot be copied"));
    }

    ~AssignOncePointer()
    {
        delete pointer;
    }

    void operator =(const AssignOncePointer &)
    {
        throw(std::runtime_error("AssignOncePointer cannot be copied"));
    }

    void operator =(type * const newPointer)
    {
        if (pointer == NULL) pointer = newPointer;
        else
        {
            delete newPointer;
            throw(std::runtime_error("Reassignment forbidden"));
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
