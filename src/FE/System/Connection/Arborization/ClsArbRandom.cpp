/****************************************************************************
 ** $Filename: ClsArbRandom.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:03:03 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <limits.h> //MARK 
#include "ClsArbRandom.h"


ClsArbRandom::ClsArbRandom() {
    strType = ClsTagLibrary::ConnectionArborizationRandom();

    pclsArbCount = addIntParameter(ClsTagLibrary::ArborizationCountTag(), "Count",
		    1, /* iValue */
		    0, /* Minimum */
                    INT_MAX, /* Maximum */ 
		    "Description", 
		    "Properties");
}
