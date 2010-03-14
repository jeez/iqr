/****************************************************************************
 ** $Filename: ClsInfoSystem.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Mon Oct 22 11:52:53 2001
 **
 *****************************************************************************/


#ifndef CLSINFOSYSTEM_H
#define CLSINFOSYSTEM_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include "../Helper/iqrUtils.h"

//#include <iostream>

namespace iqrcommon {
    
    class ClsInfoSystem
    {
    public:
	ClsInfoSystem(){ 
	    strSystemID = "";
	    strSystemName = "";
	    strHostname = "";
	    strSystemFileName = "";
	    strAuthor = "";
	    strDate = "";
	    iPort = 0;
	    iSystemStatus = 0;
	    iCyclesPerSecond = 0;
	    bSyncPlots = false;
	};
	

	ClsInfoSystem( string _strSystemID, string _strSystemName, 
		       string _strHostname, string _strAuthor, 
		       string _strDate, int _iPort, int _iCyclesPerSecond, bool _bSyncPlots ):
	    strSystemID(_strSystemID), 
	    strSystemName(_strSystemName), 
	    strHostname(_strHostname), 
	    strAuthor(_strAuthor), 
	    strDate(_strDate), 
	    iPort(_iPort),
	    iCyclesPerSecond(_iCyclesPerSecond),
	    bSyncPlots(_bSyncPlots)
	{ 
	    strSystemFileName = "";
	};
	
	void setSystemID( string _strSystemID ){
	    strSystemID = _strSystemID;
	};
	void setSystemName( string _strSystemName ){
	    strSystemName = _strSystemName;
	};
	void setHostname( string _strHostname ){
	    strHostname = _strHostname;
	};
	void setSystemFileName( string _strSystemFileName ){
	    strSystemFileName = _strSystemFileName;
	};
	

	void setAuthor( string _strAuthor ){
	    strAuthor = _strAuthor;
	};
	
	void setDate( string _strDate ){
	    strDate = _strDate;
	};
	
	void setPort( int _iPort ){
	    iPort =  _iPort;
	};
	
	void setPort( string strPort ){
	    iPort = iqrUtils::string2int(strPort);
	};


	void setCyclesPerSecond( int _iCyclesPerSecond ){
	    iCyclesPerSecond =  _iCyclesPerSecond;
	};
	
	void setCyclesPerSecond( string strCyclesPerSecond ){
	    iCyclesPerSecond = iqrUtils::string2int(strCyclesPerSecond);
	};

	void setSyncPlots( int _bSyncPlots ){
	    bSyncPlots =  _bSyncPlots;
	};
	
	void setSyncPlots( string strSyncPlots ){
	    bSyncPlots = iqrUtils::string2bool(strSyncPlots);
	};
	
	void setStatus ( int _iSystemStatus ){ 
	    iSystemStatus = _iSystemStatus;
	}; 
	
	enum SYSTEM_STATUS {
	};
	
	string getSystemID( ){ return strSystemID;};           
	string getSystemName( ){ return strSystemName;};         
	string getHostname( ){ return strHostname;};        
	string getSystemFileName( ){ return strSystemFileName;};  
	string getAuthor( ){ return strAuthor;};
	string getDate( ){ return strDate;};
	int getPort ( ){ return iPort;}; 
	string getPortAsString() { return iqrUtils::int2string(iPort); }

	int getCyclesPerSecond ( ){ return iCyclesPerSecond;}; 
	string getCyclesPerSecondAsString() { return iqrUtils::int2string(iCyclesPerSecond); }
	
	bool getSyncPlots ( ){ return bSyncPlots;}; 

	int getStatus ( ){ return iSystemStatus;}; 

	string getNotes() { return strNotes; };
	void setNotes( string _strNotes ) { strNotes = _strNotes ; };


    private:
	string strSystemID;
	string strSystemName;
	string strHostname;
	string strSystemFileName;
	string strNotes;
	string strAuthor;
	string strDate;
	int iPort;
	int iSystemStatus;
	int iCyclesPerSecond;

	bool bSyncPlots;

	friend ostream &operator<<(ostream &stream, 
				   ClsInfoSystem clsInfoSystem);
    };
}

#endif

/// Local Variables: 
/// mode: c++
/// End: 

