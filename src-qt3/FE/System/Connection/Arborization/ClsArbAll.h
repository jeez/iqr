/****************************************************************************
 ** $Filename: ClsArbAll.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri May 16 13:02:53 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSARBALL_H
#define CLSARBALL_H

#include "ClsBaseArborization.h"

using namespace iqrcommon;
using namespace std;

class ClsArbAll : public ClsBaseArborization{

public:
    ClsArbAll();
    double DistMax(){
	cerr << "NOT IMPLEMENTED " << __FILE__ << endl;
	exit(1);
	return 0;
    }
;


};

#endif // CLSARBALL_H

/// Local Variables:
/// mode: c++
/// End:
