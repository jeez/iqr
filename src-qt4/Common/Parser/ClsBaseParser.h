/****************************************************************************
 ** $Filename: ClsBaseParser.h
 **
 ** $Author: Ulysses Bernardet
 **
 *****************************************************************************/


#ifndef CLSBASEPARSER_H
#define CLSBASEPARSER_H    /*+ To stop multiple inclusions. +*/

#include <cstdlib>
#include <iostream>
#include <string>
#include <list>

#include <framework/MemBufInputSource.hpp>
#include <parsers/XercesDOMParser.hpp>
#include <dom/DOM.hpp>
#include <sax/HandlerBase.hpp>
#include <util/XMLString.hpp>
#include <util/PlatformUtils.hpp>

#include "../Helper/iqrUtils.h"

#include "ClsSysFileParserException.h"

using namespace xercesc;
using namespace std;

namespace iqrcommon {
    class ClsBaseParser {


    public:
	virtual ~ClsBaseParser(){};

    protected:
	inline string getAttributeValue(DOMNode *dn, string strAttributeName, bool bRequired) {
#ifdef DEBUG_CLSBASEPARSER
	    cout << "ClsSysFileParser::getAttributeValue(DOMNode *dn, string strAttributeName, bool bRequired)" << endl;
#endif

	    string strValue;
	    DOMNode *dnTemp = dn->getAttributes()->getNamedItem(XMLString::transcode(strAttributeName.c_str()));

	    if(dnTemp == NULL){
		if(bRequired){
		    strValue = "";
		    ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::ATTRIBUTE_NOT_FOUND, strAttributeName);
		    throw clsSysFileParserException;
		} else {
		    strValue = "";
		}
	    } else {
		strValue = XMLString::transcode(dnTemp->getNodeValue());
	    }
	    return strValue;
	}
    };
}

#endif /* CLSBASEPARSER_H */


/// Local Variables:
/// mode: c++
/// End:

