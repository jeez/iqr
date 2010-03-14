/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Sun Nov 2 14:15:00 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description: 
 **
 *****************************************************************************/

#include <iostream>
using namespace std;

#include "xRef.hpp"

#ifdef DEBUG_XREF
static const bool bDebugXRef = true;
#else
static const bool bDebugXRef = false;
#endif

iqrcommon::ClsXRef::ClsXRef(string _name, string _label)
    : name(_name),
      label(_label),
      target("")
{
    if (bDebugXRef) {
	cout << "ClsXRef::ClsXRef: name = " << name
	     << ", label = " << label << endl;
    }
}

iqrcommon::ClsXRef::~ClsXRef()
{
    if (bDebugXRef) {
	cout << "ClsXRef::~ClsXRef: name = " << name << endl;
    }
}

string iqrcommon::ClsXRef::getName() const
{
    return name;
}

string iqrcommon::ClsXRef::getLabel() const
{
    return label;
}
	
string iqrcommon::ClsXRef::getTarget() const
{
    return target;
}

void iqrcommon::ClsXRef::setTarget(string _target)
{
    target  = _target;

    if (bDebugXRef) {
	cout << "ClsXRef::setTarget: name = " << name
	     << ", target = " << target << endl;
    }
}

/*
void iqrcommon::ClsXRef::setListXRefCandidates(list<string> lst){
    lstXRefCandidates = lst;
}

const list<string>& iqrcommon::ClsXRef::getListXRefCandidates() const{
    return lstXRefCandidates;
}
*/
