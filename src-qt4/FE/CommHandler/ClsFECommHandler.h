/****************************************************************************
 ** $Filename: ClsFECommHandler.h
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Aug 31 11:46:26 2001
 ** $Date: 2002/12/08 17:33:27 $
 **
 ** $Keywords:
 ** $Description: functionality:
 **               - launch Prcs
 **               - communicated with Prc
 **               - handle messages
 **
 *****************************************************************************/


#ifndef CLSFECOMMHANDLER_H
#define CLSFECOMMHANDLER_H    /*+ To stop multiple inclusions. +*/



//#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <err.h>
#include <errno.h>
#include <fstream>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* std */
#include <list>
#include <map>
#include <queue>
#include <string>
#include <vector>

//#include <unistd.h>

#include <ace/OS.h>
#include <ace/Task.h>

#include "ClsBaseDataBroker.h"

//#include <ClsFEBasePlot.h>
//#include "ClsPlPlot.h"
#include <ClsFECommHandlerException.h>
#include "ClsFEDataSink.h"
#include <ClsFEMessageIn.h>
#include <ClsFEMessageOut.h>
#include <iqrUtils.h>

//ofstream ostreamNULL;
//#define COUT cout // ostreamNULL

//#include <ClsBaseSocketIn.h>
//#include <ClsBaseSocketOut.h>

//#include <ClsSocketInPipe.h>
//#include <ClsSocketOutPipe.h>

#include <ClsSocketInTcp.h>
#include <ClsSocketOutTcp.h>

#include <ClsMessageParser.h>
#include <ClsInfoPrc.h>

using namespace iqrcommon;

class ClsFESystemManager;

//class ClsSimControl; // forward declaration

//template <class ClsParent> class ClsFECommHandler : ACE_Task<ACE_MT_SYNCH> {
class ClsFECommHandler : public ACE_Task<ACE_MT_SYNCH>,  public ClsBaseDataBroker {

public:
//     ClsFECommHandler(ClsSimControl *_parent);
     ClsFECommHandler( );
     ~ClsFECommHandler();

/*
    void setSystemManagerPointer(ClsFESystemManager *_clsFESystemManager) {
	clsFESystemManager = _clsFESystemManager;
    };
*/

     void setSystemFile(string _strSystemFile);
     void setCondition( ACE_Condition<ACE_Thread_Mutex> *_ace_condReady ){ ace_condReady = _ace_condReady; };


     int open(void*);
     int close(u_long);
     void setTimeOutSec(int _iTimeOutSec);
     void closeSocketIn();
     void openSocketIn();
     void addDataRequest(string strPrcID, ClsFEMessageOut clsFEMessageOut);
// now in ClsFEPrcManager      void launchPrcs();
     int svc(void);

    inline static const char* prefixMessagePrcError()    { return "pe"; };
     inline static const char* prefixMessagePrcProgress() { return "pp"; };
     inline static const char* prefixMessagePrcStatus()   { return "ps"; };
     inline static const char* prefixMessageCommHandlerFatalError()   { return "cfe"; };

private:

     void startup();
     void sendCommand2Prc(string strPrcID, string strMessage);
     void sendSystemFile2Prc(string strPrcID);
     void createPrcMap();
     void connectToPrcs();
     void checkForPrcState(string strPrcMessage, int iState);
     void waitForPrcState(string strPrcID, string strPrcMessage, int iState);
     bool allPrcsInState(int iState);

     int sendMessage2Parent(string strPrcID, int iType, string strText);
     int reportStatus2Parent(string strPrcID, int iState);

//     ClsFESystemManager *clsFESystemManager;

     int iTimeOutSec;

    /* this is a global read time out: it's not specific to a single prc, but to the socketIn in general */
    int iTimeOutRead;

     int iRun;
    bool bInternalError;

    int iReportingStatus;
//     bool bManualLaunch;
     string strSystemFile;
     ACE_Condition<ACE_Thread_Mutex> *ace_condReady;

//     ClsSimControl *parent;

     ClsMessageParser clsMessageParser;
     ClsBaseSocketIn *clsSocketIn;

     multimap<string, ClsFEMessageOut > mapMessagesOut;
     map<string, ClsFEMessageIn> mapMessages;
//     map<string, pair<ClsInfoPrc*, ClsBaseSocketOut*> > mapPrcs;

     map<string, pair<ClsInfoPrc, ClsSocketOutTcp> > mapPrcs;

};


#endif /* CLSFECOMMHANDLER_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ && make -k"
//// End:
