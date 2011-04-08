#include <iostream>

#include "iqrUtils.h"

#include "ConfigTagLibrary.h"
#include "ClsDataClientConfigWriter.h"

#include <QtXml>

ClsDataClientConfigWriter::ClsDataClientConfigWriter( ) {
    bXMLPlatformInitialized = false;
};

bool ClsDataClientConfigWriter::saveConfig(string strFileName, list<ClsDataClientConfig> lstConfigs) {
//    cout << "ClsDataClientConfigWriter::saveConfig(string strFileName, list<ClsDataClientConfig> lstConfigs)" << endl;

    if(!bXMLPlatformInitialized){
        bXMLPlatformInitialized = true;
    }


    //DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode((const char*)"Range"));

    QDomImplementation impl;
    QDomDocumentType dtd = impl.ccreateDocumentType(QString.fromStdString(ConfigTagLibrary::DataManagerConfiguration()),
                            QString.fromStdString("-//INI/iqr421"),
                            QString.fromStdString("iqrDataManagerConfiguration.dtd"));

    QDomDocument ddocConfig = impl.createDocument(0, QString.fromStdString(ConfigTagLibrary::DataManagerConfiguration()), dtd);

    QDomElement delemConfig;
    delemConfig = ddocConfig.documentElement();

    list<ClsDataClientConfig>::iterator itConfigs;
    for(itConfigs=lstConfigs.begin(); itConfigs != lstConfigs.end(); itConfigs++){
    string strID = (*itConfigs).getID();
    string strType =  (*itConfigs).getType();
    pair<int,int> pPosition =  (*itConfigs).getPosition();
    pair<int,int> pGeometry =  (*itConfigs).getGeometry();

    QDomElement delemDataClient;
    delemDataClient = ddocConfig.createElement(QString.fromStdString(ConfigTagLibrary::DataClientTag()));
    delemDataClient.setAttribute(QString.fromStdString(ConfigTagLibrary::TypeTag()), QString.fromStdString(strType.c_str()));
    delemDataClient.setAttribute(QString.fromStdString(ConfigTagLibrary::IDTag()), QString.fromStdString(strID.c_str()));
    delemConfig.appendChild(delemDataClient);

    QDomElement delemPosition;
    delemPosition = ddocConfig.createElement(QString.fromStdString(ConfigTagLibrary::PositionTag()));
    delemPosition.setAttribute(QString.fromStdString(ConfigTagLibrary::PositionXTag()),
                    QString.fromStdString(iqrUtils::int2string(pPosition.first).c_str()));
    delemPosition.setAttribute(QString.fromStdString(ConfigTagLibrary::PositionYTag()),
                    QString.fromStdString(iqrUtils::int2string(pPosition.second).c_str()));
    delemDataClient.appendChild(delemPosition);

    QDomElement delemGeometry;
    delemGeometry = ddocConfig.createElement(QString.fromStdString(ConfigTagLibrary::Geometry()));
    delemGeometry.setAttribute(QString.fromStdString(ConfigTagLibrary::GeometryWidthTag()),
                    QString.fromStdString(iqrUtils::int2string(pGeometry.first).c_str()));
    delemGeometry.setAttribute(QString.fromStdString(ConfigTagLibrary::GeometryHeightTag()),
                    QString.fromStdString(iqrUtils::int2string(pGeometry.second).c_str()));
    delemDataClient.appendChild(delemGeometry);



    list<pair<string, string> > lstParameters = (*itConfigs).getListParameters();
    list<pair<string, string> >::iterator itLstParameters;
    for(itLstParameters = lstParameters.begin(); itLstParameters != lstParameters.end(); itLstParameters++){
        string strParamName = (*itLstParameters).first;
        string strParamValue = (*itLstParameters).second;
        QDomElement delemParameter;
        delemParameter = ddocConfig.createElement(QString.fromStdString(strParamName.c_str()));
        delemDataClient.appendChild(delemParameter);
        QDomText dtxtParamValue = ddocConfig.createTextNode(QString.fromStdString(strParamValue.c_str()));
        delemParameter.appendChild(dtxtParamValue);
    }

    QDomElement delemSVD;
    delemSVD = ddocConfig.createElement(QString.fromStdString(ConfigTagLibrary::StateVariableDisplayTag()));
    delemDataClient.appendChild(delemSVD);

    list<ClsStateVariableDisplayConfig>lstSVDConfigs =  (*itConfigs).getListStateVariableDisplayConfig();
    list<ClsStateVariableDisplayConfig>::iterator itSVD;
    for(itSVD = lstSVDConfigs.begin(); itSVD != lstSVDConfigs.end(); itSVD++){
        QDomElement delemStateVariable;
        delemStateVariable = ddocConfig.createElement(QString.fromStdString(ConfigTagLibrary::StateVariableDisplaysTag()));
        delemSVD.appendChild(delemStateVariable);
        delemStateVariable.setAttribute(QString.fromStdString(ConfigTagLibrary::IDTag()),
                         QString.fromStdString((*itSVD).getID().c_str()));

/*
        delemStateVariable->setAttribute(XMLString::transcode(ConfigTagLibrary::TypeTag()),
                         XMLString::transcode((*itSVD).getItemType().c_str()));
*/

        delemStateVariable.setAttribute(QString.fromStdString(ConfigTagLibrary::ItemIDTag()),
                         QString.fromStdString((*itSVD).getItemID().c_str()));

        delemStateVariable.setAttribute(QString.fromStdString(ConfigTagLibrary::SelectedIndicesTag()),
                         QString.fromStdString((*itSVD).getSelectedIndices().c_str()));

        list<pair<string, string> > lstParametersSVD = (*itSVD).getListParameters();
        list<pair<string, string> >::iterator itLstParametersSVD;
        for(itLstParametersSVD = lstParametersSVD.begin(); itLstParametersSVD != lstParametersSVD.end(); itLstParametersSVD++){
        string strParamName = (*itLstParametersSVD).first;
        string strParamValue = (*itLstParametersSVD).second;
        QDomElement delemParameter;
        delemParameter = ddocConfig.createElement(QString.fromStdString(strParamName.c_str()));
        delemStateVariable.appendChild(delemParameter);
        QDomText dtxtParamValue = ddocConfig.createTextNode(QString.fromStdString(strParamValue.c_str()));
        delemParameter.appendChild(dtxtParamValue);
        }
    }
    }

//TODO CHECK THIS
//We don't have a direct mapping between DOMLSSerializer or DOMWriter to QtXML
// Need to investigate this a bit further, but the idea would be using directly QDomDocument to write
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
    theSerializer->write(delemConfig, theOutput);
#else
    theSerializer->writeNode(myFormTarget, *delemConfig);
#endif
    }
    catch (const XMLException& toCatch) {
    char* message = QString.fromStdString(toCatch.getMessage());
    cerr << "Exception message is: \n"
         << message << "\n";
    XMLString::release(&message);
    }
    catch (const DOMException& toCatch) {
    char* message = QString.fromStdString(toCatch.msg);
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
