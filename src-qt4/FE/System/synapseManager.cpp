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

#include "synapseManager.hpp"

#ifdef DEBUG_SYNAPSEMANAGER
static const bool bDebugSynapseManager = true;
#else
static const bool bDebugSynapseManager = false;
#endif


void
iqrfe::initializeSynapseManager(list<string> lstPaths)
{
    if (bDebugSynapseManager) {
	cout << "Initializing synapse manager..."
	     << endl;
    }

    SynapseManager& synapseManager = SynapseManager::instance();
    synapseManager.setName("Synapse manager");

    // Load synapse types from each directory specified in list...
    list<string>::const_iterator itPath;
    string strPath;
    for (itPath = lstPaths.begin(); itPath != lstPaths.end(); ++itPath) {
	strPath= *itPath;
	if (bDebugSynapseManager) {
	    cout << "Loading synapses from "
		 << strPath
		 << endl;
	}
	
	synapseManager.loadTypes(strPath);
	if (bDebugSynapseManager) {
	    cout << "Done."
		 << endl;
	}    
    }    
}

