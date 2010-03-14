#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h>
using std::getenv;
#include <iostream>
using std::cout;
using std::endl;

#include <algorithm> //MARK

#include "tagLibrary.hpp"
#include "stringConversions.hpp"
#include "stringParameter.hpp"

#include <iqrUtils.h>

#include <Common/Helper/pathFunctions.hpp>
using iqrcommon::addDirectorySeparator;

#include "ClsFESettings.h"
#include "ClsSettingsReader.h"
#include "ClsSettingsWriter.h"

//#define DEBUG_CLSSETTINGS
#ifdef DEBUG_CLSSETTINGS
static const bool bDebugSettings = true;
#else
static const bool bDebugSettings = false;
#endif

/**
 * Initialize the settings object.
 *
 * This non-member function is currently only for clarity.  Writing
 * initializeSettings is cleaner than using the singleton syntax.
 */
void iqrfe::initializeSettings() {
  //    ClsFESettings::instance().loadSettings();

  int iError = 0;

  iError = ClsFESettings::instance().loadSettingsFormFile();

  if(iError){
    ClsFESettings::instance().setDefaultSettings();
  }
}

void iqrfe::saveSettings() {
  if (bDebugSettings) {
    cout << "iqrfe::saveSettings()" << endl;
  }
  ClsFESettings::instance().saveSettings();
}


/**
 * Access the settings object.
 *
 * The settings are stored in a singleton, and can only be accessed
 * through this function.  This ensures that there is only one copy of
 * the settings in a running version of IQR.
 *
 * @return Reference to the settings object.
 */
iqrfe::ClsFESettings& iqrfe::ClsFESettings::instance() {
  static ClsFESettings settings;
  return settings;
}

/**
 * Construct a settings object.
 *
 * The settings are initialized to default values.
 */
iqrfe::ClsFESettings::ClsFESettings() {

  if (bDebugSettings) {
    cout << "ClsFESettings::ClsFESettings"
	 << endl;
  }

  /* we need this for windows... -> what about mac??? */
#ifdef _WINDOWS  
  char *cwd = getcwd (NULL, 0);
  strAppDir = cwd;
  std::replace( strAppDir.begin(), strAppDir.end(), '\\', '/' );
#else
  /* we need this is if iqr is installed in a non-standard place...*/
  char * pcIQR_HOME = NULL;
  pcIQR_HOME = getenv ("IQR_HOME");
  if (pcIQR_HOME!=NULL){
      strAppDir = pcIQR_HOME;
      bLocalInstall = true;
  } else {
      strAppDir = "/usr/lib/iqr/";
      bLocalInstall = false;
  }
#endif  


#ifdef _WINDOWS
  string strHome = getenv("USERPROFILE");
  std::replace( strHome.begin(), strHome.end(), '\\', '/' );
#else
  string strHome = getenv("HOME");
#endif

  strSettingsFile = strHome + getDirectorySeparator() + ".iqrSettings";

  addIntParameter(ClsTagLibrary::AutoSaveInterval(),
		  "Auto Save",
		  5, -1, 60,
		  "Auto Save every x Minute (-1 = disable)",
		  "General");

  addStringParameter(ClsTagLibrary::BlockDiagramFontName(), "Font Name",
		     "",
		     true, false,
		     "",
		     "General");

  addIntParameter(ClsTagLibrary::BlockDiagramFontSize(),
		  "Font Size",
		  8, 1, 60,
		  "Font Size for Diagram Items",
		  "General");


  /* new: 
     - autosave
   
  */

  addBoolParameter(ClsTagLibrary::UseLocalNeurons(), "Use local Neurons", false,
		   "Description",
		   "NeuronPath");
  addBoolParameter(ClsTagLibrary::UseLocalSynapses(), "Use local Synapses", false,
		   "Description",
		   "SynapsePath");
  addBoolParameter(ClsTagLibrary::UseLocalModules(), "Use local Modules", false,
		   "Description",
		   "ModulePath");


  addBoolParameter(ClsTagLibrary::UseUserNeurons(), "Use user defined Neurons", false,
		   "Description",
		   "NeuronPath");
  addBoolParameter(ClsTagLibrary::UseUserSynapses(), "Use user defined Synapses", false,
		   "Description",
		   "SynapsePath");
  addBoolParameter(ClsTagLibrary::UseUserModules(), "Use user defined Modules", false,
		   "Description",
		   "ModulePath");

  addStringParameter(ClsTagLibrary::LocalModulePath(), "Path to local Modules",
		     "",
		     true, false,
		     "",
		     "ModulePath");

  addStringParameter(ClsTagLibrary::LocalNeuronPath(), "Path to local Neurons",
		     "",
		     true, false,
		     "",
		     "NeuronPath");

  addStringParameter(ClsTagLibrary::LocalSynapsePath(), "Path to local Synapses",
		     "",
		     true, false,
		     "",
		     "SynapsePath");

  addStringParameter(ClsTagLibrary::StandardModulePath(), "Path to standard Modules",
		     "",
		     true, false,
		     "",
		     "ModulePath");

  addStringParameter(ClsTagLibrary::StandardNeuronPath(), "Path to standard Neurons",
		     "",
		     true, false,
		     "",
		     "NeuronPath");

  addStringParameter(ClsTagLibrary::StandardSynapsePath(), "Path to standard Synapses",
		     "",
		     true, false,
		     "",
		     "SynapsePath");

  addStringParameter(ClsTagLibrary::UserModulePath(), "Path to user defined Modules",
		     "",
		     true, false,
		     "",
		     "ModulePath");

  addStringParameter(ClsTagLibrary::UserNeuronPath(), "Path to user defined Neurons",
		     "",
		     true, false,
		     "",
		     "NeuronPath");

  addStringParameter(ClsTagLibrary::UserSynapsePath(), "Path to user defined Synapses",
		     "",
		     true, false,
		     "",
		     "SynapsePath");


}

