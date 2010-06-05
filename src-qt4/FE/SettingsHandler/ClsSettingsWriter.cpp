#include <iostream>

#include <framework/StdOutFormatTarget.hpp>
#include <framework/MemBufFormatTarget.hpp>
#include <framework/LocalFileFormatTarget.hpp>

#include "tagLibrary.hpp"
#include "ClsSettingsWriter.h"




ClsSettingsWriter::ClsSettingsWriter( ) {
    bXMLPlatformInitialized = false;

};


bool ClsSettingsWriter::saveSettings(string strFileName, ParameterList parameterList, list<string> lstLFO) {
//    cout << "ClsSettingsWriter::saveSettings(string strFileName, ParameterList parameterList, list<string> lstLFO)" << endl;
    

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
//    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"LS"));

    DOMDocumentType *dtd = impl->createDocumentType(XMLString::transcode(ClsTagLibrary::iqrSettings()),
						    XMLString::transcode("-//INI/iqr421"),
						    XMLString::transcode("iqrSystem.dtd"));

    DOMDocument *ddocSetting = impl->createDocument(0, XMLString::transcode(ClsTagLibrary::iqrSettings()), dtd);


    DOMElement *delemSetting;
    delemSetting = ddocSetting->getDocumentElement();

    map<string, DOMElement *> mapDOMElements;
    

     while (parameterList.size()) {
 	string strParamName = parameterList.front()->getName();
 	string strParamValue = parameterList.front()->getValueAsString();
	string strParamCategory = parameterList.front()->getCategory();

	/*
	cout << "strParamName    : " <<    strParamName	     << endl;
	cout << "strParamValue	 : " <<   strParamValue	     << endl;
	cout << "strParamCategory: " <<    strParamCategory  << endl << endl;
	*/

	DOMElement *delemCategory;

	if(mapDOMElements.find(strParamCategory) != mapDOMElements.end()){
	    delemCategory = mapDOMElements.find(strParamCategory)->second;
	} else {
	    delemCategory = ddocSetting->createElement(XMLString::transcode(strParamCategory.c_str()));
	    delemSetting->appendChild(delemCategory);
	    pair<string, DOMElement *> pairTemp(strParamCategory, delemCategory);
	    mapDOMElements.insert(pairTemp);
	}


	DOMElement *delemParameter = ddocSetting->createElement(XMLString::transcode(strParamName.c_str()));
	delemCategory->appendChild(delemParameter);

	DOMText *dtxtValue = ddocSetting->createTextNode(XMLString::transcode(strParamValue.c_str()));
	delemParameter->appendChild(dtxtValue);

 	parameterList.pop_front();
     }


     DOMElement* delemLFO = ddocSetting->createElement(XMLString::transcode(ClsTagLibrary::lastFilesOpen()));
     delemSetting->appendChild(delemLFO);     

      while (lstLFO.size()) {
	  DOMElement *delemFile = ddocSetting->createElement(XMLString::transcode(ClsTagLibrary::FileTag()));
	  delemLFO->appendChild(delemFile);     

	  string strFilename = lstLFO.front();
	  DOMText *dtxtValue = ddocSetting->createTextNode(XMLString::transcode(strFilename.c_str()));
	  delemFile->appendChild(dtxtValue);
 	  lstLFO.pop_front();
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




    XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(strFileName.c_str());

#if XERCES_VERSION_MAJOR >= 3
    DOMLSOutput* theOutput = ((DOMImplementationLS*)impl)->createLSOutput();
    theOutput->setByteStream(myFormTarget);
#endif

    try {
#if XERCES_VERSION_MAJOR >= 3
	theSerializer->write(delemSetting, theOutput);
#else
	theSerializer->writeNode(myFormTarget, *delemSetting);
#endif
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
