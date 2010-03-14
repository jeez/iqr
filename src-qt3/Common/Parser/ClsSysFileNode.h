#ifndef CLSPRCSYSTEMFILENODE
#define CLSPRCSYSTEMFILENODE

#include <string>
#include <list>
#include "ClsParameter.h"

// 05/02/02 [jmb] Deleted using namespace iqrprc, ClsParameter is now
// in iqrcommon.

// 05/02/02 [jmb] Placed system file parser classes into namespace
// iqrcommon.

// 18/02/2002 [ulysses] ClsParameter has moved to namespace iqrcommon ( s/iqrprc::ClsParameter/iqrcommon::ClsParameter )
using namespace std;
namespace iqrcommon {
    
    class ClsSysFileNode
    {
    public:	
	ClsSysFileNode(string _strName) {
	    strName = _strName;
	};
	
	ClsSysFileNode() { };
	
	void setName( string _strName) {
	    strName = _strName;
	}
	
	string getName() {
	    return strName;
	}
	
	void addParameter(iqrcommon_old::ClsParameter _clsParameter) { 
	    lstParameter.push_back(_clsParameter);
	};
	
	iqrcommon_old::ClsParameter popParameter() {
	    iqrcommon_old::ClsParameter clsParameter = lstParameter.front();
	    lstParameter.pop_front();
	    return clsParameter;
	};
	
	int countParameters() {
	    int iNrParameters = lstParameter.size();
	    return iNrParameters;
	};
	
	void addNode(ClsSysFileNode _SysNode) {
	    lstNodes.push_back(_SysNode);
	};
  
	ClsSysFileNode  popNode() {
	    ClsSysFileNode SysNode;
	    if(lstNodes.size()>0){
		SysNode = lstNodes.front();
		lstNodes.pop_front();
	    }
	    return SysNode;
	};
	
	int countNodes() {
	    int iNrLists = lstNodes.size();
	    return iNrLists;
	};
	
    private:
	list<iqrcommon_old::ClsParameter> lstParameter;
	list<ClsSysFileNode> lstNodes;
	string strName;
    };
}

#endif

/// Local Variables: 
/// mode: c++
/// compile-command: "make -f Makefile.testSysNode"
/// End: 

