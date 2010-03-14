#ifndef CLSDIAGHYPERCONNECTION_H
#define CLSDIAGHYPERCONNECTION_H

#include <string>

#include "ClsDiagPhantomConnection.h"
#include "ClsDiagConnection.h"

class ClsDiagItemAP;
class ClsDiagItem;
class ClsDiagConnectionHandle;
class ClsDiagConnectionHandleStart;
class ClsDiagConnectionHandleEnd;

namespace iqrcommon {
    class ClsInfoDiagramLine;
}

using namespace std;
using namespace iqrcommon;

class ClsDiagHyperConnection  {

public:
    ClsDiagHyperConnection( string _strID,  ClsInfoDiagramLine clsInfoDiagramLine);
    
    ~ClsDiagHyperConnection();

    string getID(){ return strID; };
    void setSourceGroupID(string _strSourceGroupID, int iAP = -1);
    string getSourceGroupID() { return strSourceGroupID; };
    
    void setTargetGroupID(string _strTargetGroupID, int iAP = -1);
    string getTargetGroupID() { return strTargetGroupID; };
    
    void setSourceProcessID(string _strSourceProcessID);
    string getSourceProcessID() { return strSourceProcessID; };
    
    void setTargetProcessID(string _strTargetProcessID);
    string getTargetProcessID() { return strTargetProcessID; };

    void mark(bool b);
    void setConnected(bool b);

    void setConnectionType(int i);

    vector<vector<int> > getPoints(){
	vector<vector<int> > v;
	if(clsDiagConnectionGrpGrp!=NULL){
	    v = clsDiagConnectionGrpGrp->getPoints();
	}
	return v;
    }


    ClsDiagPhantomConnection* getPhantomStart(){
	return clsDiagPhantomConnectionStart;
    }

    ClsDiagPhantomConnection* getPhantomEnd(){
	return clsDiagPhantomConnectionEnd;
    }

    ClsDiagConnection* getGrpGrpConnection(){
	return clsDiagConnectionGrpGrp;
    }


    int getStartAPNumber();
    int getEndAPNumber();


    int isIPConnection() { return bInterprocessConnection; };
    bool checkIfIP(string _strSourceGroupID, string _strTargetGroupID);



private:

    void createConnectionGrpGrp(string strSourceGroupID, 
				string strTargetGroupID,
				ClsInfoDiagramLine clsInfoDiagramLine);
    
    void createConnectionPrcPrc( string strSourceProcessID, string strTargetProcessID);
    

    void createPhantomConnection(int iType, string strSourceProcessID, string strTargetProcessID, 
				 string strSourceGroupID, string strTargetGroupID,
				 ClsInfoDiagramLine clsInfoDiagramLine);
    
    void drawConnectionPhantom(ClsDiagPhantomConnection *clsDiagConnection, ClsDiagItemAP* clsDiagItemAPStart, ClsDiagItemAP* clsDiagItemAPEnd);





    string strID;
    
    string strSourceGroupID;
    string strTargetGroupID;
    string strSourceProcessID;
    string strTargetProcessID;
    int iConnectionType;
    bool bInterprocessConnection;

    ClsDiagConnection* clsDiagConnectionPrcPrc;
    ClsDiagConnection* clsDiagConnectionGrpGrp;
    ClsDiagPhantomConnection* clsDiagPhantomConnectionStart;
    ClsDiagPhantomConnection* clsDiagPhantomConnectionEnd;

};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
