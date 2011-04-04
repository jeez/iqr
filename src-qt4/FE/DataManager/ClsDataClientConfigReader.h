/****************************************************************************
 ** $Filename: ClsDataClientConfigReader.h
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Thu Oct  2 14:33:50 2003
 ** $Date: 2003/10/04 10:07:26 $
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSDATACLIENTCONFIGREADER_H
#define CLSDATACLIENTCONFIGREADER_H    /*+ To stop multiple inclusions. +*/

#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>

#include "ClsBaseParser.h"
#include "ClsDataClientConfigReaderException.h"
#include "ClsDataClientConfig.h"

using namespace iqrcommon;

class ClsDataClientConfigReader : public ClsBaseParser {

public:
    ClsDataClientConfigReader( );
    list<ClsDataClientConfig> getDataClientConfig(string strFileName);
    void terminateXMLPlatformUtils();


private:
    enum CONFIGREADER_STATES {
        PARSER_INSTANTIATED,
        PARSER_BUFFER_PARSED
    };
    int iSysConfigReaderState;
    bool bXMLPlatformInitialized;

    QDomDocument ddocConfig;
};

#endif /* CLSDATACLIENTCONFIGREADER_H */


/// Local Variables:
/// mode: c++
/// End:
