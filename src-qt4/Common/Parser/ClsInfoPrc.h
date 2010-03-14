/****************************************************************************
 ** $Filename: ClsInfoPrc.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Mon Oct 22 11:49:26 2001
 **
 *****************************************************************************/


#ifndef CLSINFOPRC_H
#define CLSINFOPRC_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <iostream>
#include "../Helper/iqrUtils.h"

#include <ClsSysFileNode.h>

// 05/02/02 [jmb] Placed system file parser classes into namespace
// iqrcommon.
namespace iqrcommon {
    
    class ClsInfoPrc
    {
    public:
	ClsInfoPrc(){ 
	    strPrcID = "";
	    strProcessName = "";
	    strHostname = "";
	    iPort = 0;
	    iPrcStatus = 0;
	    iLastUpdateTime = 0;
	    iTimeDiff = 0;
	    strCommandLine = "";
	    bHasModule = false;
	    bEnableModule = true;
	    strPath = "";
	    strColor = "#FFFFFF";
	};

	ClsInfoPrc(string _strPrcID) : strPrcID(_strPrcID) { 
	    strProcessName = "";
	    strHostname = "";
	    iPort = 0;
	    iPrcStatus = 0;
	    iLastUpdateTime = 0;
	    iTimeDiff = 0;
	    strCommandLine = "";
	    bHasModule = false;
	    bEnableModule = true;
	    strPath = "";
	    strColor = "#FFFFFF";
	};
	
	ClsInfoPrc(string _strPrcID, string _strProcessName, string _strHostname, int _iPort, bool _bEnableModule):
	    strPrcID(_strPrcID), strProcessName(_strProcessName), strHostname(_strHostname), iPort(_iPort), 
	    bEnableModule(_bEnableModule) { 
	    iLastUpdateTime = 0;
	    strCommandLine = "";
	    strPath = "";
	    strColor = "#FFFFFF";
	};

	

	enum PRC_STATUS {
	    PRC_DIED = 0,
	    PRC_LAUNCHED,
	    PRC_ALIVE,
	    PRC_SYSTEMFILEPARSED,
	    PRC_SYSTEMBUILDT,
	    PRC_CONNECTED2PRCS,
	    PRC_READY,
	    PRC_SEND,
	    PRC_SLEEP
	};

  	void setCMDL( string _strCommandLine ){ strCommandLine = _strCommandLine; };
	 string getCMDL() { return strCommandLine; };
     
	void setPrcID( string _strPrcID){ strPrcID = _strPrcID;	};
	string getPrcID( ){ return strPrcID;};           

	void setPrcUID( string _strPrcUID){ strPrcUID = _strPrcUID; };
	string getPrcUID( ){ return strPrcUID;};           

	void setProcessName( string _strProcessName){ strProcessName = _strProcessName; };
	string getProcessName( ){ return strProcessName;};         

	void setHostname( string _strHostname){ strHostname = _strHostname; };
	string getHostname( ){ return strHostname;};        

	void setPath( string _strPath){ strPath = _strPath; };
	string getPath( ){ return strPath;};        


	void setPort( int _iPort){ iPort =  _iPort; };
	void setPort( string strPort){ iPort = iqrUtils::string2int(strPort); };
	int getPort ( ){ return iPort;}; 
	string getPortAsString ( ){ return iqrUtils::int2string(iPort);}; 
	void setStatus ( int _iPrcStatus ){ iPrcStatus = _iPrcStatus;}; 
	
	int getStatus ( ){ return iPrcStatus;}; 

	void setLastUpdateTime ( int _iLastUpdateTime ){ 
	    iTimeDiff = (abs(iLastUpdateTime - _iLastUpdateTime) + iTimeDiff) / 2; // average over last two times
	    iLastUpdateTime = _iLastUpdateTime;
	}; 
	int getLastUpdateTime ( ){ return iLastUpdateTime;}; 

	int getTimeDiff () { return iTimeDiff; };

	void setHasModule(bool b) { bHasModule = b; };
	bool getHasModule() { return bHasModule; };

	void setEnableModule(bool b){ bEnableModule = b; };
	void setEnableModule(string str){
	    if(str.size()>0){
		bEnableModule = iqrUtils::string2bool(str);
	    } else {
		bEnableModule = true;
	    }
	};
	bool getEnableModule(){ return bEnableModule; };
	
	void setProcessSubNodes(ClsSysFileNode _clsSysFileNodeProcessSubNodes){
	    clsSysFileNodeProcessSubNodes = _clsSysFileNodeProcessSubNodes;
	};

	ClsSysFileNode getSysFileNodeProcessSubNodes(){
	    return clsSysFileNodeProcessSubNodes;
	};

	void setNotes( string _strNotes ) { strNotes = _strNotes ; };
	string getNotes() { return strNotes; };

	string getColor() { 
	    if(strColor.length()<=0){
		return "#FFFFFF";
	    } 
	    return strColor;
	}
	void setColor( string _strColor ) { strColor = _strColor ; };
	

    private:
	string strPrcID;
	string strPrcUID;
	string strProcessName;
	string strHostname;
	 string strCommandLine;
	string strNotes;
	int iPort;
	int iPrcStatus;
	int iLastUpdateTime;
	int iTimeDiff;
	bool bHasModule;
	bool bEnableModule;
	string strPath;
	ClsSysFileNode clsSysFileNodeProcessSubNodes;
	string strColor;

     
	friend ostream &operator<<(ostream &stream, ClsInfoPrc clsInfoPrc);
    };
}

#endif

/// Local Variables: 
/// mode: c++
/// End: 


