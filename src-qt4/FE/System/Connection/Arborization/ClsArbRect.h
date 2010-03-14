/****************************************************************************
 ** $Filename: ClsArbRect.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:02:43 2003
 **
 *****************************************************************************/


#ifndef CLSARBRECT_H
#define CLSARBRECT_H

#include "ClsBaseArborization.h"

using namespace iqrcommon;
using namespace std;

class ClsArbRect : public ClsBaseArborization {

public:
    ClsArbRect();
    int getWidth(){
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::ArborizationWidthTag())->getValueAsString());
	return pclsArborizationWidth->getValue();
    };
    int getHeight(){
//	return iqrUtils::string2int(getParameter(ClsTagLibrary::ArborizationHeightTag())->getValueAsString());
	return pclsArborizationHeight->getValue();
    };

    double DistMax(){
	return calculateDistance(1., 1., getWidth(), getHeight());
    };

private:
    ClsIntParameter *pclsArborizationWidth, *pclsArborizationHeight;

};

#endif // CLSARBRECT_H

/// Local Variables: 
/// mode: c++
/// End: 

