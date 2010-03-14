/****************************************************************************
 ** $Filename: ClsArbRect.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:03:00 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
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




