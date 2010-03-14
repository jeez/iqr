/****************************************************************************
 ** $Filename: ClsDataClientConfigReader.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Thu Oct  2 14:34:15 2003
 ** $Date: 2003/10/04 10:06:49 $
 **
 ** $Log: ClsDataClientConfigReader.cpp,v $
 ** Revision 1.1  2003/10/04 10:06:49  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <stdio.h>
#include "ClsDataClientConfigReader.h"


#include "ConfigTagLibrary.h"
#include "iqrUtils.h"

//#define  DEBUG_CLSDATACLIENTCONFIGREADER



ClsDataClientConfigReader::ClsDataClientConfigReader( ) {
#ifdef DEBUG_CLSDATACLIENTCONFIGREADER
    cout << "ClsDataClientConfigReader::ClsDataClientConfigReader" << endl;
#endif
    iSysConfigReaderState = PARSER_INSTANTIATED;
    bXMLPlatformInitialized = false;

};


list<ClsDataClientConfig> ClsDataClientConfigReader::getDataClientConfig(string strFileName)  {
#ifdef DEBUG_CLSDATACLIENTCONFIGREADER
    cout << "ClsDataClientConfigReader::getDataClientConfig()" << endl;
#endif

    list<ClsDataClientConfig> lstConfigs;
#ifdef DEBUG_CLSDATACLIENTCONFIGREADER
    cout << "reading settings from: " << strFileName << endl;
#endif

    bool errorsOccured = false;
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
	    errorsOccured = true;
//	    return;
	}
	bXMLPlatformInitialized = true;
	errorsOccured = false;
    }
    //--------------------

    if (!errorsOccured) {
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Never);
	/*
	  XercesDOMParser::Val_Never;
	  XercesDOMParser::Val_Auto;
	  XercesDOMParser::Val_Always;
	*/


	parser->setDoNamespaces(gDoNamespaces);
	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);


	try {
	    parser->parse(strFileName.c_str());

	    int errorCount = parser->getErrorCount();
	    if (errorCount > 0){
		errorsOccured = true;
	    }

	} catch (const XMLException& e) {
	    cerr << "An error occured during parsing (XMLException)\n   NMessage: " <<  XMLString::transcode(e.getMessage()) << endl;
	    ClsDataClientConfigReaderException clsDataClientConfigReaderException(XMLString::transcode(e.getMessage()));
	    errorsOccured = true;
	    throw clsDataClientConfigReaderException;
	} catch (const DOMException& e) {
	    cerr << "An error occured during parsing (DOMException)\n   DMessage: " << XMLString::transcode(e.msg) << endl;
	    ClsDataClientConfigReaderException clsDataClientConfigReaderException(XMLString::transcode(e.msg));
	    errorsOccured = true;
	    throw clsDataClientConfigReaderException;
	} catch (const SAXException& e) {
	    cerr << "An error occured during parsing (SAXException)\n   DMessage: " <<  XMLString::transcode(e.getMessage()) << endl;
	    ClsDataClientConfigReaderException clsDataClientConfigReaderException(XMLString::transcode(e.getMessage()));
	    errorsOccured = true;
	    throw clsDataClientConfigReaderException;
	} catch (...) {
	    cerr << "An error occured during parsing\n " << endl;
	    errorsOccured = true;
	    ClsDataClientConfigReaderException clsDataClientConfigReaderException(ClsDataClientConfigReaderException::PARSE_ERROR);
	    throw clsDataClientConfigReaderException;
	}

	/* DOMNode* dnIqrConfig; */
	DOMDocument *ddocConfig = parser->getDocument();

	DOMNodeList* dnlstClients = ddocConfig->getElementsByTagName(XMLString::transcode(ConfigTagLibrary::DataClientTag()));

	try{
	    if(dnlstClients->getLength()>0){
		DOMNode* dnValue = NULL;

		unsigned int ii = 0;
		while( ii< dnlstClients->getLength()){
		    DOMNode* dnClient = dnlstClients->item(ii);
		    ii++;

		    string strType = getAttributeValue(dnClient, ConfigTagLibrary::TypeTag(), true);
		    string strID = getAttributeValue(dnClient, ConfigTagLibrary::IDTag(), false);
		    ClsDataClientConfig clsDataClientConfig(strID, strType);


		    DOMNodeList* dnlstClientChildren = dnClient->getChildNodes();
		    unsigned int i2 = 0;
		    while( i2< dnlstClientChildren->getLength()){
			DOMNode* dnClientChild = dnlstClientChildren->item(i2);
			if(dnClientChild->getNodeType() == 1){
			    string strName = XMLString::transcode(dnClientChild->getNodeName());
			    if(!strName.compare(ConfigTagLibrary::PositionTag())){
				int iX = 0;
				int iY = 0;
				iX = iqrUtils::string2int(getAttributeValue(dnClientChild, ConfigTagLibrary::PositionXTag(), true));
				iY = iqrUtils::string2int(getAttributeValue(dnClientChild, ConfigTagLibrary::PositionYTag(), true));
				clsDataClientConfig.setPosition(iX, iY);
			    } else if(!strName.compare(ConfigTagLibrary::Geometry())){
				int iWidth = 0;
				int iHeight = 0;
				    iWidth = iqrUtils::string2int(getAttributeValue(dnClientChild, ConfigTagLibrary::GeometryWidthTag(), true));
				    iHeight = iqrUtils::string2int(getAttributeValue(dnClientChild, ConfigTagLibrary::GeometryHeightTag(), true));
				clsDataClientConfig.setGeometry(iWidth, iHeight);
			    } else if(!strName.compare(ConfigTagLibrary::StateVariableDisplayTag())){
				DOMNodeList* dnlstSVD = dnClientChild->getChildNodes();
				unsigned int i3 = 0;
				while( i3< dnlstSVD->getLength()){
				    DOMNode* dnSVD = dnlstSVD->item(i3);
				    if(dnSVD->getNodeType() == 1){
					string strSVDID = getAttributeValue(dnSVD, ConfigTagLibrary::IDTag(), true);
//--					string strItemType = getAttributeValue(dnSVD, ConfigTagLibrary::TypeTag(), true);
					string strItemID = getAttributeValue(dnSVD, ConfigTagLibrary::ItemIDTag(), true);
					string strSelectedIndices = getAttributeValue(dnSVD, ConfigTagLibrary::SelectedIndicesTag(), true);
					ClsStateVariableDisplayConfig clsStateVariableDisplayConfig(/*strItemType,*/ strSVDID, strItemID, strSelectedIndices);
					DOMNodeList* dnlstSVDParams = dnSVD->getChildNodes();
					unsigned int i4 = 0;
					while( i4< dnlstSVDParams->getLength()){
					    DOMNode* dnSVDParam = dnlstSVDParams->item(i4);
					    if(dnSVDParam->getNodeType() == 1){
						string strParamName = XMLString::transcode(dnSVDParam->getNodeName());
						dnValue = dnSVDParam->getFirstChild();
						string strParamValue = "";
						if(dnValue!=NULL){
						    strParamValue = XMLString::transcode(dnValue->getNodeValue());
						}
						pair<string, string> pParam(strParamName, strParamValue);
						clsStateVariableDisplayConfig.addParameter(pParam);
					    }
					    i4++;
					}
					clsDataClientConfig.addStateVariableDisplayConfig(clsStateVariableDisplayConfig);
				    }
				    i3++;
				}
			    } else {
				string strValue = "";
				dnValue = dnClientChild->getFirstChild();
				if(dnValue!=NULL){
				    strValue = XMLString::transcode(dnValue->getNodeValue());
				}
				pair<string, string> pParam(strName, strValue);
				clsDataClientConfig.addParameter(pParam);
			    }
			}
			i2++;
		    }
		    lstConfigs.push_back(clsDataClientConfig);
		}
	    }
	} catch (...) {
	    ClsDataClientConfigReaderException clsDataClientConfigReaderException(ClsDataClientConfigReaderException::PARSE_ERROR);
	    throw clsDataClientConfigReaderException;
	}

	delete errHandler;
    }

    return lstConfigs;
};


/**
 * Terminate the XMLPlatformUtils
 * To be called after the last call to createPrcDOMTree(string strPrcName, string _strPrcID, string strPrcType)
 */
void ClsDataClientConfigReader::terminateXMLPlatformUtils(){
    XMLPlatformUtils::Terminate();
};



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
