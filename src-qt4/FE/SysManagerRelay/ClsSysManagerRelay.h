#ifndef CLSSYSMANAGERRELAY_H
#define CLSSYSMANAGERRELAY_H

#include <iostream>
#include <list>

#include "ClsParamTrade.h"

using namespace std;



class ClsSysManagerRelay  {
public:

    
    static ClsSysManagerRelay* Instance();
    static void initializeParamRelais();

    list<ClsParamTrade> getParamTrades();
    int setParameterByItemID(string strType, string strItemID, string strParamID, double fValue);



private:
    ClsSysManagerRelay ();
    static ClsSysManagerRelay* _instanceParamRelais;

   
};


#endif


//// Local Variables:
//// mode: c++
//// End:
