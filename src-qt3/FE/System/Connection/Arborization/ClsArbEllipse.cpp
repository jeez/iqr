/****************************************************************************
 ** $Filename: ClsArbEllipse.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:03:08 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
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



