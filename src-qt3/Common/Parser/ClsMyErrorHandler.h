#ifndef CLSMYERRORHANDLER_H
#define CLSMYERRORHANDLER_H

#include <iostream>

//#include <util/XercesDefs.hpp>
#include <xercesc/sax/ErrorHandler.hpp>

//#include <PlatformSupport/DOMStringHelper.hpp> //xalan header

using namespace xercesc;
using namespace std;


namespace iqrcommon {

    class ClsMyErrorHandler : public ErrorHandler {
    public:
	ClsMyErrorHandler() {
	}
	
	~ClsMyErrorHandler() {
	} 
	
	void warning(const SAXParseException& toCatch);
	void error(const SAXParseException& toCatch);
	void fatalError(const SAXParseException& toCatch);
	void resetErrors(); 
    };
};

#endif

/// Local Variables: 
/// mode: c++
/// End: 
