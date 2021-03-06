#ifndef CLSFESETTINGS_H
#define CLSFESETTINGS_H


#include <string>
#include <list>
using std::string;

#include "item.hpp"
using namespace iqrcommon;

namespace iqrfe {
    
    void initializeSettings();
    void saveSettings();
    
    /** 
     * Settings class describes a singleton which stores the user's
     * settings for the IQR front end.
     *
     * NOTE: the functions implemented here are temporary versions which
     * aid implementation of the neuron, synapse and module managers.  No
     * file reading is implemented yet.
     */
    class ClsFESettings : public ClsItem {
    public:
	static ClsFESettings& instance();

	void setDefaultSettings();
	int loadSettingsFormFile();
	void saveSettings();
	
	list<string> getListPathsNeurons ();
	list<string> getListPathsSynapses ();
	list<string> getListPathsModules ();

	int getAutoSaveInterval();

	string getBlockDiagramFontName();
	int getBlockDiagramFontSize();


	string getStandardNeuronPath();
	string getLocalNeuronPath();
	string getUserNeuronPath();
	bool getUseLocalNeurons();
	bool getUseUserNeurons();

	string getStandardSynapsePath();
	string getLocalSynapsePath();
	string getUserSynapsePath();	
	bool getUseLocalSynapses();
	bool getUseUserSynapses();

	string getStandardModulePath();
	string getLocalModulePath();
	string getUserModulePath();
	bool getUseLocalModules();
	bool getUseUserModules();

	void addFileOpened(string str);
	list<string> getListLastFiles(){ return lstLFO; };
	string getAppDir() { return strAppDir; };
	bool localInstall() { return bLocalInstall; };
	

    private:
	ClsFESettings();
	~ClsFESettings();
	string strSettingsFile;
	list<string> lstLFO;
	static const unsigned int iNrLFO = 9;
	string strAppDir;
	bool bLocalInstall;
    };
}

#endif

//// Local Variables: 
//// mode: c++
//// End:  
