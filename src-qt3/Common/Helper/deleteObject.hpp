/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author$
 ** 
 ** Created: Wed Dec  5 00:28:46 2001
 ** Time-stamp: <Fri Nov 29 2002 14:50:03 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#ifndef DELETEOBJECT_HPP
#define DELETEOBJECT_HPP

namespace effective_stl {
    /**
     * Operator template to delete an object.
     *
     * DeleteObject struct definition from Meyers 2001, "Effective
     * STL", pg. 38.  This struct can be passed to for_each when
     * deleting all objects held by a list of pointers.
     *
     * @param ptr Pointer to the object to be deleted.
     */
    struct DeleteObject {
	template <typename T> void operator()(const T* ptr) const 
	{
	    delete ptr;
	}
    };

};

namespace iqrcommon {
    /**
     * Template function for packaging deletion of an object into a
     * function pointer.
     *
     * This template can be used to encapsulate the deletion of
     * built-in data types for use by the type manager class template.
     * It is the complement of the createObject function template.
     *
     * @param ptr Pointer to the object to be deleted.
     */
    template <class T> void deleteObject(const T* ptr)
    {
	delete ptr;
    }
}

#endif
