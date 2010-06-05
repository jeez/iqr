/****************************************************************************
 ** $Filename: systemFileWriter.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Jan 10 14:50:24 2001
 ** $Date: 2005/03/18 23:01:23 $
 **
 ** $Keywords: DOM XML XSL 
 ** $Description: xml document creator with the apache xerces and xalan libs
 **
 **               @@@ 14/10/01 All latin1() calls to string can be
 **               removed when UTF-16 format is specified for the DOM
 **               tree.
 **
 *****************************************************************************/

#include <vector>
#include <fstream>

#include <framework/StdOutFormatTarget.hpp>
#include <framework/MemBufFormatTarget.hpp>
#include <framework/LocalFileFormatTarget.hpp>

#include "ClsFESerializer.h"

#include "iqrUtils.h"
#include "tagLibrary.hpp"

#include "ClsFESystemManager.h"
#include "ClsFEDiagramManager.h"
#include "ClsFEDiagramManager.h"
#include "ClsFESystem.h"
#include "ClsFEProcess.h"
#include "ClsFEGroup.h"
#include "ClsFEConnection.h"

#include "ClsBaseTopology.h"
#include "ClsTopologySparse.h"
#include "ClsBaseArborization.h"
#include "ClsBaseFunction.h"
#include "ClsBasePattern.h"
#include "ClsSubPopulations.h"

#include "parameter.hpp"

#include "ClsInfoDiagramIcon.h"
#include "ClsInfoDiagramLine.h"

#include "xRefHolder.hpp"


//#define DEBUG_CLSFESYSTEMFILEWRITER


#ifdef DEBUG_CLSFESYSTEMFILEWRITER
static const bool bDebugSystemFileWriter = true;
#else
static const bool bDebugSystemFileWriter = false;
#endif

using iqrUtils::int2string;
using std::string;
//using namespace iqrfe;
using namespace iqrcommon;

void appendListIndexQuadruples(ListIndexQuadruples &liqTarget, ListIndexQuadruples &liqSource){
    for(unsigned int ii=0; ii<liqSource.size(); ii++){
	liqTarget.push_back(liqSource[ii]);
    }
}

bool operator<(ClsKey a, ClsKey b){
//    if(a.source==b.source && a.target==b.target && a.type==b.type){
    if(a.ID==b.ID){
	return false;
    }
    return true;
}


ClsFESerializer::ClsFESerializer()  {
    if (bDebugSystemFileWriter) {
//	qDebug("ClsFESerializer::ClsFESerializer");
    }
    bXMLPlatformInitialized = false;
}

ClsFESerializer::~ClsFESerializer()  {
    if (bDebugSystemFileWriter) {
//	qDebug("ClsFESerializer::~ClsFESerializer");
    }
    bXMLPlatformInitialized = false;
}

bool ClsFESerializer::SaveSystem(string _strFileName, bool bVerifySystem,bool bVerifyFile) {
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::SaveSystem(string _strFileName, bool bVerifySystem,bool bVerifyFile)" << endl;
    }
    
    
    if(!bXMLPlatformInitialized){
	try {
	    XMLPlatformUtils::Initialize();
	}
	catch(const XMLException& toCatch) {
	    cerr << "Error during Xerces-c Initialization.\n"
		 << "  Exception message:"
		 << toCatch.getMessage() << endl;
	    bXMLPlatformInitialized = false;
	    return false;
	}
	bXMLPlatformInitialized = true;
    }
    
    lstExternalProcesses.clear();
     CreateDOMTree(INCLUDE_XML_DECL_NODE);
 
     if (bVerifySystem){
 	if (!IsDOMTreeValid()) {
 // 	    qWarning("WARNING: ClsFESerializer::SaveSystem:"
 // 		     " invalid system, not saving");
 	    return false;
 	}
     }
     
      string strSystemCont = "";
      SerializeDOMTree(strSystemCont); /* pass by reference */
      if (bVerifyFile) {
  	if (!IsFileValid(_strFileName)) {
  	    return false;
  	} 
      }
  
      ofstream out(_strFileName.c_str());
      out << strSystemCont << endl;

      ddocSystem->release(); 

      /* WHERE SHOULD THIS GO?? */
      writeExternalProcesses();


    return true;
}

string ClsFESerializer::getSystemAsString( bool bVerifySystem) {
    CreateDOMTree(INCLUDE_XML_DECL_NODE);

    if (bVerifySystem){
	if (!IsDOMTreeValid()) {
// 	    qWarning("WARNING: ClsFESerializer::SaveSystem:"
//		     " invalid system, not saving");
	    return false;
	}

    }
    string strSystemCont = "";
    SerializeDOMTree(strSystemCont); /* pass by reference */
    return strSystemCont;
    
}


string ClsFESerializer::getProcessesAsXML(list<string> lstIDs) {
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::getProcessesAsXML(list<string> lstIDs)" << endl;
    }
    

    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));


    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocClipboard = impl->createDocument(0, XMLString::transcode(ClsTagLibrary::iqrEntityTag()), NULL);
    DOMElement *delemClipboard = ddocClipboard->createElement(XMLString::transcode(ClsTagLibrary::iqrEntityTag()));
    delemClipboard->setAttribute(XMLString::transcode(ClsTagLibrary::ClipboardContentsType()), XMLString::transcode(ClsTagLibrary::ProcessTag()));


    list<string>::iterator it;
    for(it=lstIDs.begin(); it!=lstIDs.end(); it++){
	string strID = (*it);

	DOMElement *delemProcess = addFEProcess(ddocClipboard, ClsFESystemManager::Instance()->getFEProcess(strID));
	delemClipboard->appendChild(delemProcess);

	map<string, ClsFEGroup*> mapGroups = ClsFESystemManager::Instance()->getMapFEGroups();
	map<string, ClsFEGroup*>::iterator itmapFEGroups;
	for (itmapFEGroups = mapGroups.begin(); itmapFEGroups != mapGroups.end(); ++itmapFEGroups){
	    if(!strID.compare((itmapFEGroups->second)->getProcessID())) {
		DOMElement *delemGroup = addFEGroup(ddocClipboard, itmapFEGroups->second);
		delemProcess->appendChild(delemGroup);
	    }
	}

	map<string, ClsFEConnection*> mapConnections = ClsFESystemManager::Instance()->getMapFEConnections();
	map<string, ClsFEConnection*>::iterator itmapFEConnections;
	for (itmapFEConnections = mapConnections.begin(); itmapFEConnections != mapConnections.end(); ++itmapFEConnections){
	    string strSourceID = (itmapFEConnections->second)->getConnectionSourceID();
	    string strTargetID = (itmapFEConnections->second)->getConnectionTargetID();
	    string strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);
	    string strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetID);
	    
	    if(!strSourceProcessID.compare(strID) && !strTargetProcessID.compare(strID)){
		DOMElement *delemConnection = addFEConnection(ddocClipboard, itmapFEConnections->second);
		delemClipboard->appendChild(delemConnection);
	    }
	}
    }

#if XERCES_VERSION_MAJOR >= 3
    DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#else
    DOMWriter* theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#endif

    MemBufFormatTarget *myFormTarget = new MemBufFormatTarget();
#if XERCES_VERSION_MAJOR >= 3
    DOMLSOutput* theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    theOutput->setByteStream(myFormTarget);
#endif



    try {
	// do the serialization through DOMWriter::writeNode();
#if XERCES_VERSION_MAJOR >= 3
	theSerializer->write(delemClipboard, theOutput);
#else
	theSerializer->writeNode(myFormTarget, *delemClipboard);
#endif


    }
    catch (const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return "";
    }
    catch (const DOMException& toCatch) {
	char* message = XMLString::transcode(toCatch.msg);
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return "";
    }
    catch (...) {
	cout << "Unexpected Exception \n" ;
	return "";
    }

    string strProcessXML = (const char*)(myFormTarget->getRawBuffer());
  
    theSerializer->release();
    delete myFormTarget;
    return strProcessXML;
}

string ClsFESerializer::getConnectionsAsXML(list<string> lstIDs) {
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::getConnectionsAsXML(list<string> lstIDs )" << endl;
    }
    
    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));
	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocClipboard = impl->createDocument(0, XMLString::transcode(ClsTagLibrary::iqrEntityTag()), NULL);
    DOMElement *delemClipboard = ddocClipboard->createElement(XMLString::transcode(ClsTagLibrary::iqrEntityTag()));
    delemClipboard->setAttribute(XMLString::transcode(ClsTagLibrary::ClipboardContentsType()), XMLString::transcode(ClsTagLibrary::ConnectionTag()));

    list<string>::iterator it;
    for(it=lstIDs.begin(); it!=lstIDs.end(); it++){
	string strID = (*it);
	DOMElement *delemConnection = addFEConnection(ddocClipboard, ClsFESystemManager::Instance()->getFEConnection(strID));
	delemClipboard->appendChild(delemConnection);
    }


#if XERCES_VERSION_MAJOR >= 3
     DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
   if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#else
    DOMWriter* theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#endif


    MemBufFormatTarget *myFormTarget = new MemBufFormatTarget();

#if XERCES_VERSION_MAJOR >= 3
    DOMLSOutput* theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    theOutput->setByteStream(myFormTarget);
#endif


    try {
#if XERCES_VERSION_MAJOR >= 3
	theSerializer->write(delemClipboard, theOutput);
#else
	theSerializer->writeNode(myFormTarget, *delemClipboard);
#endif
    }
    catch (const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return "";
    }
    catch (const DOMException& toCatch) {
	char* message = XMLString::transcode(toCatch.msg);
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return "";
    }
    catch (...) {
	cout << "Unexpected Exception \n" ;
	return "";
    }

    string strConnectionXML = (const char*)(myFormTarget->getRawBuffer());
  
    theSerializer->release();
    delete myFormTarget;
    return strConnectionXML;
}

string ClsFESerializer::getGroupsAsXML(list<string> lstIDs) {
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::getGroupAsXML(string strGroupID)" << endl;
    }
    
    
    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));
    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocClipboard = impl->createDocument(0, XMLString::transcode(ClsTagLibrary::iqrEntityTag()), NULL);
    DOMElement *delemClipboard = ddocClipboard->createElement(XMLString::transcode(ClsTagLibrary::iqrEntityTag()));
    delemClipboard->setAttribute(XMLString::transcode(ClsTagLibrary::ClipboardContentsType()), XMLString::transcode(ClsTagLibrary::GroupTag()));

    list<string>::iterator it;
    for(it=lstIDs.begin(); it!=lstIDs.end(); it++){
	string strID = (*it);
	DOMElement *delemGroup = addFEGroup(ddocClipboard, ClsFESystemManager::Instance()->getFEGroup(strID));
	delemClipboard->appendChild(delemGroup);
    }
    
#if XERCES_VERSION_MAJOR >= 3
    DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true)){
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);
    }
    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true)){
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    }
#else
    DOMWriter* theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true)){
	theSerializer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);
    }
    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true)){
	theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    }
#endif

    MemBufFormatTarget *myFormTarget = new MemBufFormatTarget();

#if XERCES_VERSION_MAJOR >= 3
    DOMLSOutput* theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    theOutput->setByteStream(myFormTarget);
#endif

    try {
	// do the serialization through DOMWriter::writeNode();
#if XERCES_VERSION_MAJOR >= 3
	theSerializer->write(delemClipboard, theOutput);
#else
	theSerializer->writeNode(myFormTarget, *delemClipboard);
#endif
}
    catch (const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return "";
    }
    catch (const DOMException& toCatch) {
	char* message = XMLString::transcode(toCatch.msg);
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return "";
    }
    catch (...) {
	cout << "Unexpected Exception \n" ;
	return "";
    }

    string strGroupXML = (const char*)(myFormTarget->getRawBuffer());
  
    theSerializer->release();
    delete myFormTarget;
    return strGroupXML;
}


string ClsFESerializer::getGroupsWidthConnectionsAsXML(list<string> lstIDGroups, list<string> lstIDConnections){
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::getGroupsWidthConnectionsAsXML(list<string> lstIDGroups, list<string> lstIDConnections)" << endl;
    }

	DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));
	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocClipboard = impl->createDocument(0, XMLString::transcode(ClsTagLibrary::iqrEntityTag()), NULL);
	DOMElement *delemClipboard = ddocClipboard->createElement(XMLString::transcode(ClsTagLibrary::iqrEntityTag()));
	delemClipboard->setAttribute(XMLString::transcode(ClsTagLibrary::ClipboardContentsType()), XMLString::transcode(ClsTagLibrary::GroupsWithConnectionsTag()));
	
	list<string>::iterator itGroup;
	for(itGroup=lstIDGroups.begin(); itGroup!=lstIDGroups.end(); itGroup++){
	    string strID = (*itGroup);
	    DOMElement *delemGroup = addFEGroup(ddocClipboard, ClsFESystemManager::Instance()->getFEGroup(strID));
	    delemClipboard->appendChild(delemGroup);
	}
	
	list<string>::iterator itConnection;
	for(itConnection=lstIDConnections.begin(); itConnection!=lstIDConnections.end(); itConnection++){
	    string strID = (*itConnection);
	    DOMElement *delemConnection = addFEConnection(ddocClipboard, ClsFESystemManager::Instance()->getFEConnection(strID));
	    delemClipboard->appendChild(delemConnection);
	}
	
#if XERCES_VERSION_MAJOR >= 3
	DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
	if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	    theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);
	
	if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	    theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#else
	DOMWriter* theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
	if (theSerializer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	    theSerializer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);
	
	if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	    theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#endif
	
	MemBufFormatTarget *myFormTarget = new MemBufFormatTarget();
#if XERCES_VERSION_MAJOR >= 3
    DOMLSOutput* theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    theOutput->setByteStream(myFormTarget);
#endif

	try {
#if XERCES_VERSION_MAJOR >= 3
	    theSerializer->write(delemClipboard, theOutput);
#else
	    theSerializer->writeNode(myFormTarget, *delemClipboard);
#endif
	}
	catch (const XMLException& toCatch) {
	    char* message = XMLString::transcode(toCatch.getMessage());
	    cout << "Exception message is: \n"
		 << message << "\n";
	    XMLString::release(&message);
	    return "";
	}
	catch (const DOMException& toCatch) {
	    char* message = XMLString::transcode(toCatch.msg);
	    cout << "Exception message is: \n"
		 << message << "\n";
	    XMLString::release(&message);
	    return "";
	}
	catch (...) {
	    cout << "Unexpected Exception \n" ;
	    return "";
	}
	
	string strGroupsWithConnectionsXML = (const char*)(myFormTarget->getRawBuffer());
	
	theSerializer->release();
	delete myFormTarget;
	return strGroupsWithConnectionsXML;
}

/** 
 * Builds the DOM tree.
 *
 * A valid system pointer is assumed here.  This is a private
 * function, the system pointer check must be made in the public
 * interface functions prior to calling this function.
 *
 * The XML declaration node can be excluded, so that runtime errors do
 * not occur during XSL transformations.  It should only be excluded
 * for non-XML files.
 *
 * @param _bIncludeXMLDeclNode Flag to select whether XML declaration
 * node should be included (true) or excluded (false).  
 */
void ClsFESerializer::CreateDOMTree(bool _bIncludeXMLDeclNode){
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::CreateDOMTree(bool _bIncludeXMLDeclNode)" << endl;
    }
    

    _bIncludeXMLDeclNode = true;
    
    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));

    DOMDocumentType *dtd = impl->createDocumentType(XMLString::transcode(ClsTagLibrary::SystemTag()),
						    XMLString::transcode("-//INI/iqr421"),
						    XMLString::transcode("iqrSystem.dtd"));

    ddocSystem = impl->createDocument(0, XMLString::transcode(ClsTagLibrary::SystemTag()), dtd);

//     if (bDebugSystemFileWriter) {
// 	qDebug("ClsFESerializer::CreateDOMTree:"
// 	       " system has %d processes, %d connections",
// 	       _pclsSystem->Elements().count(),
// 	       _pclsSystem->Connections().count());
//     }


    DOMElement *delemSystem = addFESystem(ddocSystem, ClsFESystemManager::Instance()->getFESystem());
    map<string, ClsFEProcess*> mapProcesses = ClsFESystemManager::Instance()->getMapFEProcesses();
    map<string, ClsFEGroup*> mapGroups = ClsFESystemManager::Instance()->getMapFEGroups();
    map<string, ClsFEProcess*>::iterator itmapPrcs;
    for (itmapPrcs = mapProcesses.begin(); itmapPrcs != mapProcesses.end(); ++itmapPrcs){
	ClsFEProcess* clsFEProcessTemp = itmapPrcs->second;
	string strProcessID = clsFEProcessTemp->getProcessID();
	string strPath = clsFEProcessTemp->getPath();
	if(strPath.length()<=0){
	    DOMElement *delemProcess = addFEProcess(ddocSystem, clsFEProcessTemp); 
	    delemSystem->appendChild(delemProcess);                                 
	    
	    map<string, ClsFEGroup*>::iterator itmapFEGroups;
	    for (itmapFEGroups = mapGroups.begin(); itmapFEGroups != mapGroups.end(); ++itmapFEGroups){
		if(!strProcessID.compare((itmapFEGroups->second)->getProcessID())) {
		    DOMElement *delemGroup = addFEGroup(ddocSystem, itmapFEGroups->second); 
		    delemProcess->appendChild(delemGroup);                                  
		}
	    }
	} else {
	    /* WE HAVE TO CREATE AN "EMPTY" PROCESS */
	    DOMElement *delemProcess = addFEProcess(ddocSystem, clsFEProcessTemp); 
	    delemSystem->appendChild(delemProcess);          
	    lstExternalProcesses.push_back(strProcessID);
	}
    }
    map<string, ClsFEConnection*> mapConnections  = ClsFESystemManager::Instance()->getMapFEConnections();
    map<string, ClsFEConnection*>::iterator itmapConnection;
    for (itmapConnection = mapConnections.begin(); itmapConnection != mapConnections.end(); ++itmapConnection){
	string strSourceID = (itmapConnection->second)->getConnectionSourceID();
	string strTargetID = (itmapConnection->second)->getConnectionTargetID();
	string strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);
	string strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetID);
	if(strSourceProcessID.compare(strTargetProcessID)){
	    DOMElement *delemConnection = addFEConnection(ddocSystem, itmapConnection->second); 
	    delemSystem->appendChild(delemConnection);                                          
	} else {
	    if(find(lstExternalProcesses.begin(), lstExternalProcesses.end(), strSourceProcessID)==lstExternalProcesses.end()){
		DOMElement *delemConnection = addFEConnection(ddocSystem, itmapConnection->second); 
		delemSystem->appendChild(delemConnection);                                          
	    }
	}
    }


    if (bDebugSystemFileWriter) {
	cout << "exit ClsFESerializer::CreateDOMTree(bool _bIncludeXMLDeclNode)" << endl;    
    }
    
}


int ClsFESerializer::serializeToAER(string strExportFilename){

    if(!bXMLPlatformInitialized){
	try {
	    XMLPlatformUtils::Initialize();
	}
	catch(const XMLException& toCatch) {
	    cerr << "Error during Xerces-c Initialization.\n"
		 << "  Exception message:"
		 << toCatch.getMessage() << endl;
	    bXMLPlatformInitialized = false;
	    return false;
	}
	bXMLPlatformInitialized = true;
    }

    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));
    DOMDocumentType *dtd = impl->createDocumentType(XMLString::transcode(ClsTagLibrary::AERSystemTag()),  //<-FIX
						    XMLString::transcode("-//INI/iqr421"),
						    XMLString::transcode("iqrAERSystem.dtd")); //<-FIX

    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocAERSystem = impl->createDocument(0, XMLString::transcode(ClsTagLibrary::AERSystemTag()), dtd);
    DOMElement *delemSystem = addFESystem(ddocAERSystem, ClsFESystemManager::Instance()->getFESystem());

    map<string, ClsFEProcess*> mapProcesses = ClsFESystemManager::Instance()->getMapFEProcesses();
    map<string, ClsFEGroup*> mapGroups = ClsFESystemManager::Instance()->getMapFEGroups();
    map<string, ClsFEProcess*>::iterator itmapPrcs;
    for (itmapPrcs = mapProcesses.begin(); itmapPrcs != mapProcesses.end(); ++itmapPrcs){
	DOMElement *delemProcess = addFEProcess(ddocAERSystem, itmapPrcs->second);
	delemSystem->appendChild(delemProcess);

	string strProcessID = itmapPrcs->second->getProcessID();
	
	map<string, ClsFEGroup*>::iterator itmapFEGroups;
	for (itmapFEGroups = mapGroups.begin(); itmapFEGroups != mapGroups.end(); ++itmapFEGroups){
	    if(itmapFEGroups->second==NULL){
		cout << "NNUUULLLL" << endl;
		
	    }
	    if(!strProcessID.compare((itmapFEGroups->second)->getProcessID())) {
		DOMElement *delemGroup = addFEGroup(ddocAERSystem, itmapFEGroups->second);
		delemProcess->appendChild(delemGroup);
	    }
	}
    }
    list<DOMElement*> lstDEConnections = getAERConnections(ddocAERSystem);
    list<DOMElement*>::iterator it;
    for(it=lstDEConnections.begin(); it!=lstDEConnections.end();it++){
	delemSystem->appendChild((*it));
    }

//    DOMImplementation *
    impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"LS"));
#if XERCES_VERSION_MAJOR >= 3
    DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#else
    DOMWriter* theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#endif

    LocalFileFormatTarget *myFormTarget = new LocalFileFormatTarget(strExportFilename.c_str());
#if XERCES_VERSION_MAJOR >= 3
    DOMLSOutput* theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    theOutput->setByteStream(myFormTarget);
#endif


    try {
	// do the serialization through DOMWriter::writeNode();
#if XERCES_VERSION_MAJOR >= 3
	theSerializer->write(ddocAERSystem, theOutput);
#else
	theSerializer->writeNode(myFormTarget, *ddocAERSystem);
#endif
    }
    catch (const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return -1;
    }
    catch (const DOMException& toCatch) {
	char* message = XMLString::transcode(toCatch.msg);
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return -1;
    }
    catch (...) {
	cout << "Unexpected Exception \n" ;
	return -1;
    }

//    strSystemCont = (const char*)(myFormTarget->getRawBuffer());
  
    theSerializer->release();
//    delete myErrorHandler;
//    delete myFilter;
    delete myFormTarget;

/* use exceptions.... */

    return 0;

}
 
list<DOMElement*> ClsFESerializer::getAERConnections(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocRoot){
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::getAERConnections(DOMDocument *ddocRoot)" << endl;
    }

    list<DOMElement*> lstDEConnections;

    map<ClsKey, ListIndexQuadruples> mapSourceTargetType;

    
    map<string, ClsFEConnection*> mapFEConnections = ClsFESystemManager::Instance()->getMapFEConnections();
    map<string, ClsFEConnection*>::iterator itMapConnections;
    for(itMapConnections=mapFEConnections.begin();itMapConnections!=mapFEConnections.end();itMapConnections++){

	ListIndexQuadruples listIndexQuadruple = itMapConnections->second->getQuadrupleList();

	string strAERConnectionID = itMapConnections->second->getConnectionID() + string("-AER");

	ClsKey key(strAERConnectionID,
		   itMapConnections->second->getConnectionID(),
		   itMapConnections->second->getConnectionSourceID(),
		   itMapConnections->second->getConnectionTargetID(),
		   itMapConnections->second->getConnectionTypeAsString());



   
	if(mapSourceTargetType.find(key) == mapSourceTargetType.end()){
	    pair<ClsKey, ListIndexQuadruples> pairTemp(key, listIndexQuadruple);
	    mapSourceTargetType.insert(pairTemp);
	} else {
	    appendListIndexQuadruples(mapSourceTargetType.find(key)->second, listIndexQuadruple);
	}
    }

    map<ClsKey, ListIndexQuadruples>::iterator itMapSTT;
    for(itMapSTT=mapSourceTargetType.begin();itMapSTT!=mapSourceTargetType.end();itMapSTT++){
	string strSourceID = itMapSTT->first.source;
	    
//	string strConnectionKey = itMapSTT->first.source + itMapSTT->first.target + itMapSTT->first.type;
//	cout << "strConnectionKey: " << strConnectionKey << endl;


	DOMElement* delemAERConnection = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::AERConnectionTag()));

	delemAERConnection->setAttribute(XMLString::transcode(ClsTagLibrary::IDTag()), 
					 XMLString::transcode(itMapSTT->first.ID.c_str()));
    
	delemAERConnection->setAttribute(XMLString::transcode(ClsTagLibrary::ConnectionSourceTag()), 
					 XMLString::transcode(itMapSTT->first.source.c_str()));
	
	delemAERConnection->setAttribute(XMLString::transcode(ClsTagLibrary::ConnectionTargetTag()), 
					 XMLString::transcode(itMapSTT->first.target.c_str()));
	
	delemAERConnection->setAttribute(XMLString::transcode(ClsTagLibrary::ConnectionTypeTag()), 
					 XMLString::transcode(itMapSTT->first.type.c_str()));


