#ifndef CLSMYENTITYRESOLVER_H
#define CLSMYENTITYRESOLVER_H


#include <dom/DOMEntityResolver.hpp>

//#include "iqr421_System_050501040704030101_dtd.cpp"
#include "iqr421_System_050602040805040202_dtd.cpp"

using namespace xercesc; 

namespace iqrcommon {
    
    class MyResolver : public EntityResolver {
    public:

	InputSource* resolveEntity( const XMLCh* const publicId, 
				    const XMLCh* const systemId){

//	    cout << "MyResolver::resolveEntity(...)" << endl;
	    if (XMLString::compareString(publicId, XMLString::transcode("iqrSystem.dtd")) && 
                XMLString::compareString(systemId, XMLString::transcode("-//INI/iqr421"))) {

//		cout << "\tresolving publicId: \"iqrSystem.dtd\" and systemId \"-//INI/iqr421\" to MemBufInputSource" << endl;

		static const char*  cpMemBufId = "iqr421_System";
		return new MemBufInputSource ( (const XMLByte*)iqrSystemDTD_data, strlen(iqrSystemDTD_data), cpMemBufId, false);
/*
  with the code below, we can also use a file from disk... 
  return new LocalFileInputSource ( XMLString::transcode("/home/ulysses/iqrDTD/iqrSystem.dtd"));
*/

	    }
	    return NULL;
	}
    };

}

#endif

/// Local Variables: 
/// mode: c++
/// End: 
