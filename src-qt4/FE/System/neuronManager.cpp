/****************************************************************************
 ** $Header$
 **
 ** 
 ** Created: Thu Jan  9 18:24:31 2003
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

#include "neuronManager.hpp"

//#define DEBUG_NEURONMANAGER

#ifdef DEBUG_NEURONMANAGER
static const bool bDebugNeuronManager = true;
#else
static const bool bDebugNeuronManager = false;
#endif


void
iqrfe::initializeNeuronManager(list<string> lstPaths)
{
    if (bDebugNeuronManager) {
	cout << "Initializing neuron manager..."
	     << endl;
    }

    NeuronManager& neuronManager = NeuronManager::instance();
    neuronManager.setName("Neuron manager");

    // Load neuron types from each directory specified in list...
    list<string>::const_iterator itPath;
    string strPath;
    for (itPath = lstPaths.begin(); itPath != lstPaths.end(); ++itPath) {
	strPath= *itPath;
	if (bDebugNeuronManager) {
	    cout << "Loading neurons from "
		 << strPath
		 << endl;
	}
	
	neuronManager.loadTypes(strPath);
	if (bDebugNeuronManager) {
	    cout << "Done."
		 << endl;
	}    
    }    
}

