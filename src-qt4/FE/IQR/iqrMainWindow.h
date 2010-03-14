#ifndef IQRMAINWINDOW_H
#define IQRMAINWINDOW_H    /*+ To stop multiple inclusions. +*/

#include <qapplication.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qmovie.h>
#include <qpainter.h>
#include <qpixmap.h>


#include <qprogressbar.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qtimer.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qmutex.h>
#include <qwaitcondition.h>

#include <QAction>

#include <string>
#include <fstream>
#ifdef LINUX
#include <unistd.h>
#endif
#include <list>
#include <deque>

#include "ipcserver.h"

namespace iqrcommon  {
    class ClsSysFileParser;
}


class ClsFESerializer;
class ClsFESystemManager;
class ClsFESystemBuilder;
class ClsBaseDataBroker;
class ClsFECommHandlerLocal;
class ClsQSystemBrowser;



#define COUT cout << "[01;34m"
#define ENDL "[00m" << endl

static const Q_UINT16 ipcPort = 54923;

using namespace std;
using namespace iqrcommon;


class iqrMainWindow: public QMainWindow {
    Q_OBJECT
public:
    iqrMainWindow(QWidget * parent = 0, const char * name = 0, Qt::WindowFlags f = Qt::Window );

/*
  ~iqrMainWindow();
*/

    void newSystem();
    void openSystem(string s);
    void customEvent( QEvent * e );
    void autoStartSimulation();
    void openConfig(string strConfigFileName);
    void openHarborConfig(string strHarborConfigFileName);
    void copyItemsToClipboard(int iType, list<string> lstIDs);
    void copyItemsToClipboard(list<string> lstGroupIDs,  list<string> lstConnectionIDs);
    void exportProcess(string strID);


signals:
    void sigSimulationRunning(bool);


public slots:
bool slotQuitIQR();

private slots:
    void slotOpenSystem();
    void slotOpenOldFile(int);

    void slotRunControl();
    void slotPause();

    void slotSaveSystem();
    void slotSaveSystemAs();
    void slotAutoSaveSystem();
    void slotDebugSaveSystem();
    void slotExportSystemForAER();
    void slotImportProcess();
    void slotLinkProcess();
    void slotSaveToBackup();

    void slotSaveDiagram();
    void slotPrintDiagram();


    void slotNewSystem();
    bool slotCloseSystem();

    void slotSettings();
    bool buildSystem();
    void slotSystemBuildt();
    void slotShowLogWindow();
    void advanceProgress();
    void showLoadProgress(string s);
    void slotValidateSystem();

    void slotDiagViewActivated(int,string);


/* dialogs */
    void slotSystemProperties();
    void slotDAVMenu(QAction * _qact);
    void slotHelpMenu(QAction * _qact);
/* clipboard */


    void slotPaste();
    void slotClipboardChanged();

    void slotRemoteCommand(const QString&);

    void slotShortCuts(QAction * _qact);

private:
    void createAnimation();
    void initLFO(list<string> &);
    void updateLFO(string);
    void closeEvent( QCloseEvent *e);
    void changeState();

    int startSimulation();
    void stopSimulation();
    void pauseSimulation(bool b);

    void setupRCServer();
    void startPdfHelp(int iType);

    void importORLinkProcess(bool);
    bool isValidSystem(bool bConfirm);

    enum ABOUT_MENU{
	HELP_MANUAL,
	HELP_USERDEFINED_TYPES
    };


    string strSystemFileName;
    QWaitCondition qWaitCondition;
    QMutex qmutexSysGUI;
    QTimer* qtimerAutoSave;
    int iActiveItemType;
    int iClipboardType;
    string strActiveItemID;


/* core modules */
    ClsFESerializer *clsFESerializer;
    ClsBaseDataBroker *clsFECommHandler;
    ClsQSystemBrowser *clsQSystemBrowser;





/* GUI related stuff */
    QMenu * qmenuFile;
    QMenu * qmenuEdit;
    QMenu * qmenuView;
    QMenu * qmenuDiagram;
    QMenu * qmenuData;
    QMenu * qmenuHelp;


    QToolBar *qtoolbarToolbar;
    QStatusBar *qstatusBar;

    QLabel *qlblStatus;


    QMovie* qmovRunAnimation;


    QProgressBar* progress;
    QLabel* lblStatusBar;
    QSplitter *qsplitter;
    QAction *qactionRunControl;
    QAction *qactionPause;

/* ========== short cut actions  ==========  */
    QAction *qactCutItem;
    QAction *qactCopyItem;
    QAction *qactDeleteItem;
    QActionGroup* qactgrpShortCuts;

/*  ========== menubar FILE  ==========  */
    QAction *qactOpenSystem ;
    QAction *qactNewSystem;
    QAction *qactCloseSystem;
    QAction *qactSaveSystem;
    QAction *qactSaveSystemAs;
    QAction *qactSaveToBackup;
    QAction *qactExportSystemForAER;
    QAction *qactImportProcess;
    QAction *qactLinkProcess;
    QAction *qactSystemProperties;
    QAction *qactQuitIQR;



/* ========== menubar EDIT  ==========  */
    QAction *qactPaste;
    QAction *qactSettings;
    QAction *qactValidateSystem;

/*  ========== menubar VIEW  ==========  */
    QAction *qactViewLog;

/*  ========== menubar DIAGRAM  ==========  */
    QAction *qactSaveDiagram;
    QAction *qactPrintDiagram;


/* menubar  ========== DATA  ==========  */
    QAction *qactApplyConfig;
    QAction *qactSaveConfig;
    QAction *qactDataSampler;
    QAction *qactHarbor;

    QActionGroup *qactgrpDAV;


/* menubar  ========== HELP  ==========  */
    QAction *qactPDFManual;
    QAction *qactPDFUserDefTypes;
    QAction *qactAbout;
    QActionGroup *qactgrpHelp;




//    map<int, string> mapLFO;
    deque<string> dequeLFO;

    QAction *qaccelMainWindow;

    IpcServer *rcServer;

    bool bSimulationRunning;
    friend class ClsFEDialogManager;
};





#endif /* IQRMAINWINDOW_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
