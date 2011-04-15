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

    bool parsingSuccessful = false;
    static bool gDoNamespaces = false;

    bXMLPlatformInitialized = true;

    QXmlSimpleReader *parser = new QXmlSimpleReader();
    parser->setFeature("http://xml.org/sax/features/namespaces", gDoNamespaces);

    //ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    //parser->setErrorHandler(errHandler);

    QFile *settingsFile = new QFile(strFileName.c_str());
    settingsFile->open(QFile::ReadOnly);

    QXmlInputSource *source = new QXmlInputSource(settingsFile);

    parsingSuccessful = ddocIqrSetting.setContent(source, parser);

    if (!parsingSuccessful){
    cerr << "An error occured during parsing\n " << endl;
    ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::PARSE_ERROR);
    throw clsSettingsReaderException;
    }

    if (parsingSuccessful) {
    QDomNode dnIqrSetting;
    QDomNodeList dnlstTemp;

    // first we have to find the top System node
    //dnlstTemp = ddocIqrSetting.elementsByTagName("iqrSetting");
    dnlstTemp = ddocIqrSetting.elementsByTagName(QString::fromStdString(ClsTagLibrary::iqrSettings()));

    if ( dnlstTemp.length() == 1){
        dnIqrSetting = dnlstTemp.item(0);
    } else if ( dnlstTemp.length() < 1) {
        ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::NO_IQRSETTING_NODE);
        throw clsSettingsReaderException;
    }

    iSysSettingReaderState = PARSER_BUFFER_PARSED;
    }

    settingsFile->close();
    delete settingsFile;
    //delete errHandler;
};


string ClsSettingsReader::getEntity (string strEntityName ) {
    if(iSysSettingReaderState !=  PARSER_BUFFER_PARSED){
    ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::BUFFER_NOT_PARSED);
    throw clsSettingsReaderException;
    }

    QDomNode dnTemp;
    QDomNodeList dnlstTemp = ddocIqrSetting.elementsByTagName(QString::fromStdString(strEntityName.c_str()));

    if ( dnlstTemp.length() == 1){
    dnTemp = dnlstTemp.item(0);
    } else if ( dnlstTemp.length() < 1) {
    ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::ENTITY_NOT_FOUND);
    throw clsSettingsReaderException;
    }

    string strValue = "";

    QDomNode dnValue = dnTemp.firstChild();
    if(!dnValue.isNull()){
    strValue = dnValue.nodeValue().toStdString();
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


    QDomNodeList dnlstTemp = ddocIqrSetting.elementsByTagName(QString::fromStdString(ClsTagLibrary::lastFilesOpen()));
    QDomNode dnTemp;

    if ( dnlstTemp.length() == 1){
    dnTemp = dnlstTemp.item(0);
    } else if ( dnlstTemp.length() < 1){
    ClsSettingsReaderException clsSettingsReaderException(ClsSettingsReaderException::ENTITY_NOT_FOUND);
    throw clsSettingsReaderException;
    }

    QDomNodeList dnlstFiles = dnTemp.childNodes();
    unsigned int ii = 0;
    while( ii< dnlstFiles.length()){
    QDomNode dnTempTemp = dnlstFiles.item(ii);
    if(dnTempTemp.nodeType() == QDomNode::ElementNode){
        QDomNode dnValue = dnTempTemp.firstChild();
        if(!dnValue.isNull()){
        string strValue = dnValue.nodeValue().toStdString();
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

};
