#ifndef LIBRARYLOADER_WINDOWS_HPP
#define LIBRARYLOADER_WINDOWS_HPP

#include <map>
#include <string>
#include <windows.h>

using std::map;
using std::string;


/* Use the same exception types. */
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
	
	// @@@ 29/11/2002 [jmb] Implementation of load will use 
	//
	// HINSTANCE LoadLibrary(LPCTSTR lpLibFileName)
	//
	// where lpLibFileName is the name of the DLL file.
	void load(string _strFileName);
	
	// @@@ 29/11/2002 [jmb] Implementation of close will use 
	//
	// BOOL FreeLibrary(HMODULE hLibModule)
	//
	// where hLibModule is the handle to loaded library module
	void close(string _strFileName);

	bool hasLibrary(string _strFileName);

	// @@@ 29/11/2002 [jmb] Implementation of getSymbol will use 
	//
	// FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
	//
	// where hModule is the DLL handle and lpProcName is the name
	// of the function.
	//
	// Compiler should ensure the casting of FARPROC to the
	// correct function pointer type, if not cast FARPROC to void*
	// before returning.
	FARPROC getSymbol(string _strFileName, string _strSymbol);
	
    private:
	// Disable copy constructor and assignment operator.
	ClsLibraryLoader(const ClsLibraryLoader&);
	ClsLibraryLoader& operator=(const ClsLibraryLoader&);
	
	// @@@ 29/11/2002 [jmb] Store HINSTANCE values in handle map.
	typedef HINSTANCE DlHandle;
	typedef map<string, DlHandle> DlHandleMap;
	DlHandleMap mapDlHandles;
	typedef DlHandleMap::iterator DlHandleMapIterator;
    };
}

#endif
