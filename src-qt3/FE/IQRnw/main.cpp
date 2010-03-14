#include <string>
#include <stdlib.h>


#include "ClsNGiqr.h"
#include "cloIQRnw.h"

/* 20030402 new type management */
#include "neuronManager.hpp"
using iqrfe::initializeNeuronManager;
using iqrfe::NeuronManager;

#include "synapseManager.hpp"
using iqrfe::initializeSynapseManager;
using iqrfe::SynapseManager;


#include "moduleManager.hpp"
using iqrfe::initializeModuleManager;
using iqrfe::ModuleManager;

#include <Common/Item/neuron.hpp>

//#include "settings.hpp"
#include "ClsFESettings.h"

using iqrfe::initializeSettings;

using namespace iqrfe;

int main( int argc, char **argv )
{

    string strSystemFileName = "";

     try {
 	/*
 	 * create a command line parser class and then tell it to parse your
 	 * command line.
 	 */
 	clo::parser parser;
 	parser.parse(argc, argv);
 
 	/*
 	 * get the struct of options from the parser class so that you can
 	 * access the option values.
 	 */
 	const clo::options &options = parser.get_options();
 
 	strSystemFileName = options.systemfile;

 
     } catch (clo::autoexcept &e) {
 	switch (e.get_autothrow_id()) {
 	case clo::autothrow_help:
 	    std::cout << "Usage: " << argv[0] << " [options]\n";
 	    std::cout << e.what();
 	    return 0;
 	}
     } catch (std::exception &e) {
 	std::cerr << argv[0] << ": " << e.what() << "\n";
 	return 1;
     }
 
      if(strSystemFileName.size()<=0){
  	cerr << "filename required" << endl;
  	return -1;
      }
      
 
 	initializeSettings();
 	initializeNeuronManager(ClsFESettings::instance().getListPathsNeurons()); //20031019
 	initializeSynapseManager(ClsFESettings::instance().getListPathsSynapses());  //20031019
 	initializeModuleManager(ClsFESettings::instance().getListPathsModules());

 	ClsNQiqr clsNQiqr;

 	clsNQiqr.openSystem(strSystemFileName);

 	clsNQiqr.startSimulation();


}


/* main should check for abort signal or message */
