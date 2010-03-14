/****************************************************************************
 ** $Filename: ClsArbRandom.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri May 16 13:02:45 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSARBRANDOM_H
#define CLSARBRANDOM_H

#include "ClsBaseArborization.h"

using namespace iqrcommon;
using namespace std;

class ClsArbRandom : public ClsBaseArborization {


public:
    ClsArbRandom();
    int getCount(){
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::ArborizationCountTag())->getValueAsString());
	return pclsArbCount->getValue();
    }
    double DistMax(){
	cerr << "NOT IMPLEMENTED " << __FILE__ << endl;
	exit(1);
	return 0;
    }

    

private:
    ClsIntParameter *pclsArbCount;
};

#endif // CLSARBRANDOM_H

/// Local Variables:
/// mode: c++
/// End:
