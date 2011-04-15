#include <iostream>
#include <fstream>

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
            delemCategory = ddocSetting.createElement(QString::fromStdString(strParamCategory));
            delemSetting.appendChild(delemCategory);
            pair<string, QDomElement> pairTemp(strParamCategory, delemCategory);
            mapDOMElements.insert(pairTemp);
        }

        QDomElement delemParameter = ddocSetting.createElement(QString::fromStdString(strParamName));
        delemCategory.appendChild(delemParameter);

        QDomText dtxtValue = ddocSetting.createTextNode(QString::fromStdString(strParamValue));
        delemParameter.appendChild(dtxtValue);

        parameterList.pop_front();
    }

    QDomElement delemLFO = ddocSetting.createElement(QString::fromStdString(ClsTagLibrary::lastFilesOpen()));
    delemSetting.appendChild(delemLFO);

    while (lstLFO.size()) {
        QDomElement delemFile = ddocSetting.createElement(QString::fromStdString(ClsTagLibrary::FileTag()));
        delemLFO.appendChild(delemFile);
        string strFilename = lstLFO.front();
        QDomText dtxtValue = ddocSetting.createTextNode(QString::fromStdString(strFilename));
        delemFile.appendChild(dtxtValue);
        lstLFO.pop_front();
    }

    QTextStream output(new QString());
    delemSetting.save(output, 4);

    ofstream settingsFile;
    settingsFile.open(strFileName.c_str());
    settingsFile << output.readAll().toStdString();
    settingsFile.close();
    return true;
}
