#include <iostream>

#include <framework/StdOutFormatTarget.hpp>
#include <framework/MemBufFormatTarget.hpp>
#include <framework/LocalFileFormatTarget.hpp>

#include "iqrUtils.h"

#include "ConfigTagLibrary.h"
#include "ClsDataClientConfigWriter.h"


ClsDataClientConfigWriter::ClsDataClientConfigWriter( ) {
    bXMLPlatformInitialized = false;

};

bool ClsDataClientConfigWriter::saveConfig(string strFileName, list<ClsDataClientConfig> lstConfigs) {
//    cout << "ClsDataClientConfigWriter::saveConfig(string strFileName, list<ClsDataClientConfig> lstConfigs)" << endl;

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


    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));

    DOMDocumentType *dtd = impl->createDocumentType(XMLString::transcode(ConfigTagLibrary::DataManagerConfiguration()),
						    XMLString::transcode("-//INI/iqr421"),
						    XMLString::transcode("iqrDataManagerConfiguration.dtd"));

    DOMDocument *ddocConfig = impl->createDocument(0, XMLString::transcode(ConfigTagLibrary::DataManagerConfiguration()), dtd);

    DOMElement *delemConfig;
    delemConfig = ddocConfig->getDocumentElement();

    list<ClsDataClientConfig>::iterator itConfigs;
    for(itConfigs=lstConfigs.begin(); itConfigs != lstConfigs.end(); itConfigs++){
	string strID = (*itConfigs).getID();
	string strType =  (*itConfigs).getType();
	pair<int,int> pPosition =  (*itConfigs).getPosition();
	pair<int,int> pGeometry =  (*itConfigs).getGeometry();

 	DOMElement *delemDataClient;
	delemDataClient = ddocConfig->createElement(XMLString::transcode(ConfigTagLibrary::DataClientTag()));
	delemDataClient->setAttribute(XMLString::transcode(ConfigTagLibrary::TypeTag()), XMLString::transcode(strType.c_str()));
	delemDataClient->setAttribute(XMLString::transcode(ConfigTagLibrary::IDTag()), XMLString::transcode(strID.c_str()));
	delemConfig->appendChild(delemDataClient);

 	DOMElement *delemPosition;
	delemPosition = ddocConfig->createElement(XMLString::transcode(ConfigTagLibrary::PositionTag()));
	delemPosition->setAttribute(XMLString::transcode(ConfigTagLibrary::PositionXTag()), 
				    XMLString::transcode(iqrUtils::int2string(pPosition.first).c_str()));
	delemPosition->setAttribute(XMLString::transcode(ConfigTagLibrary::PositionYTag()),
				    XMLString::transcode(iqrUtils::int2string(pPosition.second).c_str()));
	delemDataClient->appendChild(delemPosition);

 	DOMElement *delemGeometry;
	delemGeometry = ddocConfig->createElement(XMLString::transcode(ConfigTagLibrary::Geometry()));
	delemGeometry->setAttribute(XMLString::transcode(ConfigTagLibrary::GeometryWidthTag()), 
				    XMLString::transcode(iqrUtils::int2string(pGeometry.first).c_str()));
	delemGeometry->setAttribute(XMLString::transcode(ConfigTagLibrary::GeometryHeightTag()), 
				    XMLString::transcode(iqrUtils::int2string(pGeometry.second).c_str()));
	delemDataClient->appendChild(delemGeometry);



	list<pair<string, string> > lstParameters = (*itConfigs).getListParameters();
	list<pair<string, string> >::iterator itLstParameters;
	for(itLstParameters = lstParameters.begin(); itLstParameters != lstParameters.end(); itLstParameters++){
	    string strParamName = (*itLstParameters).first;
	    string strParamValue = (*itLstParameters).second;
	    DOMElement *delemParameter;
	    delemParameter = ddocConfig->createElement(XMLString::transcode(strParamName.c_str()));
	    delemDataClient->appendChild(delemParameter);
	    DOMText *dtxtParamValue = ddocConfig->createTextNode(XMLString::transcode(strParamValue.c_str()));
	    delemParameter->appendChild(dtxtParamValue);
	}

 	DOMElement *delemSVD;
	delemSVD = ddocConfig->createElement(XMLString::transcode(ConfigTagLibrary::StateVariableDisplayTag()));
	delemDataClient->appendChild(delemSVD);

	list<ClsStateVariableDisplayConfig>lstSVDConfigs =  (*itConfigs).getListStateVariableDisplayConfig();
	list<ClsStateVariableDisplayConfig>::iterator itSVD;
	for(itSVD = lstSVDConfigs.begin(); itSVD != lstSVDConfigs.end(); itSVD++){
	    DOMElement *delemStateVariable;
	    delemStateVariable = ddocConfig->createElement(XMLString::transcode(ConfigTagLibrary::StateVariableDisplaysTag()));
	    delemSVD->appendChild(delemStateVariable);
	    delemStateVariable->setAttribute(XMLString::transcode(ConfigTagLibrary::IDTag()), 
					     XMLString::transcode((*itSVD).getID().c_str()));
	    
/*
	    delemStateVariable->setAttribute(XMLString::transcode(ConfigTagLibrary::TypeTag()), 
					     XMLString::transcode((*itSVD).getItemType().c_str()));
*/
	
	    delemStateVariable->setAttribute(XMLString::transcode(ConfigTagLibrary::ItemIDTag()), 
					     XMLString::transcode((*itSVD).getItemID().c_str()));

	    delemStateVariable->setAttribute(XMLString::transcode(ConfigTagLibrary::SelectedIndicesTag()), 
					     XMLString::transcode((*itSVD).getSelectedIndices().c_str()));

	    list<pair<string, string> > lstParametersSVD = (*itSVD).getListParameters();
	    list<pair<string, string> >::iterator itLstParametersSVD;
	    for(itLstParametersSVD = lstParametersSVD.begin(); itLstParametersSVD != lstParametersSVD.end(); itLstParametersSVD++){
		string strParamName = (*itLstParametersSVD).first;
		string strParamValue = (*itLstParametersSVD).second;
		DOMElement *delemParameter;
		delemParameter = ddocConfig->createElement(XMLString::transcode(strParamName.c_str()));
		delemStateVariable->appendChild(delemParameter);
		DOMText *dtxtParamValue = ddocConfig->createTextNode(XMLString::transcode(strParamValue.c_str()));
		delemParameter->appendChild(dtxtParamValue);
	    }
	}
    }


    DOMWriter* theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
//    XMLFormatTarget *myFormTarget = new StdOutFormatTarget();
//    MemBufFormatTarget *myFormTarget = new MemBufFormatTarget();
    XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(strFileName.c_str());

    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);


    try {
	// do the serialization through DOMWriter::writeNode();
	theSerializer->writeNode(myFormTarget, *delemConfig);
    }
    catch (const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cerr << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
    }
    catch (const DOMException& toCatch) {
	char* message = XMLString::transcode(toCatch.msg);
	cerr << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
    }
    catch (...) {
	cerr << "Unexpected Exception \n" ;
    }

//    cout << myFormTarget->getRawBuffer() << endl;

    theSerializer->release();
    delete myFormTarget;


    return true;

}

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
