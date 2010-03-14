/****************************************************************************
 ** $Filename: ClsInfoGroup.h
 ** $Header: /home/iqrdev/Code/Common/Parser/ClsInfoGroup.h,v 1.1 2003/01/29 12:04:46 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Mon Oct 22 11:56:14 2001
 ** $Date: 2003/01/29 12:04:46 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSINFOGROUP_H
#define CLSINFOGROUP_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>

#include <ClsParameter.h>
#include <ClsSysFileNode.h>
#include "../Helper/iqrUtils.h"

namespace iqrcommon {
    
    class ClsInfoGroup {
    public:
	ClsInfoGroup(){ 
	    strPrcID = "";
	    strGroupID = "";
	    strGroupName = "";
	    iGroupStatus = 0;
	};

	ClsInfoGroup(string _strGroupID, string _strGroupName, string _strPrcID) : 
	    strGroupID(_strGroupID), strGroupName(_strGroupName), strPrcID(_strPrcID)  {
	    iGroupStatus = 0;
	};

	void setGroupSubNodes(ClsSysFileNode _clsSysFileNodeGroupSubNodes){
	    clsSysFileNodeGroupSubNodes = _clsSysFileNodeGroupSubNodes;
	};

	ClsSysFileNode getSysFileNodeGroupSubNodes(){
	    return clsSysFileNodeGroupSubNodes;
	};

	void setGroupID( string _strGroupID){ strGroupID = _strGroupID;};
	string getGroupID() { return strGroupID; };

	void setGroupName(string strName) { strGroupName = strName; };
	string getGroupName() { return strGroupName; };
	

	void setStatus ( int _iGroupStatus ){ iGroupStatus = _iGroupStatus;}; 
        int getStatus ( ){ return iGroupStatus;}; 

	string getProcessID() { return strPrcID; };
	void setProcessID( string _strPrcID ) { strPrcID = _strPrcID ; };

	string getNotes() { return strNotes; };
	void setNotes( string _strNotes ) { strNotes = _strNotes ; };


    private:
	string strGroupID;
	string strGroupName;
	int iGroupStatus;
    
	ClsSysFileNode clsSysFileNodeGroupSubNodes;

	string strPrcID;
	string strNotes;
	
//-------------------

	friend ostream &operator<<(ostream &stream, ClsInfoGroup clsInfoGroup);
    };
}

#endif /* CLSINFOGROUP_H */


/// Local Variables: 
/// mode: c++
/// End: 
