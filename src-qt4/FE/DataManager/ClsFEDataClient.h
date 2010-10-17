/****************************************************************************
 ** $Filename: ClsFEDataClient.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Sep 14 17:02:32 2001
 ** $Date: 2001/11/16 12:30:21 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEDATACLIENT_H
#define CLSFEDATACLIENT_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <map>
#include <qmutex.h>
#include <ClsFEDataSink.h> 
#include <ClsFEDataManager.h>

#include <ClsColorPicker.h>

using namespace std;

class ClsFEDataClient {

public:

     ClsFEDataClient(ClsFEDataManager *_clsFEDataManager, QMutex *qmutexSysGUI, string _strDataClientID);
     virtual ~ClsFEDataClient() { };
     
     enum DATA_CLIENT_TYPES{
	  CLIENT_SPACEPLOT = 0,
	  CLIENT_TIMEPLOT,
	  CLIENT_BARPLOT,
	  CLIENT_CORRPLOT,
	  CLIENT_DATASAMPLER,
	  CLIENT_DATABROADCASTER
     };
     
     void setClientType(int iType) { iDataClientType =  iType; };
     int getClientType () { return iDataClientType; };
     string getClientID() { return strDataClientID; };
   
     int getDataSinkColor(string strID);
//protected slots:

/* deprecated
    virtual void addDataSink(string strID, string strGroupID, string strParamname, string strRange );
*/

    void addDataSink(string strID, ClsItem* clsItem, string strParamname, string strRange);
    virtual void removeDataSink(string strID);
    void setSinkMath (string strSinkID, int iFlag);

//    virtual void update() {};
    virtual void init();
    virtual void DataSinkAdded(string strID, int iColor){iColor = 0; strID = "";};
    virtual void DataSinkRemoved(string strID){ strID = ""; };
    virtual void groupChanged(string strGroupID);
    virtual void connectionChanged(string _strConnectionID);

protected:
     ClsFEDataManager *clsFEDataManager; 
    QMutex *qmutexSysGUI;
     string strDataClientID;
//     map<string, ClsFEDataSink*> mapDataSinks;
//     map<string, ClsBaseDataSink*> mapDataSinks;
     map<string, ClsDataSinkCopying*> mapDataSinks;
     ClsColorPicker clsColorPicker;
 
private:
     int iDataClientType;
     

};
    
/*! \todo blabla
  \todo blublu
 */


#endif /* CLSFEDATACLIENT_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
