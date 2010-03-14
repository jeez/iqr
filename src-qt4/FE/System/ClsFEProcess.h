#ifndef CLSFEPROCESS_H
#define CLSFEPROCESS_H

#include "item.hpp"
#include "ClsSysFileNode.h"

#include "module.hpp"
#include "threadModule.hpp"

using namespace iqrcommon;
using namespace std;

class ClsFEProcess : public ClsItem {

public:
    ClsFEProcess ( );

    ClsFEProcess(string _strProcessID);
  
    ClsFEProcess(string _strProcessID, string _strProcessName, string _strHostname, int _iPort);
  

    enum PRC_STATUS {
  	PRC_DIED = 0,
  	PRC_LAUNCHED,
  	PRC_ALIVE,
  	PRC_SYSTEMFILEPARSED,
  	PRC_SYSTEMBUILDT,
  	PRC_CONNECTED2PRCS,
  	PRC_READY,
  	PRC_SEND,
  	PRC_SLEEP
    };

    /* 20040227 */
    void setMutex(QMutex* _qmutexThread) {
	if(bModuleEnabled && pModule!=NULL && bModuleThreaded){
	    dynamic_cast<ClsThreadModule*>(pModule)->setMutex(_qmutexThread);
	}
    }
    /* --------- */


    void setProcessID( string _strProcessID );
    string getProcessID();

    void setProcessUID( string _strProcessUID);
    string getProcessUID( );

    void setProcessName( string _strProcessName );
    string getProcessName();
  
    void setStatus ( int _iProcessStatus ); 
    int getStatus ( ); 

    string getNotes();
    void setNotes(string _strNotes);

    void setHostname( string _strHostname);
    string getHostname( );

    void setPath( string _strPath);
    string getPath( );
  

    void setPort( int _iPort);
    int getPort ( );
    string getPortAsString ( );

    void setColor( string _strColor);
    string getColor( );



    void setLastUpdateTime ( int _iLastUpdateTime );
    int getLastUpdateTime ( ); 	

    void setProcessSubNodes(ClsSysFileNode _clsSysFileNodeProcessParameter);

    list<string> getListOfMembers();
    ClsItem* getMemberByName(string strName);
    void setMemberTypeByName(string strName, string strValue);
    string getMemberTypeByName(string strName);
    list<string> getListOfMembersTypes(string strName);


    /* module stuff */
    string getEnableModulesAsString( );
    bool getEnableModules( );
    virtual int createModule(string strModuleType);
    virtual int createModuleByLabel(string strModuleLabel);
    void setModuleParameter(string strParamName, string strParamValue);
    virtual void setModuleParameter(ClsSysFileNode _clsSysFileNodeModuleParameter);

    ClsModule* getModule( ){return pModule; };
    virtual const ParameterList getListModuleParameters() const;    

    list<string> getListModuleXRefHolders();
    const ClsXRefHolder* getModuleXRefHolder(string strName) const;



    string getProcessModuleType() { return strModuleType; };
    string getProcessModuleLabel();


    list<string> getListGroupsOutputToModule();
    list<string> getListGroupsInputFromModule();
    /* ------------ */

    string validate();

    bool hasModule();

    moduleIcon getModuleIcon();

    void init();
    void startModule();
    void stopModule();
    void cleanup();
    void update();


private:

    void initModule();
    void cleanupModule();
    void updateModule();
    void defineParameters();

    ClsSysFileNode clsSysFileNodeProcessParameter;

    int iTimeDiff;
    string strProcessID;
    int iLastUpdateTime;
    string strProcessUID;
    int iProcessStatus;

    string strModuleType;
    ClsModule *pModule;

    bool bModuleEnabled;
    bool bModuleThreaded;
    ClsBoolParameter* clsBoolParameterEnableModule;

};



#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:


