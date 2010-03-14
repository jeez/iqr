/****************************************************************************
 ** $Filename: ClsBaseDataBroker.h
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Mon Sep 22 19:23:15 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSBASEDATABROKER_H
#define CLSBASEDATABROKER_H    /*+ To stop multiple inclusions. +*/

#include <map>
#include <ClsFEMessageOut.h>



using namespace std;
using namespace iqrcommon;

//BFS class ClsFESystemManager;

class ClsBaseDataBroker {

public:
    ClsBaseDataBroker() {};
    virtual ~ClsBaseDataBroker(){};

//BFS    void setSystemManagerPointer(ClsFESystemManager *_clsFESystemManager) {
//BFS	clsFESystemManager = _clsFESystemManager;
//BFS    };


    virtual void addDataRequest(string strPrcID, ClsFEMessageOut clsFEMessageOut)=0;


protected:

//BFS     ClsFESystemManager *clsFESystemManager;
     multimap<string, ClsFEMessageOut > mapMessagesOut;

};

#endif /* CLSBASEDATABROKER_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
