/****************************************************************************
 ** $Filename: ClsBaseArborization.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri May 16 13:02:56 2003
 **
 *****************************************************************************/


#include "ClsBaseArborization.h"
#include "boolParameter.hpp"
#include "doubleParameter.hpp"
#include "optionsParameter.hpp"

ClsBaseArborization::ClsBaseArborization() {

/*
    addBoolParameter(ClsTagLibrary::ArborizationSelfConnectTag(),
		     "Self connect", false,
		     "Description",
		     "Properties");
*/

    pclProbability = addDoubleParameter(ClsTagLibrary::ArborizationInitializationProbabilityTag(),
		       "Initialization probability",
		       1.0, 
		       0.0, 
		       1.0, 
		       3,
		       "Description",
		       "Properties");

    pclDirection = addOptionsParameter(ClsTagLibrary::ArborizationDirectionTag(),
			"Direction",
			false,
			"Description",
			"Properties");

//--    ClsOptionsParameter *op = static_cast<ClsOptionsParameter*>(getParameter(ClsTagLibrary::ArborizationDirectionTag()));
    pclDirection->addOption(ClsTagLibrary::ConnectionArborizationRFTag());
    pclDirection->addOption(ClsTagLibrary::ConnectionArborizationPFTag());

    setParameter(ClsTagLibrary::ArborizationDirectionTag(), ClsTagLibrary::ConnectionArborizationRFTag());

    bRuntimeEditable = false;


}


void ClsBaseArborization::getListOfCells( int /*iX*/, int /*iY*/ ) {

}
