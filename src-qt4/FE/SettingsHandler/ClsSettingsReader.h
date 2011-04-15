#ifndef CLSSETTINGSREADER_H
#define CLSSETTINGSREADER_H

#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>

#include <QtXml>

#include "ClsSettingsReaderException.h"


class ClsSettingsReader {

public:
     ClsSettingsReader( );
     void parseBuffer(string strFileName);
     void terminateXMLPlatformUtils();

/* new functions: */
     string getEntity (string strEntityName );
     list<string> getListLastFiles();


private:
     enum SETTINGREADER_STATES {
      PARSER_INSTANTIATED,
      PARSER_BUFFER_PARSED
     };
     int iSysSettingReaderState;
    bool bXMLPlatformInitialized;

     QDomDocument ddocIqrSetting;


};

#endif

/// Local Variables:
/// mode: c++
/// End:




