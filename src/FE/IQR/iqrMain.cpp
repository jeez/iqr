#include <string>
//--- #include "ClsQConnVis.h"
#include <qapplication.h>
#include <stdlib.h>
#include "iqrMainWindow.h"
#include "clo.h"

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
    QApplication app( argc, argv );

/*
  int scale = 2;
  ClsQConnVis *clsQConnVis = new ClsQConnVis( scale );
  app.setMainWidget( clsQConnVis );
  clsQConnVis->show();
*/

    string strSystemFileName = "";
    string strConfigFileName = "";
    string strHarborConfigFileName = "";
    bool bAutoRun = false;
    bool bShowVersion = false;

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
	strConfigFileName = options.configfile;
	strHarborConfigFileName = options.harborconfigfile;

	bAutoRun = options.autorun;
	bShowVersion = options.version;

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

    if(bShowVersion){
	cout << "iqr version compiled " << __DATE__ << endl;
	exit(0);
    } else {


/* 20030402 
   initializeSettings();
   initializeNeuronManager();
   initializeSynapseManager();
   initializeModuleManager(); 
*/

	initializeSettings();
	initializeNeuronManager(ClsFESettings::instance().getListPathsNeurons()); //20031019
	initializeSynapseManager(ClsFESettings::instance().getListPathsSynapses());  //20031019
	initializeModuleManager(ClsFESettings::instance().getListPathsModules());


	iqrMainWindow iqr;

//    m.resize(m.sizeHint());

	qApp->setMainWidget(&iqr);


//    m.setCaption("Qt Example - Canvas");
	if ( QApplication::desktop()->width() > iqr.width() + 10 && QApplication::desktop()->height() > iqr.height() +30 ){
	    iqr.show();
	} else {
	    iqr.showMaximized();
	}


	iqr.resize(800,600);
	iqr.show();
	qApp->processEvents();
	if(strSystemFileName.length()>0){
	    iqr.openSystem(strSystemFileName);
	} else {
	    iqr.newSystem();
	}

	qApp->processEvents();
	if(strConfigFileName.length()>0){
	    iqr.openConfig(strConfigFileName);
	} 


	if(strHarborConfigFileName.length()>0){
	    iqr.openHarborConfig(strHarborConfigFileName);
	} 

	qApp->processEvents();
	if(bAutoRun && strSystemFileName.length()>0){
	    iqr.autoStartSimulation();
	}

	qApp->setMainWidget(0);
   
	QObject::connect( qApp, SIGNAL(lastWindowClosed()), &iqr, SLOT(slotQuitIQR()) );


	return app.exec();
    }

}
