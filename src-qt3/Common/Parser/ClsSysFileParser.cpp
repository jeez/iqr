/****************************************************************************
 ** $Filename: ClsSysFileParser.cpp
 ** $Header: /home/ulysses/Code/iqr421_Redesign/Common/SysFileParser/ClsSysFileParser.cpp,v 0.10 2001/10/24 13:12:15 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sat Jan 13 00:43:21 2001
 ** $Date: 2001/10/24 13:12:15 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <cstdio>
#include "../Helper/tagLibrary.hpp"
#include "ClsSysFileParser.h"
#include "ClsMyEntityResolver.h"
#include "ClsMyErrorHandler.h"
#include "ClsParameter.h"
 
//#define DEBUG_CLSSYSFILEPARSER

list<iqrcommon_old::ClsParameter> 
DomNode2List(DOMNode *dnTop, int iCount, string strSep){
    using iqrcommon_old::ClsParameter;

    iCount ++;
    strSep += "\t";
    string strNodeName = XMLString::transcode(dnTop->getNodeName());
    //  cout << "DomNode2List: " << strSep << iCount << " " << strNodeName  << endl;

    iqrcommon_old::ClsParameter clsParameter;
    list<iqrcommon_old::ClsParameter> lstParam;

    // node and it's attributes ++++++++++++++++++++
    iqrcommon_old::ClsParameter clsParameterChild;
    list<iqrcommon_old::ClsParameter> lstAtt; // list with attributes
    DOMNamedNodeMap *dnnmapAtt =  dnTop->getAttributes();
    for(unsigned int ii=0; ii<dnnmapAtt->getLength();ii++){
	string strAttName = XMLString::transcode(dnnmapAtt->item(ii)->getNodeName());
	string strAttValue = XMLString::transcode(dnnmapAtt->item(ii)->getNodeValue());
	//    cout << "DomNode2List: " << strSep << iCount << " " << "AttName: " << strAttName << ", AttValue: " << strAttValue << endl; 
	iqrcommon_old::ClsParameter clsParameterAtt;
	clsParameterAtt.setParameter(strAttName, strAttValue);
	lstAtt.push_back(clsParameterAtt);
    }
    clsParameterChild.setParameter(strNodeName, lstAtt);
    lstParam.push_back(clsParameterChild);
    // +++++++++++++++++++++++++++++++++++++++++++++

//###    DOMNodeList *dnlst = dnTop->getChildNodes();
//###     for (unsigned int ii = 0; ii < dnlst->getLength(); ii++){ // level 0
//### 	DOMNode *dnChild =  dnlst->item(ii);
    DOMNode *dnChild = dnTop->getFirstChild();
    while(dnChild!=NULL){
	if(dnChild->getNodeType()==DOMNode::ELEMENT_NODE){
	    //      list<ClsParameter> lstAtt;
	    clsParameterChild.setParameter(strNodeName, DomNode2List(dnChild, iCount, strSep));
	    lstParam.push_back(clsParameterChild);
	}
	dnChild = dnChild->getNextSibling();
    }
    

    //  clsParameter.setParameter(strNodeName, lstParam); //????

    //  return clsParameter;
    return lstParam;
}


// ---------------------------------------------------------------------------
//  Forward references
// ---------------------------------------------------------------------------
//ostream& operator<<(ostream& target, const DOMString& toWrite);


/** 
 * Just a simple, does-not-much constructor.
 * .
 */
iqrcommon::ClsSysFileParser::ClsSysFileParser( ) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::ClsSysFileParser" << endl;
#endif
    bParserInitialized = false;
    bParserBufferSet = false;
    bParserBufferParsed = false;
     
};

iqrcommon::ClsSysFileParser::ClsSysFileParser(bool /*bAcceptUninitialized*/ ) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::ClsSysFileParser(bool bAcceptUninitialized)" << endl;
#endif
    bParserInitialized = true;
    bParserBufferSet = false;
    bParserBufferParsed = false;

};


/** 
 * Set the input buffer for the parser. 
 * ClsSysFileParser does not read anything from any where, but can only handle a cml file that
 * comes in through this buffer. Therefore this function has to be called before anything else is done.
 * @param _strBuffer the xml file contents
 */
void iqrcommon::ClsSysFileParser::setInputBuffer(string _strBuffer) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::setInputBuffer" << endl;
#endif
    gXMLInMemBuf = _strBuffer.c_str();
    bParserBufferSet = true;
};



void iqrcommon::ClsSysFileParser::XMLPlatformUtilsInitialize(){
    try {
	XMLPlatformUtils::Initialize();
    }
  
    catch(const XMLException& toCatch) {
	cerr << "Error during Xerces-c Initialization.\n"
	     << "  Exception message:"
//	     << DOMString(toCatch.getMessage()) << endl;
	     << toCatch.getMessage() << endl;
	//      return 1;
	bParserInitialized = false;
    }
    bParserInitialized = true;
};


/** 
 * Parse the buffer
 * This parses the contents of the buffer set via setInputBuffer(string _strBuffer)
 */
//void ClsSysFileParser::parseBuffer(string strProcessName, string _strPrcID, string strPrcType)  {
void iqrcommon::ClsSysFileParser::parseBuffer(bool bValidate)  {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::parseBuffer()" << endl;
#endif

    if(!bParserBufferSet){
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::BUFFER_NOT_SET);
	throw clsSysFileParserException;
    }

    if(!bParserInitialized){
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSER_NOT_INITIALIZED);
	throw clsSysFileParserException;
    }
    

    static bool gDoNamespaces = false;
    
/* NEW STUFF */
    MyResolver mr;
    

    XercesDOMParser* parser = new XercesDOMParser();
    parser->setEntityResolver(&mr);
    
//    bValidate = true;
    if(bValidate){
	parser->setValidationScheme(XercesDOMParser::Val_Always); 
    } else {
	parser->setValidationScheme(XercesDOMParser::Val_Never); 
    }
  
    parser->setDoNamespaces(gDoNamespaces);

    /*
    ErrorHandler *errReporter = new DOMTreeErrorReporter();
    parser->setErrorHandler(errReporter);
    */

    /*
    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);
    */


    ClsMyErrorHandler clsMyErrorHandler;
    parser->setErrorHandler(&clsMyErrorHandler);



    //  parser->setCreateEntityReferenceNodes(gDoCreate);  // not needed?
    //  parser->setToCreateXMLDeclTypeNode(true); // not needed?

    /*  Parse the XML file, catching any XML exceptions that might propogate out of it.*/


    bool errorsOccured = false;
    try {
	static const char*  gMemBufId = "System";
	MemBufInputSource* memBufIS = new MemBufInputSource ( (const XMLByte*)gXMLInMemBuf, strlen(gXMLInMemBuf), gMemBufId, false);
	parser->parse(*memBufIS);
    } catch (const XMLException& e) { 
	cerr << "An error occured during parsing (XMLException)\n   NMessage: " <<  XMLString::transcode(e.getMessage()) << endl;
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR,  XMLString::transcode(e.getMessage()));
	throw clsSysFileParserException;
	errorsOccured = true;
    } catch (const DOMException& e) {
	cerr << "An error occured during parsing (DOMException)\n   DMessage: " << XMLString::transcode(e.msg) << endl;
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR, XMLString::transcode(e.msg));
	throw clsSysFileParserException;
	errorsOccured = true;
/*
    } catch (const SAXException& e) {
	cerr << "An error occured during parsing (SAXException)\n   DMessage: " <<  XMLString::transcode(e.getMessage()) << endl;
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR,  XMLString::transcode(e.getMessage()));
	throw clsSysFileParserException;
	errorsOccured = true;
*/
    } catch (const ClsSysFileParserException& e) { 
	throw e;
	errorsOccured = true;
    } catch (...) {
	cerr << "An error occured during parsing\n " << endl;
	errorsOccured = true;
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR, "unknown");
	throw clsSysFileParserException;
    }


  
    /* If the parse was successful, output the document data from the DOM tree */
    if (!errorsOccured) {
	DOMNode *dnSystem;	      
//	DOMNode *dnLocalPrc;	
//	DOMNode *dnLocalPrcGroup;     
    
	DOMNodeList *dnlstTemp;
//	DOMNode *dnTemp;
    
	//    dnlstLocalPrcGroups = new DOMNodeList();
	//    bool fLoop = true;

//	  ddocSystemFile = parser->getDocument();
	ddocSystemFile = parser->getDocument();

//cout << "ddocSystemFile NULL?: " << (ddocSystemFile == NULL ? "NULL" : "Not Null ") << endl;

	//    DOMNode *doc = parser->getDocument();
    
	//=================================================================
	// first we have to find the top System node
	/* the new way: use "getElementsByTagName" */
	dnlstTemp = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::SystemTag()));

	//dnlstTemp = ddocSystemFile->getElementsByTagName("System");
	/*this should throw an exception if system is not found! */
	if ( dnlstTemp->getLength() == 1){   
	    dnSystem = dnlstTemp->item(0);
	} else if ( dnlstTemp->getLength() < 1)	{
	    ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::NO_SYSTEM_NODE);
	    throw clsSysFileParserException;
	}
	
	//=================================================================


	//=================================================================
	// For later use, we create here DOMNodeList for all the groups and connections in the file
	dnlstSystemGroups = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::GroupTag()));
// 	  dnlstSystemGroups = ddocSystemFile->getElementsByTagName("Group");
//	dnlstSystemConnectionsTo = ddocSystemFile->getElementsByTagName("ConnectionTo");
	dnlstSystemConnections = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::ConnectionTag()));
// 	  dnlstSystemConnections = ddocSystemFile->getElementsByTagName("Connection");
	//=================================================================

	/* we build a cache for the notes, which should be much faster, than searching for the 
	   notes at each call... */
	buildNotesCache(ddocSystemFile); 
	buildDiagramLineCache(ddocSystemFile);
	buildDiagramIconCache(ddocSystemFile);


//***********************************************
	bParserBufferParsed = true;
    }

/*    
    delete errHandler;
*/

//      //  Delete the parser itself.  Must be done prior to calling Terminate, below.

//     delete parser;
    
//      // And call the termination method
//      XMLPlatformUtils::Terminate();


    
};


void iqrcommon::ClsSysFileParser::parseFragment(string strCont, bool bValidate)  {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::parseFragment(string strCont, bool bValidate)" << endl;
#endif

    gXMLInMemBuf = strCont.c_str();
    bParserBufferSet = true;

    if(!bParserBufferSet){
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::BUFFER_NOT_SET);
	throw clsSysFileParserException;
    }

    if(!bParserInitialized){
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSER_NOT_INITIALIZED);
	throw clsSysFileParserException;
    }
    

    static bool gDoNamespaces = false;
    
    XercesDOMParser* parser = new XercesDOMParser();
    
    
    if(bValidate){
	parser->setValidationScheme(XercesDOMParser::Val_Always); 
    } else {
	parser->setValidationScheme(XercesDOMParser::Val_Never); 
    }
  
    parser->setDoNamespaces(gDoNamespaces);

/*
    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);
*/

    ClsMyErrorHandler clsMyErrorHandler;
    parser->setErrorHandler(&clsMyErrorHandler);

    bool errorsOccured = false;
    try {
	static const char*  gMemBufId = "System";
	MemBufInputSource* memBufIS = new MemBufInputSource ( (const XMLByte*)gXMLInMemBuf, strlen(gXMLInMemBuf), gMemBufId, false);
	parser->parse(*memBufIS);
    } catch (const XMLException& e) { 
	cerr << "An error occured during parsing (XMLException)\n   NMessage: " <<  XMLString::transcode(e.getMessage()) << endl;
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR,  XMLString::transcode(e.getMessage()));
	throw clsSysFileParserException;
	errorsOccured = true;
    } catch (const DOMException& e) {
	cerr << "An error occured during parsing (DOMException)\n   DMessage: " << XMLString::transcode(e.msg) << endl;
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR, XMLString::transcode(e.msg));
	throw clsSysFileParserException;
	errorsOccured = true;
/*
    } catch (const SAXException& e) {
	cerr << "An error occured during parsing (SAXException)\n   DMessage: " <<  XMLString::transcode(e.getMessage()) << endl;
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR,  XMLString::transcode(e.getMessage()));
	throw clsSysFileParserException;
	errorsOccured = true;
*/
    } catch (const ClsSysFileParserException& e) { 
	throw e;
	errorsOccured = true;
    } catch (...) {
	cerr << "An error occured during parsing\n " << endl;
	errorsOccured = true;
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR, "unknown");
	throw clsSysFileParserException;
    }

    if (!errorsOccured) {
	ddocClipboard = parser->getDocument();
	
	dnlstClipboardProcesses = ddocClipboard->getElementsByTagName(XMLString::transcode(ClsTagLibrary::ProcessTag()));
	dnlstClipboardConnections = ddocClipboard->getElementsByTagName(XMLString::transcode(ClsTagLibrary::ConnectionTag()));
	dnlstClipboardGroups = ddocClipboard->getElementsByTagName(XMLString::transcode(ClsTagLibrary::GroupTag()));

//	cout << "dnlstClipboardProcesses->getLength(): " << dnlstClipboardProcesses->getLength() << endl;
//	cout << "dnlstClipboardConnections->getLength(): " << dnlstClipboardConnections->getLength() << endl;
//	cout << "dnlstClipboardGroups->getLength(): " << dnlstClipboardGroups->getLength() << endl;	


	buildNotesCache(ddocClipboard); 
	buildDiagramLineCache(ddocClipboard);
	buildDiagramIconCache(ddocClipboard);

	bParserBufferParsed = true;
    }

/*    
    delete errHandler;
*/
};


/** 
 * Build the DOM structure of a specified Prc. 
 * This function can only be called after parseBuffer()
 * @param _strPrcID ID of the local Prc.
 */
void iqrcommon::ClsSysFileParser::createPrcDOMTree(string _strPrcID)  {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "createPrcDOMTree(string _strPrcID)" << endl;
#endif

    if(!bParserBufferParsed){
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::BUFFER_NOT_PARSED);
	throw clsSysFileParserException;

    }
    
    strPrcID = _strPrcID;
    DOMNode *dnLocalPrc = NULL; 
     
    
	    
    //=================================================================
    // Now, dnTemp points to System. Next step is to find the Node that matches the locals Prcs ID
//    cout << "We are looking for the local id: " << strPrcID << endl;
    DOMNodeList *dnlstTemp = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::ProcessTag()));
//      DOMNodeList *dnlstTemp = ddocSystemFile->getElementsByTagName("Process");
    unsigned int ii = 0;
    int iFound = 0;
    while( ii< dnlstTemp->getLength()){
	DOMNode *dnTemp = dnlstTemp->item(ii);
//	DOMNamedNodeMap *dnamdnmp = dnTemp->getAttributes();
//	  DOMString strIDTemp = dnTemp->getAttributes()->getNamedItem("id")->getNodeValue();
	string strIDTemp = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
// 	  string strIDTemp = getAttributeValue(dnTemp, "id", true);
//	  if(strIDTemp.equals(strPrcID.c_str())){
	if(!strIDTemp.compare(strPrcID)){
//	    cout << "\tFound local Node, Node ID: " << strIDTemp << endl;
	    dnLocalPrc = dnTemp;
//		ii= dnlstTemp->getLength();
	    iFound = -1;
	    break;
	} else {
	    iFound = 0;
	}
	ii++;
    }


     
    if(!iFound) {
ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::SYSTEM_ID_NOT_FOUND);
	throw clsSysFileParserException;
    }

 
    //=================================================================	 
     
    //=================================================================	 
    // now, we create a DOMNodeList, that consits only of the Groups of the Current Prc
    // sadly, this seems to be rather more complicated...
     

// original     DOMImplementation *impl;                // create am implemenation
// original     ddocLocalPrc = impl->createDocument(
// original 	0,                    // root element namespace URI.
// original 	XMLString::transcode(ClsTagLibrary::ProcessTag()),            // root element name 
// original 	DOMDocumentType*);  // document type object (DTD).


//    XMLCh* tempStr[100];
//    const char charT[] = "Range";
//    XMLString::transcode(charT, tempStr, 99);


    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));


//    XMLString::transcode(ClsTagLibrary::ProcessTag(), tempStr, 99);
    ddocLocalPrc = impl->createDocument(0, XMLString::transcode(ClsTagLibrary::ProcessTag()), 0);
    

    DOMElement *rootElem = ddocLocalPrc->getDocumentElement();  // returns a reference to the root element

    DOMNode *nn = ddocLocalPrc->importNode(dnLocalPrc, true);

    rootElem->appendChild(nn);

//###    dnlstLocalPrcGroups = DOMNodeList((ddocLocalPrc->getElementsByTagName(XMLString::transcode(ClsTagLibrary::GroupTag()))));


    dnlstLocalPrcGroups = ddocLocalPrc->getElementsByTagName(XMLString::transcode(ClsTagLibrary::GroupTag()));

//      dnlstLocalPrcGroups = DOMNodeList((ddocLocalPrc->getElementsByTagName("Group")));
//    DOMNode *dd = dnLocalPrc->getFirstChild();
    //=================================================================	 
     
//***********************************************
};

/** 
 * Terminate the XMLPlatformUtils
 * To be called after the last call to createPrcDOMTree(string strProcessName, string _strPrcID, string strPrcType)
 */
void iqrcommon::ClsSysFileParser::XMLPlatformUtilsTerminate(){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::XMLPlatformUtilsTerminate()" << endl;
#endif
    XMLPlatformUtils::Terminate();
    bParserInitialized = false;
};

//      delete errReporter;

//      //  Delete the parser itself.  Must be done prior to calling Terminate, below.
//      delete parser;

//      // And call the termination method
//      XMLPlatformUtils::Terminate();

//  };


/** 
 * Get the number of groups in the local Prc.
 * @return Number of groups in the local Prc.
 */
int iqrcommon::ClsSysFileParser::getLocalGroupCount( )
{
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupCount( )" << endl;
#endif
    int iReturn = dnlstLocalPrcGroups->getLength() ; 
//    cout << "@@ " << iReturn << endl;
    return iReturn;
};

list<iqrcommon::ClsInfoGroup> iqrcommon::ClsSysFileParser::getListClipboardGroups() {


    list<ClsInfoGroup> lstClipboardGroups;

    ClsInfoGroup clsInfoGroup;
    for (unsigned int ii=0; ii< dnlstClipboardGroups->getLength(); ii++){
	DOMNode *dnGroup =  dnlstClipboardGroups->item(ii);

	string strGroupID = getAttributeValue(dnGroup, ClsTagLibrary::IDTag(), true);
	string strGroupName = getAttributeValue(dnGroup, ClsTagLibrary::NameTag(), true);

	clsInfoGroup.setGroupID(strGroupID);
	clsInfoGroup.setGroupName(strGroupName);

	string strNotes = getNotesNode( dnGroup );
	clsInfoGroup.setNotes(strNotes);

	ClsSysFileNode clsSysFileNode = getGroupSubNodes(dnGroup);
	clsInfoGroup.setGroupSubNodes(clsSysFileNode);


	lstClipboardGroups.push_back( clsInfoGroup );
    }

    return lstClipboardGroups;
};


list<iqrcommon::ClsInfoGroup> iqrcommon::ClsSysFileParser::getListLocalGroups() {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListLocalGroups()" << endl;
#endif

    list<ClsInfoGroup> lstLocalGroups;
    ClsInfoGroup clsInfoGroup;
    for (unsigned int ii=0; ii< dnlstLocalPrcGroups->getLength(); ii++){
	DOMNode *dnGroup =  dnlstLocalPrcGroups->item(ii);

	string strGroupID = getAttributeValue(dnGroup, ClsTagLibrary::IDTag(), true);
	string strGroupName = getAttributeValue(dnGroup, ClsTagLibrary::NameTag(), true);
	

	clsInfoGroup.setGroupID(strGroupID);
	clsInfoGroup.setGroupName(strGroupName);

/*
	ClsSysFileNode clsSysFileNode0 = getGroupTopologyParameter( strGroupID );//clsInfoGroup.getGroupID());
	clsInfoGroup.setGroupTopologyParameter(clsSysFileNode0);

	ClsSysFileNode clsSysFileNode1 = getGroupNeuronParameter( strGroupID );//clsInfoGroup.getGroupID());
	clsInfoGroup.setGroupNeuronParameter(clsSysFileNode1);
*/
	string strNotes = getNotesGroup( clsInfoGroup.getGroupID());
	clsInfoGroup.setNotes(strNotes);

	
	ClsSysFileNode clsSysFileNode = getGroupSubNodes(dnGroup);
	clsInfoGroup.setGroupSubNodes(clsSysFileNode);
	
	  

	lstLocalGroups.push_back( clsInfoGroup );
    }
    return lstLocalGroups;
};


/** 
 * Return the number of mirror groups with in a Prc, i.e. groups that are in a remote Prc, but have a connection to the local Prc
 * Detailed description.
 * @return Description of int returned.
 */
list<iqrcommon::ClsInfoGroup> iqrcommon::ClsSysFileParser::getListMirrorGroups()
{
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getMirrorGroupCount()"<< endl;
#endif

//     list<string> lstIDMirrorGroups;
    list<ClsInfoGroup> lstIDMirrorGroups;

    /* loop through all connections and find the ones that:
       - connect to a local group &&
       - come from a remote Prc
    */

    for(unsigned int ii=0; ii< dnlstSystemConnections->getLength(); ii++){
	DOMNode *dnTemp = dnlstSystemConnections->item(ii);
//	  DOMNamedNodeMap *dnamdnmp = dnTemp->getAttributes();
	string strConnectionSource = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
	string strConnectionTarget = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
	if(!isLocalGroup(strConnectionSource) & isLocalGroup(strConnectionTarget)){
	    ClsInfoGroup clsInfoGroup;
	    clsInfoGroup.setGroupID(strConnectionSource);
	    clsInfoGroup.setGroupName(getGroupName(strConnectionSource));

	    /*
	    ClsSysFileNode clsSysFileNode0 = getGroupTopologyParameter( clsInfoGroup.getGroupID());
	    clsInfoGroup.setGroupTopologyParameter(clsSysFileNode0);

	    ClsSysFileNode clsSysFileNode1 = getGroupNeuronParameter( clsInfoGroup.getGroupID());
	    clsInfoGroup.setGroupNeuronParameter(clsSysFileNode1);
	    */

	    string strNotes = getNotesGroup( clsInfoGroup.getGroupID());
	    clsInfoGroup.setNotes(strNotes);

	    ClsSysFileNode clsSysFileNode = getGroupSubNodes(dnTemp);
	    clsInfoGroup.setGroupSubNodes(clsSysFileNode);


	    lstIDMirrorGroups.push_back(clsInfoGroup);
	}
    }
    return lstIDMirrorGroups;
};


string iqrcommon::ClsSysFileParser::getGroupName( string strGroupID )
{
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupName( string strGroupID)" << endl;
#endif
    string strGroupName;
    DOMNode *dnTemp;
    string strIDTemp;
    for(unsigned int ii=0; ii< dnlstSystemGroups->getLength(); ii++){
	dnTemp = dnlstSystemGroups->item(ii);
	strIDTemp = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	if(!strIDTemp.compare(strGroupID)) {
	    strGroupName = getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true);
	    break;
	}
    }
    return strGroupName;
};



/** 
 * The the name of the ith group in the local Prc.
 * @return Name of the ith group in the local Prc.
 * @param iIndex index of the group, the name is looked up.
 */
string iqrcommon::ClsSysFileParser::getLocalGroupName(int iIndex)
{
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupName(int iIndex) iIndex: " << iIndex << endl;
#endif
    string strGroupName;
    if(iIndex > ((int)dnlstLocalPrcGroups->getLength() -1)){
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::OUT_OF_GROUP_RANGE);
	throw clsSysFileParserException;
    } else {
	DOMNode *dnGroup =  dnlstLocalPrcGroups->item(iIndex);
	DOMNamedNodeMap *dnmapTemp = dnGroup->getAttributes();

	for(unsigned int ii=0; ii<dnmapTemp->getLength(); ii++){
	    DOMNode *dnTemp = dnmapTemp->item(ii);
	    string strNodeName = XMLString::transcode(dnTemp->getNodeName());
//	  cout << strNodeName << endl;
//	  cout << dnTemp->getNodeValue() << endl;
	    if(!strNodeName.compare(ClsTagLibrary::NameTag())){
// 	       if(!strNodeName.compare("name")){
		strGroupName = XMLString::transcode(dnTemp->getNodeValue());
		break;
	    }
	}
    }
    return strGroupName;
};



/** 
 * The the ID of the ith group in the local Prc.
 * @return ID of the ith group in the local Prc.
 * @param iIndex index of the group, the ID is looked up.
 */
string iqrcommon::ClsSysFileParser::getLocalGroupID(int iIndex)
{
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupID(int iIndex)" << endl;
#endif
    string strGroupID;

    if(iIndex > ((int)dnlstLocalPrcGroups->getLength() -1)){
	ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::OUT_OF_GROUP_RANGE);
	throw clsSysFileParserException;
    } else {
	DOMNode *dnGroup =  dnlstLocalPrcGroups->item(iIndex);
	DOMNamedNodeMap *dnmapTemp = dnGroup->getAttributes();
      
	for(unsigned int ii=0; ii<dnmapTemp->getLength(); ii++){
	    DOMNode *dnTemp = dnmapTemp->item(ii);
	    string strNodeName = XMLString::transcode(dnTemp->getNodeName());
//	  cout << strNodeName << endl;
//	  cout << dnTemp->getNodeValue() << endl;
	    if(!strNodeName.compare(ClsTagLibrary::IDTag())){
		strGroupID = XMLString::transcode(dnTemp->getNodeValue());
		break;
	    }
	}
    }

    return strGroupID;
};


iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getGroupTopologyParameter(string _strGroupID) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupTopologyParameterNEW(string _strGroupID)"<< endl;
#endif
    ClsSysFileNode clsSysFileNode;

    for (unsigned int i1 = 0; i1 < dnlstSystemGroups->getLength(); i1++){
	DOMNode *dnGroup =  dnlstSystemGroups->item(i1);
	string strGroupID = getAttributeValue(dnGroup, ClsTagLibrary::IDTag(), true);
	if(!strGroupID.compare(_strGroupID)) {
	    DOMNode *dnGroupChild = dnGroup->getFirstChild();
	    while (dnGroupChild!=NULL){
		if (dnGroupChild->getNodeType() == DOMNode::ELEMENT_NODE){
		    string strTemp = XMLString::transcode(dnGroupChild->getNodeName());
		    if (!strTemp.compare(ClsTagLibrary::TopologyTag())) {
			string strTopology = XMLString::transcode(dnGroupChild->getNodeName());
			clsSysFileNode.setName(strTopology);
			clsSysFileNode = DomNode2SysNode(dnGroupChild, 0, "");
		    }
		}
		dnGroupChild = dnGroupChild->getNextSibling();
	    }
	}
    }
    return clsSysFileNode;
}

iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getGroupNeuronParameter(string _strGroupID) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupNeuronParameterNEW(string _strGroupID)"<< endl;
#endif
    ClsSysFileNode clsSysFileNode;

    for (unsigned int i1 = 0; i1 < dnlstSystemGroups->getLength(); i1++){
	DOMNode *dnGroup =  dnlstSystemGroups->item(i1);
	string strGroupID = getAttributeValue(dnGroup, ClsTagLibrary::IDTag(), true);
	if(!strGroupID.compare(_strGroupID)) {
	    DOMNode *dnGroupChild = dnGroup->getFirstChild();
	    while (dnGroupChild!=NULL){
		if (dnGroupChild->getNodeType() == DOMNode::ELEMENT_NODE){
		    string strTemp = XMLString::transcode(dnGroupChild->getNodeName());
		    if (!strTemp.compare(ClsTagLibrary::NeuronTag())) {
			string strNeuronType = XMLString::transcode(dnGroupChild->getNodeName());
			clsSysFileNode.setName(strNeuronType);
			clsSysFileNode = DomNode2SysNode(dnGroupChild, 0, "");
		    }
		}
		dnGroupChild = dnGroupChild->getNextSibling();
	    }
	}
    }
    return clsSysFileNode;
}


