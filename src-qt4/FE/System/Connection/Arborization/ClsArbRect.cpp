/****************************************************************************
 ** $Filename: ClsArbRect.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:03:00 2003
 **
 *****************************************************************************/

#include <limits.h> //MARK 
#include "ClsArbRect.h"


ClsArbRect::ClsArbRect() {
    strType = ClsTagLibrary::ConnectionArborizationRectangular();

    pclsArborizationWidth = addIntParameter(ClsTagLibrary::ArborizationWidthTag(), "Width",
		    1, /* iValue */
		    1, /* Minimum */
                    INT_MAX, /* Maximum */ 
		    "Description", 
		    "Properties");

    pclsArborizationHeight = addIntParameter(ClsTagLibrary::ArborizationHeightTag(), "Height",
		    1, /* iValue */
		    1, /* Minimum */
                    INT_MAX, /* Maximum */ 
		    "Description", 
		    "Properties");
    
};




