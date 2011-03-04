/****************************************************************************
 ** $Filename: ClsSysFileParser.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Sat Jan 13 00:43:21 2001
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
DomNode2List(QDomNode dnTop, int iCount, string strSep){
    using iqrcommon_old::ClsParameter;

    iCount ++;
    strSep += "\t";
    string strNodeName = dnTop.nodeName().toStdString();
    //  cout << "DomNode2List: " << strSep << iCount << " " << strNodeName  << endl;

    iqrcommon_old::ClsParameter clsParameter;
    list<iqrcommon_old::ClsParameter> lstParam;

    // node and it's attributes ++++++++++++++++++++
    iqrcommon_old::ClsParameter clsParameterChild;
    list<iqrcommon_old::ClsParameter> lstAtt; // list with attributes
    QDomNamedNodeMap dnnmapAtt = dnTop.attributes();
    for(unsigned int ii=0; ii < dnnmapAtt.length(); ii++){
    //    cout << "DomNode2List: " << strSep << iCount << " " << "AttName: " << strAttName << ", AttValue: " << strAttValue << endl;
    iqrcommon_old::ClsParameter clsParameterAtt;
        clsParameterAtt.setParameter(dnnmapAtt.item(ii).nodeName().toStdString(), dnnmapAtt.item(ii).nodeValue().toStdString());
    lstAtt.push_back(clsParameterAtt);
    }
    clsParameterChild.setParameter(strNodeName, lstAtt);
    lstParam.push_back(clsParameterChild);
    // +++++++++++++++++++++++++++++++++++++++++++++

    QDomNode dnChild = dnTop->firstChild();
    while(!dnChild.isNull()){
        if(dnChild.nodeType() == QDomNode::ElementNode){
        clsParameterChild.setParameter(strNodeName, DomNode2List(dnChild, iCount, strSep));
        lstParam.push_back(clsParameterChild);
    }
        dnChild = dnChild.nextSibling();
    }


    return lstParam;
}



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
    //try {
        //XMLPlatformUtils::Initialize();
    //}

    //catch(const XMLException& toCatch) {
        //cerr << "Error during Xerces-c Initialization.\n"
          //   << "  Exception message:"
//	     << DOMString(toCatch.getMessage()) << endl;
            // << toCatch.getMessage() << endl;
    //      return 1;
        //bParserInitialized = false;
    //}
    bParserInitialized = true;
};


/**
 * Parse the buffer
 * This parses the contents of the buffer set via setInputBuffer(string _strBuffer)
 */
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

    QXmlSimpleReader parser;
    QXmlInputSource *source = new QXmlInputSource(QByteArray(gXMLInMemBuf));

    //TODO: I NEED TO SET the ContentHandler, the ErrorHandler and parse source
    // Also, we need to modify MyResolver and set DTD and this things
    QXmlDefaultHandler contentHandler;
    parser.setContentHandler(&contentHandler);

    parser.setEntityResolver(&mr);

    //XercesDOMParser* parser = new XercesDOMParser();
    //parser->setEntityResolver(&mr);

    //if(bValidate){
        //parser->setValidationScheme(XercesDOMParser::Val_Always);
    //} else {
        //parser->setValidationScheme(XercesDOMParser::Val_Never);
    //}

    //parser->setDoNamespaces(gDoNamespaces);
    parser.setFeature("http://xml.org/sax/features/namespaces", gDoNamespaces);

    ClsMyErrorHandler clsMyErrorHandler;
    parser.setErrorHandler(&clsMyErrorHandler);

    /*  Parse the XML file, catching any XML exceptions that might propogate out of it.*/
    //bool errorsOccured = false;
    bool parsingSuccessful = true;

    //if(!ddocSystemFile)
        //ddocSystemFile = new QDomDocument("System File");

    parsingSuccessful = ddocSystemFile.setContent(source, parser);

    /*
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
    /*} catch (const ClsSysFileParserException& e) {
    throw e;
    errorsOccured = true;
    } catch (...) {
    cerr << "An error occured during parsing\n " << endl;
    errorsOccured = true;
    ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR, "unknown");
    throw clsSysFileParserException;
    }*/



    /* If the parse was successful, output the document data from the DOM tree */
    if (parsingSuccessful) {
        QDomNode dnSystem;
        //QDomNode *dnSystem;

        //QDomNodeList *dnlstTemp;

        //ddocSystemFile = parser->getDocument();



    //=================================================================
    // first we have to find the top System node
    /* the new way: use "getElementsByTagName" */
        QDomNodeList dnlstTemp = ddocSystemFile.elementsByTagName(ClsTagLibrary::SystemTag());
        //dnlstTemp = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::SystemTag()));

    //dnlstTemp = ddocSystemFile->getElementsByTagName("System");
    /*this should throw an exception if system is not found! */
        if (dnlstTemp.length() == 1) {
            dnSystem = dnlstTemp.item(0);
        } else if (dnlstTemp.length() < 1) {
        ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::NO_SYSTEM_NODE);
        throw clsSysFileParserException;
    }

    //=================================================================


    //=================================================================
    // For later use, we create here QDomNodeList for all the groups and connections in the file
        dnlstSystemGroups = ddocSystemFile.elementsByTagName(ClsTagLibrary::GroupTag());
        dnlstSystemConnections = ddocSystemFile.elementsByTagName(ClsTagLibrary::ConnectionTag());
    //=================================================================

    /* we build a cache for the notes, which should be much faster, than searching for the
       notes at each call... */
        buildNotesCache(&ddocSystemFile);
        buildDiagramLineCache(&ddocSystemFile);
        buildDiagramIconCache(&ddocSystemFile);


    bParserBufferParsed = true;
    } else
        throw ClsSysFileParserException;

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

    //MyResolver mr;

    QXmlSimpleReader parser;
    QXmlInputSource *source = new QXmlInputSource(QByteArray(gXMLInMemBuf));

    QXmlDefaultHandler contentHandler;
    parser.setContentHandler(&contentHandler);

    //parser.setEntityResolver(&mr);

    parser.setFeature("http://xml.org/sax/features/namespaces", gDoNamespaces);

    ClsMyErrorHandler clsMyErrorHandler;
    parser.setErrorHandler(&clsMyErrorHandler);

    bool parsingSuccessful = true;

    parsingSuccessful = ddocClipboard.setContent(source, parser);

    /*
    XercesDOMParser* parser = new XercesDOMParser();


    if(bValidate){
    parser->setValidationScheme(XercesDOMParser::Val_Always);
    } else {
    parser->setValidationScheme(XercesDOMParser::Val_Never);
    }

    parser->setDoNamespaces(gDoNamespaces);*/

/*
    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);
*/
/*
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
        errorsOccured = true;*/
/*
    } catch (const SAXException& e) {
    cerr << "An error occured during parsing (SAXException)\n   DMessage: " <<  XMLString::transcode(e.getMessage()) << endl;
    ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR,  XMLString::transcode(e.getMessage()));
    throw clsSysFileParserException;
    errorsOccured = true;
*/
  /*  } catch (const ClsSysFileParserException& e) {
    throw e;
    errorsOccured = true;
    } catch (...) {
    cerr << "An error occured during parsing\n " << endl;
    errorsOccured = true;
    ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR, "unknown");
    throw clsSysFileParserException;
    }*/

    if (parsingSuccessful) {
        //ddocClipboard = parser->getDocument();

        dnlstClipboardProcesses = ddocClipboard.elementsByTagName(ClsTagLibrary::ProcessTag());
        dnlstClipboardConnections = ddocClipboard.elementsByTagName(ClsTagLibrary::ConnectionTag());
        dnlstClipboardGroups = ddocClipboard.elementsByTagName(ClsTagLibrary::GroupTag());

//	cout << "dnlstClipboardProcesses->getLength(): " << dnlstClipboardProcesses->getLength() << endl;
//	cout << "dnlstClipboardConnections->getLength(): " << dnlstClipboardConnections->getLength() << endl;
//	cout << "dnlstClipboardGroups->getLength(): " << dnlstClipboardGroups->getLength() << endl;


        buildNotesCache(&ddocClipboard);
        buildDiagramLineCache(&ddocClipboard);
        buildDiagramIconCache(&ddocClipboard);

    bParserBufferParsed = true;
    }

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
    QDomNode dnLocalPrc;
    //QDomNode *dnLocalPrc = NULL;



    //=================================================================
    // Now, dnTemp points to System. Next step is to find the Node that matches the locals Prcs ID
//    cout << "We are looking for the local id: " << strPrcID << endl;
    QDomNodeList dnlstTemp = ddocSystemFile.elementsByTagName(ClsTagLibrary::ProcessTag());
    //QDomNodeList *dnlstTemp = ddocSystemFile->getElementsByTagName(XMLString::transcode(ClsTagLibrary::ProcessTag()));
    unsigned int ii = 0;
    int iFound = 0;
    while(ii < dnlstTemp.length()) {
        //QDomNode *dnTemp = dnlstTemp->item(ii);
        QDomNode dnTemp = dnlstTemp.item(ii);
        string strIDTemp = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    if(!strIDTemp.compare(strPrcID)){
        dnLocalPrc = dnTemp;
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
    QDomImplementation impl = ddocSystemFile.implementation();
    //DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));


    ddocLocalPrc = impl.createDocument(0, ClsTagLibrary::ProcessTag(), 0);

    QDomElement rootElem = ddocLocalPrc.documentElement();  // returns the root element
    //DOMElement *rootElem = ddocLocalPrc->getDocumentElement();  // returns a reference to the root element

    QDomNode nn = ddocLocalPrc.importNode(dnLocalPrc, true);
    //QDomNode *nn = ddocLocalPrc->importNode(dnLocalPrc, true);

    rootElem.appendChild(nn);

    dnlstLocalPrcGroups = ddocLocalPrc.elementsByTagName(ClsTagLibrary::GroupTag());
    //dnlstLocalPrcGroups = ddocLocalPrc->getElementsByTagName(XMLString::transcode(ClsTagLibrary::GroupTag()));
};

/**
 * Terminate the XMLPlatformUtils
 * To be called after the last call to createPrcDOMTree(string strProcessName, string _strPrcID, string strPrcType)
 */
void iqrcommon::ClsSysFileParser::XMLPlatformUtilsTerminate(){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::XMLPlatformUtilsTerminate()" << endl;
#endif
    //XMLPlatformUtils::Terminate();
    bParserInitialized = false;
};


/**
 * Get the number of groups in the local Prc.
 * @return Number of groups in the local Prc.
 */
int iqrcommon::ClsSysFileParser::getLocalGroupCount( )
{
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupCount( )" << endl;
#endif
    return dnlstLocalPrcGroups.length();
};

list<iqrcommon::ClsInfoGroup> iqrcommon::ClsSysFileParser::getListClipboardGroups() {
    list<ClsInfoGroup> lstClipboardGroups;

    ClsInfoGroup clsInfoGroup;
    for (unsigned int ii=0; ii < dnlstClipboardGroups.length(); ii++){
        QDomNode dnGroup =  dnlstClipboardGroups.item(ii);

        string strGroupID = getAttributeValue(&dnGroup, ClsTagLibrary::IDTag(), true);
        string strGroupName = getAttributeValue(&dnGroup, ClsTagLibrary::NameTag(), true);
        string strColor = getAttributeValue(&dnGroup, ClsTagLibrary::ColorTag(), false);

    clsInfoGroup.setGroupID(strGroupID);
    clsInfoGroup.setGroupName(strGroupName);
    clsInfoGroup.setColor(strColor);

        string strNotes = getNotesNode(&dnGroup);
    clsInfoGroup.setNotes(strNotes);

        ClsSysFileNode clsSysFileNode = getGroupSubNodes(&dnGroup);
    clsInfoGroup.setGroupSubNodes(clsSysFileNode);

    lstClipboardGroups.push_back(clsInfoGroup);
    }

    return lstClipboardGroups;
};


list<iqrcommon::ClsInfoGroup> iqrcommon::ClsSysFileParser::getListLocalGroups() {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListLocalGroups()" << endl;
#endif

    list<ClsInfoGroup> lstLocalGroups;
    ClsInfoGroup clsInfoGroup;
    for (unsigned int ii=0; ii < dnlstLocalPrcGroups.length(); ii++){
        QDomNode dnGroup = dnlstLocalPrcGroups.item(ii);

        string strGroupID = getAttributeValue(&dnGroup, ClsTagLibrary::IDTag(), true);
        string strGroupName = getAttributeValue(&dnGroup, ClsTagLibrary::NameTag(), true);
        string strColor = getAttributeValue(&dnGroup, ClsTagLibrary::ColorTag(), false);

    clsInfoGroup.setGroupID(strGroupID);
    clsInfoGroup.setGroupName(strGroupName);
    clsInfoGroup.setColor(strColor);

        string strNotes = getNotesGroup(clsInfoGroup.getGroupID());
    clsInfoGroup.setNotes(strNotes);

        ClsSysFileNode clsSysFileNode = getGroupSubNodes(&dnGroup);
    clsInfoGroup.setGroupSubNodes(clsSysFileNode);

        lstLocalGroups.push_back(clsInfoGroup);
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

    list<ClsInfoGroup> lstIDMirrorGroups;

    /* loop through all connections and find the ones that:
       - connect to a local group &&
       - come from a remote Prc
    */

    for(unsigned int ii=0; ii < dnlstSystemConnections.length(); ii++){
        QDomNode dnTemp = dnlstSystemConnections.item(ii);
        string strConnectionSource = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
        string strConnectionTarget = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);

        if(!isLocalGroup(strConnectionSource) & isLocalGroup(strConnectionTarget)){
            ClsInfoGroup clsInfoGroup;
            clsInfoGroup.setGroupID(strConnectionSource);
            clsInfoGroup.setGroupName(getGroupName(strConnectionSource));

            string strNotes = getNotesGroup(clsInfoGroup.getGroupID());
            clsInfoGroup.setNotes(strNotes);

            ClsSysFileNode clsSysFileNode = getGroupSubNodes(&dnTemp);
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
    QDomNode dnTemp;
    string strIDTemp;
    for(unsigned int ii=0; ii < dnlstSystemGroups.length(); ii++){
        dnTemp = dnlstSystemGroups.item(ii);
        strIDTemp = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
        if(!strIDTemp.compare(strGroupID)) {
            strGroupName = getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true);
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
    if(iIndex > (dnlstLocalPrcGroups.length() - 1)){
        ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::OUT_OF_GROUP_RANGE);
        throw clsSysFileParserException;
    } else {
        QDomNode dnGroup = dnlstLocalPrcGroups.item(iIndex);
        QDomNamedNodeMap dnmapTemp = dnGroup.attributes();

        for(unsigned int ii=0; ii < dnmapTemp.length(); ii++){
            QDomNode dnTemp = dnmapTemp.item(ii);
            string strNodeName = dnTemp.nodeName().toStdString();
//	  cout << strNodeName << endl;
//	  cout << dnTemp->getNodeValue() << endl;
            if(!strNodeName.compare(ClsTagLibrary::NameTag())){
                strGroupName = dnTemp.nodeValue().toStdString();
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

    if(iIndex > (dnlstLocalPrcGroups.length() - 1)){
        ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::OUT_OF_GROUP_RANGE);
        throw clsSysFileParserException;
    } else {
        QDomNode dnGroup = dnlstLocalPrcGroups.item(iIndex);
        QDomNamedNodeMap dnmapTemp = dnGroup.attributes();

        for(unsigned int ii=0; ii < dnmapTemp.length(); ii++){
            QDomNode dnTemp = dnmapTemp.item(ii);
            string strNodeName = dnTemp.nodeName().toStdString();
//	  cout << strNodeName << endl;
//	  cout << dnTemp->getNodeValue() << endl;
            if(!strNodeName.compare(ClsTagLibrary::IDTag())){
                strGroupID = dnTemp.nodeValue();
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

    for (unsigned int i1 = 0; i1 < dnlstSystemGroups.length(); i1++){
        QDomNode dnGroup = dnlstSystemGroups.item(i1);
        string strGroupID = getAttributeValue(&dnGroup, ClsTagLibrary::IDTag(), true);
        if(!strGroupID.compare(_strGroupID)) {
            QDomNode dnGroupChild = dnGroup.firstChild();
            while (!dnGroupChild.isNull()){
                if (dnGroupChild.nodeType() == QDomNode::ElementNode){
                    string strTemp = dnGroupChild.nodeName();
                    if (!strTemp.compare(ClsTagLibrary::TopologyTag())) {
                        string strTopology = dnGroupChild.nodeName();
                        clsSysFileNode.setName(strTopology);
                        clsSysFileNode = DomNode2SysNode(&dnGroupChild, 0, "");
                    }
                }
                dnGroupChild = dnGroupChild.nextSibling();
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

    for (unsigned int i1 = 0; i1 < dnlstSystemGroups.length(); i1++){
        QDomNode dnGroup =  dnlstSystemGroups.item(i1);
        string strGroupID = getAttributeValue(&dnGroup, ClsTagLibrary::IDTag(), true);
        if(!strGroupID.compare(_strGroupID)) {
            QDomNode dnGroupChild = dnGroup.firstChild();
            while (!dnGroupChild.isNull()){
                if (dnGroupChild.nodeType() == QDomNode::ElementNode){
                    string strTemp = dnGroupChild.nodeName().toStdString();
                    if (!strTemp.compare(ClsTagLibrary::NeuronTag())) {
                        string strNeuronType = dnGroupChild.nodeName().toStdString();
                        clsSysFileNode.setName(strNeuronType);
                        clsSysFileNode = DomNode2SysNode(&dnGroupChild, 0, "");
                    }
                }
                dnGroupChild = dnGroupChild.nextSibling();
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

    for(unsigned int ii=0; ii < dnlstClipboardConnections.length(); ii++){
    QDomNode dnTemp = dnlstClipboardConnections.item(ii);

    string strConnectionSource = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
    string strConnectionTarget =getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
    string strConnectionID = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    string strConnectionName = getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true);
    string strConnectionType = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTypeTag(), true); // 06/02/03 [ulysses] set the connection type
    clsInfoConnection.setConnectionSourceID(strConnectionSource);
    clsInfoConnection.setConnectionTargetID(strConnectionTarget);
    clsInfoConnection.setConnectionID(strConnectionID);
    clsInfoConnection.setConnectionName(strConnectionName);

    string strNotes = getNotesNode(&dnTemp);
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

    ClsSysFileNode clsSysFileNode = getConnectionSubNodes(&dnTemp);

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

    for(unsigned int ii=0; ii< dnlstSystemConnections.length(); ii++){
//	cout << "ii: " << ii << endl;

    QDomNode dnTemp = dnlstSystemConnections.item(ii);
    string strConnectionSource = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
    string strConnectionTarget = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
    string strConnectionID = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    string strConnectionName = getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true);
    string strConnectionType = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTypeTag(), true); // 06/02/03 [ulysses] set the connection type
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

    ClsSysFileNode clsSysFileNode = getConnectionSubNodes(&dnTemp);
    clsInfoConnection.setConnectionSubNodes(clsSysFileNode);

    lstIDConnection.push_back(clsInfoConnection);
    }
    return lstIDConnection;
};


list<iqrcommon::ClsInfoConnection> iqrcommon::ClsSysFileParser::getListConnectionLocal( ) { // this list includes mirror groups
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getListConnectionLocal(string strGroupID)"<< endl;
#endif
    int iConnectionType = -1;  // 06/02/03 [ulysses] set the connection type
    list<ClsInfoConnection> lstIDConnectionLocal;
    ClsInfoConnection clsInfoConnection;

    for(unsigned int ii=0; ii< dnlstSystemConnections.length(); ii++){
    QDomNode dnTemp = dnlstSystemConnections.item(ii);
    string strConnectionSource = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
    string strConnectionTarget =getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
    string strConnectionID = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    string strConnectionName = getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true);
    string strConnectionType = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTypeTag(), true); // 06/02/03 [ulysses] set the connection type

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

    if(isLocalGroup(strConnectionTarget)){
        clsInfoConnection.setConnectionSourceID(strConnectionSource);
        clsInfoConnection.setConnectionTargetID(strConnectionTarget);
        clsInfoConnection.setConnectionID(strConnectionID);
        clsInfoConnection.setConnectionName(strConnectionName);
        clsInfoConnection.setConnectionType(iConnectionType);  // 06/02/03 [ulysses] set the connection type
        ClsSysFileNode clsSysFileNode = getConnectionSubNodes(&dnTemp);
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

    for(unsigned int ii=0; ii < dnlstSystemConnections.length(); ii++){
    QDomNode dnTemp = dnlstSystemConnections.item(ii);
    string strConnectionSource = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
    string strConnectionTarget = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
    string strConnectionID = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    string strConnectionName = getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true);
    string strConnectionType = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTypeTag(), true);
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

    if(!isLocalGroup(strConnectionSource) && isLocalGroup(strConnectionTarget)){
        clsInfoConnection.setConnectionSourceID(strConnectionSource);
        clsInfoConnection.setConnectionTargetID(strConnectionTarget);
        clsInfoConnection.setConnectionID(strConnectionID);
        clsInfoConnection.setConnectionName(strConnectionName);
        clsInfoConnection.setConnectionType(iConnectionType);
        ClsSysFileNode clsSysFileNode = getConnectionSubNodes(&dnTemp);
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
    for(unsigned int ii=0; ii< dnlstSystemConnections.length(); ii++){
    QDomNode dnTemp = dnlstSystemConnections.item(ii);
    string strConnectionSource = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionSourceTag(), true);
    string strConnectionTarget =getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTargetTag(), true);
    string strConnectionID = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    string strConnectionName = getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true);
    string strConnectionType = getAttributeValue(&dnTemp, ClsTagLibrary::ConnectionTypeTag(), true);

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
        ClsSysFileNode clsSysFileNode = getConnectionSubNodes(&dnTemp);
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
iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getConnectionSubNodes(QDomNodeList *dnlstTop, string _strConnectionID) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getConnectionSubNodes(QDomNodeList *dnlstTop, string _strConnectionID)"<< endl;
#endif


    ClsSysFileNode clsSysFileNode;
    for (unsigned int i1 = 0; i1 < dnlstTop->length(); i1++){
    QDomNode dnConnection =  dnlstTop->item(i1);
    string strConnectionID = getAttributeValue(&dnConnection, ClsTagLibrary::IDTag(), true);
    if(!strConnectionID.compare(_strConnectionID)) {
        clsSysFileNode.setName(_strConnectionID);
        clsSysFileNode = DomNode2SysNode(dnConnection, 0, "");
    }
    }
    return clsSysFileNode;
}

iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getConnectionSubNodes(QDomNode *dnConnection) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getConnectionSubNodes(QDomNode *dnConnectio)"<< endl;
#endif
    ClsSysFileNode clsSysFileNode;
    string strConnectionID = getAttributeValue(dnConnection, ClsTagLibrary::IDTag(), true);
    clsSysFileNode.setName(strConnectionID);
    clsSysFileNode = DomNode2SysNode(dnConnection, 0, "");
    return clsSysFileNode;
}



iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getGroupSubNodes(QDomNodeList *dnlstTop, string _strGroupID) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupSubNodes(QDomNodeList *dnlstTop, string _strGroupID)"<< endl;
#endif

    ClsSysFileNode clsSysFileNode;
    for (unsigned int i1 = 0; i1 < dnlstTop->length(); i1++){
    QDomNode dnGroup =  dnlstTop->item(i1);
    string strGroupID = getAttributeValue(&dnGroup, ClsTagLibrary::IDTag(), true);
    if(!strGroupID.compare(_strGroupID)) {
        clsSysFileNode.setName(_strGroupID);
        clsSysFileNode = DomNode2SysNode(&dnGroup, 0, "");
    }
    }
    return clsSysFileNode;
}

iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::getGroupSubNodes(QDomNode *dnGroup) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getGroupSubNodes(QDomNode *dnGroup)"<< endl;
#endif

    ClsSysFileNode clsSysFileNode;
    string strGroupID = getAttributeValue(&dnGroup, ClsTagLibrary::IDTag(), true);
    clsSysFileNode.setName(strGroupID);
    clsSysFileNode = DomNode2SysNode(&dnGroup, 0, "");
    return clsSysFileNode;
}


bool iqrcommon::ClsSysFileParser::isLocalGroup(string _strGroupID) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::isLocalGroup(string _strGroupID)"<< endl;
#endif
    bool bLocal = false;

    for (unsigned int ii=0; ii< dnlstLocalPrcGroups.length(); ii++){
    QDomNode dnGroup =  dnlstLocalPrcGroups.item(ii);
    string strGroupID = getAttributeValue(&dnGroup, ClsTagLibrary::IDTag(), true);

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
    for(unsigned int ii=0; ii< dnlstSystemGroups.length(); ii++){
    QDomNode dnTemp = dnlstSystemGroups.item(ii);
    string strGroupID = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    if(!strGroupID.compare(_strGroupID)) {
        strParentID = getAttributeValue(&dnTemp.parentNode(), ClsTagLibrary::IDTag(), true);
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



    for(unsigned int ii=0; ii< dnlstClipboardProcesses.length(); ii++){
    QDomNode dnTemp = dnlstClipboardProcesses.item(ii);
    ClsInfoPrc clsInfoPrcTemp;
    string strPrcID = dnTemp.attributes().namedItem(QString(ClsTagLibrary::IDTag())).nodeValue().toStdString();
    clsInfoPrcTemp.setPrcID(strPrcID);
    getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    clsInfoPrcTemp.setProcessName(getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true));
    clsInfoPrcTemp.setHostname(getAttributeValue(&dnTemp, ClsTagLibrary::HostnameTag(), true));
    clsInfoPrcTemp.setPort(getAttributeValue(&dnTemp, ClsTagLibrary::PortTag(), false));
    clsInfoPrcTemp.setEnableModule(getAttributeValue(&dnTemp, ClsTagLibrary::EnableModuleTag(), false));
    clsInfoPrcTemp.setPath(getAttributeValue(&dnTemp, ClsTagLibrary::PathTag(), false));
    clsInfoPrcTemp.setColor(getAttributeValue(&dnTemp, ClsTagLibrary::ColorTag(), false));


    QDomNode dnPrcChildTemp = dnTemp.firstChild();
    while(!dnPrcChildTemp.isNull()){
        string strNodeName = dnPrcChildTemp.nodeName().toStdString();
        if(!strNodeName.compare(ClsTagLibrary::ModuleTag())){
        clsInfoPrcTemp.setHasModule(true);
//		cout << "MODULE FOUND" << endl;
        string strModuleName = getAttributeValue(&dnPrcChildTemp, ClsTagLibrary::NameTag(), true);
//		cout << "strModuleName: " << strModuleName << endl;
        ClsSysFileNode clsSysFileNode;
        clsSysFileNode.setName(ClsTagLibrary::ModuleTag());
        clsSysFileNode.addNode(DomNode2SysNode(&dnPrcChildTemp, 0, ""));
        clsInfoPrcTemp.setProcessSubNodes(clsSysFileNode);

        }
        dnPrcChildTemp = dnPrcChildTemp.nextSibling();
    }
//###------------------------



    string strNotes = getNotesNode(&dnTemp);
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

    QDomNodeList dnlstSystemProcesses = ddocSystemFile.elementsByTagName(QString(ClsTagLibrary::ProcessTag()));
    for(unsigned int ii=0; ii< dnlstSystemProcesses.length(); ii++){
    QDomNode dnTemp = dnlstSystemProcesses.item(ii);
    ClsInfoPrc clsInfoPrcTemp;
    string strPrcID = dnTemp.attributes().namedItem(QString(ClsTagLibrary::IDTag())).nodeValue().toStdString();
    clsInfoPrcTemp.setPrcID(strPrcID);
    getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    clsInfoPrcTemp.setProcessName(getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true));
    clsInfoPrcTemp.setHostname(getAttributeValue(&dnTemp, ClsTagLibrary::HostnameTag(), true));
    clsInfoPrcTemp.setPort(getAttributeValue(&dnTemp, ClsTagLibrary::PortTag(), false));
    clsInfoPrcTemp.setEnableModule(getAttributeValue(&dnTemp, ClsTagLibrary::EnableModuleTag(), false));
    clsInfoPrcTemp.setPath(getAttributeValue(&dnTemp, ClsTagLibrary::PathTag(), false));
    clsInfoPrcTemp.setColor(getAttributeValue(&dnTemp, ClsTagLibrary::ColorTag(), false));



    QDomNode dnPrcChildTemp = dnTemp.firstChild();
    while(!dnPrcChildTemp.isNull()){
        string strNodeName = dnPrcChildTemp.nodeName().toStdString();
        if(!strNodeName.compare(ClsTagLibrary::ModuleTag())){
        clsInfoPrcTemp.setHasModule(true);
        cout << "MODULE FOUND" << endl;

        string strModuleName = getAttributeValue(&dnPrcChildTemp, ClsTagLibrary::NameTag(), true);
        cout << "strModuleName: " << strModuleName << endl;

        ClsSysFileNode clsSysFileNode;
        clsSysFileNode.setName(ClsTagLibrary::ModuleTag());
        clsSysFileNode.addNode(DomNode2SysNode(&dnPrcChildTemp, 0, ""));
        clsInfoPrcTemp.setProcessSubNodes(clsSysFileNode);

        }
        dnPrcChildTemp = dnPrcChildTemp.nextSibling();
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
    list<ClsInfoConnection> lstConnectionFromRemote = getListConnectionFromRemote();
    while (lstConnectionFromRemote.size()){
    string strConnectionID = lstConnectionFromRemote.front().getConnectionID();

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

    QDomNodeList dnlstTemp = ddocSystemFile.elementsByTagName(QString(ClsTagLibrary::SystemTag()));

    QDomNode dnTemp = dnlstTemp.item(0);
    string strSystemID = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    clsInfoSystem.setSystemID(strSystemID);
    clsInfoSystem.setSystemName(getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true));
    clsInfoSystem.setAuthor(getAttributeValue(&dnTemp, ClsTagLibrary::AuthorTag(), true));
    clsInfoSystem.setDate(getAttributeValue(&dnTemp, ClsTagLibrary::DateTag(), true));
    clsInfoSystem.setPort(iqrUtils::string2int(getAttributeValue(&dnTemp, ClsTagLibrary::PortTag(), false)));
    clsInfoSystem.setCyclesPerSecond(iqrUtils::string2int(getAttributeValue(&dnTemp, ClsTagLibrary::CyclesPerSecondTag(), false)));
    clsInfoSystem.setSyncPlots(iqrUtils::string2bool(getAttributeValue(&dnTemp, ClsTagLibrary::SyncPlotsTag(), false)));

    clsInfoSystem.setNotes(getNotesSystem(strSystemID));

    return clsInfoSystem;

}

iqrcommon::ClsInfoPrc iqrcommon::ClsSysFileParser::getProcessInfo(string _strProcessID ) {
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "ClsSysFileParser::getProcessInfo(string _strProcessID )"<< endl;
#endif

    ClsInfoPrc clsInfoPrc;


    QDomNodeList dnlstTemp = ddocSystemFile.elementsByTagName(QString(ClsTagLibrary::ProcessTag()));
    for(unsigned int ii=0; ii< dnlstTemp.length(); ii++){
    QDomNode dnTemp = dnlstTemp.item(ii);
    string strProcessID = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    if(!strProcessID.compare(_strProcessID)) {
        clsInfoPrc.setPrcID(getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true));
        clsInfoPrc.setProcessName(getAttributeValue(&dnTemp, ClsTagLibrary::NameTag(), true));
        clsInfoPrc.setHostname(getAttributeValue(&dnTemp, ClsTagLibrary::HostnameTag(), true));
        clsInfoPrc.setPort(iqrUtils::string2int(getAttributeValue(&dnTemp, ClsTagLibrary::PortTag(), false)));
        clsInfoPrc.setEnableModule(getAttributeValue(&dnTemp, ClsTagLibrary::EnableModuleTag(), false));
        clsInfoPrc.setPath(getAttributeValue(&dnTemp, ClsTagLibrary::PathTag(), false));
        clsInfoPrc.setColor(getAttributeValue(&dnTemp, ClsTagLibrary::ColorTag(), false));

        string strNotes = getNotesProcess( strProcessID );
        clsInfoPrc.setNotes(strNotes);

/* some how we have to find out whether there's a module */
        QDomNode dnPrcChildTemp = dnTemp.firstChild();
        while(!dnPrcChildTemp.isNull()){
        string strNodeName = dnPrcChildTemp.nodeName().toStdString();
        if(!strNodeName.compare(ClsTagLibrary::ModuleTag())){
// 		    if(!strNodeName.compare("Module"))
            clsInfoPrc.setHasModule(true);
        }
        dnPrcChildTemp = dnPrcChildTemp.nextSibling();
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
    return getNotesNode(ClsTagLibrary::SystemTag(), strID);
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

    QDomNodeList dnlst = ddocSystemFile.elementsByTagName(QString(ClsTagLibrary::NotesTag()));


    for (unsigned int ii = 0; ii < dnlst->getLength(); ii++){
    QDomNode dnNote = dnlst.item(ii);
    /* check on the parent of the note: System, Process, Group etc */
    if(!strParentNodeName.compare(dnNote.parentNode().nodeName())){
//	    cout << "dnNote->getNodeName(): " <<  XMLString::transcode(dnNote->getNodeName()) << endl;
//	    cout << "dnNote->getNodeType(): " << dnNote->getNodeType() << endl;
//	    cout << "dnNote->hasChildNodes(): " << dnNote->hasChildNodes() << endl;

        string strParentID = findAncestorID( dnNote);
        if(!strParentID.compare(_strNodeID)) {
//		cout << "YEP" << endl;
        QDomNode dnValue = dnNote.firstChild();
        if(!dnValue.isNull()){
//		    cout << "XMLString::transcode(dnValue->getNodeValue()): " << XMLString::transcode(dnValue->getNodeValue()) << endl;
            return dnValue.nodeValue().toStdString();
        }
        else {
            return "";
        }
        }
    }
    }
    return "";
};



void iqrcommon::ClsSysFileParser::buildNotesCache(QDomDocument* ddocRoot){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "iqrcommon::ClsSysFileParser::buildNotesCache()" << endl;
#endif
    mapNotes.clear();

    QDomNodeList dnlst = ddocRoot->elementsByTagName(QString(ClsTagLibrary::NotesTag()));

    string strNote;
    for (unsigned int ii = 0; ii < dnlst->getLength(); ii++){
    QDomNode dnNote = dnlst.item(ii);
    string strParentID = findAncestorID(dnNote);
    QDomNode dnValue = dnNote.firstChild();
    if(!dnValue.isNull()){
        string strNote = dnValue.nodeValue();
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

    QDomNodeList dnlst = ddocSystemFile.elementsByTagName(QString(strParentNodeName.c_str()));
    /* find node with the right id */

    for (unsigned int ii = 0; ii < dnlst.length(); ii++){
    QDomNode dnTemp = dnlst.item(ii);
    string strID = getAttributeValue(&dnTemp, ClsTagLibrary::IDTag(), true);
    if(!strID.compare(_strNodeID)) {


        QDomNode dnChild = dnTemp.firstChild();
        while(!dnChild.isNull()){
        string strNodeName = dnChild.nodeName().toStdString();
        if(!strNodeName.compare(ClsTagLibrary::NotesTag())){
            QDomNode dnValue = dnChild.firstChild();
            if(!dnValue.isNull()){
            return dnValue.nodeValue().toStdString();
            }
            else {
            return "";
            }
        }
        dnChild = dnChild.nextSibling();
        }
    }
    }
    return "";
};



string iqrcommon::ClsSysFileParser::getNotesNode(QDomNode *dnRoot){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "iqrcommon::ClsSysFileParser::getNotesNode(QDomNode *dnRoot)" << endl;
#endif

    QDomNode dnTemp = dnRoot->firstChild();
    while(!dnTemp.isNull()){
    string strNodeName = dnTemp.nodeName().toStdString();
    if(!strNodeName.compare(ClsTagLibrary::NotesTag())){
        QDomNode dnValue = dnTemp.firstChild();
        if(!dnValue.isNull()){
        return dnValue.nodeValue().toStdString();
        }
        else {
        return "";
        }
    }
    dnTemp = dnTemp.nextSibling();
    }
    return "";
}


/**
 * This function find the first ancestor node that has an ID attribute.
 * @return ID of the ancestor
 * @param dn the node who's ancestor we're looking for.
 */
string iqrcommon::ClsSysFileParser::findAncestorID(QDomNode *dn) {
    string strParentID;
    string strNodeName = dn->nodeName().toStdString();
    if(!strNodeName.compare(ClsTagLibrary::SystemTag())){
    strParentID = "";
    } else {
    QDomNode dnParent = dn->parentNode();
    if(dnParent.attributes().namedItem(QString(ClsTagLibrary::IDTag())).isNull()){
// 	  if(dnParent->getAttributes()->getNamedItem("id") == NULL){
        strParentID = findAncestorID(dnParent);
    }
    else {
        strParentID = dnParent.attributes().namedItem(QString(ClsTagLibrary::IDTag())).nodeValue().toStdString();
// 	       strParentID = dnParent->getAttributes()->getNamedItem("id")->getNodeValue().transcode();
    }
    }
    return strParentID;
};


iqrcommon::ClsSysFileNode iqrcommon::ClsSysFileParser::DomNode2SysNode(QDomNode *dnTop, int iCount,string strSep){
    iCount ++;
    strSep += "\t";
    string strNodeName = dnTop->nodeName().toStdString();

#ifdef DEBUG_CLSSYSFILEPARSER
//    cout << "DomNode2SysNode: " << strSep << iCount << " " << strNodeName  << endl;
#endif
    ClsSysFileNode clsSysFileNode(strNodeName);

    // node and it's attributes ++++++++++++++++++++
    QDomNamedNodeMap dnnmapAtt =  dnTop->attributes();
    iqrcommon_old::ClsParameter clsParameterAtt;
    for(unsigned int ii=0; ii < dnnmapAtt.length(); ii++){
    string strAttName = dnnmapAtt.item(ii).nodeName();
    string strAttValue = dnnmapAtt.item(ii).nodeValue();
#ifdef DEBUG_CLSSYSFILEPARSER
//	cout << "DomNode2SysNode: " << strSep << iCount << " " << "AttName: " << strAttName << ", AttValue: " << strAttValue << endl;
#endif
    clsParameterAtt.setParameter(strAttName, strAttValue);
    clsSysFileNode.addParameter(clsParameterAtt);
    }
    QDomNode dnChild = dnTop->firstChild();
    while(!dnChild.isNull()){
    if(dnChild.nodeType() == QDomNode::Element_Node){
        clsSysFileNode.addNode(DomNode2SysNode(&dnChild, iCount, strSep));
    }
    dnChild = dnChild.nextSibling();
    }
    return clsSysFileNode;
};


void iqrcommon::ClsSysFileParser::buildDiagramLineCache(QDomDocument* ddocRoot){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "iqrcommon::ClsSysFileParser::buildDiagramLineCache()" << endl;
#endif
    mapDiagramLine.clear();
    QDomNodeList dnlst = ddocRoot->elementsByTagName(QString(ClsTagLibrary::DiagramLineTag()));
    for (unsigned int ii = 0; ii < dnlst.length(); ii++){
    QDomNode dnTemp =  dnlst.item(ii);
    QDomNode dnParent = dnTemp.parentNode();
    string strParentID = getAttributeValue(&dnParent, ClsTagLibrary::IDTag(), true);

    /* extract APSource and APTarget */
    string strAPSource = getAttributeValue(&dnTemp, ClsTagLibrary::DiagramLineAPSourceTag(), false);
    string strAPTarget = getAttributeValue(&dnTemp, ClsTagLibrary::DiagramLineAPTargetTag(), false);
    ClsInfoDiagramLine clsInfoDiagramLine;
    clsInfoDiagramLine.setAPSource(iqrUtils::string2int(strAPSource));
    clsInfoDiagramLine.setAPTarget(iqrUtils::string2int(strAPTarget));
    QDomNode dnPoint = dnTemp.firstChild();
    while(!dnPoint.isNull()){
        string strNodeName = dnPoint.nodeName().toStdString();
        if( !strNodeName.compare(ClsTagLibrary::DiagramLinePointTag())){
        string strX = getAttributeValue(&dnPoint, ClsTagLibrary::DiagramLinePointXTag(), true);
        string strY = getAttributeValue(&dnPoint, ClsTagLibrary::DiagramLinePointYTag(), true);
        clsInfoDiagramLine.addPoint(strX, strY);
        }
        dnPoint = dnPoint.nextSibling();
    }
    pair<string,ClsInfoDiagramLine> pairTemp(strParentID, clsInfoDiagramLine);
    mapDiagramLine.insert(pairTemp);
    }
}


void iqrcommon::ClsSysFileParser::buildDiagramIconCache(QDomDocument* ddocRoot){
#ifdef DEBUG_CLSSYSFILEPARSER
    cout << "iqrcommon::ClsSysFileParser::buildDiagramIconCache()" << endl;
#endif
    mapDiagramIcon.clear();
    QDomNodeList dnlst = ddocRoot->elementsByTagName(QString(ClsTagLibrary::DiagramIconTag()));
    for (unsigned int ii = 0; ii < dnlst.length(); ii++){
    QDomNode dnTemp =  dnlst.item(ii);
    QDomNode dnParent = dnTemp.parentNode();
    string strParentID = getAttributeValue(&dnParent, ClsTagLibrary::IDTag(), true);
    if(!dnTemp.isNull()){
        ClsInfoDiagramIcon clsInfoDiagramIcon;
        clsInfoDiagramIcon.setX(getAttributeValue(&dnTemp, ClsTagLibrary::DiagramIconXTag(), true));
        clsInfoDiagramIcon.setY(getAttributeValue(&dnTemp, ClsTagLibrary::DiagramIconYTag(), true));
        /* these attributes are implicit, i.e. do not have to be present */
        clsInfoDiagramIcon.setWidth(getAttributeValue(&dnTemp, ClsTagLibrary::DiagramIconWidthTag(), false));
        clsInfoDiagramIcon.setHeight(getAttributeValue(&dnTemp, ClsTagLibrary::DiagramIconHeightTag(), false));
        pair<string,ClsInfoDiagramIcon> pairTemp(strParentID, clsInfoDiagramIcon);
        mapDiagramIcon.insert(pairTemp);
    }
    }
}
