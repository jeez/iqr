/****************************************************************************
 ** $Filename: ClsDataClientConfigWriter.h
 ** $Id: ClsDataClientConfigWriter.h,v 1.1 2003/10/04 10:07:47 ulysses Exp $
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Wed Oct  1 00:10:29 2003
 ** $Date: 2003/10/04 10:07:47 $
 **
 ** $Log: ClsDataClientConfigWriter.h,v $
 ** Revision 1.1  2003/10/04 10:07:47  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSDATACLIENTCONFIGWRITER_H
#define CLSDATACLIENTCONFIGWRITER_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <map>

#include <dom/DOM.hpp>
#include <dom/DOMException.hpp>
#include <framework/MemBufInputSource.hpp>
#include <framework/XMLFormatter.hpp>
#include <util/PlatformUtils.hpp>
#include <util/TranscodingException.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLString.hpp>
#include <util/PlatformUtils.hpp>

#include <parsers/XercesDOMParser.hpp>
#include <sax/HandlerBase.hpp>

#include "ClsDataClientConfig.h"

using namespace xercesc;
using namespace std;

class ClsDataClientConfigWriter {

public:
    ClsDataClientConfigWriter();
    bool saveConfig(string strFileName, list<ClsDataClientConfig> lstConfigs);

private:
    bool bXMLPlatformInitialized;
};

#endif /* CLSDATACLIENTCONFIGWRITER_H */

//// Local Variables: 
//// mode: c++
//// End: 
