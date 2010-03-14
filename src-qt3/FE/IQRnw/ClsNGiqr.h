#include <string>

#include <stdlib.h>
#include <string>

#include <qmutex.h>
#include <qwaitcondition.h>

#include "ClsFESystemManager.h"
#include "ClsFEComputeEngine.h"
#include <ClsFESystemBuilder.h>
#include "tagLibrary.hpp"

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

#include "ClsFESettings.h"

using iqrfe::initializeSettings;

using namespace iqrfe;
using namespace std;


class ClsNQiqr  {
//    Q_OBJECT

public:
    ClsNQiqr();
    ~ClsNQiqr(){};
    void openSystem(string s);
    int startSimulation();    
    void stopSimulation();
private:
    bool buildSystem();
    string strSystemFileName;


    QMutex qmutexSysGUI;
    QWaitCondition qWaitCondition;

};
