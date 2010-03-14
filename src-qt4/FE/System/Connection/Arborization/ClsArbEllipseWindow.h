/****************************************************************************
 ** $Filename: ClsArbEllipseWindow.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:02:48 2003
 **
 *****************************************************************************/


#ifndef CLSARBELLIPSEWINDOW_H
#define CLSARBELLIPSEWINDOW_H

#include "ClsBaseArborization.h"

using namespace iqrcommon;
using namespace std;


class ClsArbEllipseWindow : public ClsBaseArborization {

public:
    ClsArbEllipseWindow();
    int getInnerWidth(){
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::ArborizationInnerWidthTag())->getValueAsString());
	return pclsArborizationInnerWidth->getValue();
    }
    int getInnerHeight(){
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::ArborizationInnerHeightTag())->getValueAsString());
	return pclsArborizationInnerHeight->getValue();
    }
    int getOuterWidth(){
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::ArborizationOuterWidthTag())->getValueAsString());
	return pclsArborizationOuterWidth->getValue();
    }
    int getOuterHeight(){
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::ArborizationOuterHeightTag())->getValueAsString());
	return pclsArborizationOuterHeight->getValue();
    }

    double DistMax(){
	return calculateDistance(1., 1., getOuterWidth(), getOuterHeight());
    };

private:
    ClsIntParameter *pclsArborizationInnerWidth, *pclsArborizationInnerHeight, *pclsArborizationOuterWidth, *pclsArborizationOuterHeight;

};

#endif // CLSARBELLIPSEWINDOW_H

/// Local Variables: 
/// mode: c++
/// End: 

