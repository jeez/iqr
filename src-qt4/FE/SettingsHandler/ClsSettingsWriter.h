/****************************************************************************
 ** $Filename: ClsSettingsWriter.h
 **
 ** $Author: Ulysses Bernardet
 **
 *****************************************************************************/


#ifndef CLSSETTINGSWRITER_H
#define CLSSETTINGSWRITER_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <map>

#include <QtXml>

#include "parameter.hpp"
#include "parameterList.hpp"

using namespace iqrcommon;
using namespace std;


class ClsSettingsWriter {

public:
    ClsSettingsWriter();
    bool saveSettings(string strFileName, ParameterList parameterList, list<string> lstLFO);

private:

    bool bXMLPlatformInitialized;
};


#endif /* CLSSETTINGSWRITER_H */
