//#include <sax/SAXParseException.hpp>
#include <framework/MemBufInputSource.hpp>
#include <parsers/XercesDOMParser.hpp>
#include <dom/DOM.hpp>
#include <sax/HandlerBase.hpp>
#include <util/XMLString.hpp>
#include <util/PlatformUtils.hpp>

#include <iostream>

#include <iqrUtils.h>
#include "ClsMyErrorHandler.h"
#include "ClsSysFileParserException.h"

using namespace std;

//extern ostream& operator<<(ostream& target, const DOMString& s);


void iqrcommon::ClsMyErrorHandler::warning(const SAXParseException&) {
    //
    // Ignore all warnings.
    //
}

void iqrcommon::ClsMyErrorHandler::error(const SAXParseException& toCatch) {
    string strMessage =  
	(string)"Error at file \"" + XMLString::transcode(toCatch.getSystemId()) + 
	(string)"\", line " + iqrUtils::int2string(toCatch.getLineNumber()) + 
	(string)", column " + iqrUtils::int2string(toCatch.getColumnNumber()) + 
	(string)"\n   Message: " + XMLString::transcode(toCatch.getMessage());

    cerr << strMessage << endl;
    ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR, strMessage);
    throw clsSysFileParserException;
}

void iqrcommon::ClsMyErrorHandler::fatalError(const SAXParseException& toCatch) {
    string strMessage =  
	(string)"Fatal Error at file \"" + XMLString::transcode(toCatch.getSystemId()) + 
	(string)"\", line " + iqrUtils::int2string(toCatch.getLineNumber()) + 
	(string)", column " + iqrUtils::int2string(toCatch.getColumnNumber()) + 
	(string)"\n   Message: " + XMLString::transcode(toCatch.getMessage());

    cerr << strMessage << endl;
    ClsSysFileParserException clsSysFileParserException(ClsSysFileParserException::PARSE_ERROR, strMessage);
    throw clsSysFileParserException;
}


void iqrcommon::ClsMyErrorHandler::resetErrors() {
}



