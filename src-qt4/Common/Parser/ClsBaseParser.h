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

#include <QtXml>
#include <QString>

//#include <framework/MemBufInputSource.hpp>
//#include <parsers/XercesDOMParser.hpp>
//#include <dom/DOM.hpp>
//#include <sax/HandlerBase.hpp>
//#include <util/XMLString.hpp>
//#include <util/PlatformUtils.hpp>

#include "../Helper/iqrUtils.h"

#include "ClsSysFileParserException.h"

//using namespace xercesc;
using namespace std;

namespace iqrcommon {
    class ClsBaseParser {


    public:
	virtual ~ClsBaseParser(){};

    protected:
        inline string getAttributeValue(QDomNode *dn, string strAttributeName, bool bRequired) {
#ifdef DEBUG_CLSBASEPARSER
            cout << "ClsSysFileParser::getAttributeValue(QDomNode *dn, string strAttributeName, bool bRequired)" << endl;
#endif

	    string strValue;
            QDomNode dnTemp = dn->attributes().namedItem(QString::fromStdString(strAttributeName));
            //DOMNode *dnTemp = dn->getAttributes()->getNamedItem(XMLString::transcode(strAttributeName.c_str()));

            if(dnTemp.isNull()){
		if(bRequired){
		    strValue = "";
		    ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::ATTRIBUTE_NOT_FOUND, strAttributeName);
		    throw clsSysFileParserException;
		} else {
		    strValue = "";
		}
	    } else {
                strValue = dnTemp.nodeValue().toStdString();
	    }
	    return strValue;
	}
    };
}

#endif /* CLSBASEPARSER_H */


/// Local Variables:
/// mode: c++
/// End:

