/****************************************************************************
 ** $Filename: ClsArbEllipseWindow.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:03:06 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <limits.h> //MARK 
#include "ClsArbEllipseWindow.h"


ClsArbEllipseWindow::ClsArbEllipseWindow() {
    strType = ClsTagLibrary::ConnectionArborizationEllipseWindow();

    pclsArborizationInnerWidth = addIntParameter(ClsTagLibrary::ArborizationInnerWidthTag(), "Inner width",
		    0, /* iValue */
		    0, /* Minimum */
                    INT_MAX, /* Maximum */ 
		    "Description", 
		    "Properties");
    
    pclsArborizationInnerHeight = addIntParameter(ClsTagLibrary::ArborizationInnerHeightTag(), "Inner height",
			   0, /* iValue */
			   0, /* Minimum */
			   INT_MAX, /* Maximum */ 
			   "Description", 
			   "Properties");
    
    pclsArborizationOuterWidth = addIntParameter(ClsTagLibrary::ArborizationOuterWidthTag(), "Outer width",
			   1, /* iValue */
			   1, /* Minimum */
			   INT_MAX, /* Maximum */ 
			   "Description", 
			   "Properties");
    
    pclsArborizationOuterHeight = addIntParameter(ClsTagLibrary::ArborizationOuterHeightTag(), "Outer height",
			   1, /* iValue */
			   1, /* Minimum */
			   INT_MAX, /* Maximum */ 
			   "Description", 
			   "Properties");
    
};


