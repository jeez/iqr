/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2002 by Mark Blanchard
 ** $Author$
 ** 
 ** Created: Mon Nov 25 17:38:29 2002
 ** Time-stamp: <Sat Oct 18 2003 15:01:00 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#ifndef LIBRARYLOADER_UNIX_HPP
#define LIBRARYLOADER_UNIX_HPP

#include <map>
// using std::map;
#include <string>
// using std::string;
using namespace::std;

#include "libraryLoaderExceptions.hpp"

namespace iqrcommon {
    /**
     * Dynamic library loader handles platform-specific tasks for
     * loading and closing of dynamic libraries.
     *
     * One loader can hold the handles to multiple dynamic libraries,
     * indexed by the filename used to load the library.  These
     * libraries are closed either explicitly by calling
     * closeLibrary() or during destruction of the loader object.
     *
     * This class requires that dynamic libraries must be closed
     * explicitly before they can be reloaded.
     */
    class ClsLibraryLoader
    {
    public:
	ClsLibraryLoader();
	~ClsLibraryLoader();
	
	void load(string _strFileName);
	void close(string _strFileName);
	bool hasLibrary(string _strFileName);

	// Use of void* as the handle to a loaded library is UNIX
	// specific.  This return type can be different for different
	// platforms, the compiler will ensure validity.
	void* getSymbol(string _strFileName, string _strSymbol);
	
    private:
	// Disable copy constructor and assignment operator.
	ClsLibraryLoader(const ClsLibraryLoader&);
	ClsLibraryLoader& operator=(const ClsLibraryLoader&);
	
	// Use of void* as the handle to a loaded library is UNIX
	// specific.  @@@ 28/11/2002 [jmb] In a future Windows
	// implementation, the handle map should hold HINSTANCE for
	// each library.
	typedef void* DlHandle;
	typedef map<string, DlHandle> DlHandleMap;
	DlHandleMap mapDlHandles;
	typedef DlHandleMap::iterator DlHandleMapIterator;
    };
}

#endif
