/****************************************************************************
 ** $Filename: ClsInfoConnection.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 *****************************************************************************/


#ifndef CLSINFOCONNECTION_H
#define CLSINFOCONNECTION_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include "../Helper/iqrUtils.h"

#include <ClsSysFileNode.h>

// 05/02/02 [jmb] Placed system file parser classes into namespace
// iqrcommon.
namespace iqrcommon {
    
    class ClsInfoConnection
    {
    public:
	ClsInfoConnection(){ 
	    strConnectionID = "";
	    strConnectionName = "";
	    strConnectionTargetID = "";
	    strConnectionSourceID = "";
	    iConnectionStatus = 0;
	};
	
	enum CONNECTION_TYPE {
	    CONN_EXCITATORY,
	    CONN_INHIBITORY,
	    CONN_MODULATORY
	};
	


	
	ClsInfoConnection(string _strConnectionID, 
			  string _strConnectionName, 
			  string _strConnectionTargetID, 
			  string _strConnectionSourceID ):
	    strConnectionID(_strConnectionID), 
	    strConnectionName(_strConnectionName),
	    strConnectionTargetID(_strConnectionTargetID), 
	    strConnectionSourceID(_strConnectionSourceID) { 
	};
		
	void setConnectionID( string _strConnectionID ){
	    strConnectionID = _strConnectionID;
	};

	void setConnectionName( string _strConnectionName ){
	    strConnectionName = _strConnectionName;
	};
	
	void setConnectionTargetID(string _strConnectionTargetID ){
	    strConnectionTargetID = _strConnectionTargetID;
	};
	
	void setConnectionSourceID(string _strConnectionSourceID ){
	    strConnectionSourceID = _strConnectionSourceID;
	};
	
	void setConnectionType(int i) { iConnectionType = i; };
	
	void setStatus ( int _iConnectionStatus ){ 
	    iConnectionStatus = _iConnectionStatus;
	}; 
	
	void setConnectionSubNodes(ClsSysFileNode _clsSysFileNodeConnectionSubNodes){
	    clsSysFileNodeConnectionSubNodes = _clsSysFileNodeConnectionSubNodes;
	};

	ClsSysFileNode getSysFileNodeConnectionSubNodes(){
	    return clsSysFileNodeConnectionSubNodes;
	};

	string getConnectionID( ){ return strConnectionID;};           
	string getConnectionName( ){ return strConnectionName;};         
	
	string getConnectionTargetID( ){ return  strConnectionTargetID;};
	string getConnectionSourceID( ){ return  strConnectionSourceID;};	
	
	int getConnectionType( ) { return iConnectionType; };
	int getStatus ( ){ return iConnectionStatus;}; 
		
	string getNotes() { return strNotes; };
	void setNotes( string _strNotes ) { strNotes = _strNotes ; };

    private:
	string strConnectionID;
	string strConnectionName;
	string strConnectionTargetID;
	string strConnectionSourceID;
	string strNotes;
	int iConnectionType;
	int iConnectionStatus;

	ClsSysFileNode clsSysFileNodeConnectionSubNodes;
	    
//	friend ostream &operator<<(ostream &stream, ClsInfoConnection clsInfoConnection);	
    };
}

/// Local Variables: 
/// mode: c++
/// End: 

#endif /* CLSINFOCONNECTION_H */



