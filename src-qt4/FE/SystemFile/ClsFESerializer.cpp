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


    //DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(QString::fromStdString((const char*)"Range"));

    QDomImplementation impl;
    QDomDocument ddocClipboard = impl.createDocument(0, QString::fromStdString(ClsTagLibrary::iqrEntityTag()), NULL);
    QDomElement delemClipboard = ddocClipboard.createElement(QString::fromStdString(ClsTagLibrary::iqrEntityTag()));
    delemClipboard.setAttribute(QString::fromStdString(ClsTagLibrary::ClipboardContentsType()), QString::fromStdString(ClsTagLibrary::ProcessTag()));


    list<string>::iterator it;
    for(it=lstIDs.begin(); it!=lstIDs.end(); it++){
    string strID = (*it);

    QDomElement *delemProcess = addFEProcess(&ddocClipboard, ClsFESystemManager::Instance()->getFEProcess(strID));
    delemClipboard.appendChild(*delemProcess);

    map<string, ClsFEGroup*> mapGroups = ClsFESystemManager::Instance()->getMapFEGroups();
    map<string, ClsFEGroup*>::iterator itmapFEGroups;
    for (itmapFEGroups = mapGroups.begin(); itmapFEGroups != mapGroups.end(); ++itmapFEGroups){
        if(!strID.compare((itmapFEGroups->second)->getProcessID())) {
        QDomElement *delemGroup = addFEGroup(&ddocClipboard, itmapFEGroups->second);
        delemProcess->appendChild(*delemGroup);
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
        QDomElement *delemConnection = addFEConnection(&ddocClipboard, itmapFEConnections->second);
        delemClipboard.appendChild(*delemConnection);
        }
    }
    }

/*
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
*/
    QBuffer myFormTarget();
    QTextStream output(&myFormTarget);
    delemClipboard.save(output, 4);

/*
    try {
    // do the serialization through DOMWriter::writeNode();
#if XERCES_VERSION_MAJOR >= 3
    theSerializer->write(delemClipboard, theOutput);
#else
    theSerializer->writeNode(myFormTarget, *delemClipboard);
#endif


    }
    catch (const XMLException& toCatch) {
    char* message = QString::fromStdString(toCatch.getMessage());
    cout << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return "";
    }
    catch (const DOMException& toCatch) {
    char* message = QString::fromStdString(toCatch.msg);
    cout << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return "";
    }
    catch (...) {
    cout << "Unexpected Exception \n" ;
    return "";
    }
*/
    string strProcessXML = output.readAll().toStdString();

    return strProcessXML;
}

string ClsFESerializer::getConnectionsAsXML(list<string> lstIDs) {
    if (bDebugSystemFileWriter) {
    cout << "ClsFESerializer::getConnectionsAsXML(list<string> lstIDs )" << endl;
    }

    //DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(QString::fromStdString((const char*)"Range"));
    QDomImplementation impl;
    QDomDocument ddocClipboard = impl.createDocument(0, QString::fromStdString(ClsTagLibrary::iqrEntityTag()), NULL);
    QDomElement delemClipboard = ddocClipboard.createElement(QString::fromStdString(ClsTagLibrary::iqrEntityTag()));
    delemClipboard.setAttribute(QString::fromStdString(ClsTagLibrary::ClipboardContentsType()), QString::fromStdString(ClsTagLibrary::ConnectionTag()));

    list<string>::iterator it;
    for(it=lstIDs.begin(); it!=lstIDs.end(); it++){
    string strID = (*it);
    QDomElement *delemConnection = addFEConnection(&ddocClipboard, ClsFESystemManager::Instance()->getFEConnection(strID));
    delemClipboard.appendChild(*delemConnection);
    }

    QBuffer myFormTarget();
    QTextStream output(&myFormTarget);
    delemClipboard.save(output, 4);

/**
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
    char* message = QString::fromStdString(toCatch.getMessage());
    cout << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return "";
    }
    catch (const DOMException& toCatch) {
    char* message = QString::fromStdString(toCatch.msg);
    cout << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return "";
    }
    catch (...) {
    cout << "Unexpected Exception \n" ;
    return "";
    }
*/

    string strConnectionXML = output.readAll().toStdString();

    return strConnectionXML;
}

string ClsFESerializer::getGroupsAsXML(list<string> lstIDs) {
    if (bDebugSystemFileWriter) {
    cout << "ClsFESerializer::getGroupAsXML(string strGroupID)" << endl;
    }


    //DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(QString::fromStdString((const char*)"Range"));
    QDomImplementation impl;
    QDomDocument ddocClipboard = impl.createDocument(0, QString::fromStdString(ClsTagLibrary::iqrEntityTag()), NULL);
    QDomElement delemClipboard = ddocClipboard.createElement(QString::fromStdString(ClsTagLibrary::iqrEntityTag()));
    delemClipboard.setAttribute(QString::fromStdString(ClsTagLibrary::ClipboardContentsType()), QString::fromStdString(ClsTagLibrary::GroupTag()));

    list<string>::iterator it;
    for(it=lstIDs.begin(); it!=lstIDs.end(); it++){
    string strID = (*it);
    QDomElement *delemGroup = addFEGroup(&ddocClipboard, ClsFESystemManager::Instance()->getFEGroup(strID));
    delemClipboard.appendChild(*delemGroup);
    }
/**
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
    char* message = QString::fromStdString(toCatch.getMessage());
    cout << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return "";
    }
    catch (const DOMException& toCatch) {
    char* message = QString::fromStdString(toCatch.msg);
    cout << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return "";
    }
    catch (...) {
    cout << "Unexpected Exception \n" ;
    return "";
    }
*/
    QBuffer myFormTarget();
    QTextStream output(&myFormTarget);
    delemClipboard.save(output, 4);

    string strGroupXML = output.readAll().toStdString();

    return strGroupXML;
}


string ClsFESerializer::getGroupsWidthConnectionsAsXML(list<string> lstIDGroups, list<string> lstIDConnections){
    if (bDebugSystemFileWriter) {
    cout << "ClsFESerializer::getGroupsWidthConnectionsAsXML(list<string> lstIDGroups, list<string> lstIDConnections)" << endl;
    }

    QDomImplementation impl;
    QDomDocument ddocClipboard = impl.createDocument(0, QString::fromStdString(ClsTagLibrary::iqrEntityTag()), NULL);
    QDomElement delemClipboard = ddocClipboard.createElement(QString::fromStdString(ClsTagLibrary::iqrEntityTag()));
    delemClipboard.setAttribute(QString::fromStdString(ClsTagLibrary::ClipboardContentsType()), QString::fromStdString(ClsTagLibrary::GroupsWithConnectionsTag()));

    list<string>::iterator itGroup;
    for(itGroup=lstIDGroups.begin(); itGroup!=lstIDGroups.end(); itGroup++){
        string strID = (*itGroup);
        QDomElement *delemGroup = addFEGroup(&ddocClipboard, ClsFESystemManager::Instance()->getFEGroup(strID));
        delemClipboard.appendChild(*delemGroup);
    }

    list<string>::iterator itConnection;
    for(itConnection=lstIDConnections.begin(); itConnection!=lstIDConnections.end(); itConnection++){
        string strID = (*itConnection);
        QDomElement *delemConnection = addFEConnection(&ddocClipboard, ClsFESystemManager::Instance()->getFEConnection(strID));
        delemClipboard.appendChild(*delemConnection);
    }
/**
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
        char* message = QString::fromStdString(toCatch.getMessage());
        cout << "Exception message is: \n"
         << message << "\n";
        XMLString::release(&message);
        return "";
    }
    catch (const DOMException& toCatch) {
        char* message = QString::fromStdString(toCatch.msg);
        cout << "Exception message is: \n"
         << message << "\n";
        XMLString::release(&message);
        return "";
    }
    catch (...) {
        cout << "Unexpected Exception \n" ;
        return "";
    }
*/
    QBuffer myFormTarget();
    QTextStream output(&myFormTarget);
    delemClipboard.save(output, 4);

    string strGroupsWithConnectionsXML = output.readAll().toStdString();

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

    QDomImplementation impl;

    QDomDocumentType dtd = impl.createDocumentType(QString::fromStdString(ClsTagLibrary::SystemTag()),
                            QString::fromStdString("-//INI/iqr421"),
                            QString::fromStdString("iqrSystem.dtd"));

    ddocSystem = impl.createDocument(0, QString::fromStdString(ClsTagLibrary::SystemTag()), dtd);

//     if (bDebugSystemFileWriter) {
// 	qDebug("ClsFESerializer::CreateDOMTree:"
// 	       " system has %d processes, %d connections",
// 	       _pclsSystem->Elements().count(),
// 	       _pclsSystem->Connections().count());
//     }

    QDomElement *delemSystem = addFESystem(&ddocSystem, ClsFESystemManager::Instance()->getFESystem());
    map<string, ClsFEProcess*> mapProcesses = ClsFESystemManager::Instance()->getMapFEProcesses();
    map<string, ClsFEGroup*> mapGroups = ClsFESystemManager::Instance()->getMapFEGroups();
    map<string, ClsFEProcess*>::iterator itmapPrcs;
    for (itmapPrcs = mapProcesses.begin(); itmapPrcs != mapProcesses.end(); ++itmapPrcs){
    ClsFEProcess* clsFEProcessTemp = itmapPrcs->second;
    string strProcessID = clsFEProcessTemp->getProcessID();
    string strPath = clsFEProcessTemp->getPath();
    if(strPath.length()<=0){
        QDomElement *delemProcess = addFEProcess(&ddocSystem, clsFEProcessTemp);
        delemSystem->appendChild(*delemProcess);

        map<string, ClsFEGroup*>::iterator itmapFEGroups;
        for (itmapFEGroups = mapGroups.begin(); itmapFEGroups != mapGroups.end(); ++itmapFEGroups){
        if(!strProcessID.compare((itmapFEGroups->second)->getProcessID())) {
            QDomElement *delemGroup = addFEGroup(&ddocSystem, itmapFEGroups->second);
            delemProcess->appendChild(*delemGroup);
        }
        }
    } else {
        /* WE HAVE TO CREATE AN "EMPTY" PROCESS */
        QDomElement *delemProcess = addFEProcess(&ddocSystem, clsFEProcessTemp);
        delemSystem->appendChild(*delemProcess);
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
        QDomElement *delemConnection = addFEConnection(&ddocSystem, itmapConnection->second);
        delemSystem->appendChild(*delemConnection);
    } else {
        if(find(lstExternalProcesses.begin(), lstExternalProcesses.end(), strSourceProcessID)==lstExternalProcesses.end()){
        QDomElement *delemConnection = addFEConnection(&ddocSystem, itmapConnection->second);
        delemSystem->appendChild(*delemConnection);
        }
    }
    }


    if (bDebugSystemFileWriter) {
    cout << "exit ClsFESerializer::CreateDOMTree(bool _bIncludeXMLDeclNode)" << endl;
    }

}


int ClsFESerializer::serializeToAER(string strExportFilename){

    if(!bXMLPlatformInitialized){
        bXMLPlatformInitialized = true;
    }

    QDomImplementation impl;

    QDomDocumentType dtd = impl.createDocumentType(QString::fromStdString(ClsTagLibrary::AERSystemTag()), //<-FIX
                            QString::fromStdString("-//INI/iqr421"),
                            QString::fromStdString("iqrSystem.dtd"));

    QDomDocument ddocAERSystem = impl.createDocument(0, QString::fromStdString(ClsTagLibrary::AERSystemTag()), dtd);

    QDomElement *delemSystem = addFESystem(&ddocAERSystem, ClsFESystemManager::Instance()->getFESystem());

    map<string, ClsFEProcess*> mapProcesses = ClsFESystemManager::Instance()->getMapFEProcesses();
    map<string, ClsFEGroup*> mapGroups = ClsFESystemManager::Instance()->getMapFEGroups();
    map<string, ClsFEProcess*>::iterator itmapPrcs;
    for (itmapPrcs = mapProcesses.begin(); itmapPrcs != mapProcesses.end(); ++itmapPrcs){
    QDomElement *delemProcess = addFEProcess(&ddocAERSystem, itmapPrcs->second);
    delemSystem->appendChild(*delemProcess);

    string strProcessID = itmapPrcs->second->getProcessID();

    map<string, ClsFEGroup*>::iterator itmapFEGroups;
    for (itmapFEGroups = mapGroups.begin(); itmapFEGroups != mapGroups.end(); ++itmapFEGroups){
        if(itmapFEGroups->second==NULL){
        cout << "NNUUULLLL" << endl;

        }
        if(!strProcessID.compare((itmapFEGroups->second)->getProcessID())) {
        QDomElement *delemGroup = addFEGroup(&ddocAERSystem, itmapFEGroups->second);
        delemProcess->appendChild(*delemGroup);
        }
    }
    }
    list<QDomElement*> lstDEConnections = getAERConnections(&ddocAERSystem);
    list<QDomElement*>::iterator it;
    for(it=lstDEConnections.begin(); it!=lstDEConnections.end();it++){
    delemSystem->appendChild((*it));
    }

    QFile myFormTarget(QString::fromStdString(strExportFilename.c_str()));
    QTextStream output(&myFormTarget);
    ddocAERSystem.save(output, 4);

/*
//    DOMImplementation *
    impl = DOMImplementationRegistry::getDOMImplementation(QString::fromStdString((const char*)"LS"));
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
    char* message = QString::fromStdString(toCatch.getMessage());
    cout << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return -1;
    }
    catch (const DOMException& toCatch) {
    char* message = QString::fromStdString(toCatch.msg);
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

list<QDomElement*> ClsFESerializer::getAERConnections(QDomDocument* ddocRoot){
    if (bDebugSystemFileWriter) {
    cout << "ClsFESerializer::getAERConnections(DOMDocument *ddocRoot)" << endl;
    }

    list<QDomElement*> lstDEConnections;

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


    QDomElement delemAERConnection = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::AERConnectionTag()));

    delemAERConnection.setAttribute(QString::fromStdString(ClsTagLibrary::IDTag()),
                     QString::fromStdString(itMapSTT->first.ID.c_str()));

    delemAERConnection.setAttribute(QString::fromStdString(ClsTagLibrary::ConnectionSourceTag()),
                     QString::fromStdString(itMapSTT->first.source.c_str()));

    delemAERConnection.setAttribute(QString::fromStdString(ClsTagLibrary::ConnectionTargetTag()),
                     QString::fromStdString(itMapSTT->first.target.c_str()));

    delemAERConnection->setAttribute(QString::fromStdString(ClsTagLibrary::ConnectionTypeTag()),
                     QString::fromStdString(itMapSTT->first.type.c_str()));


/* synapses III */
    ClsFEConnection* clsFEConnection = NULL;
    clsFEConnection = ClsFESystemManager::Instance()->getFEConnection(itMapSTT->first.origID);
    if(clsFEConnection!=NULL){
        string strSynapseType = clsFEConnection->getConnectionSynapseType();
        if(strSynapseType.size()>0){
        QDomElement *delemSynapse = addParameterizedNode(ddocRoot, ClsTagLibrary::SynapseTag(), strSynapseType, clsFEConnection->getListSynapseParameters());
        delemAERConnection.appendChild(*delemSynapse);
        }
    }
/* ------------- */

    unsigned int _nNeuronsPre = ClsFESystemManager::Instance()->getFEGroup(strSourceID)->getNumberOfNeurons();
    ListIndexQuadruples listIndexQuadruple = itMapSTT->second;
    for (unsigned int pre = 0; pre < _nNeuronsPre; ++pre) {

        vector<Indices> vectIndices = listIndexQuadruple.getIndicesByFirst(pre);
        if(vectIndices.size()>0){
        QDomElement delemPre = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::pre()));
        delemPre.setAttribute(QString::fromStdString(ClsTagLibrary::index()),
                       QString::fromStdString(iqrUtils::int2string(pre).c_str()));
        delemAERConnection.appendChild(delemPre);

        for(unsigned int ii=0; ii<vectIndices.size(); ii++){
            QDomElement delemPost = ddocRoot->createElement(reateElement(QString::fromStdString(ClsTagLibrary::post()));

            delemPost.setAttribute(QString::fromStdString(ClsTagLibrary::index()),
                       QString::fromStdString(iqrUtils::int2string(vectIndices[ii].neuronPost).c_str()));

//this should write out the synapse mask index
            delemPost.setAttribute(QString::fromStdString(ClsTagLibrary::synapse_index()),
                       QString::fromStdString(iqrUtils::int2string(vectIndices[ii].synapse).c_str()));

            delemPost.setAttribute(QString::fromStdString(ClsTagLibrary::delay()),
                       QString::fromStdString(iqrUtils::int2string(vectIndices[ii].delay).c_str()));

            delemPre.appendChild(delemPost);
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

    QDomImplementation impl;
    /*
    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(QString::fromStdString((const char*)"LS"));
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
    char* message = QString::fromStdString(toCatch.getMessage());
    cout << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return -1;
    }
    catch (const DOMException& toCatch) {
    char* message = QString::fromStdString(toCatch.msg);
    cout << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    return -1;
    }
    catch (...) {
    cout << "Unexpected Exception \n" ;
    return -1;
    }
*/

    QBuffer myFormTarget();
    QTextStream output(&myFormTarget);
    ddocSystem.save(output, 4);

    strSystemCont = output.readAll().toStdString();

    return 0;
}


QDomElement* ClsFESerializer::addFESystem(QDomDocument *ddocRoot, ClsFESystem* clsFESystem) {
    // Add the system as the root element.
    QDomElement *delemSystem = new QDomElement();
    *delemSystem = ddocRoot->documentElement();

    // Add ID as an attribute.
    delemSystem->setAttribute(QString::fromStdString(ClsTagLibrary::IDTag()),
                  QString::fromStdString(clsFESystem->getSystemID().c_str()));

    // Add the system's parameter list as attributes of the root
    delemSystem->setAttribute(QString::fromStdString(ClsTagLibrary::AuthorTag()),
                  QString::fromStdString(clsFESystem->getParameter(ClsTagLibrary::AuthorTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(QString::fromStdString(ClsTagLibrary::DateTag()),
                  QString::fromStdString(clsFESystem->getParameter(ClsTagLibrary::DateTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(QString::fromStdString(ClsTagLibrary::PortTag()),
                  QString::fromStdString(clsFESystem->getParameter(ClsTagLibrary::PortTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(QString::fromStdString(ClsTagLibrary::CyclesPerSecondTag()),
                  QString::fromStdString(clsFESystem->getParameter(ClsTagLibrary::CyclesPerSecondTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(QString::fromStdString(ClsTagLibrary::SyncPlotsTag()),
                  QString::fromStdString(clsFESystem->getParameter(ClsTagLibrary::SyncPlotsTag())->getValueAsString().c_str()));

    delemSystem->setAttribute(QString::fromStdString(ClsTagLibrary::NameTag()),
                  QString::fromStdString(clsFESystem->getParameter(ClsTagLibrary::NameTag())->getValueAsString().c_str()));

    QDomElement delemNote = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::NotesTag()));

    delemSystem->appendChild(delemNote);
    QDomText dtxtNote = ddocRoot->createTextNode(QString::fromStdString(clsFESystem->getParameter(ClsTagLibrary::NotesTag())->getValueAsString().c_str()));
    delemNote.appendChild(dtxtNote);

    return delemSystem;

}

QDomElement* ClsFESerializer::addFEProcess(QDomDocument *ddocRoot, ClsFEProcess* clsFEProcess) {
    QDomElement* delemProcess = new QDomElement();
    *delemProcess = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::ProcessTag()));

    string strProcessID = clsFEProcess->getProcessID();
    string strProcessName = clsFEProcess->getProcessName();
    string strProcessPath  = clsFEProcess->getPath();
    string strColor =  clsFEProcess->getColor();

    delemProcess->setAttribute(QString::fromStdString(ClsTagLibrary::IDTag()),
                   QString::fromStdString(strProcessID.c_str()));

    delemProcess->setAttribute(QString::fromStdString(ClsTagLibrary::NameTag()),
                   QString::fromStdString(strProcessName.c_str()));


    delemProcess->setAttribute(QString::fromStdString(ClsTagLibrary::HostnameTag()),
                   QString::fromStdString(""));

    delemProcess->setAttribute(QString::fromStdString(ClsTagLibrary::PortTag()),
                   QString::fromStdString("54923"));

    delemProcess->setAttribute(QString::fromStdString(ClsTagLibrary::ColorTag()),
                   QString::fromStdString(strColor.c_str()));


    delemProcess->setAttribute(QString::fromStdString(ClsTagLibrary::EnableModuleTag()),
                   QString::fromStdString(clsFEProcess->getEnableModulesAsString().c_str()));

    delemProcess->setAttribute(QString::fromStdString(ClsTagLibrary::PathTag()),
                   QString::fromStdString(strProcessPath.c_str()));


    string strModuleType = clsFEProcess->getProcessModuleType();
    if(strModuleType.size()>0){
    QDomElement *delemModule = addParameterizedNode(ddocRoot, ClsTagLibrary::ModuleTag(), strModuleType, clsFEProcess->getListModuleParameters());
    delemProcess->appendChild(*delemModule);

    /* XRefs */
    addXRefHolders(ddocRoot, delemModule, clsFEProcess);

    } else {
//	cerr << "module type undefined" << endl;
    }

/* ------- */

    QDomElement delemNote = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::NotesTag()));
    delemProcess->appendChild(delemNote);
    QDomText dtxtNote = ddocRoot->createTextNode(QString::fromStdString(clsFEProcess->getNotes().c_str()));
    delemNote.appendChild(dtxtNote);


    /* DiagramIcon */
    QDomElement delemDiagramIcon = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::DiagramIconTag()));
    delemProcess->appendChild(delemDiagramIcon);


    ClsInfoDiagramIcon clsInfoDiagramIcon = ClsFEDiagramManager::Instance()->getDiagramIcon( strProcessID);
    delemDiagramIcon.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramIconXTag()),
                    QString::fromStdString(clsInfoDiagramIcon.getXAsString().c_str()));
    delemDiagramIcon.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramIconYTag()),
                    QString::fromStdString(clsInfoDiagramIcon.getYAsString().c_str()));
    delemDiagramIcon.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramIconWidthTag()),
                    QString::fromStdString(clsInfoDiagramIcon.getWidthAsString().c_str()));
    delemDiagramIcon.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramIconHeightTag()),
                    QString::fromStdString(clsInfoDiagramIcon.getHeightAsString().c_str()));

    return delemProcess;
}


QDomElement* ClsFESerializer::addFEGroup(QDomDocument *ddocRoot, ClsFEGroup *clsFEGroup) {
    if (bDebugSystemFileWriter) {
    cout << "ClsFESerializer::addFEGroup(ClsFEGroup *clsFEGroup" << endl;
    }


    QDomElement *delemGroup = new QDomElement();
    *delemGroup = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::GroupTag()));
    string strGroupID = clsFEGroup->getGroupID();
    string strGroupName = clsFEGroup->getGroupName();
    string strColor =  clsFEGroup->getColor();


    delemGroup->setAttribute(QString::fromStdString(ClsTagLibrary::IDTag()),
                 QString::fromStdString(strGroupID.c_str()));
    delemGroup->setAttribute(QString::fromStdString(ClsTagLibrary::NameTag()),
                 QString::fromStdString(strGroupName.c_str()));
    delemGroup->setAttribute(QString::fromStdString(ClsTagLibrary::ColorTag()),
                 QString::fromStdString(strColor.c_str()));



    /* Topology */
    QDomElement delemTopology = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::TopologyTag()));
    delemGroup->appendChild(delemTopology);


    string strTopologyType = clsFEGroup->getTopology()->Type();
    QDomElement delemTopologyType = ddocRoot->createElement(QString::fromStdString(strTopologyType.c_str()));
    delemTopology.appendChild(delemTopologyType);


    ClsBaseTopology *clsBaseTopology = clsFEGroup->getTopology();

    ParameterList parameterList = clsBaseTopology->getListParameters();

    while (parameterList.size()) {
    string strParamName = parameterList.front()->getName();
    string strParamValue = parameterList.front()->getValueAsString();
    delemTopologyType.setAttribute(QString::fromStdString(strParamName.c_str()), QString::fromStdString(strParamValue.c_str()));
    parameterList.pop_front();
    }



    if(dynamic_cast<ClsTopologySparse*>(clsBaseTopology)) {
    list<pair<int, int> > lstPInt = (dynamic_cast<ClsTopologySparse*>(clsBaseTopology))->getList();

    list<pair<int, int> >::iterator itLstPInt;
    for(itLstPInt=lstPInt.begin();itLstPInt!=lstPInt.end();itLstPInt++){
        int iX = (*itLstPInt).first;
        int iY = (*itLstPInt).second;
        QDomElement delemPoint = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::Point()));
        delemTopologyType.appendChild(delemPoint);
        delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointX()), QString::fromStdString(iqrUtils::int2string(iX).c_str()));
        delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointY()), QString::fromStdString(iqrUtils::int2string(iY).c_str()));
    }

    }


    /* neurons */
    string strNeuronType = clsFEGroup->getGroupNeuronType();
    if(strNeuronType.size()>0){
    QDomElement *delemNeuron = addParameterizedNode(ddocRoot, ClsTagLibrary::NeuronTag(), strNeuronType, clsFEGroup->getListNeuronParameters());
    delemGroup->appendChild(*delemNeuron);

    /* XRefs UH?
       addXRefHolders(ddocRoot, delemNeuron, clsFEGroup);
    */

    } else {
    cerr << "neuron type undefined" << endl;
    }


/* Note */
    QDomElement delemNote = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::NotesTag()));
    delemGroup->appendChild(delemNote);
    QDomText dtxtNote = ddocRoot->createTextNode(QString::fromStdString((dynamic_cast<ClsFEGroup*>(clsFEGroup))->getNotes().c_str()));
    delemNote.appendChild(dtxtNote);


    /* DiagramIcon */
    QDomElement delemDiagramIcon = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::DiagramIconTag()));
    delemGroup->appendChild(delemDiagramIcon);


    ClsInfoDiagramIcon clsInfoDiagramIcon =  ClsFEDiagramManager::Instance()->getDiagramIcon( strGroupID);
    delemDiagramIcon.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramIconXTag()),
                    QString::fromStdString(clsInfoDiagramIcon.getXAsString().c_str()));
    delemDiagramIcon.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramIconYTag()),
                    QString::fromStdString(clsInfoDiagramIcon.getYAsString().c_str()));
    delemDiagramIcon.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramIconWidthTag()),
                    QString::fromStdString(clsInfoDiagramIcon.getWidthAsString().c_str()));
    delemDiagramIcon.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramIconHeightTag()),
                    QString::fromStdString(clsInfoDiagramIcon.getHeightAsString().c_str()));
    return delemGroup;
}


QDomElement* ClsFESerializer::addFEConnection(QDomDocument *ddocRoot, ClsFEConnection* clsFEConnection) {
    if (bDebugSystemFileWriter) {
    cout << "ClsFESerializer::addFEConnection(DOMDocument *ddocRoot, ClsFEConnection* clsFEConnection)"<< endl;
    }

    if(clsFEConnection!=NULL){
    QDomElement *delemConnection = new QDomElement();
    *delemConnection = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::ConnectionTag()));
    string strConnectionID = clsFEConnection->getConnectionID();
    string strConnectionName = clsFEConnection->getConnectionName();

    delemConnection->setAttribute(QString::fromStdString(ClsTagLibrary::IDTag()),
                      QString::fromStdString(strConnectionID.c_str()));

    delemConnection->setAttribute(QString::fromStdString(ClsTagLibrary::NameTag()),
                      QString::fromStdString(strConnectionName.c_str()));

    delemConnection->setAttribute(QString::fromStdString(ClsTagLibrary::ConnectionSourceTag()),
                      QString::fromStdString(clsFEConnection->getConnectionSourceID().c_str()));

    delemConnection->setAttribute(QString::fromStdString(ClsTagLibrary::ConnectionTargetTag()),
                      QString::fromStdString(clsFEConnection->getConnectionTargetID().c_str()));

    delemConnection->setAttribute(QString::fromStdString(ClsTagLibrary::ConnectionTypeTag()),
                      QString::fromStdString(clsFEConnection->getConnectionTypeAsString().c_str()));

    QDomElement delemPattern = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::ConnectionPattern()));
    delemConnection->appendChild(delemPattern);

    string strPatternType = clsFEConnection->PatternType();
    QDomElement delemPatternType = ddocRoot->createElement(QString::fromStdString(strPatternType.c_str()));
    delemPattern.appendChild(delemPatternType);
    ClsBasePattern *clsBasePattern = clsFEConnection->getPattern();

    ParameterList parameterList0 = clsBasePattern->getListParameters();
    while (parameterList0.size()) {
        string strParamName = parameterList0.front()->getName();
        string strParamValue = parameterList0.front()->getValueAsString();
        delemPatternType.setAttribute(QString::fromStdString(strParamName.c_str()),
                       QString::fromStdString(strParamValue.c_str()));
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
            QDomElement delemRegion = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::SelectorRegion()));
            delemPatternType.appendChild(delemRegion);
            int iXStart = clsRegion->getXStart();
            int iYStart = clsRegion->getYStart();
            int iWidth = clsRegion->getWidth();
            int iHeight = clsRegion->getHeight();

            delemRegion.setAttribute(QString::fromStdString(ClsTagLibrary::RegionXStart()), QString::fromStdString(iqrUtils::int2string(iXStart).c_str()));
            delemRegion.setAttribute(QString::fromStdString(ClsTagLibrary::RegionYStart()), QString::fromStdString(iqrUtils::int2string(iYStart).c_str()));
            delemRegion.setAttribute(QString::fromStdString(ClsTagLibrary::RegionWidth()), QString::fromStdString(iqrUtils::int2string(iWidth).c_str()));
            delemRegion.setAttribute(QString::fromStdString(ClsTagLibrary::RegionHeight()), QString::fromStdString(iqrUtils::int2string(iHeight).c_str()));
        }
        else if(dynamic_cast<ClsList*>(clsBaseSubPopulationSource)){
            QDomElement delemList = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::SelectorList()));
            delemPatternType.appendChild(delemList);
            ClsList *clsList = dynamic_cast<ClsList*>(clsBaseSubPopulationSource);

            list<pair<int, int> > lstPInt = clsList->getData();
            list<pair<int, int> >::iterator itLstPInt;
            for(itLstPInt=lstPInt.begin();itLstPInt!=lstPInt.end();itLstPInt++){
            int iX = (*itLstPInt).first;
            int iY = (*itLstPInt).second;
            QDomElement delemPoint = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::Point()));
            delemList.appendChild(delemPoint);
            delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointX()), QString::fromStdString(iqrUtils::int2string(iX).c_str()));
            delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointY()), QString::fromStdString(iqrUtils::int2string(iY).c_str()));
            }
        }
        else if(dynamic_cast<ClsAll*>(clsBaseSubPopulationSource)){
            QDomElement delemAll = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::SelectorAll()));
            delemPatternType.appendChild(delemAll);
        }
        }

        ClsBaseSubPopulation *clsBaseSubPopulationTarget = clsBasePattern->getTargetPopulation();
        if(clsBaseSubPopulationTarget!=NULL){

        if(dynamic_cast<ClsRegion*>(clsBaseSubPopulationTarget)){
            ClsRegion *clsRegion = dynamic_cast<ClsRegion*>(clsBaseSubPopulationTarget);
            QDomElement delemRegion = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::SelectorRegion()));
            delemPatternType.appendChild(delemRegion);
            int iXStart = clsRegion->getXStart();
            int iYStart = clsRegion->getYStart();
            int iWidth = clsRegion->getWidth();
            int iHeight = clsRegion->getHeight();

            delemRegion.setAttribute(QString::fromStdString(ClsTagLibrary::RegionXStart()), QString::fromStdString(iqrUtils::int2string(iXStart).c_str()));
            delemRegion.setAttribute(QString::fromStdString(ClsTagLibrary::RegionYStart()), QString::fromStdString(iqrUtils::int2string(iYStart).c_str()));
            delemRegion.setAttribute(QString::fromStdString(ClsTagLibrary::RegionWidth()), QString::fromStdString(iqrUtils::int2string(iWidth).c_str()));
            delemRegion.setAttribute(QString::fromStdString(ClsTagLibrary::RegionHeight()), QString::fromStdString(iqrUtils::int2string(iHeight).c_str()));
        }
        else if(dynamic_cast<ClsList*>(clsBaseSubPopulationTarget)){
            QDomElement delemList = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::SelectorList()));
            delemPatternType.appendChild(delemList);
            ClsList *clsList = dynamic_cast<ClsList*>(clsBaseSubPopulationTarget);

            list<pair<int, int> > lstPInt = clsList->getData();
            list<pair<int, int> >::iterator itLstPInt;
            for(itLstPInt=lstPInt.begin();itLstPInt!=lstPInt.end();itLstPInt++){
            int iX = (*itLstPInt).first;
            int iY = (*itLstPInt).second;
            QDomElement delemPoint = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::Point()));
            delemList.appendChild(delemPoint);
            delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointX()), QString::fromStdString(iqrUtils::int2string(iX).c_str()));
            delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointY()), QString::fromStdString(iqrUtils::int2string(iY).c_str()));
            }
        }
        else if(dynamic_cast<ClsAll*>(clsBaseSubPopulationTarget)){
            QDomElement delemAll = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::SelectorAll()));
            delemPatternType.appendChild(delemAll);
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
            QDomElement delemTuple = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::Tuple()));
            delemPatternType.appendChild(delemTuple);

            tListOfPairs tlopSource = pairLOP.first;
            QDomElement delemSourceList = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::SourceList()));
            delemTuple.appendChild(delemSourceList);
            tListOfPairs::iterator itLPSource;
            for(itLPSource=tlopSource.begin(); itLPSource!=tlopSource.end(); itLPSource++){
            int iX = (*itLPSource).first;
            int iY = (*itLPSource).second;

            QDomElement delemPoint = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::Point()));
            delemSourceList.appendChild(delemPoint);
            delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointX()), QString::fromStdString(iqrUtils::int2string(iX).c_str()));
            delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointY()), QString::fromStdString(iqrUtils::int2string(iY).c_str()));
            }

            tListOfPairs tlopTarget = pairLOP.second;
            QDomElement delemTargetList = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::TargetList()));
            delemTuple.appendChild(delemTargetList);
            tListOfPairs::iterator itLPTarget;
            for(itLPTarget=tlopTarget.begin(); itLPTarget!=tlopTarget.end(); itLPTarget++){
            int iX = (*itLPTarget).first;
            int iY = (*itLPTarget).second;

            QDomElement delemPoint = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::Point()));
            delemTargetList.appendChild(delemPoint);
            delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointX()), QString::fromStdString(iqrUtils::int2string(iX).c_str()));
            delemPoint.setAttribute(QString::fromStdString(ClsTagLibrary::PointY()), QString::fromStdString(iqrUtils::int2string(iY).c_str()));
            }
        }
        }
    }


/* Arborization */
    QDomElement delemArborization = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::ConnectionArborization()));
    delemConnection->appendChild(delemArborization);
    string strArborizationType = clsFEConnection->getArborization()->Type();
    QDomElement delemArborizationType = ddocRoot->createElement(QString::fromStdString(strArborizationType.c_str()));
    delemArborization.appendChild(delemArborizationType);
    ParameterList parameterList = clsFEConnection->getArborization()->getListParameters();
    while (parameterList.size()) {
        string strParamName = parameterList.front()->getName();
        string strParamValue = parameterList.front()->getValueAsString();
        delemArborizationType.setAttribute(QString::fromStdString(strParamName.c_str()),
                        QString::fromStdString(strParamValue.c_str()));
        parameterList.pop_front();
    }
/* end Arborization */


/* AttenuationFunction */
    QDomElement delemAttenuationFunction = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::ConnectionAttenuationFunction()));
    delemConnection->appendChild(delemAttenuationFunction);
    string strAttenuationFunctionType = clsFEConnection->getAttenuationFunction()->Type();
    QDomElement delemAttenuationFunctionType = ddocRoot->createElement(QString::fromStdString(strAttenuationFunctionType.c_str()));
    delemAttenuationFunction.appendChild(delemAttenuationFunctionType);
    ParameterList parameterListAttenuationFunction = clsFEConnection->getAttenuationFunction()->getListParameters();
    while (parameterListAttenuationFunction.size()) {
        string strParamName = parameterListAttenuationFunction.front()->getName();
        string strParamValue = parameterListAttenuationFunction.front()->getValueAsString();
        delemAttenuationFunctionType->setAttribute(QString::fromStdString(strParamName.c_str()),
                          QString::fromStdString(strParamValue.c_str()));
        parameterListAttenuationFunction.pop_front();
    }
/*   end AttenuationFunction */

/* DelayFunction */
    QDomElement delemDelayFunction = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::ConnectionDelayFunction()));
    delemConnection->appendChild(delemDelayFunction);
    string strDelayFunctionType = clsFEConnection->getDelayFunction()->Type();
    QDomElement delemDelayFunctionType = ddocRoot->createElement(QString::fromStdString(strDelayFunctionType.c_str()));
    delemDelayFunction.appendChild(delemDelayFunctionType);
    ParameterList parameterListDelayFunction = clsFEConnection->getDelayFunction()->getListParameters();
    while (parameterListDelayFunction.size()) {
        string strParamName = parameterListDelayFunction.front()->getName();
        string strParamValue = parameterListDelayFunction.front()->getValueAsString();
        delemDelayFunctionType.setAttribute(QString::fromStdString(strParamName.c_str()),
                         QString::fromStdString(strParamValue.c_str()));
        parameterListDelayFunction.pop_front();
    }
/* end DelayFunction */

/* synapses */
    string strSynapseType = clsFEConnection->getConnectionSynapseType();
    if(strSynapseType.size()>0){
        QDomElement *delemSynapse = addParameterizedNode(ddocRoot, ClsTagLibrary::SynapseTag(), strSynapseType, clsFEConnection->getListSynapseParameters());
        delemConnection->appendChild(*delemSynapse);

        /* XRefs */
        addXRefHolders(ddocRoot, delemSynapse, clsFEConnection);

    } else {
        cerr << "synapse type undefined" << endl;
    }


/* Note */
    QDomElement delemNote = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::NotesTag()));
    delemConnection->appendChild(delemNote);
    QDomText dtxtNote = ddocRoot->createTextNode(QString::fromStdString((dynamic_cast<ClsFEConnection*>(clsFEConnection))->getNotes().c_str()));
    delemNote->appendChild(dtxtNote);


/* DiagramLine */
    QDomElement delemDiagramLine = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::DiagramLineTag()));
    delemConnection->appendChild(delemDiagramLine);

    ClsInfoDiagramLine clsInfoDiagramLine =  ClsFEDiagramManager::Instance()->getDiagramLine( strConnectionID);
    delemDiagramLine.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramLineAPSourceTag()),
                    QString::fromStdString(iqrUtils::int2string(clsInfoDiagramLine.getAPSource()).c_str()));
    delemDiagramLine.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramLineAPTargetTag()),
                    QString::fromStdString(iqrUtils::int2string(clsInfoDiagramLine.getAPTarget()).c_str()));

    vector<vector<int> > vectorPoints = clsInfoDiagramLine.getPoints();

    for(unsigned int ii=0; ii<vectorPoints.size(); ii++){
        int iX = vectorPoints[ii][0];
        int iY = vectorPoints[ii][1];
        QDomElement delemPoint = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::DiagramLinePointTag()));

        delemPoint.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramLinePointXTag()),
                      QString::fromStdString(iqrUtils::int2string(iX).c_str()));
        delemPoint.setAttribute( QString::fromStdString(ClsTagLibrary::DiagramLinePointYTag()),
                      QString::fromStdString(iqrUtils::int2string(iY).c_str()));
        delemDiagramLine.appendChild(delemPoint);
    }
    return delemConnection;
    }
}

QDomElement* ClsFESerializer::addParameterizedNode(QDomDocument *ddocRoot, string strNodeTag, string strNodeType, ParameterList parameterList){

    QDomElement *delemParameterizedNode = new QDomElement();
    *delemParameterizedNode = ddocRoot->createElement(QString::fromStdString(strNodeTag.c_str()));

    delemParameterizedNode->setAttribute(QString::fromStdString(ClsTagLibrary::NameTag()),
                     QString::fromStdString(strNodeType.c_str()));

    while (parameterList.size()) {
    string strParamName = parameterList.front()->getName();
    string strParamValue = parameterList.front()->getValueAsString();

    QDomElement delemParameter = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::ParameterTag()));

    delemParameter.setAttribute(QString::fromStdString(ClsTagLibrary::NameTag()),
                     QString::fromStdString(strParamName.c_str()));

    delemParameter.setAttribute(QString::fromStdString(ClsTagLibrary::ValueTag()),
                     QString::fromStdString(strParamValue.c_str()));

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
        DOMElement *delemXRefHolder = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::XRefHolderTag()));
        string strXRefHolderName = clsXRefHolder->getName();
        delemXRefHolder->setAttribute(QString::fromStdString(ClsTagLibrary::NameTag()),
                      QString::fromStdString(strXRefHolderName.c_str()));

        XRefList xRefList = clsXRefHolder->getListXRefs();

        XRefList::iterator itXRef;
        for(itXRef=xRefList.begin(); itXRef!=xRefList.end(); itXRef++){
        string strXRefName = (*itXRef)->getName();
        string strXRefTarget = (*itXRef)->getTarget();

        DOMElement *delemXRef = ddocRoot->createElement(QString::fromStdString(ClsTagLibrary::XRefTag()));
        delemXRef->setAttribute(QString::fromStdString(ClsTagLibrary::NameTag()),
                    QString::fromStdString(strXRefName.c_str()));

        delemXRef->setAttribute(QString::fromStdString(ClsTagLibrary::TargetTag()),
                    QString::fromStdString(strXRefTarget.c_str()));
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