list<iqrcommon::ClsInfoConnection> iqrcommon::ClsSysFileParser::getListClipboardConnections( ) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListClipboardConnections()"<< endl;
#endif


    int iConnectionType = -1; 
    list<ClsInfoConnection> lstCLipboardConnections;


    ClsInfoConnection clsInfoConnection;
     
    for(unsigned int ii=0; ii< dnlstClipboardConnections->getLength(); ii++){
	DOMNode *dnTemp = dnlstClipboardConnections->item(ii);

	string strConnectionSource = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
	string strConnectionTarget =getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
	string strConnectionID = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	string strConnectionName = getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true);
	string strConnectionType = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTypeTag(), true); // 06/02/03 [ulysses] set the connection type
	clsInfoConnection.setConnectionSourceID(strConnectionSource);
	clsInfoConnection.setConnectionTargetID(strConnectionTarget);
	clsInfoConnection.setConnectionID(strConnectionID);
	clsInfoConnection.setConnectionName(strConnectionName);

	string strNotes = getNotesNode( dnTemp );
	clsInfoConnection.setNotes(strNotes);

	if(!strConnectionType.compare(ClsTagLibrary::ExcitatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_EXCITATORY;
	} 
	else if(!strConnectionType.compare(ClsTagLibrary::InhibitoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_INHIBITORY;
	}
	else if(!strConnectionType.compare(ClsTagLibrary::ModulatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_MODULATORY;
	}


	clsInfoConnection.setConnectionType(iConnectionType);  // 06/02/03 [ulysses] set the connection type

	ClsSysFileNode clsSysFileNode = getConnectionSubNodes(dnTemp);

	clsInfoConnection.setConnectionSubNodes(clsSysFileNode);

	lstCLipboardConnections.push_back(clsInfoConnection);
    }

    return lstCLipboardConnections;
};

list<iqrcommon::ClsInfoConnection> iqrcommon::ClsSysFileParser::getListConnection( ) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListConnection(string strGroupID)"<< endl;
#endif
    int iConnectionType = -1;  // 06/02/03 [ulysses] set the connection type
    list<ClsInfoConnection> lstIDConnection;
    ClsInfoConnection clsInfoConnection;

//    cout << "dnlstSystemConnections->getLength(): " << dnlstSystemConnections->getLength() << endl;

    for(unsigned int ii=0; ii< dnlstSystemConnections->getLength(); ii++){
//	cout << "ii: " << ii << endl;

	DOMNode *dnTemp = dnlstSystemConnections->item(ii);
	string strConnectionSource = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
	string strConnectionTarget = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
	string strConnectionID = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	string strConnectionName = getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true);
	string strConnectionType = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTypeTag(), true); // 06/02/03 [ulysses] set the connection type
	clsInfoConnection.setConnectionSourceID(strConnectionSource);
	clsInfoConnection.setConnectionTargetID(strConnectionTarget);
	clsInfoConnection.setConnectionID(strConnectionID);
	clsInfoConnection.setConnectionName(strConnectionName);

	string strNotes = getNotesConnection( strConnectionID );
	clsInfoConnection.setNotes(strNotes);


	if(!strConnectionType.compare(ClsTagLibrary::ExcitatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_EXCITATORY;
	} 
	else if(!strConnectionType.compare(ClsTagLibrary::InhibitoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_INHIBITORY;
	}
	else if(!strConnectionType.compare(ClsTagLibrary::ModulatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_MODULATORY;
	}
	// ------------------------------------------


	clsInfoConnection.setConnectionType(iConnectionType);  // 06/02/03 [ulysses] set the connection type

	ClsSysFileNode clsSysFileNode = getConnectionSubNodes(dnTemp);
	clsInfoConnection.setConnectionSubNodes(clsSysFileNode);

	lstIDConnection.push_back(clsInfoConnection);
    }
    return lstIDConnection;
};


//list<string> ClsSysFileParser::getListConnectionLocal( ) { // this list includes mirror groups
list<iqrcommon::ClsInfoConnection> iqrcommon::ClsSysFileParser::getListConnectionLocal( ) { // this list includes mirror groups
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListConnectionLocal(string strGroupID)"<< endl;
#endif
//     list<string> lstIDConnectionLocal;
    int iConnectionType = -1;  // 06/02/03 [ulysses] set the connection type
    list<ClsInfoConnection> lstIDConnectionLocal;
    ClsInfoConnection clsInfoConnection;

    for(unsigned int ii=0; ii< dnlstSystemConnections->getLength(); ii++){
	DOMNode *dnTemp = dnlstSystemConnections->item(ii);
	string strConnectionSource = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
	string strConnectionTarget =getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
	string strConnectionID = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	string strConnectionName = getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true);
	string strConnectionType = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTypeTag(), true); // 06/02/03 [ulysses] set the connection type

	string strNotes = getNotesConnection( strConnectionID );
	clsInfoConnection.setNotes(strNotes);

	// 06/02/03 [ulysses] set the connection type
	if(!strConnectionType.compare(ClsTagLibrary::ExcitatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_EXCITATORY;
	} 
	else if(!strConnectionType.compare(ClsTagLibrary::InhibitoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_INHIBITORY;
	}
	else if(!strConnectionType.compare(ClsTagLibrary::ModulatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_MODULATORY;
	}
	// ------------------------------------------
	     



// 	  string strConnectionSource = getAttributeValue(dnTemp, "source", true);
// 	  string strConnectionTarget =getAttributeValue(dnTemp, "target", true);
// 	  string strConnectionID = getAttributeValue(dnTemp, "id", true);

	  

	if(isLocalGroup(strConnectionTarget)){
	    clsInfoConnection.setConnectionSourceID(strConnectionSource);
	    clsInfoConnection.setConnectionTargetID(strConnectionTarget);
	    clsInfoConnection.setConnectionID(strConnectionID);
	    clsInfoConnection.setConnectionName(strConnectionName);
	    clsInfoConnection.setConnectionType(iConnectionType);  // 06/02/03 [ulysses] set the connection type
	    ClsSysFileNode clsSysFileNode = getConnectionSubNodes(dnTemp);
	    clsInfoConnection.setConnectionSubNodes(clsSysFileNode);
	    lstIDConnectionLocal.push_back(clsInfoConnection);
	}
    }
    return lstIDConnectionLocal;
};



list<iqrcommon::ClsInfoConnection> iqrcommon::ClsSysFileParser::getListConnectionFromRemote() {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListConnectionFromRemote()"<< endl;
#endif
    int iConnectionType = -1;  // 06/02/03 [ulysses] set the connection type
    list<ClsInfoConnection> lstIDConnectionFromRemote;
    ClsInfoConnection clsInfoConnection;

    for(unsigned int ii=0; ii< dnlstSystemConnections->getLength(); ii++){
	DOMNode *dnTemp = dnlstSystemConnections->item(ii);
	string strConnectionSource = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
	string strConnectionTarget =getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
	string strConnectionID = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	string strConnectionName = getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true);
	string strConnectionType = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTypeTag(), true); // 06/02/03 [ulysses] set the connection type
// 	  string strConnectionSource = getAttributeValue(dnTemp, "source", true);
// 	  string strConnectionTarget = getAttributeValue(dnTemp, "target", true);
// 	  string strConnectionID = getAttributeValue(dnTemp, "id", true);

	string strNotes = getNotesConnection( strConnectionID );
	clsInfoConnection.setNotes(strNotes);

	// 06/02/03 [ulysses] set the connection type
	if(!strConnectionType.compare(ClsTagLibrary::ExcitatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_EXCITATORY;
	} 
	else if(!strConnectionType.compare(ClsTagLibrary::InhibitoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_INHIBITORY;
	}
	else if(!strConnectionType.compare(ClsTagLibrary::ModulatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_MODULATORY;
	}
	// ------------------------------------------

	if(!isLocalGroup(strConnectionSource) && isLocalGroup(strConnectionTarget)){
	    clsInfoConnection.setConnectionSourceID(strConnectionSource);
	    clsInfoConnection.setConnectionTargetID(strConnectionTarget);
	    clsInfoConnection.setConnectionID(strConnectionID);
	    clsInfoConnection.setConnectionName(strConnectionName);
	    clsInfoConnection.setConnectionType(iConnectionType);  
	    ClsSysFileNode clsSysFileNode = getConnectionSubNodes(dnTemp);
	    clsInfoConnection.setConnectionSubNodes(clsSysFileNode);

	    lstIDConnectionFromRemote.push_back(clsInfoConnection);
	}
    }
    return lstIDConnectionFromRemote;
};


list<iqrcommon::ClsInfoConnection> iqrcommon::ClsSysFileParser::getListConnectionToRemote() {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListConnectionToRemote()"<< endl;
#endif
    int iConnectionType = -1; 
    list<ClsInfoConnection> lstIDConnectionToRemote;
    ClsInfoConnection clsInfoConnection;
    for(unsigned int ii=0; ii< dnlstSystemConnections->getLength(); ii++){
	DOMNode *dnTemp = dnlstSystemConnections->item(ii);
	string strConnectionSource = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
	string strConnectionTarget =getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
	string strConnectionID = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	string strConnectionName = getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true);
	string strConnectionType = getAttributeValue(dnTemp, ClsTagLibrary::ConnectionTypeTag(), true); 

	string strNotes = getNotesConnection( strConnectionID );
	clsInfoConnection.setNotes(strNotes);

	if(!strConnectionType.compare(ClsTagLibrary::ExcitatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_EXCITATORY;
	} 
	else if(!strConnectionType.compare(ClsTagLibrary::InhibitoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_INHIBITORY;
	}
	else if(!strConnectionType.compare(ClsTagLibrary::ModulatoryConnectionTag())) {
	    iConnectionType=ClsInfoConnection::CONN_MODULATORY;
	}
	// ------------------------------------------

	if(!isLocalGroup(strConnectionTarget) && isLocalGroup(strConnectionSource)){
	    clsInfoConnection.setConnectionSourceID(strConnectionSource);
	    clsInfoConnection.setConnectionTargetID(strConnectionTarget);
	    clsInfoConnection.setConnectionID(strConnectionID);
	    clsInfoConnection.setConnectionName(strConnectionName);
	    clsInfoConnection.setConnectionType(iConnectionType);  // 06/02/03 [ulysses] set the connection type
	    ClsSysFileNode clsSysFileNode = getConnectionSubNodes(dnTemp);
	    clsInfoConnection.setConnectionSubNodes(clsSysFileNode);

	    lstIDConnectionToRemote.push_back(clsInfoConnection);
	}
    }
    return lstIDConnectionToRemote;
};


/** 
 * This function return a list with all the attributes/values of a specified ConectionTo node.
 * @return List with all the attributes and names for a connection. Note that all values are stored as strings.
 * @param _strConnectionID ID of the connection the parameters of which we're searching .
 */
iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getConnectionSubNodes(DOMNodeList *dnlstTop, string _strConnectionID) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getConnectionSubNodes(DOMNodeList *dnlstTop, string _strConnectionID)"<< endl;
#endif


    ClsSysFileNode clsSysFileNode;
    for (unsigned int i1 = 0; i1 < dnlstTop->getLength(); i1++){
	DOMNode *dnConnection =  dnlstTop->item(i1);
	string strConnectionID = getAttributeValue(dnConnection, ClsTagLibrary::IDTag(), true);
	if(!strConnectionID.compare(_strConnectionID)) {
	    clsSysFileNode.setName(_strConnectionID);
	    clsSysFileNode = DomNode2SysNode(dnConnection, 0, "");
	}
    }
    return clsSysFileNode;
}

iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getConnectionSubNodes(DOMNode *dnConnection) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getConnectionSubNodes(DOMNode *dnConnectio)"<< endl;
#endif
    ClsSysFileNode clsSysFileNode;
    string strConnectionID = getAttributeValue(dnConnection, ClsTagLibrary::IDTag(), true);
    clsSysFileNode.setName(strConnectionID);
    clsSysFileNode = DomNode2SysNode(dnConnection, 0, "");
    return clsSysFileNode;
}



iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getGroupSubNodes(DOMNodeList *dnlstTop, string _strGroupID) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupSubNodes(DOMNodeList *dnlstTop, string _strGroupID)"<< endl;
#endif

    ClsSysFileNode clsSysFileNode;
    for (unsigned int i1 = 0; i1 < dnlstTop->getLength(); i1++){
	DOMNode *dnGroup =  dnlstTop->item(i1);
	string strGroupID = getAttributeValue(dnGroup, ClsTagLibrary::IDTag(), true);
	if(!strGroupID.compare(_strGroupID)) {
	    clsSysFileNode.setName(_strGroupID);
	    clsSysFileNode = DomNode2SysNode(dnGroup, 0, "");
	}
    }
    return clsSysFileNode;
}

iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getGroupSubNodes(DOMNode *dnGroup) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupSubNodes(DOMNode *dnGroup)"<< endl;
#endif

    ClsSysFileNode clsSysFileNode;
    string strGroupID = getAttributeValue(dnGroup, ClsTagLibrary::IDTag(), true);
    clsSysFileNode.setName(strGroupID);
    clsSysFileNode = DomNode2SysNode(dnGroup, 0, "");
    return clsSysFileNode;
}


bool iqrcommon::ClsSysFileParser::isLocalGroup(string _strGroupID) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::isLocalGroup(string _strGroupID)"<< endl;
#endif
    bool bLocal = false;

    for (unsigned int ii=0; ii< dnlstLocalPrcGroups->getLength(); ii++){
	DOMNode *dnGroup =  dnlstLocalPrcGroups->item(ii);
	string strGroupID = getAttributeValue(dnGroup, ClsTagLibrary::IDTag(), true);

	if(!strGroupID.compare(_strGroupID)){
	    bLocal = true;
	    break;
	}   
    }
    //  cout << (bLocal==true ? "true" : "false" ) << endl;
    return bLocal;
};


string iqrcommon::ClsSysFileParser::getGroupParent(string _strGroupID) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupParent(string _strGroupID)"<< endl;
#endif
    string strParentID;
    for(unsigned int ii=0; ii< dnlstSystemGroups->getLength(); ii++){
	DOMNode *dnTemp = dnlstSystemGroups->item(ii);
	string strGroupID = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	if(!strGroupID.compare(_strGroupID)) {
	    strParentID = getAttributeValue(dnTemp->getParentNode(), ClsTagLibrary::IDTag(), true);
	    break;
	}
    }

    return strParentID;
};


