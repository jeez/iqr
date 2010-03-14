/****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $Description: Provides a mechanism for holding named pointers which
 **               allows a base class to initialize a pointer which derived
 **               classes can use transparently using operator->().  The
 **               target pointer can be set either by calling setTarget 
 **               or using operator=().
 **
 *****************************************************************************/

#ifndef NAMEDPTR_HPP
#define NAMEDPTR_HPP

#include <string>
using namespace std;

namespace iqrcommon {
    template <typename T> class ClsNamedPtr
    {
    public:
	ClsNamedPtr(const string& _name) : name(_name), pTarget(0) {}
	~ClsNamedPtr() {}

	/**
	 * Get the pointer name.
	 */
	const string& getName() const { return name; };
	// Don't allow the name to be changed after construction

	/**
	 * Get the target pointer.
	 */
	T* operator->() const {return pTarget;}
	T* getTarget()  const {return pTarget;}

	/**
	 * Set the target pointer.
	 */
	void operator=(T *_pTarget) {pTarget = _pTarget;}
	void setTarget(T *_pTarget) {pTarget = _pTarget;}

	void resetTarget() {pTarget = 0;}

    private:
	ClsNamedPtr(const ClsNamedPtr &);
	string name;
	T *pTarget;
    };
}

#endif
