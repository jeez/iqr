/****************************************************************************
 ** $Filename: ClsBaseFunction.h
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed May 21 14:01:33 2003
 **
 *****************************************************************************/


#ifndef CLSBASEFUNCTION_H
#define CLSBASEFUNCTION_H

#include "tagLibrary.hpp"
#include "item.hpp"
#include "ClsHyperLists.h"
#include "iqrUtils.h"

using namespace iqrcommon;
using namespace std;


class ClsBaseFunction : public ClsItem {

public:
    ClsBaseFunction(int _iTarget) : iTarget(_iTarget){ bRuntimeEditable = false;};
    string Type() const {return strType;}

    virtual void Calculate(ListIndexQuadruples &listIndexQuadruples) = 0;

    virtual double max() = 0;

    enum TARGET {
	delay,
	attenuation
    };

protected:
    string strType;
    int iTarget;

};

#endif // CLSBASEFUNCTION_H


/// Local Variables: 
/// mode: c++
/// End: 