list<iqrcommon::ClsInfoPrc> iqrcommon::ClsSysFileParser::getListClipboardProcesses(){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "iqrcommon::ClsSysFileParser::getListClipboardProcesses()" << endl;
#endif
    
    list<ClsInfoPrc> lstInfoPrc;



    for(unsigned int ii=0; ii< dnlstClipboardProcesses->getLength(); ii++){
	DOMNode *dnTemp = dnlstClipboardProcesses->item(ii);
	ClsInfoPrc clsInfoPrcTemp;
	string strPrcID = XMLString::transcode(dnTemp->getAttributes()->getNamedItem(XMLString::transcode(ClsTagLibrary::IDTag()))->getNodeValue());
	clsInfoPrcTemp.setPrcID(strPrcID);
	getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	clsInfoPrcTemp.setProcessName(getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true));
	clsInfoPrcTemp.setHostname(getAttributeValue(dnTemp, ClsTagLibrary::HostnameTag(), true));
	clsInfoPrcTemp.setPort(getAttributeValue(dnTemp, ClsTagLibrary::PortTag(), false));
	clsInfoPrcTemp.setEnableModule(getAttributeValue(dnTemp, ClsTagLibrary::EnableModuleTag(), false));
	clsInfoPrcTemp.setPath(getAttributeValue(dnTemp, ClsTagLibrary::PathTag(), false));
	
	
	

	DOMNode *dnPrcChildTemp = dnTemp->getFirstChild();
	while(dnPrcChildTemp!=NULL){  
	    string strNodeName = XMLString::transcode(dnPrcChildTemp->getNodeName());
	    if(!strNodeName.compare(ClsTagLibrary::ModuleTag())){
		clsInfoPrcTemp.setHasModule(true); 
//		cout << "MODULE FOUND" << endl;
		string strModuleName = getAttributeValue(dnPrcChildTemp, ClsTagLibrary::NameTag(), true);
//		cout << "strModuleName: " << strModuleName << endl;
		ClsSysFileNode clsSysFileNode;
		clsSysFileNode.setName(ClsTagLibrary::ModuleTag());
		clsSysFileNode.addNode(DomNode2SysNode(dnPrcChildTemp, 0, ""));
		clsInfoPrcTemp.setProcessSubNodes(clsSysFileNode);
		
	    }
	    dnPrcChildTemp = dnPrcChildTemp->getNextSibling();
	}
//###------------------------



	string strNotes = getNotesNode( dnTemp );
	clsInfoPrcTemp.setNotes(strNotes);
	lstInfoPrc.push_back(clsInfoPrcTemp);
    }
    return lstInfoPrc;
}

list<iqrcommon::ClsInfoPrc>  iqrcommon::ClsSysFileParser::getListProcesses(){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListProcesses()"<< endl;
#endif

    list<ClsInfoPrc> lstInfoPrc;

    DOMNodeList *dnlstSystemProcesses = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::ProcessTag()));
    for(unsigned int ii=0; ii< dnlstSystemProcesses->getLength(); ii++){
	DOMNode *dnTemp = dnlstSystemProcesses->item(ii);
	ClsInfoPrc clsInfoPrcTemp;
	string strPrcID = XMLString::transcode(dnTemp->getAttributes()->getNamedItem(XMLString::transcode(ClsTagLibrary::IDTag()))->getNodeValue());
	clsInfoPrcTemp.setPrcID(strPrcID);
	getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	clsInfoPrcTemp.setProcessName(getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true));
	clsInfoPrcTemp.setHostname(getAttributeValue(dnTemp, ClsTagLibrary::HostnameTag(), true));
	clsInfoPrcTemp.setPort(getAttributeValue(dnTemp, ClsTagLibrary::PortTag(), false));
	clsInfoPrcTemp.setEnableModule(getAttributeValue(dnTemp, ClsTagLibrary::EnableModuleTag(), false));
	clsInfoPrcTemp.setPath(getAttributeValue(dnTemp, ClsTagLibrary::PathTag(), false));
	
	

	DOMNode *dnPrcChildTemp = dnTemp->getFirstChild();
	while(dnPrcChildTemp!=NULL){
	    string strNodeName = XMLString::transcode(dnPrcChildTemp->getNodeName());
	    if(!strNodeName.compare(ClsTagLibrary::ModuleTag())){
		clsInfoPrcTemp.setHasModule(true); 
		cout << "MODULE FOUND" << endl;
		
		string strModuleName = getAttributeValue(dnPrcChildTemp, ClsTagLibrary::NameTag(), true);
		cout << "strModuleName: " << strModuleName << endl;
		
		ClsSysFileNode clsSysFileNode;
		clsSysFileNode.setName(ClsTagLibrary::ModuleTag());
		clsSysFileNode.addNode(DomNode2SysNode(dnPrcChildTemp, 0, ""));
		clsInfoPrcTemp.setProcessSubNodes(clsSysFileNode);
		
	    }
	    dnPrcChildTemp = dnPrcChildTemp->getNextSibling();
	}
//###-----------------------
 
	string strNotes = getNotesProcess( strPrcID );
	clsInfoPrcTemp.setNotes(strNotes);
	lstInfoPrc.push_back(clsInfoPrcTemp);

    }
    return lstInfoPrc;
};


list<string> iqrcommon::ClsSysFileParser::getListReceiverProcesses( ){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListReceiverProcesses( )"<< endl;
#endif

    list<string> lstReceiverProcesses;

//     list<string> lstConnectionToRemote= getListConnectionToRemote();
    list<ClsInfoConnection> lstConnectionToRemote= getListConnectionToRemote();
  
    while (lstConnectionToRemote.size()){
//	  string strConnectionID = lstConnectionToRemote.front();
	string strConnectionID = lstConnectionToRemote.front().getConnectionID();

      
	string strConnectionTarget = lstConnectionToRemote.front().getConnectionTargetID();

	string strProcessName = getGroupParent(strConnectionTarget);
	lstReceiverProcesses.push_back(strProcessName);

	lstConnectionToRemote.pop_front();
    }

    lstReceiverProcesses.unique();
    return lstReceiverProcesses;
};


list<string> iqrcommon::ClsSysFileParser::getListSenderProcesses( ){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListSenderProcesses( )"<< endl;
#endif
    list<string> lstSenderProcesses;
//     list<string> lstConnectionFromRemote = getListConnectionFromRemote();
    list<ClsInfoConnection> lstConnectionFromRemote = getListConnectionFromRemote();
    while (lstConnectionFromRemote.size()){
//	  string strConnectionID = lstConnectionFromRemote.front();
	string strConnectionID = lstConnectionFromRemote.front().getConnectionID();

//  	  ClsSysFileNode clsSysFileNodeConnectionParameter = getConnectionSubNodes(strConnectionID);
//  	  ClsParameter clsParameterTemp;
//  	  string strConnectionSource;
      
//  	  while (clsSysFileNodeConnectionParameter.countParameters()){
//  	       clsParameterTemp = clsSysFileNodeConnectionParameter.popParameter();
//  	       string strParamName = clsParameterTemp.getName();
//  	       if(!strParamName.compare("source")){
//  		    strConnectionSource =clsParameterTemp.getValueAsString();
//  	       }

//  	  }

	string strConnectionSource = lstConnectionFromRemote.front().getConnectionSourceID();

	string strProcessName = getGroupParent(strConnectionSource);
	lstSenderProcesses.push_back(strProcessName);
	lstConnectionFromRemote.pop_front();
    }

    lstSenderProcesses.unique();
    return lstSenderProcesses;
};

iqrcommon::ClsInfoSystem iqrcommon::ClsSysFileParser::getSystemInfo() {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getSystemInfo( )"<< endl;
#endif
    ClsInfoSystem clsInfoSystem;

    DOMNodeList *dnlstTemp = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::SystemTag()));
    
    DOMNode *dnTemp = dnlstTemp->item(0);
    string strSystemID = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
    clsInfoSystem.setSystemID(strSystemID);
    clsInfoSystem.setSystemName(getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true));
    clsInfoSystem.setAuthor(getAttributeValue(dnTemp, ClsTagLibrary::AuthorTag(), true));
    clsInfoSystem.setDate(getAttributeValue(dnTemp, ClsTagLibrary::DateTag(), true));
    clsInfoSystem.setPort(iqrUtils::string2int(getAttributeValue(dnTemp, ClsTagLibrary::PortTag(), false)));
    clsInfoSystem.setCyclesPerSecond(iqrUtils::string2int(getAttributeValue(dnTemp, ClsTagLibrary::CyclesPerSecondTag(), false)));
    clsInfoSystem.setSyncPlots(iqrUtils::string2bool(getAttributeValue(dnTemp, ClsTagLibrary::SyncPlotsTag(), false)));
     
    clsInfoSystem.setNotes(getNotesSystem(strSystemID));
     
    return clsInfoSystem;
    
}

iqrcommon::ClsInfoPrc iqrcommon::ClsSysFileParser::getProcessInfo(string _strProcessID ) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getProcessInfo(string _strProcessID )"<< endl;
#endif

    ClsInfoPrc clsInfoPrc;


    DOMNodeList *dnlstTemp = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::ProcessTag()));
    for(unsigned int ii=0; ii< dnlstTemp->getLength(); ii++){
	DOMNode *dnTemp = dnlstTemp->item(ii);
	string strProcessID = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	if(!strProcessID.compare(_strProcessID)) {
	    clsInfoPrc.setPrcID(getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true));
	    clsInfoPrc.setProcessName(getAttributeValue(dnTemp, ClsTagLibrary::NameTag(), true));
	    clsInfoPrc.setHostname(getAttributeValue(dnTemp, ClsTagLibrary::HostnameTag(), true));
	    clsInfoPrc.setPort(iqrUtils::string2int(getAttributeValue(dnTemp, ClsTagLibrary::PortTag(), false)));
	    clsInfoPrc.setEnableModule(getAttributeValue(dnTemp, ClsTagLibrary::EnableModuleTag(), false));
	    clsInfoPrc.setPath(getAttributeValue(dnTemp, ClsTagLibrary::PathTag(), false));

	    string strNotes = getNotesProcess( strProcessID );
	    clsInfoPrc.setNotes(strNotes);

/* some how we have to find out whether there's a module */
	    DOMNode *dnPrcChildTemp = dnTemp->getFirstChild();
	    while(dnPrcChildTemp!=NULL){
		string strNodeName = XMLString::transcode(dnPrcChildTemp->getNodeName());
		if(!strNodeName.compare(ClsTagLibrary::ModuleTag())){
// 		    if(!strNodeName.compare("Module"))
		    clsInfoPrc.setHasModule(true); 
		}
		dnPrcChildTemp = dnPrcChildTemp->getNextSibling();
	    }
//###---------------




	}
    }

    return clsInfoPrc;
}

/** 
 * Get the notes for the system.
 * @return the notes as string
 */
string iqrcommon::ClsSysFileParser::getNotesSystem(string strID){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getNotesSystem(string strID)" << endl;
#endif
//     string strSysID = getSystemInfo().getSystemID();
    return getNotesNode(ClsTagLibrary::SystemTag(), strID);
//      return getNotesNode("System", strSysID);
};

/** 
 * Get the notes for a Process
 * @return the notes as string
 * @param _strProcessID ID of the Process.
 */
string iqrcommon::ClsSysFileParser::getNotesProcess(string _strProcessID){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getNotesProcess(string _strProcessID)" << endl;
#endif
    return getNotesNode(ClsTagLibrary::ProcessTag(), _strProcessID);
//      return getNotesNode("Process", _strProcessID);
};

/** 
 * Get the notes for a Connection
 * @return the notes as string
 * @param _strConnectionID ID of the Connection.
 */
string iqrcommon::ClsSysFileParser::getNotesConnection(string _strConnectionID){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getNotesConnection(string _strConnectionID)" << endl;
#endif

    return getNotesNode(ClsTagLibrary::ConnectionTag(), _strConnectionID);
//      return getNotesNode("Connection", _strConnectionID);
};

/** 
 * Get the notes for a Group
 * @return the notes as string
 * @param _strGroupID ID of the Group.
 */
string iqrcommon::ClsSysFileParser::getNotesGroup(string _strGroupID){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getNotesGroup(string _strGroupID)" << endl;
#endif

    return getNotesNode(ClsTagLibrary::GroupTag(), _strGroupID);
//      return getNotesNode("Group", _strGroupID);
};

/** 
 * Get the notes for a Neuron
 * @return the notes as string
 * @param _strGroupID ID of the Group the Neuron belongs to.
 */
string iqrcommon::ClsSysFileParser::getNotesNeuron(string _strGroupID){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getNotesNeuron(string _strGroupID)" << endl;
#endif


    return getNotesNode(ClsTagLibrary::NeuronTag(), _strGroupID);
//      return getNotesNode("Neuron", _strGroupID);
};

/** 
 * Get the notes for a Synapse
 * @return the notes as string
 * @param _strConnectionID ID of the Connection the Synapse belongs to.
 */
string iqrcommon::ClsSysFileParser::getNotesSynapse(string _strConnectionID){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getNotesSynapse(string _strConnectionID)" << endl;
#endif

//    return getNotesNode("SubConnection", _strConnectionID);
    return getNotesNode(ClsTagLibrary::SynapseTag(), _strConnectionID);
};

/** 
 * Get the notes for a Module
 * @return the notes as string
 * @param _strPrcessID ID of the Process the Module belongs.
 */
string iqrcommon::ClsSysFileParser::getNotesModule(string _strProcessID){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getNotesModule(string _strProcessID)" << endl;
#endif

    return getNotesNode(ClsTagLibrary::ModuleTag(), _strProcessID);
//      return getNotesNode("Module", _strProcessID);
};

/** 
 * General purpose notes extraction routine. Don't call this directly, rather use the wrappers getNotes(System|Process|...)
 * @return .
 * @param Name of the Node that's the parent of the notes.
 * @param ID of the notes' ancestor.
 */
string iqrcommon::ClsSysFileParser::getNotesNode_old(string strParentNodeName, string _strNodeID){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getNotesNode(string strParentNodeName, string _strNodeID)" << endl;
#endif

//    cout << "_strNodeID: " <<  _strNodeID << endl;
    
    DOMNodeList *dnlst = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::NotesTag()));
    DOMNode *dnNote = NULL;
    
    for (unsigned int ii = 0; ii < dnlst->getLength(); ii++){
	dnNote =  dnlst->item(ii);
	/* check on the parent of the note: System, Process, Group etc */
	if(!strParentNodeName.compare(XMLString::transcode(dnNote->getParentNode()->getNodeName()))){
//	    cout << "dnNote->getNodeName(): " <<  XMLString::transcode(dnNote->getNodeName()) << endl;
//	    cout << "dnNote->getNodeType(): " << dnNote->getNodeType() << endl;
//	    cout << "dnNote->hasChildNodes(): " << dnNote->hasChildNodes() << endl;
	    
	    string strParentID = findAncestorID( dnNote);
	    if(!strParentID.compare(_strNodeID)) {
//		cout << "YEP" << endl;
		DOMNode *dnValue = dnNote->getFirstChild();
		if(dnValue!=NULL){
//		    cout << "XMLString::transcode(dnValue->getNodeValue()): " << XMLString::transcode(dnValue->getNodeValue()) << endl;
		    return XMLString::transcode(dnValue->getNodeValue());     
		}
		else {
		    return "";
		}
	    }
	}
    }
    return "";
};



void iqrcommon::ClsSysFileParser::buildNotesCache(DOMDocument* ddocRoot){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "iqrcommon::ClsSysFileParser::buildNotesCache()" << endl;
#endif
    mapNotes.clear();

    DOMNodeList *dnlst = ddocRoot->getElementsByTagName(XMLString::transcode(ClsTagLibrary::NotesTag()));
    DOMNode *dnNote = NULL;
    string strNote;
    for (unsigned int ii = 0; ii < dnlst->getLength(); ii++){
	dnNote =  dnlst->item(ii);
	string strParentID = findAncestorID( dnNote);
	DOMNode *dnValue = dnNote->getFirstChild();
	if(dnValue!=NULL){
	    string strNote = XMLString::transcode(dnValue->getNodeValue());     
	    pair<string,string> pairTemp(strParentID, strNote);
	    mapNotes.insert(pairTemp);
	}
    }
}



string iqrcommon::ClsSysFileParser::getNotesNode(string /* strParentNodeName */, string _strNodeID){
    map<string,string>::iterator it = mapNotes.find(_strNodeID);
    if(it != mapNotes.end()){
	return it->second;
    }
    return "";
}

/** 
 * General purpose notes extraction routine. Don't call this directly, rather use the wrappers getNotes(System|Process|...)
 * @return .
 * @param Name of the Node that's the parent of the notes.
 * @param ID of the notes' ancestor.
 */
string iqrcommon::ClsSysFileParser::getNotesNode_WorkingButSlow(string strParentNodeName, string _strNodeID){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getNotesNode(string strParentNodeName, string _strNodeID)" << endl;
#endif
    
/* this would be sooo nice but DOMDocument::getElementById is pure virtual!!! F***
   DOMElement *deTemp = ddocSystemFile->DOMDocument::getElementById(XMLString::transcode(_strNodeID.c_str()));
   if(deTemp!=NULL){
   DOMNodeList *dnlstNotes = deTemp->getElementsByTagName(XMLString::transcode(ClsTagLibrary::NotesTag()));
   if(dnlstNotes->getLength()>=1){
   DOMNode *dnNote = dnlstNotes->item(1);
   DOMNode *dnValue = dnNote->getFirstChild();
   if(dnValue!=NULL){
   return XMLString::transcode(dnValue->getNodeValue());     
   }
   else {
   return "";
   }
   }
   }
*/

    DOMNodeList *dnlst = ddocSystemFile->getElementsByTagName(XMLString::transcode(strParentNodeName.c_str()));
    /* find node with the right id */
    DOMNode *dnTemp = NULL;
    for (unsigned int ii = 0; ii < dnlst->getLength(); ii++){
	dnTemp =  dnlst->item(ii);
	string strID = getAttributeValue(dnTemp, ClsTagLibrary::IDTag(), true);
	if(!strID.compare(_strNodeID)) {


	    DOMNode *dnChild = dnTemp->getFirstChild();
	    while(dnChild!=NULL){
		string strNodeName = XMLString::transcode(dnChild->getNodeName());
		if(!strNodeName.compare(ClsTagLibrary::NotesTag())){
		    DOMNode *dnValue = dnChild->getFirstChild();
		    if(dnValue!=NULL){
			return XMLString::transcode(dnValue->getNodeValue());     
		    }
		    else {
			return "";
		    }
		}
		dnChild = dnChild->getNextSibling();
	    }
//###-----------------------



	}
    }
    return "";
};



string iqrcommon::ClsSysFileParser::getNotesNode(DOMNode *dnRoot){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "iqrcommon::ClsSysFileParser::getNotesNode(DOMNode *dnRoot)" << endl;
#endif
    
    DOMNode *dnTemp = dnRoot->getFirstChild();
    while(dnTemp!=NULL){
	string strNodeName = XMLString::transcode(dnTemp->getNodeName());
	if(!strNodeName.compare(ClsTagLibrary::NotesTag())){
	    DOMNode *dnValue = dnTemp->getFirstChild();
	    if(dnValue!=NULL){
		return XMLString::transcode(dnValue->getNodeValue());     
	    }
	    else {
		return "";
	    }
	}
	dnTemp = dnTemp->getNextSibling();
    }
    return "";
}



/** 
 * This function find the first ancestor node that has an ID attribute.
 * @return ID of the ancestor
 * @param dn the node who's ancestor we're looking for.
 */
string iqrcommon::ClsSysFileParser::findAncestorID( DOMNode *dn) {
    string strParentID;
    string strNodeName = XMLString::transcode(dn->getNodeName());
    if(!strNodeName.compare(ClsTagLibrary::SystemTag())){
	strParentID = "";
    } else {
	DOMNode *dnParent = dn->getParentNode();
	if(dnParent->getAttributes()->getNamedItem(XMLString::transcode(ClsTagLibrary::IDTag())) == NULL){
// 	  if(dnParent->getAttributes()->getNamedItem("id") == NULL){
	    strParentID = findAncestorID(dnParent);
	} 
	else {
	    strParentID = XMLString::transcode(dnParent->getAttributes()->getNamedItem(XMLString::transcode(ClsTagLibrary::IDTag()))->getNodeValue());
// 	       strParentID = dnParent->getAttributes()->getNamedItem("id")->getNodeValue().transcode();
	}
    }
    return strParentID;     
};


iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::DomNode2SysNode(DOMNode *dnTop, int iCount,string strSep){
    iCount ++;
    strSep += "\t";
    string strNodeName = XMLString::transcode(dnTop->getNodeName());

#ifdef DEBUG_CLSSYSFILEPARSER
//    cout << "DomNode2SysNode: " << strSep << iCount << " " << strNodeName  << endl;
#endif
    ClsSysFileNode clsSysFileNode(strNodeName);

    // node and it's attributes ++++++++++++++++++++
    DOMNamedNodeMap *dnnmapAtt =  dnTop->getAttributes();
    iqrcommon_old::ClsParameter clsParameterAtt;
    for(unsigned int ii=0; ii<dnnmapAtt->getLength();ii++){
	string strAttName = XMLString::transcode(dnnmapAtt->item(ii)->getNodeName());
	string strAttValue = XMLString::transcode(dnnmapAtt->item(ii)->getNodeValue());
#ifdef DEBUG_CLSSYSFILEPARSER
//	cout << "DomNode2SysNode: " << strSep << iCount << " " << "AttName: " << strAttName << ", AttValue: " << strAttValue << endl; 
#endif
	clsParameterAtt.setParameter(strAttName, strAttValue);
	clsSysFileNode.addParameter(clsParameterAtt);
    }
    DOMNode *dnChild = dnTop->getFirstChild();
    while(dnChild!=NULL){
	if(dnChild->getNodeType()==DOMNode::ELEMENT_NODE){
	    clsSysFileNode.addNode(DomNode2SysNode(dnChild, iCount, strSep));
	}
	dnChild = dnChild->getNextSibling();
    }
    return clsSysFileNode;
};


void iqrcommon::ClsSysFileParser::buildDiagramLineCache(DOMDocument* ddocRoot){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "iqrcommon::ClsSysFileParser::buildDiagramLineCache()" << endl;
#endif
    mapDiagramLine.clear();
    DOMNodeList *dnlst = ddocRoot->getElementsByTagName(XMLString::transcode(ClsTagLibrary::DiagramLineTag()));
    for (unsigned int ii = 0; ii < dnlst->getLength(); ii++){
	DOMNode *dnTemp =  dnlst->item(ii);
	DOMNode *dnParent = dnTemp->getParentNode();
	string strParentID = getAttributeValue(dnParent, ClsTagLibrary::IDTag(), true);

	/* extract APSource and APTarget */
	string strAPSource = getAttributeValue(dnTemp, ClsTagLibrary::DiagramLineAPSourceTag(), false);
	string strAPTarget = getAttributeValue(dnTemp, ClsTagLibrary::DiagramLineAPTargetTag(), false);
	ClsInfoDiagramLine clsInfoDiagramLine;
	clsInfoDiagramLine.setAPSource(iqrUtils::string2int(strAPSource));
	clsInfoDiagramLine.setAPTarget(iqrUtils::string2int(strAPTarget));
	DOMNode *dnPoint = dnTemp->getFirstChild();
	while(dnPoint!=NULL){
	    string strNodeName = XMLString::transcode(dnPoint->getNodeName());
	    if( !strNodeName.compare(ClsTagLibrary::DiagramLinePointTag())){
		string strX = getAttributeValue(dnPoint, ClsTagLibrary::DiagramLinePointXTag(), true);
		string strY = getAttributeValue(dnPoint, ClsTagLibrary::DiagramLinePointYTag(), true);
		clsInfoDiagramLine.addPoint(strX, strY);
	    }
	    dnPoint = dnPoint->getNextSibling();
	}
	pair<string,ClsInfoDiagramLine> pairTemp(strParentID, clsInfoDiagramLine);
	mapDiagramLine.insert(pairTemp);	
    }
}


void iqrcommon::ClsSysFileParser::buildDiagramIconCache(DOMDocument* ddocRoot){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "iqrcommon::ClsSysFileParser::buildDiagramIconCache()" << endl;
#endif
    mapDiagramIcon.clear();
    DOMNodeList *dnlst = ddocRoot->getElementsByTagName(XMLString::transcode(ClsTagLibrary::DiagramIconTag()));
    for (unsigned int ii = 0; ii < dnlst->getLength(); ii++){
	DOMNode *dnTemp =  dnlst->item(ii);
	DOMNode *dnParent = dnTemp->getParentNode();
	string strParentID = getAttributeValue(dnParent, ClsTagLibrary::IDTag(), true);
	if(dnTemp!=NULL){
	    ClsInfoDiagramIcon clsInfoDiagramIcon;
	    clsInfoDiagramIcon.setX(getAttributeValue(dnTemp, ClsTagLibrary::DiagramIconXTag(), true));
	    clsInfoDiagramIcon.setY(getAttributeValue(dnTemp, ClsTagLibrary::DiagramIconYTag(), true));
	    /* these attributes are implicit, i.e. do not have to be present */
	    clsInfoDiagramIcon.setWidth(getAttributeValue(dnTemp, ClsTagLibrary::DiagramIconWidthTag(), false));
	    clsInfoDiagramIcon.setHeight(getAttributeValue(dnTemp, ClsTagLibrary::DiagramIconHeightTag(), false));
	    pair<string,ClsInfoDiagramIcon> pairTemp(strParentID, clsInfoDiagramIcon);
	    mapDiagramIcon.insert(pairTemp);
	}
    }
}
