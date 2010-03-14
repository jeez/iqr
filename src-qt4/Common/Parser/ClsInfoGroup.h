/****************************************************************************
 ** $Filename: ClsInfoGroup.h
 **
 ** $Author: Ulysses Bernardet
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
	    strColor = "#FFFFFF";
	};

	ClsInfoGroup(string _strGroupID, string _strGroupName, string _strPrcID) : 
	    strGroupID(_strGroupID), strGroupName(_strGroupName), strPrcID(_strPrcID)  {
	    iGroupStatus = 0;
	    strColor = "#FFFFFF";
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

	string getColor() { 
	    if(strColor.length()<=0){
		return "#FFFFFF";
	    } 
	    return strColor; 
	};
	void setColor( string _strColor ) { strColor = _strColor ; };

    private:
	string strGroupID;
	string strGroupName;
	int iGroupStatus;
    
	ClsSysFileNode clsSysFileNodeGroupSubNodes;

	string strPrcID;
	string strNotes;
	string strColor;


	friend ostream &operator<<(ostream &stream, ClsInfoGroup clsInfoGroup);
    };
}

#endif /* CLSINFOGROUP_H */


/// Local Variables: 
/// mode: c++
/// End: 
