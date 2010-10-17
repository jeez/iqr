#include "iqrMainWindow.h"

#include <ctime>
#include <sstream>


#include <QRegExp>
#include <QMessageBox>
#include <QClipboard>
#include <QDir>
#include <QFileInfo>     
#include <QStringList>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>

#include "ClsFECommHandlerLocal.h"
#include "ClsFEComputeEngine.h"
#include "ClsFEDiagramManager.h"
#include "ClsFEDialogManager.h"
#include "ClsFESerializer.h"

#include "ClsFESettings.h"
using iqrfe::saveSettings;

#include "ClsFESystemManager.h"
#include "ClsQCPSCustomEvent.h"
#include "ClsQLogWindow.h"
#include "ClsSysFileParser.h"
#include <ClsSysFileParserException.h>
#include <ClsFESystemBuilder.h>
#include <ClsFESystemBuilderException.h>
#include "ClsFEDataManager.h"
#include "ClsFEDataClient.h"
#include "ClsQSystemBrowser.h"
#include "ClsFEParamRelais.h"
#include "ClsQSysBrwsrFrame.h"

#include "ClsFEProcess.h"
#include "ClsFEGroup.h"
#include "ClsFEConnection.h"

#include "iqrUtils.h"
#include "tagLibrary.hpp"
#include "diagramTypes.h"
using namespace iqrfe;


namespace {
#include <fileOpen.xpm>
#include <fileSave.xpm>
#include <fileSaveAs.xpm>
#include <fileNew.xpm>
#include <fileClose.xpm>
#include <properties.xpm>
#include <quit.xpm>
#include <paste.xpm>
#include <print.xpm>
#include <SimControl_2_32x32.xpm>
#include <iqr_1_33x33.xpm>
#include <stop2_13x13.xpm>
#include <pause_13x13.xpm>
#include <run2_13x13.xpm>
#include <icon_timeplot_14x14.xpm>
#include <icon_groupplot_14x14.xpm>
#include <iqr_mini_13x12.xpm>
}


#define LFO_BASE_ID 1000
//#define DEBUG_IQRMAINWINDOW

iqrMainWindow::iqrMainWindow(QWidget * parent, const char * name, Qt::WindowFlags f) :
    QMainWindow( parent, name,f)  {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "INSTANTIATE iqrMainWindow" << endl;
#endif


    bSimulationRunning = false;
    iClipboardType = -99;
    iActiveItemType = -99;
    strActiveItemID = "";
    strSystemFileName = "";

    this->setIcon(QPixmap(SimControl_2));
    this->setCaption("iqr");

    qsplitter = new QSplitter(this);
    setCentralWidget(qsplitter);

/* core elements */
    ClsFEDialogManager::initializeDialogManager(this); 
    ClsFEDiagramManager::initializeDiagramManager(qsplitter); 
    ClsFEDataManager::initializeDataManager(this, "", &qmutexSysGUI); 
//    clsQSystemBrowser = new ClsQSystemBrowser(NULL/*qsplitter*/);
  clsQSystemBrowser = new ClsQSystemBrowser(qsplitter);


//--    QFrame *qfrmBroswer = new QFrame(qsplitter);
  ClsQSysBrwsrFrame* clsQSysBrwsrFrame = new ClsQSysBrwsrFrame(qsplitter, clsQSystemBrowser);


    QList<int> lst;
    lst << clsQSystemBrowser->getWidth();
    qsplitter->setSizes (lst );

    ClsFEComputeEngine::initializeComputeEngine(this, &qmutexSysGUI, &qWaitCondition); 
    ClsFESystemManager::initializeSystemManager(this);
    clsFESerializer = new ClsFESerializer();
    ClsQLogWindow::initializeLogger(this);
    clsFECommHandler = new ClsFECommHandlerLocal();

    ClsFEParamRelais::initializeParamRelais(this);

/* end core elements */


/* general signaling */
    connect( QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(slotClipboardChanged()));
    connect(ClsFEDiagramManager::Instance(), SIGNAL(sigDiagItemActivated(int,string)), this, SLOT(slotDiagViewActivated(int, string)));
    connect(ClsFEDiagramManager::Instance(), SIGNAL(sigDiagViewActivated(int,string)), this, SLOT(slotDiagViewActivated(int, string)));


/* to/from browser */
    connect(clsQSystemBrowser, SIGNAL(sigDataDisplay(int, string, string, string)), ClsFEDataManager::Instance(), SLOT(DataClientCreate(int, string, string, string )));
    connect(clsQSystemBrowser, SIGNAL(sysbrowserRightClick(int ,string, QPoint)), ClsFEDialogManager::Instance(), SLOT(createPopup(int , string, QPoint)));
    connect(clsQSystemBrowser, SIGNAL(sigShowBlockDiagram(string)), ClsFEDiagramManager::Instance(), SLOT(slotBlockDiagramShow(string)));
    connect(ClsFEDiagramManager::Instance(), SIGNAL(sigDiagItemActivated(int,string)), clsQSystemBrowser, SLOT(slotDiagItemActivated(int, string)));


/* ------------------------ */

/* signaling from the SystemManager */
    connect(ClsFESystemManager::Instance(), SIGNAL(systemBuildt()), this, SLOT(slotSystemBuildt()));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigAdvanceProgress()), this, SLOT(advanceProgress()));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigLoadProgress(string)), this, SLOT(showLoadProgress(string)));

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemAdded(int, string) ), ClsFEDiagramManager::Instance(), SLOT(slotItemAdded(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDuplicated(int, string) ),ClsFEDiagramManager::Instance(), SLOT(slotItemDuplicated(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(int, string) ), ClsFEDiagramManager::Instance(), SLOT(slotItemChanged(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ), ClsFEDiagramManager::Instance(), SLOT(slotItemDeleted(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemUnDeleted(int, string) ), ClsFEDiagramManager::Instance(), SLOT(slotItemUnDeleted(int, string) ));


    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemAdded(int, string) ), clsQSystemBrowser, SLOT(slotItemAdded(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDuplicated(int, string) ), clsQSystemBrowser, SLOT(slotItemAdded(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(int, string) ), clsQSystemBrowser, SLOT(slotItemChanged(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ), clsQSystemBrowser, SLOT(slotItemDeleted(int, string) ));

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(int, string) ), ClsFEDataManager::Instance(), SLOT(slotItemChanged(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ), ClsFEDataManager::Instance(), SLOT(slotItemDeleted(int, string) ));

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ), ClsFEDialogManager::Instance(), SLOT(slotItemDeleted(int, string) ));

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(int, string) ), ClsFEParamRelais::Instance(), SLOT(slotItemChanged(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ), ClsFEParamRelais::Instance(), SLOT(slotItemDeleted(int, string) ));


/* ------------------------ */

    connect(this, SIGNAL(sigSimulationRunning(bool)), ClsFESystemManager::Instance(), SLOT(slotSimulationRunning(bool)));
    connect(this, SIGNAL(sigSimulationRunning(bool)), ClsFEDialogManager::Instance(), SLOT(slotSimulationRunning(bool)));
    connect(this, SIGNAL(sigSimulationRunning(bool)), ClsFEParamRelais::Instance(), SLOT(slotSimulationRunning(bool)));
    connect(this, SIGNAL(sigSimulationRunning(bool)), ClsFEDiagramManager::Instance(), SLOT(slotSimulationRunning(bool)));


/* ------------------------ */

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemUnDeleted(int, string) ), clsQSystemBrowser, SLOT(slotItemUnDeleted(int, string) ));

/*
    serverRC = new IpcServer( ipcPort, this );
    connect( serverRC, SIGNAL(receivedText(const QString&)), this, SLOT(slotRemoteCommand(const QString&)) );
*/

#ifdef RC_TCP
    serverRC = NULL;
#else
    socketRC = NULL;
#endif



    setupRCServer(ipcPort);

    qactgrpShortCuts = new QActionGroup(this);

    qactCopyItem = new QAction(this);
    qactCopyItem->setShortcut(Qt::CTRL+Qt::Key_C);
    qactCopyItem->setActionGroup(qactgrpShortCuts );
    this->addAction(qactCopyItem);

    qactCutItem = new QAction(this);
    qactCutItem->setShortcut(Qt::CTRL+Qt::Key_X);
    qactCutItem->setActionGroup(qactgrpShortCuts );
    this->addAction(qactCutItem);

    qactDeleteItem = new QAction(this);
    qactDeleteItem->setShortcut(Qt::Key_Delete);
    qactDeleteItem->setActionGroup(qactgrpShortCuts );
    this->addAction(qactDeleteItem);

    connect( qactgrpShortCuts, SIGNAL( selected( QAction * ) ), this, SLOT( slotShortCuts( QAction * ) ) );



/*  ========== menubar FILE  ==========  */
    qmenuFile = menuBar()->addMenu( "&File");

    qactOpenSystem = new QAction(QPixmap( fileOpen_xpm ), "&Open", this);
    qactOpenSystem->setShortcut(Qt::CTRL+Qt::Key_O);
    connect(qactOpenSystem, SIGNAL(triggered()), this, SLOT(slotOpenSystem()));
    qmenuFile->addAction(qactOpenSystem);
 
    qactNewSystem = new QAction(QPixmap( fileNew_xpm ), "&New System", this);
    qactNewSystem->setShortcut(Qt::CTRL+Qt::Key_N);
    connect(qactNewSystem, SIGNAL(triggered()), this, SLOT(slotNewSystem()));
    qmenuFile->addAction(qactNewSystem);
    
    qmenuFile->addSeparator();
    
    qactCloseSystem = new QAction(QPixmap( fileClose_xpm ), "&Close System", this);
    qactCloseSystem->setShortcut(Qt::CTRL+Qt::Key_W);
    connect(qactCloseSystem, SIGNAL(triggered()), this, SLOT(slotCloseSystem()));
    qmenuFile->addAction(qactCloseSystem);
    
    qactSaveSystem = new QAction(QPixmap( fileSave_xpm ), "Save &System", this);
    qactSaveSystem->setShortcut(Qt::CTRL+Qt::Key_S);
    connect(qactSaveSystem, SIGNAL(triggered()), this, SLOT(slotSaveSystem()));
    qmenuFile->addAction(qactSaveSystem);
    
    qactSaveSystemAs = new QAction(QPixmap( fileSaveAs_xpm ), "Save System &As", this);
    qactSaveSystemAs->setShortcut(Qt::CTRL+Qt::Key_A);
    connect(qactSaveSystemAs, SIGNAL(triggered()), this, SLOT(slotSaveSystemAs()));
    qmenuFile->addAction(qactSaveSystemAs);

    qmenuFile->addSeparator();

    qactSaveToBackup = new QAction("Save to &Backup", this);
    qactSaveToBackup->setShortcut(Qt::CTRL+Qt::Key_B);
    connect(qactSaveToBackup, SIGNAL(triggered()), this, SLOT(slotSaveToBackup()));
    qmenuFile->addAction(qactSaveToBackup);

    qactExportSystemForAER = new QAction("&Export for AER", this);
    connect(qactExportSystemForAER, SIGNAL(triggered()), this, SLOT(slotExportSystemForAER()));
    qmenuFile->addAction(qactExportSystemForAER);

    qmenuFile->addSeparator();

    qactImportProcess = new QAction("Import Process", this);
    connect(qactImportProcess, SIGNAL(triggered()), this, SLOT(slotImportProcess()));
    qmenuFile->addAction(qactImportProcess);

    qactLinkProcess = new QAction("Link-in Process", this);
    connect(qactLinkProcess, SIGNAL(triggered()), this, SLOT(slotLinkProcess()));
    qmenuFile->addAction(qactLinkProcess);

    qmenuFile->addSeparator();

    qactSystemProperties = new QAction(QPixmap( properties_xpm ), "System Properties", this);
    connect(qactSystemProperties, SIGNAL(triggered()), this, SLOT(slotSystemProperties()));
    qmenuFile->addAction(qactSystemProperties);

    qmenuFile->addSeparator();

    qactQuitIQR = new QAction(QPixmap( quit_xpm ), "&Quit iqr", this);
    qactQuitIQR->setShortcut(Qt::CTRL+Qt::Key_Q);
    connect(qactQuitIQR, SIGNAL(triggered()), this, SLOT(slotQuitIQR()));
    qmenuFile->addAction(qactQuitIQR);

    qmenuFile->addSeparator();
    
    list<string> lstLFO = ClsFESettings::instance().getListLastFiles();
    initLFO(lstLFO);
    qactSaveSystem->setEnabled(false);
    qactSaveToBackup->setEnabled(false);
    qactExportSystemForAER->setEnabled(false);
 



/* ========== menubar EDIT  ==========  */
    qmenuEdit = menuBar()->addMenu( "&Edit");

    qactPaste = new QAction(QPixmap( paste_xpm ), "Paste", this);
    qactPaste->setShortcut(Qt::CTRL+Qt::Key_V);
    connect(qactPaste, SIGNAL(triggered()), this, SLOT(slotPaste()));
    qmenuEdit->addAction(qactPaste);
    
    qmenuEdit->addSeparator();
    
    qactSettings = new QAction("Settings", this);
    connect(qactSettings, SIGNAL(triggered()), this, SLOT(slotSettings()));
    qmenuEdit->addAction(qactSettings);
    
    qactValidateSystem = new QAction("Validate System", this);
    connect(qactValidateSystem, SIGNAL(triggered()), this, SLOT(slotValidateSystem()));
    qmenuEdit->addAction(qactValidateSystem);



/*  ========== menubar VIEW  ==========  */
    qmenuView = menuBar()->addMenu( "&View");

    qactViewLog = new QAction("&Log", this);
    connect(qactViewLog, SIGNAL(triggered()), this, SLOT(slotShowLogWindow()));
    qmenuView->addAction(qactViewLog);
 


/*  ========== menubar DIAGRAM  ==========  */
    qmenuDiagram = menuBar()->addMenu("&Diagram");

    qactSaveDiagram = new QAction("Save Diagram", this);
    connect(qactSaveDiagram, SIGNAL(triggered()), this, SLOT(slotSaveDiagram()));
    qmenuDiagram->addAction(qactSaveDiagram);

    qactPrintDiagram = new QAction(QPixmap( print_xpm ), "Print Diagram", this);
    connect(qactPrintDiagram, SIGNAL(triggered()), this, SLOT(slotPrintDiagram()));
    qmenuDiagram->addAction(qactPrintDiagram);
 


//QT3: /* menubar  ========== DATA  ==========  */
    qmenuData = menuBar()->addMenu( "D&ata");
    qactgrpDAV = new QActionGroup( this);

    qactDataSampler = new QAction("Data Sampler", this);
    qactDataSampler->setActionGroup(qactgrpDAV );
    qmenuData->addAction(qactDataSampler);

    qactDataBroadcaster = new QAction("Data Broadcaster", this);
    qactDataBroadcaster->setActionGroup(qactgrpDAV );
    qmenuData->addAction(qactDataBroadcaster);

    qmenuData->addSeparator();
    
    qactApplyConfig = new QAction("Open Configuration", this);
    connect(qactApplyConfig, SIGNAL(triggered()), ClsFEDataManager::Instance(), SLOT(applyConfig()));
    qmenuData->addAction(qactApplyConfig);
    
    qactSaveConfig = new QAction("&Save Configuration", this);
    connect(qactSaveConfig, SIGNAL(triggered()), ClsFEDataManager::Instance(), SLOT(saveConfig()));
    qmenuData->addAction(qactSaveConfig);
    
    qmenuData->addSeparator();
    
    qactHarbor = new QAction("Harbor", this);
    qactHarbor->setActionGroup(qactgrpDAV );
    qmenuData->addAction(qactHarbor);

    connect( qactgrpDAV, SIGNAL( selected( QAction * ) ), this, SLOT( slotDAVMenu( QAction * ) ) );
    menuBar()->addSeparator();


/* menubar  ========== HELP  ==========  */
    qmenuHelp = menuBar()->addMenu( "&Help");
    qactgrpHelp = new QActionGroup( this);

    qactPDFManual = new QAction("Manual(pdf)", this);
    qactPDFManual->setActionGroup(qactgrpHelp );
    qmenuHelp->addAction(qactPDFManual);
    
    qactPDFUserDefTypes = new QAction("Writing user-defined types(pdf)", this);
    qactPDFUserDefTypes->setActionGroup(qactgrpHelp );
    qmenuHelp->addAction(qactPDFUserDefTypes);
    
    qmenuHelp->addSeparator();
    
    qactAbout = new QAction(QPixmap( iqr_mini ), "About iqr", this);
    qactAbout->setActionGroup(qactgrpHelp );
    qmenuHelp->addAction(qactAbout);
    
    connect( qactgrpHelp, SIGNAL( selected( QAction * ) ), this, SLOT( slotHelpMenu( QAction * ) ) );



/* end menubar */


/* ----------------- */


/* start toolbar */
    qtoolbarToolbar = addToolBar("toolbar");

    qtoolbarToolbar->addAction(qactNewSystem);    
    qtoolbarToolbar->addAction(qactOpenSystem);
    qtoolbarToolbar->addAction(qactSaveSystem);

    qtoolbarToolbar->addSeparator();

    qactionRunControl = new QAction( "run_control", this) ;

    qtoolbarToolbar->addAction(qactionRunControl);
    qactionRunControl->setIcon(QIcon(QPixmap( run )));
    qactionRunControl->setCheckable(true);
    qactionRunControl->setEnabled(false);
    connect(qactionRunControl,SIGNAL(triggered()), this, SLOT(slotRunControl()));


    qactionPause = new QAction( "run_control", this) ;

    qtoolbarToolbar->addAction(qactionPause);
    qactionPause->setIcon(QIcon(QPixmap( pause_icon )));
    qactionPause->setCheckable(true);
    qactionPause->setEnabled(false);
    connect(qactionPause,SIGNAL(triggered()), this, SLOT(slotPause())); //



/* end toolbar */

    lblStatusBar = new QLabel( statusBar() );
    statusBar()->addWidget( lblStatusBar, 2, TRUE );
    lblStatusBar->setText("Initializing System");

    progress = new QProgressBar( statusBar() );
    statusBar()->addWidget( progress, 1, TRUE );

    createAnimation();

    qtimerAutoSave = new QTimer( this );
    connect( qtimerAutoSave, SIGNAL(timeout()), this, SLOT(slotAutoSaveSystem()) );

}

void iqrMainWindow::updateLFO(string str){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::updateLFO(string str)" << endl;
#endif

    ClsFESettings::instance().addFileOpened(str);

    if(dequeLFO.size()>0){
	if(str.compare(dequeLFO.front())){ /* element not at the beginning */
	    for(unsigned int ii=0; ii<dequeLFO.size(); ii++){
		qmenuFile->removeItem(ii+LFO_BASE_ID);
	    }
	    deque<string>::iterator itd = std::find(dequeLFO.begin(), dequeLFO.end(), str);
	    if(itd != dequeLFO.end()){
		dequeLFO.erase(itd); 
	    }
	    dequeLFO.push_front(str);
	    for(unsigned int ii=0; ii<dequeLFO.size(); ii++){
		string strFilename = dequeLFO[ii];
		string strNumbered = "&" + iqrUtils::int2string(ii+1) + ": ";
		strNumbered = strNumbered + strFilename;
		int iID = ii + LFO_BASE_ID;
		qmenuFile->insertItem( strNumbered.c_str(), this, SLOT( slotOpenOldFile(int)), 0, iID);
		qmenuFile->setItemParameter( iID, ii );
	    }
	}
    }
}

void iqrMainWindow::initLFO(list<string> &lstLFO){
    int iCount = 0;
    while(lstLFO.size()) {
	string strFilename = lstLFO.front();
	string strNumbered = "&" + iqrUtils::int2string(iCount+1) + ": ";
	strNumbered = strNumbered + strFilename;
	int iID = iCount + LFO_BASE_ID;
	qmenuFile->insertItem( strNumbered.c_str(), this, SLOT( slotOpenOldFile(int)), 0, iID);
	qmenuFile->setItemParameter( iID, iCount );
	dequeLFO.push_back(strFilename);
	lstLFO.pop_front();
	iCount++;
    }
}


void iqrMainWindow::createAnimation() {
    /* all this frame and layout stuff is required because qt does not support to put buttons 
       on the right side of a toolbar ...*/


    QString filename = string(ClsFESettings::instance().getAppDir() + "/iqr-logo.gif").c_str();
    qmovRunAnimation = new QMovie( filename );
    qlblStatus = new QLabel(this);
    qlblStatus->setMovie(new QMovie(filename));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins ( 0, 0, 0, 0 );


    layout->addStretch(10);
    layout->addWidget(qlblStatus);


    QFrame *qframeSpace = new QFrame(this);
    qtoolbarToolbar->addWidget(qframeSpace);
    qframeSpace->setLayout(layout);
    

    if(qlblStatus->movie()!=NULL){
	qlblStatus->movie()->pause();
	qlblStatus->movie()->step();
    }
}


void iqrMainWindow::slotShowLogWindow() {
    if(ClsQLogWindow::Instance()->isHidden()){
	ClsQLogWindow::Instance()->show();
    }
};



void iqrMainWindow::newSystem() {
//    clsFESystemManager->newSystem();
    ClsFESystemManager::Instance()->newSystem();        
};



void iqrMainWindow::slotNewSystem(){
    if( slotCloseSystem()){
	newSystem();
	qactCloseSystem->setEnabled( true);
	qactSaveSystem->setEnabled(false);
	qactSaveSystemAs->setEnabled(true);
	qactSaveToBackup->setEnabled( false);
	qactExportSystemForAER->setEnabled( false);
	qactSystemProperties->setEnabled( true);
	qactImportProcess->setEnabled( true);
	qactLinkProcess->setEnabled( true);
	
	progress->reset();
	lblStatusBar->setText("");
	this->setCaption(QString("iqr: <New System>"));
    }
}


void iqrMainWindow::slotSystemProperties() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotSystemProperties()" << endl;
#endif
    if(ClsFEDialogManager::Instance()!=NULL){
	QPoint qp;

	ClsFEDialogManager::Instance()->createPropertyDialog(ClsFESystemManager::ITEM_SYSTEM, ClsFESystemManager::Instance()->getSystemID(), qp);
    }
}


void iqrMainWindow::slotDAVMenu(QAction * _qact){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotDAVMenu(QAction * _qact)" << endl;
#endif

/*
  if(_qact == case MENU_TIME_PLOT){
  ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_TIMEPLOT , "", "", "");
  }
*/
    
    if(_qact == qactDataSampler){
	ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_DATASAMPLER , "", "", "");
    }
    else if(_qact == qactHarbor){
	ClsFEParamRelais::Instance()->showHarbor();
    }
    else if(_qact == qactDataBroadcaster){
	ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_DATABROADCASTER , "", "", "");
    }

};

void iqrMainWindow::slotHelpMenu(QAction * _qact){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotHelpMenu(QAction * _qact)" << endl;
#endif
    if(_qact == qactPDFManual){
	startPdfHelp(HELP_MANUAL);
    }
    else if(_qact == qactPDFUserDefTypes){
	startPdfHelp(HELP_USERDEFINED_TYPES);
    }
    else if(_qact == qactAbout){
	ClsFEDialogManager::Instance()->createAboutDialog();
    }
}




void iqrMainWindow::openSystem(string s) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::openSystem(string s)" << endl;
#endif

    if(QDir::isRelativePath(s.c_str())){
//	cout << "RELATIVE PATH" << endl;
	string str(1,QDir::separator().toLatin1());
	strSystemFileName = string(QDir::currentDirPath().latin1()) + str + s;
    } else {
//	cout << "ABSOLUTE PATH" << endl;
	strSystemFileName = s;
    }

    /* we have to add a check here, because this function can be called from the command line... */
    QFileInfo fileinfo( strSystemFileName.c_str() );
    if( !fileinfo.exists() ) {
	string strErrorMessage;
	strErrorMessage = "Cannot find system file\n\"" + strSystemFileName + "\"";
	QMessageBox::warning( NULL, "iqr error", strErrorMessage.c_str());
    } else {
//    cout << "**********strSystemFileName: " << strSystemFileName << endl;
	if(!buildSystem()){
	    qactCloseSystem->setEnabled( true);
	    qactSaveSystem->setEnabled(true);
	    qactSaveSystemAs->setEnabled(true);
	    qactSaveToBackup->setEnabled( true);
	    qactExportSystemForAER->setEnabled( true);
	    qactSystemProperties->setEnabled( true);
	    qactImportProcess->setEnabled( true);
	    qactLinkProcess->setEnabled( true);
	    
	    progress->reset();
	    lblStatusBar->setText("");
	    this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
	    setupRCServer(ClsFESystemManager::Instance()->getSystemPort());
	}
    }
}


void iqrMainWindow::slotOpenSystem(){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotOpenSystem()" << endl;
#endif

    QString qstrFileName = QFileDialog::getOpenFileName(
	this,
	"Open File",
	"",
	"Systemfile(*.iqr)"
	);

    if(qstrFileName.length()>0){
	if( slotCloseSystem()){
	    strSystemFileName = qstrFileName.latin1();
	    if(!buildSystem()){
		qactCloseSystem->setEnabled( true);
		qactSaveSystem->setEnabled(true);
		qactSaveSystemAs->setEnabled(true);
		qactSaveToBackup->setEnabled( true);
		qactExportSystemForAER->setEnabled( true);
		qactSystemProperties->setEnabled( true);
		qactImportProcess->setEnabled( true);
		qactLinkProcess->setEnabled( true);

		updateLFO(strSystemFileName);

//		int iID = qmenuFile->insertItem( strSystemFileName.c_str(), this, SLOT( slotOpenOldFile(int)));
//		qmenuFile->setItemParameter( iID, iID );
		lblStatusBar->setText("");
		this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
		setupRCServer(ClsFESystemManager::Instance()->getSystemPort());
	    }
	}
    }
}



void iqrMainWindow::slotOpenOldFile(int ii) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotOpenOldFile()" << endl;
    cout << "ii: " << ii  << endl;
#endif

 

    

    if(slotCloseSystem()){
	QString qstrFileName = dequeLFO[ii].c_str();
	if(qstrFileName.length()>0){
	    strSystemFileName = qstrFileName.latin1();
	    if(!buildSystem()){
		qactCloseSystem->setEnabled( true);
		qactSaveSystem->setEnabled(true);
		qactSaveSystemAs->setEnabled(true);
		qactSaveToBackup->setEnabled( true);
		qactExportSystemForAER->setEnabled( true);
		qactSystemProperties->setEnabled( true);
		qactImportProcess->setEnabled( true);
		qactLinkProcess->setEnabled( true);

		lblStatusBar->setText("");
		this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
		setupRCServer(ClsFESystemManager::Instance()->getSystemPort());
		updateLFO(strSystemFileName);
	    }
	}
    }
}



bool iqrMainWindow::slotCloseSystem() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotCloseSystem()" << endl;
#endif

    int iReturn = 1;

//20080214
    qtimerAutoSave->stop();


    if(qactionRunControl->isChecked()){
	cout << "stopping running simulation" << endl;
	stopSimulation();
	qactionRunControl->setChecked(false);
    }


    if(ClsFESystemManager::Instance()->systemChanged()){
	iReturn = QMessageBox::warning( this, "IQR",
					"The system contains unsaved changes\n"
					"Do you want to save the changes before exiting?",
					"&Save", "&Discard", "Cancel", 0, 2 );
    }

//0 save
//1 exit
//2 don't close

//    cout << "iReturn: " << iReturn << endl;

    if(iReturn == 2){ /* cancel */
	return false;
    }

    if(iReturn == 0){
	slotSaveSystem();
	qactionRunControl->setEnabled(false);
    }

    if(iReturn == 0 || iReturn == 1 ){
	ClsFEDataManager::Instance()->closeAllPlots();
	ClsFEDialogManager::Instance()->closeAllDialogs();
	ClsFEParamRelais::Instance()->cleanHarbor();
	clsQSystemBrowser->closeSystem();
	ClsFEDiagramManager::Instance()->closeSystem();
	ClsFESystemManager::Instance()->closeSystem();

	qactionRunControl->setEnabled(false);

	qactCloseSystem->setEnabled( false);
	qactSaveSystem->setEnabled(false);
	qactSaveSystemAs->setEnabled(false);
	qactSaveToBackup->setEnabled( false);
	qactExportSystemForAER->setEnabled( false);
	qactSystemProperties->setEnabled( false);
	qactImportProcess->setEnabled( false);
	qactLinkProcess->setEnabled( false);

	strSystemFileName = "";
    }
    return true;
}


bool iqrMainWindow::buildSystem() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::buildSystem()" << endl;
#endif
    setCursor(Qt::WaitCursor);
    ClsFESystemManager::Instance()->clsFESystemBuilder->readSystemFileFromDisk(strSystemFileName);
    try {
	ClsFESystemManager::Instance()->clsFESystemBuilder->parseSystemFile( );
    }
    catch(ClsSysFileParserException &e){
	cerr << e.getMessage() << endl;
	string strError =(string)" in system file parser\n" + e.getMessage() ;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
//	reportError(strError);
	setCursor(Qt::ArrowCursor);
	return true;
    }

    try{
	ClsFESystemManager::Instance()->buildSystem( );
    }
    catch(ClsSysFileParserException &e){
	cerr << e.getMessage() << endl;
	string strError =(string)" reading system file\n" + e.getMessage() ;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
//	reportError(strError);
	setCursor(Qt::ArrowCursor);
	return true;
    }
    catch(ClsFESystemBuilderException &e){
	cerr << e.getMessage() << endl;
	string strError =(string)" builing system\n" + e.getMessage() ;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	setCursor(Qt::ArrowCursor);
	return true;
    }


#ifdef DEBUG_IQRMAINWINDOW
    cout << "done building system" << endl;
#endif
    setCursor(Qt::ArrowCursor);
    return false;

};


void iqrMainWindow::slotSystemBuildt(){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotSystemBuildt()" << endl;
#endif
    ClsFEDiagramManager::Instance()->DiagramCreate(diagramTypes::DIAGRAM_BLOCK);
    ClsFEDiagramManager::Instance()->slotSystemChanged();
    clsQSystemBrowser->slotSystemChanged();
    qactionRunControl->setEnabled(true);

//20080214
    if(ClsFESettings::instance().getAutoSaveInterval()>0){
	int iAutoSaveInterval = ClsFESettings::instance().getAutoSaveInterval();
	qtimerAutoSave->start(iAutoSaveInterval*60000, FALSE);
    }
};

void iqrMainWindow::slotSaveSystem() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotSaveSystem()" << endl;
#endif

    bool bValidFile = true;
    if(strSystemFileName.length()>0){
	QFileInfo qFileInfo(strSystemFileName.c_str() );
	if( !qFileInfo.isWritable() ) {
	    string str = "No permission to write file \"" + strSystemFileName + "\"\n";
	    int iReturn = QMessageBox::critical( this, "iqr error",
						 str.c_str(),
						 "Select other file", "Cancel", 0, 0, 1 );
	    if(iReturn == 1){
		return;
	    } else {
		bValidFile = false;
	    }
	} else {
	    bValidFile = true;
	} 
    } else {
	bValidFile = false;
    }

    if(bValidFile){
	if(clsFESerializer!=NULL){
	    string strValidationResult = ClsFESystemManager::Instance()->validateConnections();
	    if(strValidationResult.size()>0){
		strValidationResult.append("\nThis might make the file unloadable");
		ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_WARNING, strValidationResult);
	    }
	    clsFESerializer->SaveSystem(strSystemFileName, false, false);
	    ClsFESettings::instance().addFileOpened(strSystemFileName);
	    ClsFESystemManager::Instance()->systemSaved();
	    this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
//	    updateLFO(strSystemFileName);
	}
    }
    else {
	slotSaveSystemAs();
    }
}


void iqrMainWindow::slotSaveSystemAs() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotSaveSystemAs()" << endl;
#endif

    string strValidationResult = ClsFESystemManager::Instance()->validateConnections();
    if(strValidationResult.size()>0){
	strValidationResult.append("\nThis might make the file unloadable");
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_WARNING, strValidationResult);
    }


    string strFilename = "";
    if(ClsFEDialogManager::Instance()!=NULL){
	string strFileTypeDescription = "Systemfile(*.iqr)";
	string strExtension = ".iqr";
	strFilename = ClsFEDialogManager::Instance()->createSaveDialog(strFileTypeDescription, strExtension);
    }

    if(strFilename.length()>0){
	if(clsFESerializer!=NULL){
//	string strFilename = qstrFileName.latin1();
	    strSystemFileName = strFilename;
	    qactSaveSystem->setEnabled(true);
//	    ClsFESettings::instance().addFileOpened(strFilename);
	    clsFESerializer->SaveSystem(strFilename, false, false);
	    ClsFESystemManager::Instance()->systemSaved();
	    this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
	    updateLFO(strSystemFileName);
	}
    }
}

void iqrMainWindow::slotDebugSaveSystem() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotDebugSaveSystem()" << endl;
#endif
    if(clsFESerializer!=NULL){
	clsFESerializer->SaveSystem("/home/ulysses/Code/iqr421_Redesign/CodeExec/WriteTest.xml", false, false);
    }
}


void iqrMainWindow::slotAutoSaveSystem() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotAutoSaveSystem()" << endl;
#endif

    if(strSystemFileName.length()>0){
	string strAutoSaveFilename = strSystemFileName;
	strAutoSaveFilename.append(",autosave");
	if(clsFESerializer!=NULL){
	    clsFESerializer->SaveSystem(strAutoSaveFilename, false, false);
	}
    }
}

void iqrMainWindow::slotSaveToBackup() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotSaveToBackup()" << endl;
#endif

    if(strSystemFileName.length()>0){
	if(clsFESerializer!=NULL){
	    string strFilename = strSystemFileName + string(",") + iqrUtils::getDateTimeSuffix();
	    clsFESerializer->SaveSystem(strFilename, false, false);
	    ClsFESystemManager::Instance()->systemSaved();
	}
    }
}

void iqrMainWindow::slotExportSystemForAER(){
    string strFileTypeDescription = "AER iqr systemfile(*.aeriqr)";
    string strExtension = ".aeriqr";
    string strFilename = ClsFEDialogManager::Instance()->createSaveDialog(strFileTypeDescription, strExtension);

    if(strFilename.length()>9){
	if(clsFESerializer!=NULL){
	    clsFESerializer->serializeToAER(strFilename);
	}
    }
}


void iqrMainWindow::slotSaveDiagram(){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotSaveDiagram()" << endl;
#endif
    ClsFEDiagramManager::Instance()->saveBlockDiagram();
};


void iqrMainWindow::slotPrintDiagram(){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotPrintDiagram()" << endl;
#endif
    ClsFEDiagramManager::Instance()->printBlockDiagram();
};



void iqrMainWindow::slotSettings() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotSettings()" << endl;
#endif
    if(ClsFEDialogManager::Instance()!=NULL){
	ClsFEDialogManager::Instance()->createSettingsDialog();
    }

}



void iqrMainWindow::slotRunControl(){
    /* the state of qactionRunControl is used as a flag for wether a simulation is running or not... */
    if(qactionRunControl->isChecked()){
	/* start simulation */
	startSimulation();
    }
    else {
	/* stop simulation */
	stopSimulation();
    }
}


void iqrMainWindow::slotPause(){
   /* the state of qactionRunControl is used as a flag for wether a simulation is running or not... */
    if(qactionPause->isChecked()){
	pauseSimulation(true);
    }
    else {
	pauseSimulation(false);
    }
}


void iqrMainWindow::autoStartSimulation(){
    qactionRunControl->setChecked(true);
    startSimulation();
}


int iqrMainWindow::startSimulation() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::startSimulation()" << endl;
#endif
    int iError = 0;

/*
  QProgressDialog progress( "Copying files...", "Abort Copy", numFiles,
  this, "progress", TRUE );
*/


    if(isValidSystem(false)){
	lblStatusBar->setText("Initializing System");
	progress->reset();
//zzz	progress->setTotalSteps( ClsFESystemManager::Instance()->getTotalItemCount());
	progress->setMaximum( ClsFESystemManager::Instance()->getTotalItemCount());

	try{
	    ClsFESystemManager::Instance()->initSystem();
	}
	catch(runtime_error &e){
	    cerr << "System Error: " << e.what() << endl;
	    ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, e.what());
	    iError = -1;
	}
	
	if(!iError){
	    /* hack plot syncing in here */

	    bool bSyncPlots =  ClsFESystemManager::Instance()->getSyncPlots();
	    ClsFEComputeEngine::Instance()->prepare(bSyncPlots); 
	    ClsFEDataManager::Instance()->start(bSyncPlots);
	    ClsFEComputeEngine::Instance()->start(); 
	}
	
	if(!iError){
	    progress->reset();
	    lblStatusBar->setText("");
	    if(qlblStatus->movie()!=NULL){
		qlblStatus->movie()->unpause();
	    }
	    qactionRunControl->setIcon(QIcon(QPixmap( stop )));

	    /* fix: disable autosave while running 
	       qactionFileSave->setEnabled(false);
	       qactSaveSystem->setEnabled(false);
	       qactSaveSystemAs->setEnabled(false);
	       ----------------------------------- */

	    bSimulationRunning = true;
	    emit sigSimulationRunning(true);
	}
    } else {
	iError = -1;
    }

    if(iError){
	qactionRunControl->setChecked(false);
    } else {
	qactionPause->setEnabled(true);
    }
    return iError;
}

void iqrMainWindow::stopSimulation() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::stopSimulation()" << endl;
#endif
    ClsFEComputeEngine::Instance()->pause(false); 
    qactionPause->setChecked(false);
    ClsFEComputeEngine::Instance()->stop(); 
    cout << "iqrMainWindow::waiting..." << endl;
//zzz IS THIS THE RIGHT MUTEX???    qWaitCondition.wait();
    qWaitCondition.wait(&qmutexSysGUI);


    if(qlblStatus->movie()!=NULL){
	qlblStatus->movie()->pause();
    }
//    qactionRunControl->toggle();


    ClsFEDataManager::Instance()->stop();
    ClsFESystemManager::Instance()->cleanupSystem();
    qactionRunControl->setIcon(QIcon(QPixmap( run )));
    /* fix: disable autosave while running 
       qactionFileSave->setEnabled(true);
       qactSaveSystem->setEnabled(true);
       qactSaveSystemAs->setEnabled(true);
       ----------------------------------- */

    bSimulationRunning = false;
    qactionPause->setEnabled(false);
    emit sigSimulationRunning(false);
}


void iqrMainWindow::pauseSimulation(bool b) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::pauseSimulation(bool b)" << endl;
#endif
    /* where to check if we are running?? */
    ClsFEComputeEngine::Instance()->pause(b); 
    ClsFEDataManager::Instance()->pause(b);

}


void iqrMainWindow::closeEvent( QCloseEvent *e) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::closeEvent( QCloseEvent *e)" << endl;
#endif
    if(slotQuitIQR()){
	e->accept();
    }
};


void iqrMainWindow::slotValidateSystem(){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotValidateSystem()" << endl;
#endif
    ClsFESystemManager::Instance()->isValidSystem(true);
}



void iqrMainWindow::advanceProgress(){
//zzz    int p = progress->progress();
//zzz    progress->setProgress( ++p );
    progress->setValue(progress->value()+1 );

    qApp->processEvents();
}

void iqrMainWindow::showLoadProgress(string s){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::showLoadProgress(string s)" << endl;
#endif
    lblStatusBar->setText(s.c_str());
    lblStatusBar->update();
    qApp->processEvents();
}


void iqrMainWindow::customEvent( QEvent* e ) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::customEvent( QEvent* e )" << endl;
#endif
    if(dynamic_cast<ClsQCPSCustomEvent*>(e)){
	double fCPS = dynamic_cast<ClsQCPSCustomEvent*>(e)->getCPS();
	QString qstrCPS;
	qstrCPS.sprintf("CPS: %.2f", fCPS);
	lblStatusBar->setText(qstrCPS);
    }
 

   
}
 

void iqrMainWindow::changeState() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::changeState()" << endl;
#endif

    if(iActiveItemType == ClsFESystemManager::ITEM_SYSTEM){
	if(iClipboardType == ClsFESystemManager::ITEM_PROCESS){
	    qactPaste->setText( QString("Paste Process(es)"));
	    qactPaste->setEnabled( true );
	}
	else {
	    qactPaste->setText("Paste");
	    qactPaste->setEnabled( false );
	}
    }
    else if(iActiveItemType == ClsFESystemManager::ITEM_PROCESS){

	if(iClipboardType == ClsFESystemManager::ITEM_GROUP){
	    qactPaste->setText("Paste Group(s)");
	    qactPaste->setEnabled( true );
	}
	else if(iClipboardType == ClsFESystemManager::ITEM_CONNECTION){
	    qactPaste->setText("Paste Connection(s)");
	    qactPaste->setEnabled( true );
	}
	else if(iClipboardType == ClsFESystemManager::ITEM_GROUPSWITHCONNECTIONS){
	    qactPaste->setText("Paste Groups and Connection(s)");
	    qactPaste->setEnabled( true );
	}
	else {
	    qactPaste->setText("Paste");
	    qactPaste->setEnabled( false );
	}

    } else {
	qactPaste->setText( QString("Paste "));
	qactPaste->setEnabled( false );
    }
}

void iqrMainWindow::slotDiagViewActivated(int iType, string strID) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotDiagViewActivated(int,string)" << endl;
    cout << "Type: " << iType << endl;
    cout << "ID: " << strID << endl;
#endif
    iActiveItemType = iType;
    strActiveItemID = strID;
    changeState();

}



/* clipboard stuff */
void iqrMainWindow::slotClipboardChanged() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotClipboardChanged()" << endl;
#endif

    QClipboard *cb = QApplication::clipboard();
    QString text;

    text = cb->text(QClipboard::Clipboard);
//--    cout << "CB:" << endl << text.latin1() << endl;

    QString qstrType = "";
    QRegExp rx( "(<iqrEntity\\stype=\")(\\w*)" );
    if( rx.search( text ) != -1 ) {
	qstrType = rx.cap( 2 );
	if(qstrType==ClsTagLibrary::ProcessTag()){
//	    cout << "ITEM_PROCESS" << endl;
	    iClipboardType = ClsFESystemManager::ITEM_PROCESS;
	}
	else if(qstrType==ClsTagLibrary::GroupTag()){
//	    cout << "ITEM_GROUP" << endl;
	    iClipboardType = ClsFESystemManager::ITEM_GROUP;
	}
	else if(qstrType==ClsTagLibrary::ConnectionTag()){
//	    cout << "ITEM_CONNECTION" << endl;
	    iClipboardType = ClsFESystemManager::ITEM_CONNECTION;
	} 
	else if(qstrType==ClsTagLibrary::GroupsWithConnectionsTag()){
	    iClipboardType = ClsFESystemManager::ITEM_GROUPSWITHCONNECTIONS;
	}else {
	    iClipboardType = -99;
	}
    } else {
//	cout << "no match" << endl;
	iClipboardType = -99;
    }

    changeState();
}




void iqrMainWindow::copyItemsToClipboard(int iType, list<string> lstIDs){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::copyItemsToClipboard(int iType, list<string> lstIDs)" << endl;
#endif


    if(isValidSystem(false)){
	string strCBprefix = "CB"+iqrUtils::getEpoch();
	string strTemp;

	QClipboard *cb = QApplication::clipboard();
	QString qstr = "";
	if(iType == ClsFESystemManager::ITEM_PROCESS){
	    qstr.append(clsFESerializer->getProcessesAsXML(lstIDs).c_str());
	    strTemp = "id=\"" + strCBprefix;     qstr.replace( "id=\"", strTemp.c_str() );
	    strTemp = "source=\"" + strCBprefix; qstr.replace( "source=\"", strTemp.c_str() );
	    strTemp = "target=\"" + strCBprefix; qstr.replace( "target=\"", strTemp.c_str() );
	}
	else if(iType == ClsFESystemManager::ITEM_GROUP){
	    qstr.append(clsFESerializer->getGroupsAsXML(lstIDs).c_str());
	    strTemp = "id=\""+strCBprefix; qstr.replace( "id=\"", strTemp.c_str() );
	}
	else if(iType == ClsFESystemManager::ITEM_CONNECTION){
	    qstr.append(clsFESerializer->getConnectionsAsXML(lstIDs).c_str());
	    strTemp = "id=\""+strCBprefix; qstr.replace( "id=\"", strTemp.c_str() );
	}
	
	/* NEW 2008/06/07: 
	   the changes of IDs happens now when copying to the clipboard
	   this means that at the system manager level, we should not have to worry about dublicates anymore!
	   this should be a major simplification
	*/

	cb->setText( qstr, QClipboard::Clipboard) ;
    }
};



void iqrMainWindow::copyItemsToClipboard(list<string> lstGroupIDs, list<string> lstConnectionIDs) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "copyItemsToClipboard(list<string> lstGroupIDs, list<string> lstConnectionIDs)" << endl;
#endif
    if(isValidSystem(false)){
	string strCBprefix = "CB"+iqrUtils::getEpoch();
	string strTemp;

	QClipboard *cb = QApplication::clipboard();
	QString qstr = "";
	qstr.append(clsFESerializer->getGroupsWidthConnectionsAsXML(lstGroupIDs, lstConnectionIDs).c_str());
	strTemp = "id=\""+strCBprefix;     qstr.replace( "id=\"", strTemp.c_str() );
	strTemp = "source=\""+strCBprefix; qstr.replace( "source=\"", strTemp.c_str() );
	strTemp = "target=\""+strCBprefix; qstr.replace( "target=\"", strTemp.c_str() );

	cb->setText( qstr, QClipboard::Clipboard) ;
    }
};



void iqrMainWindow::exportProcess(string strID){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::exportProcess(string strID)" << endl;
#endif

    string strProcessXML = "";
    if(isValidSystem(false)){
	list<string> lst;
	lst.push_back(strID);
	strProcessXML = clsFESerializer->getProcessesAsXML(lst);
    }



    if(ClsFEDialogManager::Instance()!=NULL){
	string strFileTypeDescription = "iqr Process(*.iqrProcess)";
	string strExtension = ".iqrProcess";
	string strFilename = ClsFEDialogManager::Instance()->createSaveDialog(strFileTypeDescription, strExtension);

	if(strFilename.length()>0){
	    ofstream out(strFilename.c_str());
	    out << strProcessXML << endl;
	}

    }



}

