/****************************************************************************
 ** $Filename: ClsArbEllipse.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:02:50 2003
 **
 *****************************************************************************/


#ifndef CLSARBELLIPSE_H
#define CLSARBELLIPSE_H

#include "ClsBaseArborization.h"

using namespace iqrcommon;
using namespace std;


class ClsArbEllipse : public ClsBaseArborization {


public:
    ClsArbEllipse();
    int getWidth(){
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::ArborizationWidthTag())->getValueAsString());
	return pclsArborizationWidth->getValue();
    };
    int getHeight(){
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::ArborizationHeightTag())->getValueAsString());
	return pclsArborizationHeight->getValue();
    };

    double DistMax(){
	return calculateDistance(1., 1., getWidth(), getHeight());
    };

private:
    ClsIntParameter *pclsArborizationWidth, *pclsArborizationHeight;
};

#endif // CLSARBELLIPSE_H

/// Local Variables: 
/// mode: c++
/// End: 

