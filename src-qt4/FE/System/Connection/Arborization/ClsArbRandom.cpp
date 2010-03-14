/****************************************************************************
 ** $Filename: ClsArbRandom.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 16 13:03:03 2003
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