#ifndef RC_TCP
void iqrMainWindow::slotReadRCUDP(){

    QByteArray datagram;
    datagram.resize(socketRC->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    
    socketRC->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    
    
    
    slotRemoteCommand(datagram);
}
#endif

void iqrMainWindow::slotRemoteCommand(const QString& qstrMessage){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotRemoteCommand(const QString&)" << endl;
#endif
    
    /* protocol:

       cmd:start
       cmd:stop
       cmd:quit
       cmd:startsampler
       cmd:stopsampler
       cmd:param;itemtype:<>;itemName:<>;paramName:<>;value<>
       cmd:param;itemtype:<>;itemID:<>;paramID:<>;value<>
NEW:
       cmd:plot;itemtype:<>;itemID:<>;paramID:<>

    */

    QString qstrCommand = "";
    QRegExp qRegExp;
//    qRegExp.setPattern("(cmd:)(.*?)(;)");
    qRegExp.setPattern("(cmd:)(start|stop|quit|startsampler|stopsampler|param|plot)");
    int pos = qRegExp.search(qstrMessage);
    if( pos > -1 ) {
	qstrCommand = qRegExp.cap( 2 );  
    }

    cout << "Command received: " << qstrCommand.latin1() << endl;



    if(!qstrCommand.compare("start")){
	cout << "START" << endl;
	if(!qactionRunControl->isChecked()){
	    cout << "starting running simulation" << endl;
	    startSimulation();
	    qactionRunControl->setChecked(true);
	}
    } else if(!qstrCommand.compare("stop")){
	cout << "STOP" << endl;
	if(qactionRunControl->isChecked()){
	    cout << "stopping running simulation" << endl;
	    stopSimulation();
	    qactionRunControl->setChecked(false);
	}
    } else if(!qstrCommand.compare("quit")){
	cout << "QUIT" << endl;
	slotQuitIQR();
    } else if(!qstrCommand.compare("startsampler")){
	cout << "STARTSAMPLER" << endl;
	ClsFEDataManager::Instance()->startDataSampler(true);
    } else if(!qstrCommand.compare("stopsampler")){
	cout << "STOPSAMPLER" << endl;
	ClsFEDataManager::Instance()->stopDataSampler(true);
    } else if(!qstrCommand.compare("param")){
	cout << "PARAM" << endl;
	qRegExp.setPattern("(.*;)(itemType:)(.*)(;)(itemID|itemName)(:)(.*)(;)(paramID|paramName)(:)(.*)(;value:)(.*)(;)");
	pos = qRegExp.search(qstrMessage);
//	cout << "pos: " << pos << endl;
	if( pos > -1 ) {
	    string strItemType = qRegExp.cap(3).toStdString();
	    string strItemIdentifierType = qRegExp.cap(5).toStdString();
	    string strItemIdentifier = qRegExp.cap(7).toStdString();
	    string strParamIdentifierType = qRegExp.cap(9).toStdString();
	    string strParamIdentifier = qRegExp.cap(11).toStdString();
	    string strValue = qRegExp.cap(13).toStdString();

	    if(!strItemType.compare("GROUP")){
		cout << "Changing : " << strParamIdentifier << " for GROUP: " << strItemIdentifier << " to: " << strValue << endl;
		if(!strItemIdentifierType.compare("itemID")){
		    ClsFESystemManager::Instance()->setParameterByItemID(ClsFESystemManager::ITEM_GROUP, strItemIdentifier, strParamIdentifier, strValue);
		} else if(!strItemIdentifierType.compare("itemName")){
		    ClsFESystemManager::Instance()->setParameterByItemName(ClsFESystemManager::ITEM_GROUP, strItemIdentifier, strParamIdentifier, strValue);
		} else {
		    cerr << "Unknown Item Identifier Type: " << strItemIdentifierType << endl;
		}
	    } 
	    else if(!strItemType.compare("NEURON")){
		cout << "Changing : " << strParamIdentifier << " for NEURON: " << strItemIdentifier << " to: " << strValue << endl;
		if(!strItemIdentifierType.compare("itemID")){
		    ClsFESystemManager::Instance()->setParameterByItemID(ClsFESystemManager::ITEM_NEURON, strItemIdentifier, strParamIdentifier, strValue);
		} else if(!strItemIdentifierType.compare("itemName")){
		    ClsFESystemManager::Instance()->setParameterByItemName(ClsFESystemManager::ITEM_NEURON, strItemIdentifier, strParamIdentifier, strValue);
		} else {
		    cerr << "Unknown Item Identifier Type: " << strItemIdentifierType << endl;
		}
	    } 
	    else if(!strItemType.compare("CONNECTION")){
		cout << "Changing : " << strParamIdentifier << " for CONNECTION: " << strItemIdentifier << " to: " << strValue << endl;
		if(!strItemIdentifierType.compare("itemID")){
		    ClsFESystemManager::Instance()->setParameterByItemID(ClsFESystemManager::ITEM_CONNECTION, strItemIdentifier, strParamIdentifier, strValue);
		} else if(!strItemIdentifierType.compare("itemName")){
		    ClsFESystemManager::Instance()->setParameterByItemName(ClsFESystemManager::ITEM_CONNECTION, strItemIdentifier, strParamIdentifier, strValue);
		} else {
		    cerr << "Unknown Item Identifier Type: " << strItemIdentifierType << endl;
		}
	    }
	    else if(!strItemType.compare("SYNAPSE")){
		cout << "Changing : " << strParamIdentifier << " for SYNAPSE: " << strItemIdentifier << " to: " << strValue << endl;
		if(!strItemIdentifierType.compare("itemID")){
		    ClsFESystemManager::Instance()->setParameterByItemID(ClsFESystemManager::ITEM_SYNAPSE, strItemIdentifier, strParamIdentifier, strValue);
		} else if(!strItemIdentifierType.compare("itemName")){
		    ClsFESystemManager::Instance()->setParameterByItemName(ClsFESystemManager::ITEM_SYNAPSE, strItemIdentifier, strParamIdentifier, strValue);
		} else {
		    cerr << "Unknown Item Identifier Type: " << strItemIdentifierType << endl;
		}
	    }
	    else if(!strItemType.compare("PROCESS")){
		cout << "Changing : " << strParamIdentifier << " for PROCESS: " << strItemIdentifier << " to: " << strValue << endl;
		if(!strItemIdentifierType.compare("itemID")){
		    ClsFESystemManager::Instance()->setParameterByItemID(ClsFESystemManager::ITEM_PROCESS, strItemIdentifier, strParamIdentifier, strValue);
		} else if(!strItemIdentifierType.compare("itemName")){
		    ClsFESystemManager::Instance()->setParameterByItemName(ClsFESystemManager::ITEM_PROCESS, strItemIdentifier, strParamIdentifier, strValue);
		} else {
		    cerr << "Unknown Item Identifier Type: " << strItemIdentifierType << endl;
		}
	    }
	    else if(!strItemType.compare("MODULE")){
		cout << "Changing : " << strParamIdentifier << " for MODULE: " << strItemIdentifier << " to: " << strValue << endl;
		if(!strItemIdentifierType.compare("itemID")){
		    ClsFESystemManager::Instance()->setParameterByItemID(ClsFESystemManager::ITEM_MODULE, strItemIdentifier, strParamIdentifier, strValue);
		} else if(!strItemIdentifierType.compare("itemName")){
		    ClsFESystemManager::Instance()->setParameterByItemName(ClsFESystemManager::ITEM_MODULE, strItemIdentifier, strParamIdentifier, strValue);
		} else {
		    cerr << "Unknown Item Identifier Type: " << strItemIdentifierType << endl;
		}
	    }
	}
    }  else if(!qstrCommand.compare("plot")){
	cout << "PLOT" << endl;
	qRegExp.setPattern("(.*;)(itemType:)(.*)(;)(itemID)(:)(.*)(;)(paramID)(:)(.*)(;)");
	pos = qRegExp.search(qstrMessage);
	cout << "pos: " << pos << endl;
	if( pos > -1 ) {
	    string strItemType = qRegExp.cap(3).toStdString();
	    string strItemIdentifierType = qRegExp.cap(5).toStdString();
	    string strItemIdentifier = qRegExp.cap(7).toStdString();
	    string strParamIdentifierType = qRegExp.cap(9).toStdString();
	    string strParamIdentifier = qRegExp.cap(11).toStdString();
	    
	    if(!strItemType.compare("GROUP")){
		cout << "Open plot for GROUP: " << strItemIdentifier << " parameter: " << strParamIdentifier << endl;
		if(!strItemIdentifierType.compare("itemID")){
		    ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_SPACEPLOT, strItemIdentifier, strParamIdentifier, "");
		} else {
		    cerr << "Unknown Item Identifier Type: " << strItemIdentifierType << endl;
		}
	    } 
	    else if(!strItemType.compare("CONNECTION")){
		cout << "Open plot for CONNECTION: " << strItemIdentifier << " parameter: " << strParamIdentifier  << endl;
		if(!strItemIdentifierType.compare("itemID")){
		    ClsFEDataManager::Instance()->DataClientCreate(diagramTypes::DIAGRAM_CONNECTION, strItemIdentifier, strParamIdentifier, "");
		} else {
		    cerr << "Unknown Item Identifier Type: " << strItemIdentifierType << endl;
		}
	    } else {
		cout << "unknown itenType: " << strItemType << endl;
	    }
	}
    } else {
	cout << "UNKNOWN COMMAND" << endl;
    }

/*
  QStringList qstrlst = QStringList::split(";", qstrMessage);
  cout << qstrlst.size() << endl;
  for( QStringList::Iterator it = qstrlst.begin(); it != qstrlst.end(); ++it ) {
  cout << *it << endl;
  }
*/


}



void iqrMainWindow::openConfig(string strConfigFileName){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::openConfig(string strConfigFileName)" << endl;
#endif
    if(ClsFEDataManager::Instance()!=NULL){
	ClsFEDataManager::Instance()->applyConfig(strConfigFileName) ;
    }

}



void iqrMainWindow::openHarborConfig(string strHarborConfigFileName){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::openHarborConfig(string strConfigFileName)" << endl;
#endif

    if(ClsFEParamRelais::Instance()!=NULL){
	ClsFEParamRelais::Instance()->applyConfig(strHarborConfigFileName) ;
    }

}

void iqrMainWindow::slotPaste() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotPaste()" << endl;
#endif

    QClipboard *cb = QApplication::clipboard();
    QString text;

    // Copy text from the clipboard(paste)
    text = cb->text(QClipboard::Clipboard);

    if(text.size()>0){

//	cout << "CB:" << endl << text.latin1() << endl;

	try {
	    ClsFESystemManager::Instance()->clsFESystemBuilder->parseFragment(text.latin1(), false, iActiveItemType, strActiveItemID );
	}
	catch(ClsSysFileParserException &e){
	    cout << e.getMessage() << endl;
//	    iError = -1;
	    string strError =(string)" in system file parser\n" + e.getMessage() ;
//	    reportError(strError);
	    ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	    return;
	}
    }
}

bool iqrMainWindow::slotQuitIQR() {
//    cout << "iqrMainWindow::slotQuitIQR()" << endl;
    if( slotCloseSystem()){
	saveSettings();
	qApp->quit();
	return true;
    }
    return false;
}


void iqrMainWindow::slotShortCuts(QAction * _qact){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotShortCuts(QAction * _qact)" << endl;
#endif

    if(_qact == qactCopyItem || _qact == qactCutItem || _qact == qactDeleteItem){
	list<string> lstGroups;
	list<string> lstProcesses;
	map<string, int> mapSelections = ClsFEDiagramManager::Instance()->getListSelectedItems();
	for(map<string, int>::iterator it=mapSelections.begin(); it!=mapSelections.end(); it++){
	    int iItemType = it->second;
	    if(iItemType == ClsFESystemManager::ITEM_PROCESS){
		lstProcesses.push_back(it->first);
	    } 
	    else if(iItemType == ClsFESystemManager::ITEM_GROUP){
		lstGroups.push_back(it->first);
	    }
	}
	list<string> lstAffectedConnections = ClsFESystemManager::Instance()->getConnectionsByInvolvedGroups(lstGroups);	

	if(_qact == qactCopyItem || _qact== qactCutItem){
//	    cout << "COPY || CUT" << endl;
	    if(lstProcesses.size() > 0){
		copyItemsToClipboard(ClsFESystemManager::ITEM_PROCESS, lstProcesses);
	    }
	    if(lstGroups.size()>0){
		if(lstAffectedConnections.size()>0){
		    copyItemsToClipboard(lstGroups, lstAffectedConnections);
		}
		else {
		    copyItemsToClipboard(ClsFESystemManager::ITEM_GROUP, lstGroups);
		}
	    }
	}
	if(!bSimulationRunning){
	    if(_qact == qactDeleteItem || _qact== qactCutItem){
//		cout << "DELETE || CUT" << endl;
		for(list<string>::iterator it=lstProcesses.begin(); it!=lstProcesses.end(); it++){
		    ClsFESystemManager::Instance()->deleteItem(ClsFESystemManager::ITEM_PROCESS, *it);
		}
		for(list<string>::iterator it=lstGroups.begin(); it!=lstGroups.end(); it++){
		    ClsFESystemManager::Instance()->deleteItem(ClsFESystemManager::ITEM_GROUP, *it);
		}
		for(list<string>::iterator it=lstAffectedConnections.begin(); it!=lstAffectedConnections.end(); it++){
		    ClsFESystemManager::Instance()->deleteItem(ClsFESystemManager::ITEM_CONNECTION, *it);
		}
	    }
	}
    }
    else if(_qact == qactPaste && !bSimulationRunning){
//	cout << "PASTE" << endl;
	slotPaste();
    }
}

void iqrMainWindow::setupRCServer(int iPort){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::setupRCServer(int iPort)" << endl;
#endif

#ifdef RC_TCP
    if(serverRC != NULL){
	delete serverRC;
    }
    cout << "iqr listening on TCP/IP port: " << iPort << endl;
    serverRC = new IpcServer( iPort, this );
    connect( serverRC, SIGNAL(receivedText(const QString&)), this, SLOT(slotRemoteCommand(const QString&)) );
#else
    if(socketRC != NULL){
	delete socketRC;
    }

    cout << "iqr listening on UDP port: " << iPort << endl;
    socketRC = new QUdpSocket(this);
    socketRC->bind(QHostAddress::LocalHost, iPort);

    connect(socketRC, SIGNAL(readyRead()), this, SLOT(slotReadRCUDP()));

#endif




}



void iqrMainWindow::startPdfHelp(int iType){
  #ifdef DEBUG_IQRMAINWINDOW
    cout << "siqrMainWindow::startPdfHelp(int iType)" << endl;
#endif

    QString qstrErrorMessage;
    QString qstrPdfFilename;

    switch(iType){
    case HELP_MANUAL:
#ifdef _WINDOWS
      qstrPdfFilename = string(ClsFESettings::instance().getAppDir() + "/doc/iqrUserManual.pdf").c_str();
#else
      if(ClsFESettings::instance().localInstall()){
	  qstrPdfFilename = string(ClsFESettings::instance().getAppDir() + "/doc/iqrUserManual.pdf").c_str();
      } else {
	  qstrPdfFilename = "/usr/share/doc/iqr/iqrUserManual.pdf";
      }
#endif
	qstrErrorMessage = "Cannot find pdf file for manual: " + qstrPdfFilename;
	break;
    case HELP_USERDEFINED_TYPES:
#ifdef _WINDOWS
      qstrPdfFilename = string(ClsFESettings::instance().getAppDir() + "/doc/iqrUserdefinedTypes.pdf").c_str();
#else
      if(ClsFESettings::instance().localInstall()){
	  qstrPdfFilename = string(ClsFESettings::instance().getAppDir() + "/doc/iqrUserdefinedTypes.pdf").c_str();
      } else {
	qstrPdfFilename = "/usr/share/doc/iqr-devel/iqrUserdefinedTypes.pdf";
      }
#endif
	qstrErrorMessage = "Cannot find pdf file for manual.\nMake sure the package \"iqr-devel\" is installed";
	break;
    default:
	break;
    }


    QFileInfo fileinfo( qstrPdfFilename );
    if( !fileinfo.exists() ) {
	QMessageBox::warning( this, "iqr error", qstrErrorMessage);
    } else {
#ifdef _WINDOWS
	QString qstrURL = "file:///";
	qstrURL.append(qstrPdfFilename);
	QDesktopServices::openUrl(QUrl(qstrURL, QUrl::TolerantMode));
#else
	QProcess proc( NULL );    
	proc.startDetached("xdg-open", QStringList(qstrPdfFilename));
#endif

    }

}


void iqrMainWindow::slotImportProcess(){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotImportProcess()" << endl;
#endif
    importORLinkProcess(false);
}

void iqrMainWindow::slotLinkProcess(){
    importORLinkProcess(true);
}

void iqrMainWindow::importORLinkProcess(bool bLink){
    QString qstrFileName = QFileDialog::getOpenFileName(
	this, /* parent */
	"", /* dir? */
	"Open File" , /*caption */
	"iqr process(*.iqrProcess)" /*filter*/
	);
    
    

    if(qstrFileName.length()>0){
	try {
	    ClsFESystemManager::Instance()->clsFESystemBuilder->importProcess(qstrFileName.latin1(), true, bLink);
	}
	catch(ClsSysFileParserException &e){
	    cout << e.getMessage() << endl;
	    string strError =(string)" in system file parser\n" + e.getMessage() ;
	    ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	    return;
	}
	catch(ClsFESystemBuilderException &e){
	    cerr << e.getMessage() << endl;
	    string strError = e.getMessage() ;
	    ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	    return;
	}
    }

}


bool iqrMainWindow::isValidSystem(bool bConfirm){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::isValidSystem(bool bConfirm)" << endl;
#endif

    if(ClsFESystemManager::Instance()->isValidSystem(bConfirm)){
	return true;
    } else {
	list<pair<int, string> > lstInvalidItems = ClsFESystemManager::Instance()->getListInvalidItems();
	list<pair<int, string> >::iterator it;
	for(it=lstInvalidItems.begin(); it!=lstInvalidItems.end(); it++){
	    pair<int, string> pairTemp = *it;
	    ClsFEDiagramManager::Instance()->markItem(pairTemp.first, pairTemp.second);
	}
	return false;
    }
    return false;

}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
 
