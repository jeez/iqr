/****************************************************************************
 ** $Filename: ClsFECommHandlerLocal.h
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Thu Aug  7 18:21:33 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include "ClsBaseDataBroker.h"
//#include <ClsMessageParser.h>

#ifndef CLSFECOMMHANDLERLOCAL_H
#define CLSFECOMMHANDLERLOCAL_H    /*+ To stop multiple inclusions. +*/

class ClsFECommHandlerLocal : public ClsBaseDataBroker{

public:
    ClsFECommHandlerLocal() {};
     void addDataRequest(string strPrcID, ClsFEMessageOut clsFEMessageOut);


private:

};



#endif /* CLSFECOMMHANDLERLOCAL_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
