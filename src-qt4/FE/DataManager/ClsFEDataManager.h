/****************************************************************************
 ** $Filename: ClsFEDataManager.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sat Sep 15 20:59:34 2001
 ** $Date: 2003/12/22 21:06:02 $
 **
 ** $log:
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSFEDATAMGR_H
#define CLSFEDATAMGR_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <map>

#include <qmutex.h>
#include <qobject.h>
#include <qwidget.h>
#include <qtimer.h>


#include "item.hpp"

class ClsFEDataClient; 
class ClsFEBasePlot;
class ClsFEGroup;
class ClsDataSinkCopying;
class ClsFEDataSampler;
class ClsFEDataBroadcaster;

#define SINGLE_DATASAMPLER 

using namespace std;
using namespace iqrcommon;


//--class ClsFEDataManagerEvent : public QEvent {
//--
//--public:
//--//    const QEvent::Type ClsFEDataManagerEvent::PROPERTY_CHANGED = (const QEvent::Type)QEvent::registerEventType();
//--    ClsFEDataManagerEvent(Type _type) : QEvent(_type) {
//--	
//--    }
//--
//--    
//--
//--};



class ClsFEDataManager: public QObject {
     Q_OBJECT
public:

    static ClsFEDataManager* Instance();
    static void initializeDataManager(QWidget *_parent, const char * name, QMutex* _qmutexSysGUI);

    void closeAllPlots();

//     ClsFEGroup* getGroup(string strID);

    void customEvent(QEvent *);

    enum EVENT_TYPES {
	EVENT_UPDATE = 65440,
    };

public slots:
//     void slotHandleDataRequest(int , string, string, string, string);
//     void slotHandleDataRequest(int);
     void updateClients();
     void DataClientCreate(int iClientType , string strSystemElementID, string strParamName, string strRange);
//    void addDataSink( ClsFEDataSink *clsFEDataSink);//, string strGroupID, string strParamName, string strRange );
//--    ClsDataSinkCopying* createDataSink( string strGroupID, string strParamName, string strRange );
    ClsDataSinkCopying* createDataSink(ClsItem* clsItem, string strParamName, string strRange );
     void cancelDataRequest( ClsDataSinkCopying* clsDataSink, string strGroupID, string strParamName, string strRange ); // TODO
    void start(bool _bSyncPlots);
    void stop();
    void pause(bool b);

    void startDataSampler(bool bForce = false);
    void stopDataSampler(bool bForce = false);
    
    void slotItemChanged(int iType, string strID );
    void slotItemDeleted(int iType, string strID );
     void saveConfig();
     void applyConfig(string strFilename="");

 private slots:
 void slotPlotClosed(string strID);

private:
     ClsFEDataManager(QWidget * parent, const char * name, QMutex* _qmutexSysGUI);
    static ClsFEDataManager* _instanceDataManager;

    QTimer *qtimerPaceMaker;
    QWidget *parent;

    QMutex *qmutexSysGUI;
    int iDataClientCounter;
    bool bSyncPlots;

    map<string, ClsFEBasePlot*> mapPlots;

#ifdef SINGLE_DATASAMPLER 
    ClsFEDataSampler* clsFEDataSampler;
    string strDataSamplerID;
#else
    map<string, ClsFEDataSampler*> mapDataSamplers;
#endif

    ClsFEDataBroadcaster* clsFEDataBroadcaster;
    string strDataBroadcasterID;


    friend class ClsFEDataSampler;

    bool bPaused;
};

#endif /* CLSFEDATAMGR_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:

