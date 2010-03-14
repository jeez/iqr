#include "iqrMainWindow.h"

#include <ctime>
#include <sstream>


#include <qregexp.h>
#include <qmessagebox.h>
#include <qclipboard.h>
#include <qcstring.h>
#include <qdir.h>
#include <qfileinfo.h>     
#include <qregexp.h>
#include <qstringlist.h>
#include <qprocess.h>

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


#include "ClsFEProcess.h"
#include "ClsFEGroup.h"
#include "ClsFEConnection.h"

#include "iqrUtils.h"
#include "tagLibrary.hpp"
#include "diagramTypes.h"
using namespace iqrfe;


namespace {
#include <fileopen_16x14.xpm>
#include <filesave_14x14.xpm>
#include <filesaveas_16x14.xpm>
#include <filenew_14x14.xpm>
#include <fileclose_16x14.xpm>
#include <systemproperties_14x14.xpm>
#include <quit_14x14.xpm>
#include <paste_16x14.xpm>
#include <close_14x14.xpm>
#include <SimControl_2_32x32.xpm>
#include <iqr_1_33x33.xpm>
#include <stop2_13x13.xpm>
#include <run2_13x13.xpm>
#include <icon_timeplot_14x14.xpm>
#include <icon_groupplot_14x14.xpm>
#include <iqr_mini_13x12.xpm>
}


#define LFO_BASE_ID 1000
//#define DEBUG_IQRMAINWINDOW

iqrMainWindow::iqrMainWindow(QWidget * parent, const char * name, WFlags f) :
    QMainWindow ( parent, name,f)  {
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
    clsQSystemBrowser = new ClsQSystemBrowser(qsplitter);
    ClsFEComputeEngine::initializeComputeEngine(this, &qmutexSysGUI, &qWaitCondition); 
    ClsFESystemManager::initializeSystemManager(this);
    clsFESerializer = new ClsFESerializer();
    ClsQLogWindow::initializeLogger(this);
    clsFECommHandler = new ClsFECommHandlerLocal();

    ClsFEParamRelais::initializeParamRelais(this);

/* end core elements */


/* general signaling */
    connect( QApplication::clipboard(), SIGNAL(dataChanged ()), this, SLOT(slotClipboardChanged()));

    connect(clsQSystemBrowser, SIGNAL(sigDataDisplay(int, string, string, string)), ClsFEDataManager::Instance(), SLOT(DataClientCreate(int, string, string, string )));
    connect(clsQSystemBrowser, SIGNAL(sysbrowserRightClick(int ,string,  QPoint)), ClsFEDialogManager::Instance(), SLOT(createPopup(int , string, QPoint)));
    connect(clsQSystemBrowser, SIGNAL(sigShowBlockDiagram(string)), ClsFEDiagramManager::Instance(), SLOT(slotBlockDiagramShow(string)));
    connect(ClsFEDiagramManager::Instance(), SIGNAL(sigItemActivated(int,string)), this, SLOT(slotItemActivated(int, string)));
/* ------------------------ */

/* signaling from the SystemManager */
    connect(ClsFESystemManager::Instance(), SIGNAL(systemBuildt()), this, SLOT(slotSystemBuildt()));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigAdvanceProgress()), this, SLOT(advanceProgress()));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigLoadProgress(string)), this, SLOT(showLoadProgress(string)));

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemAdded(int, string) ),     ClsFEDiagramManager::Instance(), SLOT(slotItemAdded(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDuplicated(int, string) ),ClsFEDiagramManager::Instance(), SLOT(slotItemDuplicated(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(int, string) ),   ClsFEDiagramManager::Instance(), SLOT(slotItemChanged(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ),   ClsFEDiagramManager::Instance(), SLOT(slotItemDeleted(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemUnDeleted(int, string) ), ClsFEDiagramManager::Instance(), SLOT(slotItemUnDeleted(int, string) ));


    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemAdded(int, string) ),      clsQSystemBrowser, SLOT(slotItemAdded(int, string) ));
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


    rcServer = new IpcServer( ipcPort, this );
    connect( rcServer, SIGNAL(receivedText(const QString&)), this, SLOT(slotRemoteCommand(const QString&)) );

    qaccelMainWindow = new QAccel( this);


    qaccelMainWindow->insertItem(Key_C+CTRL, ACCEL_COPY);
    qaccelMainWindow->insertItem(Key_X+CTRL, ACCEL_CUT);
    qaccelMainWindow->insertItem(Key_V+CTRL, ACCEL_PASTE);
    qaccelMainWindow->insertItem(Key_Delete, ACCEL_DELETE);
    connect( qaccelMainWindow, SIGNAL(activated(int)), SLOT(slotAccel(int)));


/* menubar FILE  */
    file = new QPopupMenu( this );
    menuBar()->insertItem( "&File", file );
    file-> insertTearOffHandle();
    file->insertItem( QPixmap( fileopen ), "&Open", this, SLOT(slotOpenSystem()), CTRL+Key_O, FILE_OPEN );
    file->insertItem( QPixmap( filenew ), "&New System", this, SLOT(slotNewSystem()), CTRL+Key_N, FILE_NEW_SYSTEM);
    file->insertSeparator();
    file->insertItem(QPixmap( fileclose ), "&Close System", this, SLOT( slotCloseSystem() ), 0, FILE_CLOSE_SYSTEM);
//    file->insertItem( "Save System (DEBUG)", this, SLOT(slotDebugSaveSystem()));
    file->insertItem(QPixmap( filesave ), "Save &System", this, SLOT(slotSaveSystem()), CTRL+Key_S, FILE_SAVE_SYSTEM);
    file->insertItem(QPixmap( filesaveas ), "Save System &As", this, SLOT(slotSaveSystemAs()), CTRL+Key_A, FILE_SAVE_SYSTEM_AS);
    file->insertSeparator();
    file->insertItem("Save to &Backup", this, SLOT(slotSaveToBackup()), CTRL+Key_B, FILE_SAVE_TO_BACKUP);
    file->insertItem( "&Export for AER", this, SLOT(slotExportSystemForAER()), CTRL+Key_E, FILE_EXPORT_FOR_AER);
    file->insertSeparator();
    file->insertItem( "Import Process", this, SLOT(slotImportProcess()), 0, FILE_IMPORT_PROCESS);    
    file->insertItem( "Link-in Process", this, SLOT(slotLinkProcess()), 0, FILE_LINK_PROCESS);    
	
    file->insertSeparator();



    file->insertItem(QPixmap( systemproperties ), "System Properties", this, SLOT( slotSystemProperties() ), 0, FILE_SYSTEM_PROPERTIES);
    file->insertSeparator();
    file->insertItem( QPixmap( quit ), "&Quit iqr", this, SLOT( slotQuitIQR() ), 0, FILE_QUIT);

    file->insertSeparator();

    list<string> lstLFO = ClsFESettings::instance().getListLastFiles();
    initLFO(lstLFO);

    file->setItemEnabled (FILE_SAVE_SYSTEM,    false);
    file->setItemEnabled (FILE_SAVE_TO_BACKUP, false);
    file->setItemEnabled (FILE_EXPORT_FOR_AER, false);

/* menubar EDIT  */
    edit = new QPopupMenu( this );
    menuBar()->insertItem( "&Edit", edit );
//    edit->insertItem( "cut", this, SLOT( slotCut() ));
//    edit->insertItem( "copy", this, SLOT( slotCopy() ));
    edit->insertItem(QPixmap(paste),  "Paste", this, SLOT( slotPaste() ), 0, iqrMainWindow::ITEM_PASTE);
    edit->insertSeparator();
    edit->insertItem( "Settings", this, SLOT( slotSettings() ));
    edit->insertItem( "Validate System", this, SLOT( slotValidateSystem() ));

/* menubar VIEW  */
    view = new QPopupMenu( this );
    menuBar()->insertItem( "&View", view );
    view->insertItem(  "&Log", this, SLOT(slotShowLogWindow()), CTRL+Key_L );

/* menubar DIAGRAM  */
    QPopupMenu * qpmDiagram = new QPopupMenu( this );
    menuBar()->insertItem( "&Diagram", qpmDiagram );
    qpmDiagram-> insertTearOffHandle();
    qpmDiagram->insertItem("Save Diagram", this, SLOT(slotSaveDiagram()));
    qpmDiagram->insertItem("Print Diagram", this, SLOT(slotPrintDiagram()));


/* menubar DATA  */
    QPopupMenu * DAV = new QPopupMenu( this );
    menuBar()->insertItem( "D&ata", DAV );
    DAV-> insertTearOffHandle();
//     DAV->insertItem( "Space plot", MENU_SPACE_PLOT, -1 );
//    DAV->insertItem( QPixmap(icon_timeplot), "Time Plot", MENU_TIME_PLOT, -1 );
    DAV->insertItem( "Data Sampler", MENU_DATA_SAMPLER, -1 );
    DAV->insertSeparator();
    DAV->insertItem( "Open Configuration", ClsFEDataManager::Instance(), SLOT( applyConfig() ));
    DAV->insertItem( "&Save Configuration", ClsFEDataManager::Instance(), SLOT( saveConfig() ));
    DAV->insertSeparator();
    DAV->insertItem( "Harbor", MENU_PARAMRELAIS, -1 );

    connect( DAV, SIGNAL(activated(int)), SLOT(slotDAVMenu(int)) ); 

    menuBar()->insertSeparator();

    QPopupMenu * qpopMenuHelp = new QPopupMenu( this );
    menuBar()->insertItem( "&Help", qpopMenuHelp,-1 );
//OUTDATED!!    qpopMenuHelp->insertItem("Release Notes", HELP_RELEASENOTES, -1 );

    qpopMenuHelp->insertItem("Manual (pdf)", HELP_MANUAL, -1 );
    qpopMenuHelp->insertItem("Writing user-defined types (pdf)", HELP_USERDEFINED_TYPES, -1 );

    qpopMenuHelp->insertSeparator();
    qpopMenuHelp->insertItem(QPixmap( iqr_mini ), "About iqr", HELP_ABOUT, -1 );


    connect( qpopMenuHelp, SIGNAL(activated(int)), SLOT(slotHelpMenu(int)) );



/* end menubar */


/* ----------------- */




/* start toolbar */
    qtoolbarToolbar = new QToolBar(this, "toolbar");
    qtoolbarToolbar->setHorizontallyStretchable (true);
    qtoolbarToolbar->setFixedExtentHeight (28);


    QToolButton *qtoolbtnFileNew = new QToolButton( QIconSet(QPixmap( filenew )), "New system", QString::null,
						    this, SLOT(slotNewSystem()), qtoolbarToolbar, "new system" );

    QToolButton *qtoolbtnFileOpen = new QToolButton( QIconSet(QPixmap( fileopen )), "Open ", QString::null,
						     this, SLOT(slotOpenSystem()), qtoolbarToolbar, "open system" );

    qtoolbtnFileSave = new QToolButton( QIconSet(QPixmap( filesave )), "Save system", QString::null,
					this, SLOT(slotSaveSystem()), qtoolbarToolbar, "save system" );

/*    qtoolbtnFileSave->setDisabled (true ); */
    qtoolbarToolbar->addSeparator ();

    qtoolbtnRunControl = new QToolButton (qtoolbarToolbar, "run_control") ;
    qtoolbtnRunControl->setPixmap(QPixmap( run ));
    qtoolbtnRunControl->setToggleButton(true);
    qtoolbtnRunControl->setEnabled(false);
    qtoolbtnRunControl->setAutoRaise (true );
    connect(qtoolbtnRunControl,SIGNAL(clicked()), SLOT(slotRunControl()));
/* end toolbar */

//    qstatusBar = new QStatusBar(this, "statusbar");

    lblStatusBar = new QLabel( statusBar() );
    statusBar()->addWidget( lblStatusBar, 2, TRUE );
    lblStatusBar->setText("Initializing System");

    progress = new QProgressBar( statusBar() );
    statusBar()->addWidget( progress, 1, TRUE );


    createAnimation();

//20080214
//    cout << "AutoSave interval: " << ClsFESettings::instance().getAutoSaveInterval() << endl;
//20080214    if(ClsFESettings::instance().getAutoSaveInterval()>0){
//20080214	int iAutoSaveInterval = ClsFESettings::instance().getAutoSaveInterval();
    qtimerAutoSave = new QTimer( this );
    connect( qtimerAutoSave, SIGNAL(timeout()), this, SLOT(slotAutoSaveSystem()) );
//20080214	qtimerAutoSave->start(iAutoSaveInterval*60000, FALSE);
//20080214    }


}

void iqrMainWindow::updateLFO(string str){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::updateLFO(string str)" << endl;
#endif

    ClsFESettings::instance().addFileOpened(str);

    if(dequeLFO.size()>0){
	if(str.compare(dequeLFO.front())){ /* element not at the beginning */
	    for(unsigned int ii=0; ii<dequeLFO.size(); ii++){
		file->removeItem (ii+LFO_BASE_ID);
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
		file->insertItem( strNumbered.c_str(), this, SLOT( slotOpenOldFile(int)), 0, iID);
		file->setItemParameter( iID, ii );
	    }
	}
    }
}

void iqrMainWindow::initLFO(list<string> &lstLFO){
    int iCount = 0;
    while (lstLFO.size()) {
	string strFilename = lstLFO.front();
	string strNumbered = "&" + iqrUtils::int2string(iCount+1) + ": ";
	strNumbered = strNumbered + strFilename;
	int iID = iCount + LFO_BASE_ID;
	file->insertItem( strNumbered.c_str(), this, SLOT( slotOpenOldFile(int)), 0, iID);
	file->setItemParameter( iID, iCount );
	dequeLFO.push_back(strFilename);
	lstLFO.pop_front();
	iCount++;
    }
}


void iqrMainWindow::createAnimation() {
    QFrame *qframeSpace = new QFrame(qtoolbarToolbar);
    qtoolbarToolbar->setStretchableWidget ( qframeSpace );


    QFrame *qframeAnimation = new QFrame(qtoolbarToolbar);
    qframeAnimation->setBackgroundColor(QColor(255,0,0));
    qframeAnimation->setFixedWidth(30);
    qlblStatus = new QLabel(qframeAnimation, "animation");



    QString filename = "/usr/lib/iqr/iqr-logo.gif";
    if(QFile::exists(filename)){
	qlblStatus->setMovie(QMovie(filename));
    }


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
	file->setItemEnabled (iqrMainWindow::FILE_CLOSE_SYSTEM,      true);
	file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM,       false);
	file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM_AS,    true);
	file->setItemEnabled (iqrMainWindow::FILE_SAVE_TO_BACKUP,    false);
	file->setItemEnabled (iqrMainWindow::FILE_EXPORT_FOR_AER,    false);
	file->setItemEnabled (iqrMainWindow::FILE_SYSTEM_PROPERTIES, true);
	file->setItemEnabled (iqrMainWindow::FILE_IMPORT_PROCESS, true);
	file->setItemEnabled (iqrMainWindow::FILE_LINK_PROCESS, true);
	
	qtoolbtnFileSave->setDisabled(false);
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


void iqrMainWindow::slotDAVMenu(int iMenuID){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotDAVMenu(int iMenuID)" << endl;
#endif

    switch (iMenuID){
//     case MENU_SPACE_PLOT:
//	  ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_SPACEPLOT , clsQlstItemGroup->getGroupID(), "", 0);
//	  break;
    case MENU_TIME_PLOT:
	ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_TIMEPLOT , "", "", "");
	break;
    case MENU_DATA_SAMPLER:
	ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_DATASAMPLER , "", "", "");
	break;
    case MENU_PARAMRELAIS:
	ClsFEParamRelais::Instance()->showHarbor();
	break;
    default:
	break;
    }
};

void iqrMainWindow::slotHelpMenu(int iMenuID){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotHelpMenu(int iMenuID)" << endl;
#endif
    switch(iMenuID){
    case HELP_RELEASENOTES:
	ClsFEDialogManager::Instance()->createReleaseNotesDialog();
	break;

    case HELP_MANUAL:
	startPdfHelp(iMenuID);
	break;

    case HELP_USERDEFINED_TYPES:
	startPdfHelp(iMenuID);
	break;


    case HELP_ABOUT:
	ClsFEDialogManager::Instance()->createAboutDialog();
	break;
    default:
	break;
    }




};


void iqrMainWindow::openSystem(string s) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::openSystem(string s)" << endl;
#endif

    if(QDir::isRelativePath(s.c_str())){
//	cout << "RELATIVE PATH" << endl;
	string str(1,QDir::separator());
	strSystemFileName = string(QDir::currentDirPath().latin1()) + str + s;
    } else {
//	cout << "ABSOLUTE PATH" << endl;
	strSystemFileName = s;
    }

    /* we have to add a check here, because this function can be called from the command line... */
    QFileInfo fileinfo( strSystemFileName );
    if ( !fileinfo.exists() ) {
	QString qstrErrorMessage;
	qstrErrorMessage = "Cannot find system file\n\"" + strSystemFileName + "\"";
	QMessageBox::warning( NULL, "iqr error", qstrErrorMessage);
    } else {
//    cout << "**********strSystemFileName: " << strSystemFileName << endl;
	if(!buildSystem()){
	    file->setItemEnabled (iqrMainWindow::FILE_CLOSE_SYSTEM,      true);
	    file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM,       true);
	    file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM_AS,    true);
	    file->setItemEnabled (iqrMainWindow::FILE_SAVE_TO_BACKUP,    true);
	    file->setItemEnabled (iqrMainWindow::FILE_EXPORT_FOR_AER,    true);
	    file->setItemEnabled (iqrMainWindow::FILE_SYSTEM_PROPERTIES, true);
	    file->setItemEnabled (iqrMainWindow::FILE_IMPORT_PROCESS,    true);
	    file->setItemEnabled (iqrMainWindow::FILE_LINK_PROCESS,      true);
	    
	    qtoolbtnFileSave->setDisabled(false);
	    progress->reset();
	    lblStatusBar->setText("");
	    this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
	    setupRCServer();
	}
    }
}


void iqrMainWindow::slotOpenSystem(){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotOpenSystem()" << endl;
#endif
    QString qstrFileName = QFileDialog::getOpenFileName(
	"",
	"Systemfile (*.iqr)",
	this,
	"Open File"
	"Choose a file" );

    if(qstrFileName.length()>0){
	if( slotCloseSystem()){
	    strSystemFileName = qstrFileName.latin1();
	    if(!buildSystem()){
		file->setItemEnabled (iqrMainWindow::FILE_CLOSE_SYSTEM,      true);
		file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM,       true);
		file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM_AS,    true);
		file->setItemEnabled (iqrMainWindow::FILE_SAVE_TO_BACKUP,    true);
		file->setItemEnabled (iqrMainWindow::FILE_EXPORT_FOR_AER,    true);
		file->setItemEnabled (iqrMainWindow::FILE_SYSTEM_PROPERTIES, true);
		file->setItemEnabled (iqrMainWindow::FILE_IMPORT_PROCESS,    true);
		file->setItemEnabled (iqrMainWindow::FILE_LINK_PROCESS,      true);

		qtoolbtnFileSave->setDisabled(false);
		updateLFO(strSystemFileName);

//		int iID = file->insertItem( strSystemFileName.c_str(), this, SLOT( slotOpenOldFile(int)));
//		file->setItemParameter( iID, iID );
		lblStatusBar->setText("");
		this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
		setupRCServer();
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
	QString qstrFileName = dequeLFO[ii];
	if(qstrFileName.length()>0){
	    strSystemFileName = qstrFileName.latin1();
	    if(!buildSystem()){
		file->setItemEnabled (iqrMainWindow::FILE_CLOSE_SYSTEM,      true);
		file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM,       true);
		file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM_AS,    true);
		file->setItemEnabled (iqrMainWindow::FILE_SAVE_TO_BACKUP,    true);
		file->setItemEnabled (iqrMainWindow::FILE_EXPORT_FOR_AER,    true);
		file->setItemEnabled (iqrMainWindow::FILE_SYSTEM_PROPERTIES, true);
		file->setItemEnabled (iqrMainWindow::FILE_IMPORT_PROCESS,    true);
		file->setItemEnabled (iqrMainWindow::FILE_LINK_PROCESS,      true);

		qtoolbtnFileSave->setDisabled(false);
		lblStatusBar->setText("");
		this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
		setupRCServer();
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


    if(qtoolbtnRunControl->isOn()){
	cout << "stopping running simulation" << endl;
	stopSimulation();
	qtoolbtnRunControl->setOn(false);
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
	qtoolbtnRunControl->setEnabled(false);
    }

    if(iReturn == 0 || iReturn == 1 ){
	ClsFESystemManager::Instance()->closeSystem();
	ClsFEDiagramManager::Instance()->closeSystem();
	clsQSystemBrowser->closeSystem();
	ClsFEDialogManager::Instance()->closeAllDialogs();
	ClsFEDataManager::Instance()->closeAllPlots();
	ClsFEParamRelais::Instance()->cleanHarbor();

	qtoolbtnRunControl->setEnabled(false);

	file->setItemEnabled (iqrMainWindow::FILE_CLOSE_SYSTEM,      false);
	file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM,       false);
	file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM_AS,    false);
	file->setItemEnabled (iqrMainWindow::FILE_SAVE_TO_BACKUP,    false);
	file->setItemEnabled (iqrMainWindow::FILE_EXPORT_FOR_AER,    false);
	file->setItemEnabled (iqrMainWindow::FILE_SYSTEM_PROPERTIES, false);
	file->setItemEnabled (iqrMainWindow::FILE_IMPORT_PROCESS,    false);
	file->setItemEnabled (iqrMainWindow::FILE_LINK_PROCESS,      false);

	qtoolbtnFileSave->setDisabled(true);
	strSystemFileName = "";
    }
    return true;
}


bool iqrMainWindow::buildSystem() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::buildSystem()" << endl;
#endif
    setCursor(WaitCursor);
    ClsFESystemManager::Instance()->clsFESystemBuilder->readSystemFileFromDisk(strSystemFileName);
    try {
	ClsFESystemManager::Instance()->clsFESystemBuilder->parseSystemFile( );
    }
    catch (ClsSysFileParserException &e){
	cerr << e.getMessage() << endl;
	string strError = (string)" in system file parser\n" + e.getMessage() ;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
//	reportError(strError);
	setCursor(ArrowCursor);
	return true;
    }

    try{
	ClsFESystemManager::Instance()->buildSystem( );
    }
    catch (ClsSysFileParserException &e){
	cerr << e.getMessage() << endl;
	string strError = (string)" reading system file\n" + e.getMessage() ;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
//	reportError(strError);
	setCursor(ArrowCursor);
	return true;
    }
    catch (ClsFESystemBuilderException &e){
	cerr << e.getMessage() << endl;
	string strError = (string)" builing system\n" + e.getMessage() ;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	setCursor(ArrowCursor);
	return true;
    }


#ifdef DEBUG_IQRMAINWINDOW
    cout << "done building system" << endl;
#endif
    setCursor(ArrowCursor);
    return false;

};


void iqrMainWindow::slotSystemBuildt(){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotSystemBuildt()" << endl;
#endif
    ClsFEDiagramManager::Instance()->DiagramCreate(diagramTypes::DIAGRAM_BLOCK);
    ClsFEDiagramManager::Instance()->slotSystemChanged();
    clsQSystemBrowser->slotSystemChanged();
    qtoolbtnRunControl->setEnabled(true);

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
	QFileInfo qFileInfo(strSystemFileName );
	if ( !qFileInfo.isWritable() ) {
	    int iReturn = QMessageBox::critical( this, "iqr error",
						 "No permission to write file \"" + strSystemFileName + "\"\n",
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
	string strFileTypeDescription = "Systemfile (*.iqr)";
	string strExtension = ".iqr";
	strFilename = ClsFEDialogManager::Instance()->createSaveDialog(strFileTypeDescription, strExtension);
    }

    if(strFilename.length()>0){
	if(clsFESerializer!=NULL){
//	string strFilename = qstrFileName.latin1();
	    strSystemFileName = strFilename;
	    file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM, true);
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
    string strFileTypeDescription = "AER iqr systemfile (*.aeriqr)";
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
    /* the state of qtoolbtnRunControl is used as a flag for wether a simulation is running or not... */
    if(qtoolbtnRunControl->isOn()){
	/* start simulation */
	startSimulation();
    }
    else {
	/* stop simulation */
	stopSimulation();
    }
}

void iqrMainWindow::autoStartSimulation(){
    qtoolbtnRunControl->setOn(true);
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
	progress->setTotalSteps( ClsFESystemManager::Instance()->getTotalItemCount());

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
	    qtoolbtnRunControl->setPixmap(QPixmap( stop ));

	    /* fix: disable autosave while running 
	       qtoolbtnFileSave->setEnabled(false);
	       file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM,       false);
	       file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM_AS,    false);
	       ----------------------------------- */

	    bSimulationRunning = true;
	    emit sigSimulationRunning(true);
	}
    } else {
	iError = -1;
    }

    if(iError){
	qtoolbtnRunControl->setOn(false);
    }
    return iError;
}

void iqrMainWindow::stopSimulation() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::stopSimulation()" << endl;
#endif
    ClsFEComputeEngine::Instance()->stop(); 
    cout << "iqrMainWindow::waiting..." << endl;
    qWaitCondition.wait();
    
    if(qlblStatus->movie()!=NULL){
	qlblStatus->movie()->pause();
    }
//    qtoolbtnRunControl->toggle();


    ClsFEDataManager::Instance()->stop();
    ClsFESystemManager::Instance()->cleanupSystem();
    qtoolbtnRunControl->setPixmap(QPixmap( run ));
    /* fix: disable autosave while running 
       qtoolbtnFileSave->setEnabled(true);
       file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM,       true);
       file->setItemEnabled (iqrMainWindow::FILE_SAVE_SYSTEM_AS,    true);
       ----------------------------------- */

    bSimulationRunning = false;
    emit sigSimulationRunning(false);
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
    int p = progress->progress();
    progress->setProgress( ++p );
    qApp->processEvents();
}

void iqrMainWindow::showLoadProgress(string s){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::showLoadProgress(string s)" << endl;
#endif
    lblStatusBar->setText(s);
    lblStatusBar->update ();
    qApp->processEvents();
}


void iqrMainWindow::customEvent( QCustomEvent* e ) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::customEvent( QCustomEvent* e )" << endl;
#endif
    if (dynamic_cast<ClsQCPSCustomEvent*>(e)){
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
	    edit->changeItem(iqrMainWindow::ITEM_PASTE, QString("Paste Process(es)"));
	    edit->setItemEnabled (iqrMainWindow::ITEM_PASTE, true );
	}
	else {
	    edit->changeItem(iqrMainWindow::ITEM_PASTE, QString("Paste "));
	    edit->setItemEnabled (iqrMainWindow::ITEM_PASTE, false );
	}
    }
    else if(iActiveItemType == ClsFESystemManager::ITEM_PROCESS){
//	cout << "iClipboardType: " << iClipboardType << endl;
	if(iClipboardType == ClsFESystemManager::ITEM_GROUP){
	    edit->changeItem(iqrMainWindow::ITEM_PASTE, QString("Paste Group(s)"));
	    edit->setItemEnabled (iqrMainWindow::ITEM_PASTE, true );
	}
	else if(iClipboardType == ClsFESystemManager::ITEM_CONNECTION){
	    edit->changeItem(iqrMainWindow::ITEM_PASTE, QString("Paste Connection(s)"));
	    edit->setItemEnabled (iqrMainWindow::ITEM_PASTE, true );
	}
	else if(iClipboardType == ClsFESystemManager::ITEM_GROUPSWITHCONNECTIONS){
	    edit->changeItem(iqrMainWindow::ITEM_PASTE, QString("Paste Groups and Connection(s)"));
	    edit->setItemEnabled (iqrMainWindow::ITEM_PASTE, true );
	}
	else {
	    edit->changeItem(iqrMainWindow::ITEM_PASTE, QString("Paste "));
	    edit->setItemEnabled (iqrMainWindow::ITEM_PASTE, false );
	}

    } else {
	edit->changeItem(iqrMainWindow::ITEM_PASTE, QString("Paste "));
	edit->setItemEnabled (iqrMainWindow::ITEM_PASTE, false );
    }
}

void iqrMainWindow::slotItemActivated(int iType, string strID) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotItemActivated(int,string)" << endl;
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
//    cout << "CB:" << endl << text.latin1() << endl;

    QString qstrType = "";
    QRegExp rx( "(<Clipboard\\stype=\")(\\w*)" );
    if ( rx.search( text ) != -1 ) {
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
	QString qstrTemp;

	QClipboard *cb = QApplication::clipboard();
	QString qstr = "";
	if(iType == ClsFESystemManager::ITEM_PROCESS){
	    qstr.append(clsFESerializer->getProcessesAsXML(lstIDs).c_str());
	    qstrTemp = "id=\""+strCBprefix;     qstr.replace( "id=\"", qstrTemp );
	    qstrTemp = "source=\""+strCBprefix; qstr.replace( "source=\"", qstrTemp );
	    qstrTemp = "target=\""+strCBprefix; qstr.replace( "target=\"", qstrTemp );
	}
	else if(iType == ClsFESystemManager::ITEM_GROUP){
	    qstr.append(clsFESerializer->getGroupsAsXML(lstIDs).c_str());
	    qstrTemp = "id=\""+strCBprefix; qstr.replace( "id=\"", qstrTemp );
	}
	else if(iType == ClsFESystemManager::ITEM_CONNECTION){
	    qstr.append(clsFESerializer->getConnectionsAsXML(lstIDs).c_str());
	    qstrTemp = "id=\""+strCBprefix; qstr.replace( "id=\"", qstrTemp );
	}
	
	/* NEW 2008/06/07: 
	   the changes of IDs happens now when copying to the clipboard
	   this means that at the system manager level, we should not have to worry about dublicates anymore!
	   this should be a major simplification
	*/

	cb->setText ( qstr, QClipboard::Clipboard) ;
    }
};



void iqrMainWindow::copyItemsToClipboard(list<string> lstGroupIDs,  list<string> lstConnectionIDs) {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "copyItemsToClipboard(list<string> lstGroupIDs,  list<string> lstConnectionIDs)" << endl;
#endif
    if(isValidSystem(false)){
	string strCBprefix = "CB"+iqrUtils::getEpoch();
	QString qstrTemp;

	QClipboard *cb = QApplication::clipboard();
	QString qstr = "";
	qstr.append(clsFESerializer->getGroupsWidthConnectionsAsXML(lstGroupIDs, lstConnectionIDs).c_str());
	qstrTemp = "id=\""+strCBprefix;     qstr.replace( "id=\"", qstrTemp );
	qstrTemp = "source=\""+strCBprefix; qstr.replace( "source=\"", qstrTemp );
	qstrTemp = "target=\""+strCBprefix; qstr.replace( "target=\"", qstrTemp );

	cb->setText ( qstr, QClipboard::Clipboard) ;
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
	string strFileTypeDescription = "iqr Process (*.iqrProcess)";
	string strExtension = ".iqrProcess";
	string strFilename = ClsFEDialogManager::Instance()->createSaveDialog(strFileTypeDescription, strExtension);

	if(strFilename.length()>0){
	    ofstream out(strFilename.c_str());
	    out << strProcessXML << endl;
	}

    }



}


