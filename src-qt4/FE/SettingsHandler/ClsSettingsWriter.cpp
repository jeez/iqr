#include <iostream>

#include "tagLibrary.hpp"
#include "ClsSettingsWriter.h"


ClsSettingsWriter::ClsSettingsWriter( ) {
    bXMLPlatformInitialized = false;
};


bool ClsSettingsWriter::saveSettings(string strFileName, ParameterList parameterList, list<string> lstLFO) {
    if(!bXMLPlatformInitialized){
        bXMLPlatformInitialized = true;
    }

    QDomImplementation impl;

    QDomDocumentType dtd = impl.createDocumentType(QString::fromStdString(ClsTagLibrary::iqrSettings()),
                            "-//INI/iqr421", "iqrSystem.dtd");

    QDomDocument ddocSetting = impl.createDocument(0, QString::fromStdString(ClsTagLibrary::iqrSettings()), dtd);

    QDomElement delemSetting;
    delemSetting = ddocSetting.documentElement();

    map<string, QDomElement> mapDOMElements;

    while (parameterList.size()) {
        string strParamName = parameterList.front()->getName();
        string strParamValue = parameterList.front()->getValueAsString();
        string strParamCategory = parameterList.front()->getCategory();

        QDomElement delemCategory;

        if(mapDOMElements.find(strParamCategory) != mapDOMElements.end()){
            delemCategory = mapDOMElements.find(strParamCategory)->second;
        } else {
            delemCategory = ddocSetting.createElement(QString::fromStdString(strParamCategory.c_str()));
            delemSetting.appendChild(delemCategory);
            pair<string, QDomElement> pairTemp(strParamCategory, delemCategory);
            mapDOMElements.insert(pairTemp);
        }

        QDomElement delemParameter = ddocSetting.createElement(QString::fromStdString(strParamName.c_str()));
        delemCategory.appendChild(delemParameter);

        QDomText dtxtValue = ddocSetting.createTextNode(QString::fromStdString(strParamValue.c_str()));
        delemParameter.appendChild(dtxtValue);

        parameterList.pop_front();
    }

    QDomElement delemLFO = ddocSetting.createElement(QString::fromStdString(ClsTagLibrary::lastFilesOpen()));
    delemSetting.appendChild(delemLFO);

    while (lstLFO.size()) {
        QDomElement delemFile = ddocSetting.createElement(QString::fromStdString(ClsTagLibrary::FileTag()));
        delemLFO.appendChild(delemFile);
        string strFilename = lstLFO.front();
        QDomText dtxtValue = ddocSetting.createTextNode(QString::fromStdString(strFilename.c_str()));
        delemFile.appendChild(dtxtValue);
        lstLFO.pop_front();
    }

    QFile myFormTarget(QString::fromStdString(strFileName.c_str()));
    myFormTarget.open(QFile::ReadWrite);
    QTextStream output(&myFormTarget);
    delemSetting.save(output, 4);
    myFormTarget.close();

    return true;
}
