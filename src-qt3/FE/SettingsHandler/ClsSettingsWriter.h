/****************************************************************************
 ** $Filename: ClsSettingsWriter.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sat Aug 16 22:48:25 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSSETTINGSWRITER_H
#define CLSSETTINGSWRITER_H    /*+ To stop multiple inclusions. +*/

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

#include "parameter.hpp"
#include "parameterList.hpp"

using namespace xercesc;
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
