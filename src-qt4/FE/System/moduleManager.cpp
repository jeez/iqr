/****************************************************************************
 ** $Header$
 **
 ** $Author$ 
 ** 
 ** Created: Tue Sep  2 11:02:30 2003
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#include <iostream>
using std::cout;
using std::endl;
#include <stdexcept>
using std::runtime_error;
#include <string>
using std::string;

#include "moduleManager.hpp"

#ifdef DEBUG_MODULEMANAGER
static const bool bDebugModuleManager = true;
#else
static const bool bDebugModuleManager = false;
#endif


void
iqrfe::initializeModuleManager(list<string> lstPaths)
{
     if (bDebugModuleManager) {
	cout << "Initializing module manager..."
	     << endl;
    }

    ModuleManager& moduleManager = ModuleManager::instance();
    moduleManager.setName("Module manager");

    // Load module types from each directory specified in list...
    list<string>::const_iterator itPath;
    string strPath;
    for (itPath = lstPaths.begin(); itPath != lstPaths.end(); ++itPath) {
	strPath= *itPath;
	if (bDebugModuleManager) {
	    cout << "Loading modules from "
		 << strPath
		 << endl;
	}
	
	moduleManager.loadTypes(strPath);
	if (bDebugModuleManager) {
	    cout << "Done."
		 << endl;
	}    
    }    
}

