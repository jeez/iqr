/**
 * Exceptions thrown by library loader.
 *
 * This header file should be included into each platform specific
 * library loader.
 */

#ifndef LIBRARYLOADEREXCEPTIONS_HPP
#define LIBRARYLOADEREXCEPTIONS_HPP

#include <stdexcept>
#include <string>

using std::runtime_error;
using std::string;

namespace iqrcommon {

    class OpenLibraryError : public runtime_error
    {
    public:	
	/**
	 * Thrown when a library could not be opened.
	 *
	 * @param _strLibrary Name of the library.
	 */
	OpenLibraryError(const string& _strLibrary)
	    : runtime_error(_strLibrary)
	{}
    };

    class CloseLibraryError : public runtime_error
    {
    public:
	/** 
	 * Thrown when a library could not be closed.
	 *
	 * @param _strLibrary Name of the library.
	 */
	CloseLibraryError(const string& _strLibrary)
	    : runtime_error(_strLibrary)
	{}
    };

    class UnresolvedSymbolError : public runtime_error
    {
    public:
	/**	
	 * Thrown when a symbol could not be resolved for a
	 * dynamically loaded dynamicLibrary.
	 *
	 * @param _strSymbol Name of unresolved symbol.
	 */
	UnresolvedSymbolError(const string& _strSymbol)
	    : runtime_error(_strSymbol)
	{}
    };
}

#endif