/* synapses III */
	ClsFEConnection* clsFEConnection = NULL;
	clsFEConnection = ClsFESystemManager::Instance()->getFEConnection(itMapSTT->first.origID);
	if(clsFEConnection!=NULL){
	    string strSynapseType = clsFEConnection->getConnectionSynapseType();
	    if(strSynapseType.size()>0){
		DOMElement *delemSynapse = addParameterizedNode(ddocRoot, ClsTagLibrary::SynapseTag(), strSynapseType, clsFEConnection->getListSynapseParameters());
		delemAERConnection->appendChild(delemSynapse);
	    } 
	}
/* ------------- */	

//	cout << "delemAERConnection->getTagName (): " << XMLString::transcode(delemAERConnection->getTagName()) << endl;

	unsigned int _nNeuronsPre = ClsFESystemManager::Instance()->getFEGroup(strSourceID)->getNumberOfNeurons();
	ListIndexQuadruples listIndexQuadruple = itMapSTT->second;
	for (unsigned int pre = 0; pre < _nNeuronsPre; ++pre) {

	    vector<Indices> vectIndices = listIndexQuadruple.getIndicesByFirst(pre);
	    if(vectIndices.size()>0){
		DOMElement *delemPre = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::pre()));
		delemPre->setAttribute(XMLString::transcode(ClsTagLibrary::index()), 
				       XMLString::transcode(iqrUtils::int2string(pre).c_str()));
		delemAERConnection->appendChild(delemPre);
		
		for(unsigned int ii=0; ii<vectIndices.size(); ii++){
		    DOMElement *delemPost = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::post()));


		    delemPost->setAttribute(XMLString::transcode(ClsTagLibrary::index()), 
					   XMLString::transcode(iqrUtils::int2string(vectIndices[ii].neuronPost).c_str()));


//this should write out the synapse mask index
		    delemPost->setAttribute(XMLString::transcode(ClsTagLibrary::synapse_index()), 
					   XMLString::transcode(iqrUtils::int2string(vectIndices[ii].synapse).c_str()));


		    delemPost->setAttribute(XMLString::transcode(ClsTagLibrary::delay()), 
					   XMLString::transcode(iqrUtils::int2string(vectIndices[ii].delay).c_str()));
		    
		    
//--		    DOMText *dtxtIndexPost = ddocRoot->createTextNode(XMLString::transcode(iqrUtils::int2string(vectIndices[ii].neuronPost).c_str()));
//--		    delemPost->appendChild(dtxtIndexPost);
		    
		    delemPre->appendChild(delemPost);
		}
	    }
	}
	lstDEConnections.push_back(delemAERConnection);
    }
    return lstDEConnections;
}



/** 
 * Write the system into a plain text XML file.
 * 
 * @param _strFileName The filename.
 */
int ClsFESerializer::SerializeDOMTree(string &strSystemCont)  {
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::SerializeDOMTree(string &strSystemCont)" << endl;
    }
    

    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"LS"));
#if XERCES_VERSION_MAJOR >= 3
  DOMLSSerializer* theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#else
    DOMWriter* theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
#endif


    MemBufFormatTarget *myFormTarget = new MemBufFormatTarget();
#if XERCES_VERSION_MAJOR >= 3
    DOMLSOutput* theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    theOutput->setByteStream(myFormTarget);
#endif

    try {
	// do the serialization through DOMWriter::writeNode();
#if XERCES_VERSION_MAJOR >= 3
	theSerializer->write(ddocSystem, theOutput);
#else
	theSerializer->writeNode(myFormTarget, *ddocSystem);
#endif
    }
    catch (const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return -1;
    }
    catch (const DOMException& toCatch) {
	char* message = XMLString::transcode(toCatch.msg);
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
	return -1;
    }
    catch (...) {
	cout << "Unexpected Exception \n" ;
	return -1;
    }

    strSystemCont = (const char*)(myFormTarget->getRawBuffer());
  
    theSerializer->release();
//    delete myErrorHandler;
//    delete myFilter;
    delete myFormTarget;

/* use exceptions.... */

    return 0;
}


DOMElement *ClsFESerializer::addFESystem(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocRoot, ClsFESystem* clsFESystem) {
    // Add the system as the root element.
    DOMElement *delemSystem;
    delemSystem = ddocRoot->getDocumentElement();

    // Add ID as an attribute.
    delemSystem->setAttribute(XMLString::transcode(ClsTagLibrary::IDTag()), 
			      XMLString::transcode(clsFESystem->getSystemID().c_str()));

    // Add the system's parameter list as attributes of the root
    delemSystem->setAttribute(XMLString::transcode(ClsTagLibrary::AuthorTag()),
			      XMLString::transcode(clsFESystem->getParameter(ClsTagLibrary::AuthorTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(XMLString::transcode(ClsTagLibrary::DateTag()),
			      XMLString::transcode(clsFESystem->getParameter(ClsTagLibrary::DateTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(XMLString::transcode(ClsTagLibrary::PortTag()),
			      XMLString::transcode(clsFESystem->getParameter(ClsTagLibrary::PortTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(XMLString::transcode(ClsTagLibrary::CyclesPerSecondTag()),
			      XMLString::transcode(clsFESystem->getParameter(ClsTagLibrary::CyclesPerSecondTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(XMLString::transcode(ClsTagLibrary::SyncPlotsTag()),
			      XMLString::transcode(clsFESystem->getParameter(ClsTagLibrary::SyncPlotsTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(XMLString::transcode(ClsTagLibrary::NameTag()),
			      XMLString::transcode(clsFESystem->getParameter(ClsTagLibrary::NameTag())->getValueAsString().c_str()));

    DOMElement *delemNote = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::NotesTag()));

    delemSystem->appendChild(delemNote);
    DOMText *dtxtNote = ddocRoot->createTextNode(XMLString::transcode(clsFESystem->getParameter(ClsTagLibrary::NotesTag())->getValueAsString().c_str()));
    delemNote->appendChild(dtxtNote);
    
    return delemSystem;
     
}

DOMElement *ClsFESerializer::addFEProcess(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocRoot, ClsFEProcess* clsFEProcess) {
    DOMElement *delemProcess;
    delemProcess = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::ProcessTag()));
    
    string strProcessID = clsFEProcess->getProcessID();
    string strProcessName = clsFEProcess->getProcessName();
    string strProcessPath  = clsFEProcess->getPath();
    string strColor =  clsFEProcess->getColor();

    delemProcess->setAttribute(XMLString::transcode(ClsTagLibrary::IDTag()), 
			       XMLString::transcode(strProcessID.c_str()));

    delemProcess->setAttribute(XMLString::transcode(ClsTagLibrary::NameTag()), 
			       XMLString::transcode(strProcessName.c_str()));


    delemProcess->setAttribute(XMLString::transcode(ClsTagLibrary::HostnameTag()), 
			       XMLString::transcode(""));

    delemProcess->setAttribute(XMLString::transcode(ClsTagLibrary::PortTag()), 
			       XMLString::transcode("54923"));

    delemProcess->setAttribute(XMLString::transcode(ClsTagLibrary::ColorTag()), 
			       XMLString::transcode(strColor.c_str()));
    
    
    delemProcess->setAttribute(XMLString::transcode(ClsTagLibrary::EnableModuleTag()), 
			       XMLString::transcode(clsFEProcess->getEnableModulesAsString().c_str()));
    
    delemProcess->setAttribute(XMLString::transcode(ClsTagLibrary::PathTag()), 
			       XMLString::transcode(strProcessPath.c_str()));

    
    string strModuleType = clsFEProcess->getProcessModuleType();
    if(strModuleType.size()>0){
	DOMElement *delemModule = addParameterizedNode(ddocRoot, ClsTagLibrary::ModuleTag(), strModuleType, clsFEProcess->getListModuleParameters());
	delemProcess->appendChild(delemModule);
	
	/* XRefs */
	addXRefHolders(ddocRoot, delemModule, clsFEProcess);
	
    } else {
//	cerr << "module type undefined" << endl;
    }
    
/* ------- */
    
    DOMElement *delemNote = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::NotesTag()));
    delemProcess->appendChild(delemNote);
    DOMText *dtxtNote = ddocRoot->createTextNode(XMLString::transcode(clsFEProcess->getNotes().c_str()));
    delemNote->appendChild(dtxtNote);


    /* DiagramIcon */
    DOMElement *delemDiagramIcon = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::DiagramIconTag()));
    delemProcess->appendChild(delemDiagramIcon);


    ClsInfoDiagramIcon clsInfoDiagramIcon = ClsFEDiagramManager::Instance()->getDiagramIcon( strProcessID);
    delemDiagramIcon->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramIconXTag()),     
				    XMLString::transcode(clsInfoDiagramIcon.getXAsString().c_str()));	
    delemDiagramIcon->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramIconYTag()),     
				    XMLString::transcode(clsInfoDiagramIcon.getYAsString().c_str()));	
    delemDiagramIcon->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramIconWidthTag()), 
				    XMLString::transcode(clsInfoDiagramIcon.getWidthAsString().c_str()));	
    delemDiagramIcon->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramIconHeightTag()),
				    XMLString::transcode(clsInfoDiagramIcon.getHeightAsString().c_str()));

    return delemProcess;
}


DOMElement* ClsFESerializer::addFEGroup(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocRoot, ClsFEGroup *clsFEGroup) {
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::addFEGroup(ClsFEGroup *clsFEGroup" << endl;
    }
    
    
    DOMElement *delemGroup;
    delemGroup = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::GroupTag()));
    string strGroupID = clsFEGroup->getGroupID();
    string strGroupName = clsFEGroup->getGroupName();
    string strColor =  clsFEGroup->getColor();


    delemGroup->setAttribute(XMLString::transcode(ClsTagLibrary::IDTag()), 
			     XMLString::transcode(strGroupID.c_str()));
    delemGroup->setAttribute(XMLString::transcode(ClsTagLibrary::NameTag()), 
			     XMLString::transcode(strGroupName.c_str()));
    delemGroup->setAttribute(XMLString::transcode(ClsTagLibrary::ColorTag()), 
			     XMLString::transcode(strColor.c_str()));
    


    /* Topology */
    DOMElement *delemTopology = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::TopologyTag()));
    delemGroup->appendChild(delemTopology);


    string strTopologyType = clsFEGroup->getTopology()->Type();
    DOMElement *delemTopologyType = ddocRoot->createElement(XMLString::transcode(strTopologyType.c_str()));
    delemTopology->appendChild(delemTopologyType);


    ClsBaseTopology *clsBaseTopology = clsFEGroup->getTopology();
    
    ParameterList parameterList = clsBaseTopology->getListParameters();

    while (parameterList.size()) {
	string strParamName = parameterList.front()->getName();
	string strParamValue = parameterList.front()->getValueAsString();
	delemTopologyType->setAttribute(XMLString::transcode(strParamName.c_str()), 
					XMLString::transcode(strParamValue.c_str()));
	parameterList.pop_front();
    }
    


    if(dynamic_cast<ClsTopologySparse*>(clsBaseTopology)) {
	list<pair<int, int> > lstPInt = (dynamic_cast<ClsTopologySparse*>(clsBaseTopology))->getList();
	
	list<pair<int, int> >::iterator itLstPInt;
	for(itLstPInt=lstPInt.begin();itLstPInt!=lstPInt.end();itLstPInt++){
	    int iX = (*itLstPInt).first;
	    int iY = (*itLstPInt).second;
	    DOMElement *delemPoint = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::Point()));
	    delemTopologyType->appendChild(delemPoint);
	    delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointX()), XMLString::transcode(iqrUtils::int2string(iX).c_str()));
	    delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointY()), XMLString::transcode(iqrUtils::int2string(iY).c_str()));
	}

    }
       
 
    /* neurons */
    string strNeuronType = clsFEGroup->getGroupNeuronType();
    if(strNeuronType.size()>0){
	DOMElement *delemNeuron = addParameterizedNode(ddocRoot, ClsTagLibrary::NeuronTag(), strNeuronType, clsFEGroup->getListNeuronParameters());
	delemGroup->appendChild(delemNeuron);
	
	/* XRefs UH? 
	   addXRefHolders(ddocRoot, delemNeuron, clsFEGroup);
	*/
	
    } else {
	cerr << "neuron type undefined" << endl;
    }


/* Note */
    DOMElement *delemNote = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::NotesTag()));
    delemGroup->appendChild(delemNote);
    DOMText *dtxtNote = ddocRoot->createTextNode(XMLString::transcode((dynamic_cast<ClsFEGroup*>(clsFEGroup))->getNotes().c_str()));
    delemNote->appendChild(dtxtNote);

    
    /* DiagramIcon */
    DOMElement *delemDiagramIcon = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::DiagramIconTag()));
    delemGroup->appendChild(delemDiagramIcon);
    

    ClsInfoDiagramIcon clsInfoDiagramIcon =  ClsFEDiagramManager::Instance()->getDiagramIcon( strGroupID);
    delemDiagramIcon->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramIconXTag()),     
				    XMLString::transcode(clsInfoDiagramIcon.getXAsString().c_str()));	
    delemDiagramIcon->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramIconYTag()),     
				    XMLString::transcode(clsInfoDiagramIcon.getYAsString().c_str()));	
    delemDiagramIcon->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramIconWidthTag()), 
				    XMLString::transcode(clsInfoDiagramIcon.getWidthAsString().c_str()));	
    delemDiagramIcon->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramIconHeightTag()),
				    XMLString::transcode(clsInfoDiagramIcon.getHeightAsString().c_str()));
    return delemGroup;
    

}


DOMElement *ClsFESerializer::addFEConnection(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocRoot, ClsFEConnection* clsFEConnection) {
    if (bDebugSystemFileWriter) {
	cout << "ClsFESerializer::addFEConnection(DOMDocument *ddocRoot, ClsFEConnection* clsFEConnection)"<< endl;
    }
    
    if(clsFEConnection!=NULL){
	DOMElement *delemConnection;
	delemConnection = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::ConnectionTag()));
	string strConnectionID = clsFEConnection->getConnectionID();
	string strConnectionName = clsFEConnection->getConnectionName();
	
	delemConnection->setAttribute(XMLString::transcode(ClsTagLibrary::IDTag()), 
				      XMLString::transcode(strConnectionID.c_str()));
	
	delemConnection->setAttribute(XMLString::transcode(ClsTagLibrary::NameTag()), 
				      XMLString::transcode(strConnectionName.c_str()));

	delemConnection->setAttribute(XMLString::transcode(ClsTagLibrary::ConnectionSourceTag()), 
				      XMLString::transcode(clsFEConnection->getConnectionSourceID().c_str()));
	
	delemConnection->setAttribute(XMLString::transcode(ClsTagLibrary::ConnectionTargetTag()), 
				      XMLString::transcode(clsFEConnection->getConnectionTargetID().c_str()));
	
	delemConnection->setAttribute(XMLString::transcode(ClsTagLibrary::ConnectionTypeTag()), 
				      XMLString::transcode(clsFEConnection->getConnectionTypeAsString().c_str()));
	
	DOMElement *delemPattern = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::ConnectionPattern()));
	delemConnection->appendChild(delemPattern);
	
	string strPatternType = clsFEConnection->PatternType();
	DOMElement *delemPatternType = ddocRoot->createElement(XMLString::transcode(strPatternType.c_str()));
	delemPattern->appendChild(delemPatternType);
	ClsBasePattern *clsBasePattern = clsFEConnection->getPattern();
	
	ParameterList parameterList0 = clsBasePattern->getListParameters();
	while (parameterList0.size()) {
	    string strParamName = parameterList0.front()->getName();
	    string strParamValue = parameterList0.front()->getValueAsString();
	    delemPatternType->setAttribute(XMLString::transcode(strParamName.c_str()), 
					   XMLString::transcode(strParamValue.c_str()));
	    parameterList0.pop_front();
	}
	
	/*
	  ClsRegion
	  ClsList
	  ClsAll
	  ClsTuples
	*/
	
	if(!strPatternType.compare(ClsTagLibrary::ConnectionPatternMapped()) || !strPatternType.compare(ClsTagLibrary::ConnectionPatternForeach())){
	    
	    ClsBaseSubPopulation *clsBaseSubPopulationSource = clsBasePattern->getSourcePopulation();	
	    if(clsBaseSubPopulationSource!=NULL){
		
		if(dynamic_cast<ClsRegion*>(clsBaseSubPopulationSource)){
		    ClsRegion *clsRegion = dynamic_cast<ClsRegion*>(clsBaseSubPopulationSource);
		    DOMElement *delemRegion = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::SelectorRegion()));
		    delemPatternType->appendChild(delemRegion);
		    int iXStart = clsRegion->getXStart();
		    int iYStart = clsRegion->getYStart();
		    int iWidth = clsRegion->getWidth();
		    int iHeight = clsRegion->getHeight();
		    
		    delemRegion->setAttribute(XMLString::transcode(ClsTagLibrary::RegionXStart()), XMLString::transcode(iqrUtils::int2string(iXStart).c_str()));
		    delemRegion->setAttribute(XMLString::transcode(ClsTagLibrary::RegionYStart()), XMLString::transcode(iqrUtils::int2string(iYStart).c_str()));
		    delemRegion->setAttribute(XMLString::transcode(ClsTagLibrary::RegionWidth()), XMLString::transcode(iqrUtils::int2string(iWidth).c_str()));
		    delemRegion->setAttribute(XMLString::transcode(ClsTagLibrary::RegionHeight()), XMLString::transcode(iqrUtils::int2string(iHeight).c_str()));
		}
		else if(dynamic_cast<ClsList*>(clsBaseSubPopulationSource)){
		    DOMElement *delemList = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::SelectorList()));
		    delemPatternType->appendChild(delemList);
		    ClsList *clsList = dynamic_cast<ClsList*>(clsBaseSubPopulationSource);
		    
		    list<pair<int, int> > lstPInt = clsList->getData();
		    list<pair<int, int> >::iterator itLstPInt;
		    for(itLstPInt=lstPInt.begin();itLstPInt!=lstPInt.end();itLstPInt++){
			int iX = (*itLstPInt).first;
			int iY = (*itLstPInt).second;
			DOMElement *delemPoint = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::Point()));
			delemList->appendChild(delemPoint);
			delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointX()), XMLString::transcode(iqrUtils::int2string(iX).c_str()));
			delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointY()), XMLString::transcode(iqrUtils::int2string(iY).c_str()));
		    }
		}
		else if(dynamic_cast<ClsAll*>(clsBaseSubPopulationSource)){
		    DOMElement *delemAll = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::SelectorAll()));
		    delemPatternType->appendChild(delemAll);
		}
	    }
	    
	    ClsBaseSubPopulation *clsBaseSubPopulationTarget = clsBasePattern->getTargetPopulation();	
	    if(clsBaseSubPopulationTarget!=NULL){
		
		if(dynamic_cast<ClsRegion*>(clsBaseSubPopulationTarget)){
		    ClsRegion *clsRegion = dynamic_cast<ClsRegion*>(clsBaseSubPopulationTarget);
		    DOMElement *delemRegion = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::SelectorRegion()));
		    delemPatternType->appendChild(delemRegion);
		    int iXStart = clsRegion->getXStart();
		    int iYStart = clsRegion->getYStart();
		    int iWidth = clsRegion->getWidth();
		    int iHeight = clsRegion->getHeight();
		    
		    delemRegion->setAttribute(XMLString::transcode(ClsTagLibrary::RegionXStart()), XMLString::transcode(iqrUtils::int2string(iXStart).c_str()));
		    delemRegion->setAttribute(XMLString::transcode(ClsTagLibrary::RegionYStart()), XMLString::transcode(iqrUtils::int2string(iYStart).c_str()));
		    delemRegion->setAttribute(XMLString::transcode(ClsTagLibrary::RegionWidth()), XMLString::transcode(iqrUtils::int2string(iWidth).c_str()));
		    delemRegion->setAttribute(XMLString::transcode(ClsTagLibrary::RegionHeight()), XMLString::transcode(iqrUtils::int2string(iHeight).c_str()));
		}
		else if(dynamic_cast<ClsList*>(clsBaseSubPopulationTarget)){
		    DOMElement *delemList = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::SelectorList()));
		    delemPatternType->appendChild(delemList);
		    ClsList *clsList = dynamic_cast<ClsList*>(clsBaseSubPopulationTarget);
		    
		    list<pair<int, int> > lstPInt = clsList->getData();
		    list<pair<int, int> >::iterator itLstPInt;
		    for(itLstPInt=lstPInt.begin();itLstPInt!=lstPInt.end();itLstPInt++){
			int iX = (*itLstPInt).first;
			int iY = (*itLstPInt).second;
			DOMElement *delemPoint = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::Point()));
			delemList->appendChild(delemPoint);
			delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointX()), XMLString::transcode(iqrUtils::int2string(iX).c_str()));
			delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointY()), XMLString::transcode(iqrUtils::int2string(iY).c_str()));
		    }
		}
		else if(dynamic_cast<ClsAll*>(clsBaseSubPopulationTarget)){
		    DOMElement *delemAll = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::SelectorAll()));
		    delemPatternType->appendChild(delemAll);
		}
	    }
	    
	    
	    
	}
	else if(!strPatternType.compare(ClsTagLibrary::ConnectionPatternTuples())){
	    ClsTuples *clsTuples = clsBasePattern->getTuples();
	    if(clsTuples!=NULL){
		list<pair<tListOfPairs, tListOfPairs> > lstTuples = clsTuples->getData();
		list<pair<tListOfPairs, tListOfPairs> >::iterator itLPLP;
		
		for(itLPLP=lstTuples.begin(); itLPLP!=lstTuples.end(); itLPLP++){
		    pair<tListOfPairs, tListOfPairs> pairLOP = *itLPLP;
		    DOMElement *delemTuple = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::Tuple()));
		    delemPatternType->appendChild(delemTuple);
		    
		    tListOfPairs tlopSource = pairLOP.first;
		    DOMElement *delemSourceList = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::SourceList()));
		    delemTuple->appendChild(delemSourceList);
		    tListOfPairs::iterator itLPSource;
		    for(itLPSource=tlopSource.begin(); itLPSource!=tlopSource.end(); itLPSource++){
			int iX = (*itLPSource).first;
			int iY = (*itLPSource).second;
			
			DOMElement *delemPoint = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::Point()));
			delemSourceList->appendChild(delemPoint);
			delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointX()), XMLString::transcode(iqrUtils::int2string(iX).c_str()));
			delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointY()), XMLString::transcode(iqrUtils::int2string(iY).c_str()));
		    }
		    
		    tListOfPairs tlopTarget = pairLOP.second;
		    DOMElement *delemTargetList = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::TargetList()));
		    delemTuple->appendChild(delemTargetList);
		    tListOfPairs::iterator itLPTarget;
		    for(itLPTarget=tlopTarget.begin(); itLPTarget!=tlopTarget.end(); itLPTarget++){
			int iX = (*itLPTarget).first;
			int iY = (*itLPTarget).second;
			
			DOMElement *delemPoint = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::Point()));
			delemTargetList->appendChild(delemPoint);
			delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointX()), XMLString::transcode(iqrUtils::int2string(iX).c_str()));
			delemPoint->setAttribute(XMLString::transcode(ClsTagLibrary::PointY()), XMLString::transcode(iqrUtils::int2string(iY).c_str()));
		    }
		}
	    }
	}
	
	
/* Arborization */
	DOMElement *delemArborization = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::ConnectionArborization()));
	delemConnection->appendChild(delemArborization);
	string strArborizationType = clsFEConnection->getArborization()->Type();
	DOMElement *delemArborizationType = ddocRoot->createElement(XMLString::transcode(strArborizationType.c_str()));
	delemArborization->appendChild(delemArborizationType);
	ParameterList parameterList = clsFEConnection->getArborization()->getListParameters();
	while (parameterList.size()) {
	    string strParamName = parameterList.front()->getName();
	    string strParamValue = parameterList.front()->getValueAsString();
	    delemArborizationType->setAttribute(XMLString::transcode(strParamName.c_str()), 
						XMLString::transcode(strParamValue.c_str()));
	    parameterList.pop_front();
	}
/* end Arborization */
	
	
/* AttenuationFunction */
	DOMElement *delemAttenuationFunction = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::ConnectionAttenuationFunction()));
	delemConnection->appendChild(delemAttenuationFunction);
	string strAttenuationFunctionType = clsFEConnection->getAttenuationFunction()->Type();
	DOMElement *delemAttenuationFunctionType = ddocRoot->createElement(XMLString::transcode(strAttenuationFunctionType.c_str()));
	delemAttenuationFunction->appendChild(delemAttenuationFunctionType);
	ParameterList parameterListAttenuationFunction = clsFEConnection->getAttenuationFunction()->getListParameters();
	while (parameterListAttenuationFunction.size()) {
	    string strParamName = parameterListAttenuationFunction.front()->getName();
	    string strParamValue = parameterListAttenuationFunction.front()->getValueAsString();
	    delemAttenuationFunctionType->setAttribute(XMLString::transcode(strParamName.c_str()), 
						  XMLString::transcode(strParamValue.c_str()));
	    parameterListAttenuationFunction.pop_front();
	}
/*   end AttenuationFunction */
    
/* DelayFunction */
	DOMElement *delemDelayFunction = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::ConnectionDelayFunction()));
	delemConnection->appendChild(delemDelayFunction);    
	string strDelayFunctionType = clsFEConnection->getDelayFunction()->Type();
	DOMElement *delemDelayFunctionType = ddocRoot->createElement(XMLString::transcode(strDelayFunctionType.c_str()));
	delemDelayFunction->appendChild(delemDelayFunctionType);
	ParameterList parameterListDelayFunction = clsFEConnection->getDelayFunction()->getListParameters();
	while (parameterListDelayFunction.size()) {
	    string strParamName = parameterListDelayFunction.front()->getName();
	    string strParamValue = parameterListDelayFunction.front()->getValueAsString();
	    delemDelayFunctionType->setAttribute(XMLString::transcode(strParamName.c_str()), 
						 XMLString::transcode(strParamValue.c_str()));
	    parameterListDelayFunction.pop_front();
	}
/* end DelayFunction */
	
/* synapses */
	string strSynapseType = clsFEConnection->getConnectionSynapseType();
	if(strSynapseType.size()>0){
	    DOMElement *delemSynapse = addParameterizedNode(ddocRoot, ClsTagLibrary::SynapseTag(), strSynapseType, clsFEConnection->getListSynapseParameters());
	    delemConnection->appendChild(delemSynapse);
	    
	    /* XRefs */
	    addXRefHolders(ddocRoot, delemSynapse, clsFEConnection);
	    
	} else {
	    cerr << "synapse type undefined" << endl;
	}
	
	
/* Note */
	DOMElement *delemNote = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::NotesTag()));
	delemConnection->appendChild(delemNote);
	DOMText *dtxtNote = ddocRoot->createTextNode(XMLString::transcode((dynamic_cast<ClsFEConnection*>(clsFEConnection))->getNotes().c_str()));
	delemNote->appendChild(dtxtNote);
	
	
/* DiagramLine */
	DOMElement *delemDiagramLine = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::DiagramLineTag()));
	delemConnection->appendChild(delemDiagramLine);
	
	ClsInfoDiagramLine clsInfoDiagramLine =  ClsFEDiagramManager::Instance()->getDiagramLine( strConnectionID);
	delemDiagramLine->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramLineAPSourceTag()),     
					XMLString::transcode(iqrUtils::int2string(clsInfoDiagramLine.getAPSource()).c_str()));	
	delemDiagramLine->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramLineAPTargetTag()),     
					XMLString::transcode(iqrUtils::int2string(clsInfoDiagramLine.getAPTarget()).c_str()));	
	
	vector<vector<int> > vectorPoints = clsInfoDiagramLine.getPoints();
	
	for(unsigned int ii=0; ii<vectorPoints.size(); ii++){
	    int iX = vectorPoints[ii][0];
	    int iY = vectorPoints[ii][1];
	    DOMElement *delemPoint = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::DiagramLinePointTag()));
	    
	    delemPoint->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramLinePointXTag()),     
				      XMLString::transcode(iqrUtils::int2string(iX).c_str()));	
	    delemPoint->setAttribute( XMLString::transcode(ClsTagLibrary::DiagramLinePointYTag()),     
				      XMLString::transcode(iqrUtils::int2string(iY).c_str()));	
	    delemDiagramLine->appendChild(delemPoint);
	}
	return delemConnection;
    }    
}

DOMElement* ClsFESerializer::addParameterizedNode(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocRoot, string strNodeTag, string strNodeType, ParameterList parameterList){

    DOMElement *delemParameterizedNode;
    delemParameterizedNode = ddocRoot->createElement(XMLString::transcode(strNodeTag.c_str()));

    delemParameterizedNode->setAttribute(XMLString::transcode(ClsTagLibrary::NameTag()), 
					 XMLString::transcode(strNodeType.c_str()));
	
    while (parameterList.size()) {
	string strParamName = parameterList.front()->getName();
	string strParamValue = parameterList.front()->getValueAsString();
	    
	DOMElement *delemParameter = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::ParameterTag()));
	    
	delemParameter->setAttribute(XMLString::transcode(ClsTagLibrary::NameTag()), 
				     XMLString::transcode(strParamName.c_str()));
	    
	delemParameter->setAttribute(XMLString::transcode(ClsTagLibrary::ValueTag()), 
				     XMLString::transcode(strParamValue.c_str()));
	    
	delemParameterizedNode->appendChild(delemParameter);
	parameterList.pop_front();
    }
    return delemParameterizedNode;
}

void ClsFESerializer::addXRefHolders(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *ddocRoot, DOMElement *delemParent, ClsItem* itemParent){
    list<string> lstXRefHolders;
    if(dynamic_cast<ClsFEProcess*>(itemParent)){
	lstXRefHolders = dynamic_cast<ClsFEProcess*>(itemParent)->getListModuleXRefHolders();
    }else if(dynamic_cast<ClsFEConnection*>(itemParent)){
	lstXRefHolders = dynamic_cast<ClsFEConnection*>(itemParent)->getListSynapseXRefHolders();
    }

    list<string>::iterator itXRefHolders;
    for(itXRefHolders=lstXRefHolders.begin(); itXRefHolders!=lstXRefHolders.end(); itXRefHolders++){
	const ClsXRefHolder* clsXRefHolder = NULL;
	if(dynamic_cast<ClsFEProcess*>(itemParent)){
	    clsXRefHolder = dynamic_cast<ClsFEProcess*>(itemParent)->getModuleXRefHolder(*itXRefHolders);
	}else if(dynamic_cast<ClsFEConnection*>(itemParent)){
	    clsXRefHolder = dynamic_cast<ClsFEConnection*>(itemParent)->getSynapseXRefHolder(*itXRefHolders);
	}
	if(clsXRefHolder!=NULL){
	    DOMElement *delemXRefHolder = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::XRefHolderTag()));
	    string strXRefHolderName = clsXRefHolder->getName();
	    delemXRefHolder->setAttribute(XMLString::transcode(ClsTagLibrary::NameTag()), 
					  XMLString::transcode(strXRefHolderName.c_str()));
	    
	    XRefList xRefList = clsXRefHolder->getListXRefs();

	    XRefList::iterator itXRef;
	    for(itXRef=xRefList.begin(); itXRef!=xRefList.end(); itXRef++){
		string strXRefName = (*itXRef)->getName();
		string strXRefTarget = (*itXRef)->getTarget();
		
		DOMElement *delemXRef = ddocRoot->createElement(XMLString::transcode(ClsTagLibrary::XRefTag()));
		delemXRef->setAttribute(XMLString::transcode(ClsTagLibrary::NameTag()), 
					XMLString::transcode(strXRefName.c_str()));
		
		delemXRef->setAttribute(XMLString::transcode(ClsTagLibrary::TargetTag()), 
					XMLString::transcode(strXRefTarget.c_str()));
		delemXRefHolder->appendChild(delemXRef);
	    }
	    delemParent->appendChild(delemXRefHolder);
	}
    }  
}


bool ClsFESerializer::IsDOMTreeValid() {

//+++     // For verification, write the system into a string stream in XML
//+++     // format.  At present, Xerces does not support verification of
//+++     // the DOM tree using a DTD.
//+++     std::ostrstream outputBuffer;
//+++     ClsStrstreamTarget formatTarget(outputBuffer);
//+++     
//+++     static XMLFormatter::UnRepFlags gUnRepFlags = XMLFormatter::UnRep_CharRef;
//+++     
//+++     try {
//+++ 	
//+++ 	DOMString encNameStr("UTF-8");
//+++ 	unsigned int lent = encNameStr.length();
//+++ 	gEncodingName = new XMLCh[lent + 1];
//+++ 	XMLString::copyNString(gEncodingName, encNameStr.rawBuffer(), lent);
//+++ 	gEncodingName[lent] = 0;
//+++ 	
//+++ 	gFormatter = new XMLFormatter(gEncodingName, &formatTarget, 
//+++ 				      XMLFormatter::NoEscapes, gUnRepFlags);
//+++ 
//+++ 	outputBuffer << ddocSystem << std::ends;
//+++     }
//+++     catch (XMLException& e) {
//+++ 	cerr << "An error occurred during creation of output transcoder."
//+++ 	     << endl
//+++ 	     << " Message: "
//+++ 	     << DOMString(e.getMessage())
//+++ 	     << endl;
//+++     }
//+++     
//+++     delete gFormatter;
//+++     delete[] gEncodingName;
//+++     
//+++     DOMParser parser;
//+++     
//+++     static const char*  gMemBufId    = ClsTagLibrary::SystemTag();
//+++ 
//+++     // Initialize the XML input buffer with the string created above.
//+++     const char*         gXMLInMemBuf = outputBuffer.str();
//+++     outputBuffer.freeze(false);
//+++ 
//+++     // @@@ 26/09/01 Can the reinterpret cast be replaced with
//+++     // something safer here?
//+++     MemBufInputSource* memBufIS 
//+++ 	= new MemBufInputSource(reinterpret_cast<const XMLByte*>(gXMLInMemBuf), 
//+++ 				strlen(gXMLInMemBuf), 
//+++ 				gMemBufId, 
//+++ 				false);
//+++     
//+++     static bool                  gDoNamespaces = false;
//+++     static bool                  gDoCreate     = true;
//+++     static bool                  gDoDecl       = true;
//+++     static DOMParser::ValSchemes gValScheme    = DOMParser::Val_Auto;
//+++     
//+++     parser.setValidationScheme(gValScheme);    
//+++     parser.setDoNamespaces(gDoNamespaces);
//+++     parser.setCreateEntityReferenceNodes(gDoCreate);
//+++     parser.setToCreateXMLDeclTypeNode(gDoDecl);
//+++ 
//+++     DOMErrorReporter errReporter;
//+++     parser.setErrorHandler(&errReporter);
//+++     
//+++     //  Parse the XML string.
//+++     bool bErrorsOccured = false;
//+++     try {
//+++ 	parser.parse(*memBufIS);
//+++     }
//+++     
//+++     catch (const XMLException& e) { 
//+++ 	cerr << "An error occured during parsing." 
//+++ 	     << endl
//+++ 	     << " Message: "
//+++ 	     << DOMString(e.getMessage()) 
//+++ 	     << endl;
//+++ 	bErrorsOccured = true;
//+++     }
//+++         
//+++     catch (const DOMDOMException& e) {
//+++ 	cerr << "An error occured during parsing." 
//+++ 	     << endl
//+++ 	     << " Message: "
//+++ 	     << DOMString(e.msg) 
//+++ 	     << endl;
//+++ 	bErrorsOccured = true;
//+++     }
//+++     
//+++     catch (...) {
//+++ 	cerr << "An error occured during parsing." << endl;
//+++ 	bErrorsOccured = true;
//+++     }
//+++     
//+++     // Return true if no errors occured.  If any errors occured, DOM
//+++     // tree is not valid - return false.
//+++     return !(bErrorsOccured || errReporter.getSawErrors());
//+++ 
    return true;
    
}

bool ClsFESerializer::IsFileValid(string) { //  _strFileName

//     qDebug("ClsFESerializer::IsFileValid: not implemented");
//     return true;

//+++     DOMParser parser;
//+++     
//+++     static bool                  gDoNamespaces = false;
//+++     static bool                  gDoCreate     = true;
//+++     static bool                  gDoDecl       = true;
//+++     static DOMParser::ValSchemes gValScheme    = DOMParser::Val_Auto;
//+++     
//+++     parser.setValidationScheme(gValScheme);    
//+++     parser.setDoNamespaces(gDoNamespaces);
//+++     parser.setCreateEntityReferenceNodes(gDoCreate);
//+++     parser.setToCreateXMLDeclTypeNode(gDoDecl);
//+++ 
//+++     DOMErrorReporter errReporter;
//+++     parser.setErrorHandler(&errReporter);
//+++     
//+++     //  Parse the XML string.
//+++     bool bErrorsOccured = false;
//+++     try {
//+++ 	parser.parse(_strFileName.c_str());
//+++     }
//+++     
//+++     catch (const XMLException& e) { 
//+++ 	cerr << "An error occured during parsing." 
//+++ 	     << endl
//+++ 	     << " Message: "
//+++ 	     << DOMString(e.getMessage()) 
//+++ 	     << endl;
//+++ 	bErrorsOccured = true;
//+++     }
//+++         
//+++     catch (const DOMDOMException& e) {
//+++ 	cerr << "An error occured during parsing." 
//+++ 	     << endl
//+++ 	     << " Message: "
//+++ 	     << DOMString(e.msg) 
//+++ 	     << endl;
//+++ 	bErrorsOccured = true;
//+++      }
//+++     
//+++     catch (...) {
//+++ 	cerr << "An error occured during parsing." << endl;
//+++ 	bErrorsOccured = true;
//+++     }
//+++     
//+++     // Return true if no errors occured.  If any errors occured, DOM
//+++     // tree is not valid - return false.
//+++     return !(bErrorsOccured || errReporter.getSawErrors());
//+++ 
    return true;
}



void ClsFESerializer::writeExternalProcesses() {
    list<string>::iterator it;
    for(it=lstExternalProcesses.begin(); it!=lstExternalProcesses.end();it++){
	string strID = (*it);
	ClsFEProcess* clsFEProcessTemp = ClsFESystemManager::Instance()->getFEProcess(strID);
	if(clsFEProcessTemp!=NULL){
	    string strPath = clsFEProcessTemp->getPath();
//	    cout << "PATH: " << strPath << endl;
	    list<string> lst;
	    lst.push_back(strID);
	    string strTxtProc = getProcessesAsXML(lst);

	    /* WE NEED SOME CHECK HERE
	       valid path???
	    */

	    ofstream out(strPath.c_str());
	    out << strTxtProc << endl;
	    close(out);
	}
    }

    

};



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