iqrfe::ClsFESettings::~ClsFESettings() {
  if (bDebugSettings) {
    cout << "ClsFESettings::~ClsFESettings" << endl;
  }
}




void iqrfe::ClsFESettings::setDefaultSettings() {
  if (bDebugSettings) {
    cout << "ClsFESettings::setDefaultSettings" << endl;
  }

  string strNeuronDirectory("Neurons");
  string strSynapseDirectory("Synapses");
  string strModuleDirectory("Modules");

  string strStandardPath(addDirectorySeparator("/opt/iqr/lib/")); //FIX
  string strStandardNeuronPath  = addDirectorySeparator(strStandardPath + strNeuronDirectory);
  string strStandardSynapsePath = addDirectorySeparator(strStandardPath + strSynapseDirectory);
  string strStandardModulePath  = addDirectorySeparator(strStandardPath + strModuleDirectory);

  setParameter(ClsTagLibrary::StandardModulePath(), strStandardModulePath);
  setParameter(ClsTagLibrary::StandardNeuronPath(), strStandardNeuronPath);
  setParameter(ClsTagLibrary::StandardSynapsePath(), strStandardSynapsePath);

  /* ------------------- */

  string strLocalPath(addDirectorySeparator(strAppDir)); //FIX
  string strLocalNeuronPath  = addDirectorySeparator(strLocalPath + strNeuronDirectory);
  string strLocalSynapsePath = addDirectorySeparator(strLocalPath + strSynapseDirectory);
  string strLocalModulePath  = addDirectorySeparator(strLocalPath + strModuleDirectory);

  setParameter(ClsTagLibrary::LocalModulePath(), strLocalModulePath);
  setParameter(ClsTagLibrary::LocalNeuronPath(), strLocalNeuronPath);
  setParameter(ClsTagLibrary::LocalSynapsePath(), strLocalSynapsePath);

  /* ------------------- */

#ifdef _WINDOWS
  string strHome = getenv("USERPROFILE");
  std::replace( strHome.begin(), strHome.end(), '\\', '/' );
  string strUserPath(addDirectorySeparator(strHome + string("/iqr/lib/")));  
#else
  string strUserPath(addDirectorySeparator(getenv("HOME") + string("/iqr/lib/"))); 
#endif



  string strUserNeuronPath  = addDirectorySeparator(strUserPath + strNeuronDirectory);
  string strUserSynapsePath = addDirectorySeparator(strUserPath + strSynapseDirectory);
  string strUserModulePath  = addDirectorySeparator(strUserPath + strModuleDirectory);

  setParameter(ClsTagLibrary::UserModulePath(), strUserModulePath);
  setParameter(ClsTagLibrary::UserSynapsePath(), strUserSynapsePath);
  setParameter(ClsTagLibrary::UserNeuronPath(), strUserNeuronPath);


  setParameter(ClsTagLibrary::UseLocalNeurons(), bool2string(true));
  setParameter(ClsTagLibrary::UseUserNeurons(), bool2string(true));
  setParameter(ClsTagLibrary::UseLocalSynapses(), bool2string(true));
  setParameter(ClsTagLibrary::UseUserSynapses(), bool2string(true));
  setParameter(ClsTagLibrary::UseLocalModules(), bool2string(true));
  setParameter(ClsTagLibrary::UseUserModules(), bool2string(true));
}



int iqrfe::ClsFESettings::loadSettingsFormFile() {
  if (bDebugSettings) {
    cout << "ClsFESettings::loadSettings" << endl;
  }

  /* do existence test for
     strSettingsFile
  */

  struct stat status;
  if(stat(strSettingsFile.c_str(), &status)){
    cerr << strSettingsFile << " does not exist" << endl;
    return -1;

  }


  ClsSettingsReader clsSettingsReader;

  try {
    clsSettingsReader.parseBuffer(strSettingsFile);
  }
  catch (ClsSettingsReaderException &e){
    cout << e.getMessage() << endl;
    cerr << "*** Error occured loading settings file \"" << strSettingsFile << "\"" << endl;
    cerr << "*** Using defaults instead" << endl;

    return -1;
  }


  string strAutoSaveInterval  = clsSettingsReader.getEntity(ClsTagLibrary::AutoSaveInterval());
  setParameter(ClsTagLibrary::AutoSaveInterval(), strAutoSaveInterval);

  string strBlockDiagramFontName  = clsSettingsReader.getEntity(ClsTagLibrary::BlockDiagramFontName());
  string strBlockDiagramFontSize  = clsSettingsReader.getEntity(ClsTagLibrary::BlockDiagramFontSize());

  setParameter(ClsTagLibrary::BlockDiagramFontName(), strBlockDiagramFontName);
  setParameter(ClsTagLibrary::BlockDiagramFontSize(), strBlockDiagramFontSize);


  string strStandardModulePath  = clsSettingsReader.getEntity(ClsTagLibrary::StandardModulePath());
  string strStandardNeuronPath  = clsSettingsReader.getEntity(ClsTagLibrary::StandardNeuronPath());
  string strStandardSynapsePath = clsSettingsReader.getEntity(ClsTagLibrary::StandardSynapsePath());

  setParameter(ClsTagLibrary::StandardModulePath(), strStandardModulePath);
  setParameter(ClsTagLibrary::StandardNeuronPath(), strStandardNeuronPath);
  setParameter(ClsTagLibrary::StandardSynapsePath(), strStandardSynapsePath);

  string strLocalModulePath  = strAppDir + "/Modules";
  string strLocalNeuronPath  = strAppDir + "/Neurons";
  string strLocalSynapsePath = strAppDir + "/Synapses";

    /* ================ */

    setParameter(ClsTagLibrary::LocalModulePath(), strLocalModulePath);
  setParameter(ClsTagLibrary::LocalNeuronPath(), strLocalNeuronPath);
  setParameter(ClsTagLibrary::LocalSynapsePath(), strLocalSynapsePath);


  string strUserModulePath  = clsSettingsReader.getEntity(ClsTagLibrary::UserModulePath());
  string strUserNeuronPath  = clsSettingsReader.getEntity(ClsTagLibrary::UserNeuronPath());
  string strUserSynapsePath = clsSettingsReader.getEntity(ClsTagLibrary::UserSynapsePath());

  setParameter(ClsTagLibrary::UserModulePath(), strUserModulePath);
  setParameter(ClsTagLibrary::UserSynapsePath(), strUserSynapsePath);
  setParameter(ClsTagLibrary::UserNeuronPath(), strUserNeuronPath);


  /* =================
     2008/12/04
     WE NEED A HACK HERE: IN IQR 1.1.3 THE PATHS TO THE RPM INSTALL PATH TO STANDARD TYPES HAS
     CHANGED: THEREFOR WE WILL ENFORCE THE NEW PATH
     ================== */


  string strValue = "false";
  strValue = clsSettingsReader.getEntity(ClsTagLibrary::UseLocalNeurons());
  strValue = "true"; /* 2008/12/04 see comment above */
  setParameter(ClsTagLibrary::UseLocalNeurons(), strValue);

  strValue = clsSettingsReader.getEntity(ClsTagLibrary::UseLocalModules());
  strValue = "true"; /* 2008/12/04 see comment above */
  setParameter(ClsTagLibrary::UseLocalModules(), strValue);
    
  strValue = clsSettingsReader.getEntity(ClsTagLibrary::UseLocalSynapses());
  strValue = "true"; /* 2008/12/04 see comment above */
  setParameter(ClsTagLibrary::UseLocalSynapses(), strValue);
  /* =================== */

  strValue = clsSettingsReader.getEntity(ClsTagLibrary::UseUserSynapses());
  setParameter(ClsTagLibrary::UseUserSynapses(), strValue);

  strValue = clsSettingsReader.getEntity(ClsTagLibrary::UseUserNeurons());
  setParameter(ClsTagLibrary::UseUserNeurons(), strValue);

  strValue = clsSettingsReader.getEntity(ClsTagLibrary::UseUserModules());
  setParameter(ClsTagLibrary::UseUserModules(), strValue);




  try {
    lstLFO  = clsSettingsReader.getListLastFiles();
  }
  catch (ClsSettingsReaderException &e){
    cout << e.getMessage() << endl;
  }

  return 0;

}



void iqrfe::ClsFESettings::saveSettings() {
  if (bDebugSettings) {
    cout << "ClsFESettings::saveSettings" << endl;
  }

  ParameterList parameterList = getListParameters();


  ClsSettingsWriter clsSettingsWriter;
  clsSettingsWriter.saveSettings(strSettingsFile, parameterList, lstLFO);
}

int iqrfe::ClsFESettings::getAutoSaveInterval(){
  return iqrUtils::string2int(getParameter(ClsTagLibrary::AutoSaveInterval())->getValueAsString());
}



string iqrfe::ClsFESettings::getBlockDiagramFontName(){
  return getParameter(ClsTagLibrary::BlockDiagramFontName())->getValueAsString();
}

int iqrfe::ClsFESettings::getBlockDiagramFontSize(){
  return iqrUtils::string2int(getParameter(ClsTagLibrary::BlockDiagramFontSize())->getValueAsString());
}


/**
 * @return Path to the standard neurons.
 */
string iqrfe::ClsFESettings::getStandardNeuronPath() {
  //    return strStandardNeuronPath;
  return getParameter(ClsTagLibrary::StandardNeuronPath())->getValueAsString();
}

/**
 * @return Path to the local neurons.
 */
string iqrfe::ClsFESettings::getLocalNeuronPath() {
  //    return strLocalNeuronPath;
  return getParameter(ClsTagLibrary::LocalNeuronPath())->getValueAsString();

}

/**
 * @return Path to the user's neurons.
 */
string iqrfe::ClsFESettings::getUserNeuronPath() {
  //    return strUserNeuronPath;
  return getParameter(ClsTagLibrary::UserNeuronPath())->getValueAsString();
}

/**
 * @return True if local neurons should be used, otherwise false.
 */
bool iqrfe::ClsFESettings::getUseLocalNeurons() {
  //    return bUseLocalNeurons;
  return string2bool(getParameter(ClsTagLibrary::UseLocalNeurons())->getValueAsString());
}

/**
 * @return True if user neurons should be used, otherwise false.
 */
bool iqrfe::ClsFESettings::getUseUserNeurons() {
  //    return bUseUserNeurons;
  return string2bool(getParameter(ClsTagLibrary::UseUserNeurons())->getValueAsString());
}

/**
 * @return Path to the standard synapses.
 */
string iqrfe::ClsFESettings::getStandardSynapsePath() {
  //    return strStandardSynapsePath;
  return getParameter(ClsTagLibrary::StandardSynapsePath())->getValueAsString();

}

/**
 * @return Path to the local synapses.
 */
string iqrfe::ClsFESettings::getLocalSynapsePath() {
  //    return strLocalSynapsePath;
  return getParameter(ClsTagLibrary::LocalSynapsePath())->getValueAsString();

}

/**
 * @return Path to the user's synapses.
 */
string iqrfe::ClsFESettings::getUserSynapsePath() {
  //    return strUserSynapsePath;
  return getParameter(ClsTagLibrary::UserSynapsePath())->getValueAsString();

}

/**
 * @return True if local synapses should be used, otherwise false.
 */
bool iqrfe::ClsFESettings::getUseLocalSynapses() {
  //    return bUseLocalSynapses;
  return string2bool(getParameter(ClsTagLibrary::UseLocalSynapses())->getValueAsString());
}

/**
 * @return True if user synapses should be used, otherwise false.
 */
bool iqrfe::ClsFESettings::getUseUserSynapses() {
  //    return bUseUserSynapses;
  return string2bool(getParameter(ClsTagLibrary::UseUserSynapses())->getValueAsString());

}

/**
 * @return Path to the standard modules.
 */
string iqrfe::ClsFESettings::getStandardModulePath() {
  //    return strStandardModulePath;
  return getParameter(ClsTagLibrary::StandardModulePath())->getValueAsString();

}

/**
 * @return Path to the local modules.
 */
string iqrfe::ClsFESettings::getLocalModulePath() {
  //    return strLocalModulePath;
  return getParameter(ClsTagLibrary::LocalModulePath())->getValueAsString();
}

/**
 * @return Path to the user's modules.
 */
string iqrfe::ClsFESettings::getUserModulePath() {
  //    return strUserModulePath;
  return getParameter(ClsTagLibrary::UserModulePath())->getValueAsString();

}

/**
 * @return True if local modules should be used, otherwise false.
 */
bool iqrfe::ClsFESettings::getUseLocalModules() {
  //    return bUseLocalModules;
  return string2bool(getParameter(ClsTagLibrary::UseLocalModules())->getValueAsString());
}

/**
 * @return True if user modules should be used, otherwise false.
 */
bool iqrfe::ClsFESettings::getUseUserModules() {
  //    return bUseUserModules;
  return string2bool(getParameter(ClsTagLibrary::UseUserModules())->getValueAsString());
}


void iqrfe::ClsFESettings::addFileOpened(string str){
  //    cout << "ClsFESettings::addFileOpened(string str)" << str << endl;

  if(lstLFO.size()>0){
    if(str.compare(lstLFO.front())){
      list<string>::iterator it;
      it = find(lstLFO.begin(), lstLFO.end(), str);
      if(it != lstLFO.end()){
	lstLFO.erase(it) ;
      }
      lstLFO.push_front(str) ;
    }
  } else {
    lstLFO.push_front(str) ;
  }

  if(lstLFO.size()>iNrLFO) {
    lstLFO.pop_back();
  }
};


list<string> iqrfe::ClsFESettings::getListPathsNeurons() {
  if (bDebugSettings) {
    cout << "ClsFESettings::getListPathsNeurons()" << endl;
  }
  list<string> lstPathsNeurons;
  lstPathsNeurons.push_back(getParameter(ClsTagLibrary::StandardNeuronPath())->getValueAsString());
  if(string2bool(getParameter(ClsTagLibrary::UseLocalNeurons())->getValueAsString())){
    lstPathsNeurons.push_back(getParameter(ClsTagLibrary::LocalNeuronPath())->getValueAsString());
  }
  if(string2bool(getParameter(ClsTagLibrary::UseUserNeurons())->getValueAsString())){
    lstPathsNeurons.push_back(getParameter(ClsTagLibrary::UserNeuronPath())->getValueAsString());
  }
  return lstPathsNeurons;
}


list<string> iqrfe::ClsFESettings::getListPathsSynapses() {
  if (bDebugSettings) {
    cout << "ClsFESettings::getListPathsSynapses()" << endl;
  }
  list<string> lstPathsSynapses;
  lstPathsSynapses.push_back(getParameter(ClsTagLibrary::StandardSynapsePath())->getValueAsString());
  if(string2bool(getParameter(ClsTagLibrary::UseLocalSynapses())->getValueAsString())){
    lstPathsSynapses.push_back(getParameter(ClsTagLibrary::LocalSynapsePath())->getValueAsString());
  }
  if(string2bool(getParameter(ClsTagLibrary::UseUserSynapses())->getValueAsString())){
    lstPathsSynapses.push_back(getParameter(ClsTagLibrary::UserSynapsePath())->getValueAsString());
  }
  return lstPathsSynapses;
}

list<string> iqrfe::ClsFESettings::getListPathsModules() {
  if (bDebugSettings) {
    cout << "ClsFESettings::getListPathsModules()" << endl;
  }
  list<string> lstPathsModules;
  lstPathsModules.push_back(getParameter(ClsTagLibrary::StandardModulePath())->getValueAsString());
  if(string2bool(getParameter(ClsTagLibrary::UseLocalModules())->getValueAsString())){
    lstPathsModules.push_back(getParameter(ClsTagLibrary::LocalModulePath())->getValueAsString());

  }
  if(string2bool(getParameter(ClsTagLibrary::UseUserModules())->getValueAsString())){
    lstPathsModules.push_back(getParameter(ClsTagLibrary::UserModulePath())->getValueAsString());
  }
  return lstPathsModules;
}




//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:


