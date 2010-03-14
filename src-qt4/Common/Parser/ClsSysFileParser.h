#ifndef ClsSysFileParser_h_h
#define ClsSysFileParser_h_h


//#define XERCES23s


#include <util/XMLString.hpp>
#include <framework/LocalFileInputSource.hpp>
#include <map>
#include "ClsBaseParser.h"
#include "ClsSysFileNode.h"
#include "ClsSysFileParserException.h"
#include "ClsInfoSystem.h"
#include "ClsInfoPrc.h"
#include "ClsInfoGroup.h"
#include "ClsInfoConnection.h"
#include "ClsInfoDiagramLine.h"
#include "ClsInfoDiagramIcon.h"


using namespace xercesc;  

namespace iqrcommon {

    class ClsSysFileParser : public ClsBaseParser {
    public:
	ClsSysFileParser( );
	ClsSysFileParser(bool bAcceptUninitialized );
	void setInputBuffer(string _strBuffer);
	void parseBuffer(bool bValidate);
	void parseFragment(string strCont, bool bValidate);
	void XMLPlatformUtilsInitialize();
	void XMLPlatformUtilsTerminate();
	
	/* system related infomation */
	ClsInfoSystem getSystemInfo();

	/* process related information */
	void createPrcDOMTree(string strPrcID);
	
	list<ClsInfoPrc> getListProcesses(); //NEW
	list<ClsInfoPrc> getListClipboardProcesses(); //NEW
	
	list<string> getListReceiverProcesses( );
	list<string> getListSenderProcesses( );
	ClsInfoPrc getProcessInfo(string _strProcessID );
	
	/* group related information */
	int getLocalGroupCount( );
	string getLocalGroupName(int iIndex);
	string getLocalGroupID(int iIndex);
	bool isLocalGroup(string strGroupID);
	string getGroupParent(string _strGroupID);
	string getGroupName( string strGroupID);
	
	// return string list with the ids of specific groups
	list<ClsInfoGroup> getListLocalGroups( );

	list<ClsInfoGroup> getListClipboardGroups( );

	//     list<string> getListMirrorGroups();
	list<ClsInfoGroup> getListMirrorGroups();
	//---------------------------------------------------
	
	ClsSysFileNode getGroupTopologyParameter(string _strGroupID);
	
	ClsSysFileNode getGroupNeuronParameter(string _strGroupID);
	
	/* connection related information */
	/* return string list with the ids of specific connections */
	list<ClsInfoConnection> getListConnection( ); // which includes mirror groups
	list<ClsInfoConnection> getListClipboardConnections( ); // which includes mirror groups
	list<ClsInfoConnection> getListConnectionFromRemote( );
	
	list<ClsInfoConnection> getListConnectionToRemote( );
	
	list<ClsInfoConnection> getListConnectionLocal( ); // which includes mirror groups
	//---------------------------------------------------
	
	ClsSysFileNode getConnectionSubNodes(DOMNodeList *dnlstTop, string _strConnectionID);
	ClsSysFileNode getConnectionSubNodes(DOMNode *dnConnection);

	ClsSysFileNode getGroupSubNodes(DOMNodeList *dnlstTop, string _strGroupID);
	ClsSysFileNode getGroupSubNodes(DOMNode *dnGroup);
		

	map<string,string> getMapNotes() {return mapNotes;};
	map<string, ClsInfoDiagramLine> getMapDiagramLine() {return mapDiagramLine;};
	map<string,ClsInfoDiagramIcon> getMapDiagramIcon() {return mapDiagramIcon;};


	// @@@ 31/10/01 [jmb] Function names changed to getNotes* from
	string getNotesSystem(string strID);
	string getNotesProcess(string strProcessID);
	string getNotesConnection(string strConnectionID);
	string getNotesGroup(string strGroupID);
	string getNotesNeuron(string strGroupID);
	string getNotesSynapse(string strConnectionID);
	string getNotesModule(string strProcessID);
    
      DOMDocument* getDOMDocSystemFile() {
	      return ddocSystemFile;
	 };
	 
    private:
	bool bParserInitialized;
	bool bParserBufferSet;
	bool bParserBufferParsed;
	
	string getNotesNode(string strNodeName, string strNodeID);
	string getNotesNode_WorkingButSlow(string strNodeName, string strNodeID);
	string getNotesNode_old(string strNodeName, string strNodeID);
	string getNotesNode(DOMNode *dnRoot);
	
	void buildNotesCache(DOMDocument* ddocRoot);
	void buildDiagramLineCache(DOMDocument* ddocRoot);
	void buildDiagramIconCache(DOMDocument* ddocRoot);

	ClsSysFileNode DomNode2SysNode(DOMNode *dnTop, int iCount,string strSep);
	string findAncestorID( DOMNode *dn);
	
	const char*  gXMLInMemBuf;
	DOMDocument *ddocSystemFile;
	DOMDocument *ddocClipboard;
	DOMDocument *ddocLocalPrc;
	
	DOMNodeList *dnlstSystemGroups;
	DOMNodeList *dnlstSystemConnections;
	DOMNodeList *dnlstLocalPrcGroups;
	string strPrcID;


	DOMNodeList *dnlstClipboardConnections;
	DOMNodeList *dnlstClipboardProcesses;
	DOMNodeList *dnlstClipboardGroups;

	map<string,string> mapNotes;
	map<string, ClsInfoDiagramLine> mapDiagramLine;
	map<string,ClsInfoDiagramIcon> mapDiagramIcon;

    };
}

#endif

/// Local Variables: 
/// mode: c++
/// End: 




