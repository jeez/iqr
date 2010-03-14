/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author$
 ** 
 ** Created: Wed Dec  5 12:47:57 2001
 ** Time-stamp: <Fri Nov 29 2002 14:49:14 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#ifndef CREATEOBJECT_HPP
#define CREATEOBJECT_HPP

namespace iqrcommon {

    /** 
     * Function template for wrapping the construction of objects.
     *
     * This function creates objects of a derived type and returns
     * them via a pointer to the base class.  The destructor of the
     * base class B must be virtual to ensure correct destruction of
     * the new object of the derived class D.
     *
     * If class D is not derived from class B, this function template
     * will fail to compile.
     *
     * @return Base class pointer to new object.
     */
    template <class B, class D> B* createObject()
    {
	return new D;
    }

};

#endif
