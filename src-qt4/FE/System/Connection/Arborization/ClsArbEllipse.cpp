/****************************************************************************
 ** $Filename: ClsArbEllipse.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:03:08 2003
 **
 *****************************************************************************/

#include <limits.h> //MARK 
#include "ClsArbEllipse.h"


ClsArbEllipse::ClsArbEllipse() {
    strType = ClsTagLibrary::ConnectionArborizationEllipse();

    pclsArborizationWidth = addIntParameter(ClsTagLibrary::ArborizationWidthTag(), "Width",
		    1, /* iValue */
		    1, /* Minimum */
                    INT_MAX, /* Maximum */ 
		    "Description", 
		    "Properties");

    pclsArborizationHeight =  addIntParameter(ClsTagLibrary::ArborizationHeightTag(), "Height",
		    1, /* iValue */
		    1, /* Minimum */
                    INT_MAX, /* Maximum */ 
		    "Description", 
		    "Properties");
    
};