void iqrMainWindow::slotRemoteCommand(const QString& qstrMessage){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotRemoteCommand(const QString&)" << endl;
#endif
    
    /* protocol:

       cmd:start
       cmd:stop
       cmd:quit
       cmd:param;itemtype:<>;itemName:<>;paramName:<>;value<>
       cmd:param;itemtype:<>;itemID:<>;paramID:<>;value<>

    */

    QString qstrCommand = "";
    QRegExp qRegExp;
//    qRegExp.setPattern("(cmd:)(.*?)(;)");
    qRegExp.setPattern("(cmd:)(start|stop|quit|startsampler|stopsampler|param)");
    int pos = qRegExp.search(qstrMessage);
    if ( pos > -1 ) {
	qstrCommand = qRegExp.cap( 2 );  // "cm"
    }

    if(!qstrCommand.compare("start")){
	cout << "START" << endl;
	if(!qtoolbtnRunControl->isOn()){
	    cout << "starting running simulation" << endl;
	    startSimulation();
	    qtoolbtnRunControl->setOn(true);
	}
    } else if(!qstrCommand.compare("stop")){
	cout << "STOP" << endl;
	if(qtoolbtnRunControl->isOn()){
	    cout << "stopping running simulation" << endl;
	    stopSimulation();
	    qtoolbtnRunControl->setOn(false);
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
//qRegExp.setPattern("(.*;)(itemType:)(.*)(;itemID:)(.*)(;paramID:)(.*)(;value:)(.*)(;)");
	qRegExp.setPattern("(.*;)(itemType:)(.*)(;)(itemID|itemName)(:)(.*)(;)(paramID|paramName)(:)(.*)(;value:)(.*)(;)");
	pos = qRegExp.search(qstrMessage);
//	cout << "pos: " << pos << endl;
	if ( pos > -1 ) {
	    string strItemType = qRegExp.cap(3);
	    string strItemIdentifierType = qRegExp.cap(5);
	    string strItemIdentifier = qRegExp.cap(7);
	    string strParamIdentifierType = qRegExp.cap(9);
	    string strParamIdentifier = qRegExp.cap(11);
	    string strValue = qRegExp.cap(13);

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
    } else {
	cout << "UNKNOWN COMMAND" << endl;
    }

/*
  QStringList qstrlst = QStringList::split(";", qstrMessage);
  cout << qstrlst.size() << endl;
  for ( QStringList::Iterator it = qstrlst.begin(); it != qstrlst.end(); ++it ) {
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

    // Copy text from the clipboard (paste)
    text = cb->text(QClipboard::Clipboard);

    if(text){

//	cout << "CB:" << endl << text.latin1() << endl;

	try {
	    ClsFESystemManager::Instance()->clsFESystemBuilder->parseFragment(text.latin1(), false, iActiveItemType, strActiveItemID );
	}
	catch (ClsSysFileParserException &e){
	    cout << e.getMessage() << endl;
//	    iError = -1;
	    string strError = (string)" in system file parser\n" + e.getMessage() ;
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


void iqrMainWindow::slotAccel(int ii){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::slotAccel(int i)" << endl;
#endif

    if(ii == ACCEL_COPY || ii == ACCEL_CUT || ii == ACCEL_DELETE){
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

	if(ii == ACCEL_COPY || ii== ACCEL_CUT){
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
	    if(ii == ACCEL_DELETE || ii== ACCEL_CUT){
//	    cout << "DELETE || CUT" << endl;
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
    else if(ii == ACCEL_PASTE && !bSimulationRunning){
//	cout << "PASTE" << endl;
	slotPaste();
    }
}

void iqrMainWindow::setupRCServer(){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "iqrMainWindow::setupRCServer()" << endl;
#endif
    if(rcServer != NULL){
	delete rcServer;
    }
    int iPort = ClsFESystemManager::Instance()->getSystemPort();
    cout << "iqr listening on port: " << iPort << endl;
    rcServer = new IpcServer( iPort, this );
    connect( rcServer, SIGNAL(receivedText(const QString&)), this, SLOT(slotRemoteCommand(const QString&)) );
}



void iqrMainWindow::startPdfHelp(int iType){
#ifdef DEBUG_IQRMAINWINDOW
    cout << "siqrMainWindow::tartPdfHelp(int iType)" << endl;
#endif

    QString qstrErrorMessage;
    QString qstrPdfFilename;

    switch(iType){
    case HELP_MANUAL:
	qstrPdfFilename = "/usr/share/doc/iqr/iqrUserManual.pdf";
	qstrErrorMessage = "Cannot find pdf file for manual";
	break;
    case HELP_USERDEFINED_TYPES:
	qstrPdfFilename = "/usr/share/doc/iqr-devel/iqrUserdefinedTypes.pdf";
	qstrErrorMessage = "Cannot find pdf file for manual.\nMake sure the package \"iqr-devel\" is installed";
	break;
    default:
	break;
    }


    QFileInfo fileinfo( qstrPdfFilename );
    if ( !fileinfo.exists() ) {
	QMessageBox::warning( this, "iqr error", qstrErrorMessage);
    } else {
	QProcess proc( NULL );    
	proc.addArgument( "xdg-open" );
	proc.addArgument( qstrPdfFilename );
	proc.start();
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
	"",
	"iqr process (*.iqrProcess)",
	this,
	"Open File"
	"Choose a file" );
    if(qstrFileName.length()>0){
	try {
	    ClsFESystemManager::Instance()->clsFESystemBuilder->importProcess(qstrFileName.latin1(), true, bLink);
	}
	catch (ClsSysFileParserException &e){
	    cout << e.getMessage() << endl;
	    string strError = (string)" in system file parser\n" + e.getMessage() ;
	    ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	    return;
	}
	catch (ClsFESystemBuilderException &e){
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
 
