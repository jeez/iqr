/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Dec 9 21:50:00 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Provides a mechanism for holding indirect pointers which
 **               allows a base class to initialize a pointer which derived
 **               classes can use transparently using operator->().  The
 **               target pointer can be set either by calling setTarget 
 **               or using operator=().
 **
 *****************************************************************************/

#ifndef INDIRECTPTR_HPP
#define INDIRECTPTR_HPP

namespace iqrcommon {
    template <typename T> class ClsIndirectPtr
    {
    public:
	ClsIndirectPtr() : pTarget(0) {}
	~ClsIndirectPtr() {}

	/**
	 * Get the target pointer.
	 */
	T* operator->() {return pTarget;}
	T* getTarget()  {return pTarget;}

	/**
	 * Set the target pointer.
	 */
	void operator=(T *_pTarget) {pTarget = _pTarget;}
	void setTarget(T *_pTarget) {pTarget = _pTarget;}

	void resetTarget() {pTarget = 0;}

    private:
	ClsIndirectPtr(const ClsIndirectPtr &);
	T *pTarget;
    };
}

#endif
