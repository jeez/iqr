/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author$
 ** 
 ** Created: Mon Nov 25 17:38:29 2002
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::make_pair;
using std::map;
using std::runtime_error;
using std::string;

// Old C-style headers for loading shared objects.
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>

#include "libraryLoader_unix.hpp"

namespace iqrcommon {
#ifdef DEBUG_CLSLIBRARYLOADER
    static const bool bDebugLibraryLoader = true;
#else
    static const bool bDebugLibraryLoader = false;
#endif
}

/** 
 * Constructor implemented only for debugging reasons.
 */
iqrcommon::ClsLibraryLoader::ClsLibraryLoader()
{
    if (bDebugLibraryLoader) {
	cout << "ClsLibraryLoader::ClsLibraryLoader"
	     << endl;
    }
}

/** 
 * Destructor closes all dynamic libraries.
 */
iqrcommon::ClsLibraryLoader::~ClsLibraryLoader()
{
    for (DlHandleMapIterator it = mapDlHandles.begin();
	 it != mapDlHandles.end(); ++it) {
	// Close all dynamic libraries.
	// @@@ 27/11/2002 [jmb] What can be done if one of the
	// libraries cannot be closed?  No exceptions should leave
	// the destructor.
	dlclose(it->second);
    }
    
    if (bDebugLibraryLoader) {
	cout << "ClsLibraryLoader::~ClsLibraryLoader"
	     << endl;
    }
}

/** 
 * Check whether specified library is already loaded.
 *
 * @return True if the library is already loaded, otherwise false.
 * @param _strFileName Name of the file containing the library.
 */
bool 
iqrcommon::ClsLibraryLoader::hasLibrary(string _strFileName)
{
    DlHandleMapIterator it = mapDlHandles.find(_strFileName);
    if (it == mapDlHandles.end()) {
	return false;
    }
    
    return true;
}

/** 
 * Load a dynamic library from the specified file.
 *
 * If the library is already open, this function does nothing.  If
 * the caller intends to reload the library, they must call close
 * first.
 *
 * @param _strFileName Absolute name of the file to load.
 */
void
iqrcommon::ClsLibraryLoader::load(string _strFileName)
{
    if (bDebugLibraryLoader) {
	cout << "ClsLibraryLoader::load: loading library from "
	     << _strFileName
	     << endl;
    }
    
    char* pcMessage = 0;
    
    // Check whether the file has already been loaded.
    DlHandleMapIterator it;
    it = mapDlHandles.find(_strFileName);
    
    if (it != mapDlHandles.end()) {
	// File has already been loaded, return quietly.  If the
	// user wanted to reload the library, they should have
	// called close first.
	return;
    }
    
    // dlopen loads shared object code.  RTLD_NOW forces
    // resolution of all symbols needed by the library during
    // loading, any unresolved symbols are reported by dlerror.
    // If the file cannot be opened, dlopen returns 0.
    DlHandle dlHandle = dlopen(_strFileName.c_str(), RTLD_NOW);
    if (dlHandle == 0) {
	// The library couldn't be opened.  Pass back a
	// description of the error, if any, via the what() field
	// of an exception.
	pcMessage = dlerror();
	throw OpenLibraryError(pcMessage ? 
			       pcMessage : "dlopen: unknown error");
    }
    
    if (bDebugLibraryLoader) {
	cout << "ClsLibraryLoader::load: library loaded"
	     << endl;
    }
    
    // Add handle for dynamic library to local map.  Use the
    // filename for indexing.
    mapDlHandles.insert(make_pair(_strFileName, dlHandle));
}

/** 
 * Close a library.
 *
 * If the library is not found, this function does nothing.
 *
 * @param _strFileName Name of the file from which the library was
 * opened.
 */
void
iqrcommon::ClsLibraryLoader::close(string _strFileName)
{
    if (bDebugLibraryLoader) {
	cout << "ClsLibraryLoader::close: closing library from "
	     << _strFileName
	     << endl;
    }
    
    DlHandleMapIterator it = mapDlHandles.find(_strFileName);
    if (it == mapDlHandles.end()) {
	// Library is not refered to in the map of handles.
	// Assume that the library is not open and return quietly.
	return;
    }
    
    if (dlclose(it->second) != 0) {
	// An error occured during dlclose operation.
	// @@@ 27/11/2002 [jmb] What should happen here?  Throw an
	// exception?  (could be useful to prevent reloading
	// attempts).
	throw CloseLibraryError(_strFileName);
    }
    
    // Remove entry for dynamic library from handle map.
    mapDlHandles.erase(_strFileName);
}

/** 
 * Get a named symbol from a library.
 *
 * The library is indexed by its filename.
 *
 * @return Pointer to the symbol, if found; 0 otherwise.  The caller
 * must check the return value before use.  void* is UNIX specific,
 * the comparable Windows implementation will return a different type
 * - this will be resolved by the compiler, assuming that the return
 * type can be cast to the correct pointer type in the caller.
 * @param _strFileName Name of the library.
 * @param _strSymbol Name of the symbol.
 */
void*
iqrcommon::ClsLibraryLoader::getSymbol(string _strFileName, 
				       string _strSymbol)
{
    DlHandleMapIterator it = mapDlHandles.find(_strFileName);
    if (it == mapDlHandles.end()) {
	// Return a null pointer if the library was not found.
	return 0;
    }
    
    void* pSymbol = dlsym(it->second, _strSymbol.c_str());
    const char* pcMessage = dlerror();
    if (pcMessage) {
	// An error occured finding the symbol.
	throw UnresolvedSymbolError(_strSymbol);
    }
    
    return pSymbol;
}
