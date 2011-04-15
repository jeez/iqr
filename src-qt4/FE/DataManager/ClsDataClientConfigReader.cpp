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

    bool parsingSuccessful = false;
    static bool gDoNamespaces = false;

    QXmlSimpleReader *parser = new QXmlSimpleReader();

    parser->setFeature("http://xml.org/sax/features/namespaces", gDoNamespaces);

    QFile *settingsFile = new QFile(strFileName.c_str());
    settingsFile->open(QFile::ReadOnly);

    QXmlInputSource *source = new QXmlInputSource(settingsFile);

    QDomDocument ddocConfig;
    parsingSuccessful = ddocConfig.setContent(source, parser);

    QDomNodeList dnlstClients = ddocConfig.elementsByTagName(ConfigTagLibrary::DataClientTag());

    if (parsingSuccessful) {
        if(dnlstClients.length() > 0){
        QDomNode dnValue;

        unsigned int ii = 0;
        while(ii < dnlstClients.length()){
            QDomNode dnClient = dnlstClients.item(ii);
            ii++;

            string strType = getAttributeValue(&dnClient, ConfigTagLibrary::TypeTag(), true);
            string strID = getAttributeValue(&dnClient, ConfigTagLibrary::IDTag(), false);
            ClsDataClientConfig clsDataClientConfig(strID, strType);


            QDomNodeList dnlstClientChildren = dnClient.childNodes();
            unsigned int i2 = 0;
            while(i2 < dnlstClientChildren.length()){
            QDomNode dnClientChild = dnlstClientChildren.item(i2);
            if(dnClientChild.nodeType() == QDomNode::ElementNode){
                string strName = dnClientChild.nodeName().toStdString();
                if(!strName.compare(ConfigTagLibrary::PositionTag())){
                int iX = 0;
                int iY = 0;
                iX = iqrUtils::string2int(getAttributeValue(&dnClientChild, ConfigTagLibrary::PositionXTag(), true));
                iY = iqrUtils::string2int(getAttributeValue(&dnClientChild, ConfigTagLibrary::PositionYTag(), true));
                clsDataClientConfig.setPosition(iX, iY);
                } else if(!strName.compare(ConfigTagLibrary::Geometry())){
                int iWidth = 0;
                int iHeight = 0;
                    iWidth = iqrUtils::string2int(getAttributeValue(&dnClientChild, ConfigTagLibrary::GeometryWidthTag(), true));
                    iHeight = iqrUtils::string2int(getAttributeValue(&dnClientChild, ConfigTagLibrary::GeometryHeightTag(), true));
                clsDataClientConfig.setGeometry(iWidth, iHeight);
                } else if(!strName.compare(ConfigTagLibrary::StateVariableDisplayTag())){
                QDomNodeList dnlstSVD = dnClientChild.childNodes();
                unsigned int i3 = 0;
                while(i3 < dnlstSVD.length()){
                    QDomNode dnSVD = dnlstSVD.item(i3);
                    if(dnSVD.nodeType() == QDomNode::ElementNode){
                    string strSVDID = getAttributeValue(&dnSVD, ConfigTagLibrary::IDTag(), true);
                    string strItemID = getAttributeValue(&dnSVD, ConfigTagLibrary::ItemIDTag(), true);
                    string strSelectedIndices = getAttributeValue(&dnSVD, ConfigTagLibrary::SelectedIndicesTag(), true);
                    ClsStateVariableDisplayConfig clsStateVariableDisplayConfig(/*strItemType,*/ strSVDID, strItemID, strSelectedIndices);
                    QDomNodeList dnlstSVDParams = dnSVD.childNodes();
                    unsigned int i4 = 0;
                    while(i4 < dnlstSVDParams.length()){
                        QDomNode dnSVDParam = dnlstSVDParams.item(i4);
                        if(dnSVDParam.nodeType() == QDomNode::ElementNode){
                        string strParamName = dnSVDParam.nodeName().toStdString();
                        dnValue = dnSVDParam.firstChild();
                        string strParamValue = "";
                        if(!dnValue.isNull()){
                            strParamValue = dnValue.nodeValue().toStdString();
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
                dnValue = dnClientChild.firstChild();
                if(!dnValue.isNull()){
                    strValue = dnValue.nodeValue().toStdString();
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
        } else {
        ClsDataClientConfigReaderException clsDataClientConfigReaderException(ClsDataClientConfigReaderException::PARSE_ERROR);
        throw clsDataClientConfigReaderException;
        }

    settingsFile->close();
    delete settingsFile;
    return lstConfigs;
};


/**
 * Terminate the XMLPlatformUtils
 * To be called after the last call to createPrcDOMTree(string strPrcName, string _strPrcID, string strPrcType)
 */
void ClsDataClientConfigReader::terminateXMLPlatformUtils(){
    //XMLPlatformUtils::Terminate();
};



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
