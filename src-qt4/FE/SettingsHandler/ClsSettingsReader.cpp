/****************************************************************************
 ** $Filename: ClsSettingsReader.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 *****************************************************************************/


#include <stdio.h>
#include "ClsSettingsReader.h"
#include "tagLibrary.hpp"

using namespace iqrcommon;


ClsSettingsReader::ClsSettingsReader( ) {
#ifdef DEBUG_CLSSETTINGSREADER
    cout << "ClsSettingsReader::ClsSettingsReader" << endl;
#endif
    iSysSettingReaderState = PARSER_INSTANTIATED;
    bXMLPlatformInitialized = false;

};


void ClsSettingsReader::parseBuffer(string strFileName)  {

#ifdef DEBUG_CLSSETTINGSREADER
    cout << "ClsSettingsReader::parseBuffer()" << endl;
#endif

    cout << "reading settings from: " << strFileName << endl;

    static bool gDoNamespaces = false;



    if(!bXMLPlatformInitialized){
	try {
	    XMLPlatformUtils::Initialize();
	}
	catch(const XMLException& toCatch) {
	    cerr << "Error during Xerces-c Initialization.\n"
		 << "  Exception message:"
		 << toCatch.getMessage() << endl;
	    bXMLPlatformInitialized = false;
	    return;
	}
	bXMLPlatformInitialized = true;
    }
    //--------------------


    XercesDOMParser* parser = new XercesDOMParser();
    parser->setValidationScheme(XercesDOMParser::Val_Never);
    /*
      XercesDOMParser::Val_Never;
      XercesDOMParser::Val_Auto;
      XercesDOMParser::Val_Always;
    */

//    cout << __FILE__ << ":" << __LINE__ << endl;
    parser->setDoNamespaces(gDoNamespaces);
    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);
//    cout << __FILE__ << ":" << __LINE__ << endl;

    bool errorsOccured = false;
    try {
	parser->parse(strFileName.c_str());
	int errorCount = parser->getErrorCount();
	if (errorCount > 0){
	    errorsOccured = true;
	}
    } catch (const XMLException& e) {
	cerr << "An error occured during parsing (XMLException)\n   NMessage: " <<  XMLString::transcode(e.getMessage()) << endl;
	ClsSettingsReaderException clsSettingsReaderException(XMLString::transcode(e.getMessage()));
	errorsOccured = true;
	throw clsSettingsReaderException;
    } catch (const DOMException& e) {
	cerr << "An error occured during parsing (DOMException)\n   DMessage: " << XMLString::transcode(e.msg) << endl;
	ClsSettingsReaderException clsSettingsReaderException(XMLString::transcode(e.msg));
	errorsOccured = true;
	throw clsSettingsReaderException;
    } catch (const SAXException& e) {
	cerr << "An error occured during parsing (SAXException)\n   DMessage: " <<  XMLString::transcode(e.getMessage()) << endl;
	ClsSettingsReaderException clsSettingsReaderException(XMLString::transcode(e.getMessage()));
	errorsOccured = true;
	throw clsSettingsReaderException;
    } catch (...) {
	cerr << "An error occured during parsing\n " << endl;
	errorsOccured = true;
	ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::PARSE_ERROR);
	throw clsSettingsReaderException;
    }

    if (errorsOccured){
	cerr << "An error occured during parsing\n " << endl;
	ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::PARSE_ERROR);
	throw clsSettingsReaderException;
    }

    if (!errorsOccured) {
	DOMNode* dnIqrSetting;
	DOMNodeList* dnlstTemp;

	ddocIqrSetting = parser->getDocument();

	// first we have to find the top System node
	dnlstTemp = ddocIqrSetting->getElementsByTagName(XMLString::transcode((const char*)"iqrSetting"));

	dnlstTemp = ddocIqrSetting->getElementsByTagName(XMLString::transcode(ClsTagLibrary::iqrSettings()));




	if ( dnlstTemp->getLength() == 1){
	    dnIqrSetting = dnlstTemp->item(0);
	} else if ( dnlstTemp->getLength() < 1)	{
	    ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::NO_IQRSETTING_NODE);
	    throw clsSettingsReaderException;
	}

	iSysSettingReaderState = PARSER_BUFFER_PARSED;
    }

    delete errHandler;
//     delete parser;
};


string ClsSettingsReader::getEntity (string strEntityName ) {
    if(iSysSettingReaderState !=  PARSER_BUFFER_PARSED){
	ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::BUFFER_NOT_PARSED);
	throw clsSettingsReaderException;
    }

    DOMNode* dnTemp=NULL;
    DOMNodeList* dnlstTemp = ddocIqrSetting->getElementsByTagName(XMLString::transcode(strEntityName.c_str()));

    if ( dnlstTemp->getLength() == 1){
	dnTemp = dnlstTemp->item(0);
    } else if ( dnlstTemp->getLength() < 1)	{
	ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::ENTITY_NOT_FOUND);
	throw clsSettingsReaderException;
    }

    string strValue = "";

    DOMNode* dnValue = dnTemp->getFirstChild();
    if(dnValue!=NULL){
	strValue = XMLString::transcode(dnValue->getNodeValue());
    }

    return strValue;
};


list<string> ClsSettingsReader::getListLastFiles() {
#ifdef DEBUG_CLSSETTINGSREADER
    cout << "ClsSettingsReader::getListLastFiles()" << endl;
#endif
    list<string> lstLFO;

    if(iSysSettingReaderState !=  PARSER_BUFFER_PARSED){
	ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::BUFFER_NOT_PARSED);
	throw clsSettingsReaderException;
    }


    DOMNodeList* dnlstTemp = ddocIqrSetting->getElementsByTagName(XMLString::transcode(ClsTagLibrary::lastFilesOpen()));
    DOMNode* dnTemp=NULL;

    if ( dnlstTemp->getLength() == 1){
	dnTemp = dnlstTemp->item(0);
    } else if ( dnlstTemp->getLength() < 1)	{
	ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::ENTITY_NOT_FOUND);
	throw clsSettingsReaderException;
    }

    DOMNodeList* dnlstFiles = dnTemp->getChildNodes();
    unsigned int ii = 0;
    while( ii< dnlstFiles->getLength()){
	DOMNode* dnTempTemp = dnlstFiles->item(ii);
	if(dnTempTemp->getNodeType() == 1){
	    DOMNode* dnValue = dnTempTemp->getFirstChild();
	    if(dnValue!=NULL){
		string strValue = XMLString::transcode(dnValue->getNodeValue());
		lstLFO.push_back(strValue);
	    }
	}
	ii++;
    }

    return lstLFO;
};




/**
 * Terminate the XMLPlatformUtils
 * To be called after the last call to createPrcDOMTree(string strPrcName, string _strPrcID, string strPrcType)
 */
void ClsSettingsReader::terminateXMLPlatformUtils(){
    XMLPlatformUtils::Terminate();
};
