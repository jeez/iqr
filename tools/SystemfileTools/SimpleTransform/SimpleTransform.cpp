#include <Include/PlatformDefinitions.hpp>
#include <iostream>
#include <util/PlatformUtils.hpp>
#include <XalanTransformer/XalanTransformer.hpp>


int
main( int argc, char* []) {
    XALAN_USING_STD(cerr)
	XALAN_USING_STD(endl)

	int	theResult = -1;

    if (argc != 1) {
	cerr << "Usage: SimpleTransform"
	     << endl
	     << endl;
    } else {
	try {
	    XALAN_USING_XERCES(XMLPlatformUtils)
		
		XALAN_USING_XALAN(XalanTransformer)


		// Call the static initializer for Xerces.
		XMLPlatformUtils::Initialize();

	    // Initialize Xalan.
	    XalanTransformer::initialize();

	    {
		// Create a XalanTransformer.
		XalanTransformer theXalanTransformer;

		// The assumption is that the executable will be run
		// from same directory as the input files.
		theResult = theXalanTransformer.transform("foo.xml", "foo.xsl", "foo.out");

		if(theResult != 0)
		{
		    cerr << "SimpleTransform Error: \n" << theXalanTransformer.getLastError()
			 << endl
			 << endl;
		}
	    }

	    // Terminate Xalan...
	    XalanTransformer::terminate();

	    // Terminate Xerces...
	    XMLPlatformUtils::Terminate();

	    // Clean up the ICU, if it's integrated...
	    XalanTransformer::ICUCleanUp();
	}
	catch(...) {
	    cerr << "Initialization failed!" << endl;
	}
    }

    return theResult;
}
