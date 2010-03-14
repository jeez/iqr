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
#include <windows.h>
#include <errno.h>


#include "libraryLoader_windows.hpp"

//#define DEBUG_CLSLIBRARYLOADER

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
    FreeLibrary(it->second);
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
  DlHandle dlHandle = LoadLibrary(_strFileName.c_str());

  if (dlHandle == 0) {
    // The library couldn't be opened.  Pass back a
    // description of the error, if any, via the what() field
    // of an exception.
    DWORD err = GetLastError();
    LPTSTR Error = 0;
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		   NULL,
		   err,
		   0,
		   (LPTSTR)&Error,
		   0,
		   NULL);

		
    pcMessage = Error;


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
    
  if (FreeLibrary(it->second) == 0) { //Windows returns 0 if there is an error
    // An error occured during FreeLibrary operation.
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
	 

FARPROC
iqrcommon::ClsLibraryLoader::getSymbol(string _strFileName, 
				       string _strSymbol)
{

  if (bDebugLibraryLoader) {
    cout << "iqrcommon::ClsLibraryLoader::getSymbol(string _strFileName, string _strSymbol)" << endl;
  }
 
  DlHandleMapIterator it = mapDlHandles.find(_strFileName);
  if (it == mapDlHandles.end()) {
    // Return a null pointer if the library was not found.
    return 0;
  }
    
  //void* pSymbol = GetProcAddress(it->second, _strSymbol.c_str());
  FARPROC pSymbol = GetProcAddress(it->second, _strSymbol.c_str());

  if(pSymbol==NULL){
    DWORD err = GetLastError();
    LPTSTR Error = 0;
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		   NULL,
		   err,
		   0,
		   (LPTSTR)&Error,
		   0,
		   NULL);
    
    const char* pcMessage = Error;
    
    if (pcMessage) {
      // An error occured finding the symbol.
      
      if (bDebugLibraryLoader) {
	cerr << "ERROR: An error occured finding the symbol." << endl;
      }
      
      throw UnresolvedSymbolError(_strSymbol);
    }
  }
  
  return pSymbol;
}



