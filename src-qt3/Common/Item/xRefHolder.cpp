/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Sun Nov 2 14:30:00 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <algorithm> //MARK
#include <iostream>
using namespace std;

#include <Common/Helper/deleteObject.hpp>
using namespace effective_stl;

#include "xRefHolder.hpp"

#ifdef DEBUG_CLSXREFHOLDER
static const bool bDebugXRefHolder = true;
#else
static const bool bDebugXRefHolder = false;
#endif

iqrcommon::ClsXRefHolder::ClsXRefHolder() 
{
    if (bDebugXRefHolder) {
	cout << "ClsXRefHolder::ClsXRefHolder" << endl;
    }    
    strCategory = "";
}
    
iqrcommon::ClsXRefHolder::~ClsXRefHolder() 
{    
    if (bDebugXRefHolder) {
	cout << "ClsXRefHolder::~ClsXRefHolder" << endl;
    }

    // Delete the cross references explicitly.
    for_each(lstXRefs.begin(), lstXRefs.end(), DeleteObject());
}

void
iqrcommon::ClsXRefHolder::setName(string _strName){
    strName = _strName;
}

const string
iqrcommon::ClsXRefHolder::getName() const{
    return strName;
}

void
iqrcommon::ClsXRefHolder::setCategory(string _strCategory){
    strCategory = _strCategory;
}

const string
iqrcommon::ClsXRefHolder::getCategory() const{
    return strCategory;
}


/** 
 * Add a new cross reference
 *
 * @param _name Name of the cross reference.
 * @param _label Human-readable label for the cross reference.
 */
iqrcommon::ClsXRef*
iqrcommon::ClsXRefHolder::addXRef(string _name, string _label)
{
    ClsXRef *pXRef = new ClsXRef(_name, _label);
    lstXRefs.push_back(pXRef);
    return pXRef;
}

/** 
 * @return Reference to the list of cross references.
 */
const iqrcommon::XRefList& 
iqrcommon::ClsXRefHolder::getListXRefs() const
{
    return lstXRefs;
}

/** 
 * @return count of XRefs stored.
 */
const int iqrcommon::ClsXRefHolder::getXRefsCount(){
    return lstXRefs.size();
}


/** 
 * Get the specified cross reference by name.
 *
 * This function searches the list for the specified name.  If the
 * name is found, a pointer to the cross reference is returned; if the
 * name is not found, a NULL pointer is returned.
 *
 * @return Pointer to the specified cross reference.  WARNING: If the
 * name is not found, a NULL pointer is returned.
 * @param _name Name of desired cross reference.
 */
iqrcommon::ClsXRef*
iqrcommon::ClsXRefHolder::getXRefByName(string _name)
{
    ClsXRef *pXRef = 0;

    // Find _name in list.
    XRefList::const_iterator it = lstXRefs.begin();
    while (pXRef == 0 && it != lstXRefs.end()) {
	if ((*it)->getName() == _name) {
	    pXRef = *it;
	} else {
	    ++it;
	}
    }
    
    return pXRef;
}

iqrcommon::ClsXRef*
iqrcommon::ClsXRefHolder::getXRefByLabel(string _label)
{
    ClsXRef *pXRef = 0;

    // Find _label in list.
    XRefList::const_iterator it = lstXRefs.begin();
    while (pXRef == 0 && it != lstXRefs.end()) {
	if ((*it)->getLabel() == _label) {
	    pXRef = *it;
	} else {
	    ++it;
	}
    }
    
    return pXRef;
}

void
iqrcommon::ClsXRefHolder::setXRefTargetByName(string _name, string _target)
{
    ClsXRef *pXRef = getXRefByName(_name);

    if (pXRef) {
	pXRef->setTarget(_target);
    } else {
	// @todo Throw an exception (UnknownXRef) here?
    }
}

void
iqrcommon::ClsXRefHolder::setXRefTargetByLabel(string _label, string _target)
{
    ClsXRef *pXRef = getXRefByLabel(_label);

    if (pXRef) {
	pXRef->setTarget(_target);
    } else {
	// @todo Throw an exception (UnknownXRef) here?
    }
}
