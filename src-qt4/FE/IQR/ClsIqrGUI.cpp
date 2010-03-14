#include <stdlib.h>
#include <iostream>
#include <sstream>


#include "ClsFESettings.h"
using iqrfe::saveSettings;

#include "ClsIqrGUI.h"

#include "ClsCanvasNode.h"
#include "ClsFECommHandlerLocal.h"
#include "ClsFEComputeEngine.h"
#include "ClsFEDataClient.h"
#include "ClsFEDataManager.h"
#include "ClsFEDiagramManager.h"
#include "ClsFEDialogManager.h"
#include "ClsFESerializer.h"
#include "ClsFESettings.h"
#include "ClsFESystemManager.h"
#include "ClsQCPSCustomEvent.h"
#include "ClsQLogWindow.h"
#include "ClsQSystemBrowser.h"
#include "ClsSysFileParser.h"
#include <ClsFEDialogManager.h>
#include <ClsFESystemBuilder.h>
#include <ClsFESystemManager.h>
#include <ClsSysFileParserException.h>


#include "tagLibrary.hpp"

namespace {
#include <stop2_13x13.xpm>
#include <run2_13x13.xpm>
#include <icon_timeplot_14x14.xpm>
#include <icon_groupplot_14x14.xpm>
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
#ifdef USE_MEMARRAY_LOGO
#include "/home/ulysses/Code/iqr421_Redesign/FE/icons/run_animation/iqr_2.26x22.cpp"
#endif

}

using namespace iqrfe;


//#define DEBUG_CLSIQRGUI

#include <valarray>

ClsIqrGUI::ClsIqrGUI( QWidget* parent, const char* name, WFlags f) : QMainWindow( parent, name, f ) {

    iClipboardType = -99;
    iActiveItemType = -99;
    strActiveItemID = "";
    strSystemFileName = "";

    this->setIcon(QPixmap(SimControl_2));
    this->setCaption("iqr");

    qsplitter = new QSplitter(this);
    setCentralWidget(qsplitter);

    ClsFESystemManager::initializeSystemManager();
    ClsQLogWindow::initializeLogger(this);


    clsFESerializer = new ClsFESerializer();

    ClsFEDialogManager::initializeDialogManager(this); 
    ClsFEDiagramManager::initializeDiagramManager(qsplitter); 
    ClsFEDataManager::initializeDataManager(this, "", &qmutexSysGUI); 
    

    clsFECommHandler = new ClsFECommHandlerLocal();
    clsQSystemBrowser = new ClsQSystemBrowser(qsplitter);


    ClsFEComputeEngine::initializeComputeEngine(this, &qmutexSysGUI, &qWaitCondition); 


/* general signaling */
    connect( QApplication::clipboard(), SIGNAL(dataChanged ()), this, SLOT(slotClipboardChanged()));

    connect(clsQSystemBrowser, SIGNAL(sigDataDisplay(int, string, string, string)), ClsFEDataManager::Instance(), SLOT(DataClientCreate(int, string, string, string )));
    connect(clsQSystemBrowser, SIGNAL(sysbrowserRightClick(int ,string,  QPoint)), ClsFEDialogManager::Instance(), SLOT(createPopup(int , string, QPoint)));
    connect(clsQSystemBrowser, SIGNAL(sigShowBlockDiagram(string)), ClsFEDiagramManager::Instance(), SLOT(slotBlockDiagramShow(string)));
    connect(ClsFEDiagramManager::Instance(), SIGNAL(sigItemActivated(int,string)), this, SLOT(slotItemActivated(int, string)));
/* ------------------------ */

/* -------------- */

/* signaling from the SystemManager */
    connect(ClsFESystemManager::Instance(), SIGNAL(systemBuildt()), this, SLOT(slotSystemBuildt()));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigAdvanceProgress()), this, SLOT(advanceProgress()));

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemAdded(int, string) ),     ClsFEDiagramManager::Instance(), SLOT(slotItemAdded(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(int, string) ),   ClsFEDiagramManager::Instance(), SLOT(slotItemChanged(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ),   ClsFEDiagramManager::Instance(), SLOT(slotItemDeleted(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemUnDeleted(int, string) ), ClsFEDiagramManager::Instance(), SLOT(slotItemUnDeleted(int, string) ));


    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemAdded(int, string) ),   clsQSystemBrowser, SLOT(slotItemAdded(int, string) ));

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(int, string) ), ClsFEDataManager::Instance(), SLOT(slotItemChanged(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ), ClsFEDataManager::Instance(), SLOT(slotItemDeleted(int, string) ));

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ), ClsFEDialogManager::Instance(), SLOT(slotItemDeleted(int, string) ));


    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(int, string) ), clsQSystemBrowser, SLOT(slotItemChanged(int, string) ));
    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ), clsQSystemBrowser, SLOT(slotItemDeleted(int, string) ));
/* ------------------------ */

    connect(this, SIGNAL(sigSimulationRunning(bool)), ClsFESystemManager::Instance(), SLOT(slotSimulationRunning(bool)));
    connect(this, SIGNAL(sigSimulationRunning(bool)), ClsFEDialogManager::Instance(), SLOT(slotSimulationRunning(bool)));

/* ------------------------ */

    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemUnDeleted(int, string) ), clsQSystemBrowser, SLOT(slotItemUnDeleted(int, string) ));


    createMenubar();
    createToolbar();

    lblStatusBar = new QLabel( statusBar() );
    statusBar()->addWidget( lblStatusBar, 2, TRUE );
    lblStatusBar->setText("Initializing System");

    progress = new QProgressBar( statusBar() );
    statusBar()->addWidget( progress, 1, TRUE );


    createAnimation();

//    cout << "AutoSave interval: " << ClsFESettings::instance().getAutoSaveInterval() << endl;
    if(ClsFESettings::instance().getAutoSaveInterval()>0){
	int iAutoSaveInterval = ClsFESettings::instance().getAutoSaveInterval();
	qtimerAutoSave = new QTimer( this );
	connect( qtimerAutoSave, SIGNAL(timeout()), this, SLOT(slotAutoSaveSystem()) );
	qtimerAutoSave->start(iAutoSaveInterval*60000, FALSE);
    }

}


ClsIqrGUI::~ClsIqrGUI() {
//     delete clsFESystemManager;
//     delete ClsFEDialogManager::Instance();
}


void ClsIqrGUI::createMenubar() {
    /* FILE */
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
    file->insertItem( "&Export for AER", this, SLOT(slotExportSystemForAER()), CTRL+Key_E, FILE_EXPORT_FOR_AER);
    file->insertSeparator();
    file->insertItem(QPixmap( systemproperties ), "System Properties", this, SLOT( slotSystemProperties() ), 0, FILE_SYSTEM_PROPERTIES);
    file->insertSeparator();
    file->insertItem( QPixmap( quit ), "&Quit iqr", this, SLOT( slotQuitIQR() ), 0, FILE_QUIT);
    file->insertSeparator();

    list<string> lstLFO = ClsFESettings::instance().getListLastFiles();
    while (lstLFO.size()) {
	string strFilename = lstLFO.front();
	int iID = file->insertItem( strFilename.c_str(), this, SLOT( slotOpenOldFile(int)));
	file->setItemParameter( iID, iID );
	lstLFO.pop_front();
    }

    file->setItemEnabled (FILE_SAVE_SYSTEM,       false);

    /* EDIT */
    edit = new QPopupMenu( this );
    menuBar()->insertItem( "&Edit", edit );
    edit->insertItem(QPixmap(paste),  "Paste", this, SLOT( slotPaste() ), 0, ClsIqrGUI::ITEM_PASTE);
    edit->insertSeparator();
    edit->insertItem( "Settings", this, SLOT( slotSettings() ));
    edit->insertItem( "Validate System", this, SLOT( slotValidateSystem() ));


    /* VIEW */
    view = new QPopupMenu( this );
    menuBar()->insertItem( "&View", view );
    view->insertItem(  "&Log", this, SLOT(slotShowLogWindow()), CTRL+Key_L );


    /* DIAGRAM */
    QPopupMenu * qpmDiagram = new QPopupMenu( this );
    menuBar()->insertItem( "&Diagram", qpmDiagram );
    qpmDiagram-> insertTearOffHandle();
    qpmDiagram->insertItem("Save Diagram", this, SLOT(slotSaveDiagram()));
    qpmDiagram->insertItem("Print Diagram", this, SLOT(slotPrintDiagram()));


    /* DATA */
    QPopupMenu * DAV = new QPopupMenu( this );
    menuBar()->insertItem( "D&ata", DAV );
    DAV-> insertTearOffHandle();
    DAV->insertItem( "Data Sampler", MENU_DATA_SAMPLER, -1 );
    DAV->insertSeparator();
    DAV->insertItem( "Open Configuration", ClsFEDataManager::Instance(), SLOT( applyConfig() ));
    DAV->insertItem( "&Save Configuration", ClsFEDataManager::Instance(), SLOT( saveConfig() ));

    connect( DAV, SIGNAL(activated(int)), SLOT(slotDAVMenu(int)) );
}


void ClsIqrGUI::createToolbar() {
    qtoolbarToolbar = new QToolBar(this, "toolbar");
    qtoolbarToolbar->setHorizontallyStretchable (true);
    qtoolbarToolbar->setFixedExtentHeight (28);


    QToolButton *qtoolbtnFileNew = new QToolButton( QIconSet(QPixmap( filenew )), "New system", QString::null,
						    this, SLOT(slotNewSystem()), qtoolbarToolbar, "new system" );

    QToolButton *qtoolbtnFileOpen = new QToolButton( QIconSet(QPixmap( fileopen )), "Open ", QString::null,
						     this, SLOT(slotOpenSystem()), qtoolbarToolbar, "open system" );

    qtoolbtnFileSave = new QToolButton( QIconSet(QPixmap( filesave )), "Save system", QString::null,
					this, SLOT(slotSaveSystem()), qtoolbarToolbar, "save system" );
    qtoolbarToolbar->addSeparator ();

    qtoolbtnRunControl = new QToolButton (qtoolbarToolbar, "run_control") ;
    qtoolbtnRunControl->setPixmap(QPixmap( run ));
    qtoolbtnRunControl->setToggleButton(true);
    qtoolbtnRunControl->setEnabled(false);
    qtoolbtnRunControl->setAutoRaise (true );
    connect(qtoolbtnRunControl,SIGNAL(clicked()), SLOT(slotRunControl()));

}

void ClsIqrGUI::createAnimation() {
    QFrame *qframeSpace = new QFrame(qtoolbarToolbar);
    qtoolbarToolbar->setStretchableWidget ( qframeSpace );


    QFrame *qframeAnimation = new QFrame(qtoolbarToolbar);
    QPalette palette;
    palette.setColor(qframeAnimation->backgroundRole(), QColor(255,0,0));
    qframeAnimation->setPalette(palette);
    

    qframeAnimation->setFixedWidth(30);
    qlblStatus = new QLabel(qframeAnimation, "animation");

#ifdef USE_MEMARRAY_LOGO
    QByteArray qbaMoview;
    qbaMoview.setRawData( movie_data, sizeof(movie_data) );
    qlblStatus->setMovie(QMovie(qbaMoview));
#else
    QString filename = "/usr/lib/iqr/iqr-logo.gif";
    if(QFile::exists(filename)){
	qlblStatus->setMovie(QMovie(filename));
    }
#endif

    if(qlblStatus->movie()!=NULL){
	qlblStatus->movie()->pause();
	qlblStatus->movie()->step();
    }
}

void ClsIqrGUI::newSystem() {
//    clsFESystemManager->newSystem();
    ClsFESystemManager::Instance()->newSystem();
};


void ClsIqrGUI::slotShowLogWindow() {
    if(ClsQLogWindow::Instance()->isHidden()){
	ClsQLogWindow::Instance()->show();
    }
};


void ClsIqrGUI::slotSystemProperties() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotSystemProperties()" << endl;
#endif
    if(ClsFEDialogManager::Instance()!=NULL){
	QPoint qp;
	ClsFEDialogManager::Instance()->createPropertyDialog(ClsFESystemManager::ITEM_SYSTEM, ClsFESystemManager::Instance()->getSystemID(), qp);
    }
}

void ClsIqrGUI::openSystem(string s) {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::openSystem(string s)" << endl;
#endif

    if(QDir::isRelativePath(strSystemFileName.c_str())){
	string str(1,QDir::separator());
	strSystemFileName = string(QDir::currentDirPath().latin1()) + str + s;
    } else {
	strSystemFileName = s;
    }

    if(!buildSystem()){
	file->setItemEnabled (ClsIqrGUI::FILE_CLOSE_SYSTEM,      true);
	file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM,       true);
	file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM_AS,    true);
	file->setItemEnabled (ClsIqrGUI::FILE_SYSTEM_PROPERTIES, true);
	qtoolbtnFileSave->setDisabled(false);
    }
}

void ClsIqrGUI::slotOpenSystem(){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotOpenSystem()" << endl;
#endif

    QString qstrFileName = QFileDialog::getOpenFileName(
	"",
	"Systemfile (*.xml *.iqr *)",
	this,
	"Open File"
	"Choose a file" );

    if(qstrFileName.length()>0){
	if( slotCloseSystem()){
	    strSystemFileName = qstrFileName.latin1();
	    if(!buildSystem()){
		file->setItemEnabled (ClsIqrGUI::FILE_CLOSE_SYSTEM,      true);
		file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM,       true);
		file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM_AS,    true);
		file->setItemEnabled (ClsIqrGUI::FILE_SYSTEM_PROPERTIES, true);
		qtoolbtnFileSave->setDisabled(false);
		ClsFESettings::instance().addFileOpened(strSystemFileName);
		int iID = file->insertItem( strSystemFileName.c_str(), this, SLOT( slotOpenOldFile(int)));
		file->setItemParameter( iID, iID );
		this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
	    }
	}
    }
}


void ClsIqrGUI::openConfig(string strConfigFileName){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::openConfig(string strConfigFileName)" << endl;
#endif
    if(ClsFEDataManager::Instance()!=NULL){
	ClsFEDataManager::Instance()->applyConfig(strConfigFileName) ;
    }
}


void ClsIqrGUI::slotOpenOldFile(int ii) {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotOpenOldFile()" << endl;
    cout << "ii: " << ii  << endl;
#endif
    if(slotCloseSystem()){

	QString qstrFileName =  file->text(ii);
	if(qstrFileName.length()>0){
	    strSystemFileName = qstrFileName.latin1();
	    if(!buildSystem()){
		file->setItemEnabled (ClsIqrGUI::FILE_CLOSE_SYSTEM,      true);
		file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM,       true);
		file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM_AS,    true);
		file->setItemEnabled (ClsIqrGUI::FILE_SYSTEM_PROPERTIES, true);
		qtoolbtnFileSave->setDisabled(false);
		this->setCaption(QString("iqr: ") + QString(strSystemFileName.c_str()));
	    }
	}
    }
}

bool ClsIqrGUI::buildSystem() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::buildSystem()" << endl;
#endif
    ClsFESystemManager::Instance()->clsFESystemBuilder->readSystemFileFromDisk(strSystemFileName);
    try {
	ClsFESystemManager::Instance()->clsFESystemBuilder->parseSystemFile( );
    }
    catch (ClsSysFileParserException &e){
	cerr << e.getMessage() << endl;
	string strError = (string)" in system file parser\n" + e.getMessage() ;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
//	reportError(strError);
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
	return true;
    }
#ifdef DEBUG_CLSIQRGUI
    cout << "done building system" << endl;
#endif
    return false;

};

void ClsIqrGUI::slotSaveSystem() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotSaveSystem()" << endl;
#endif

    /* we need the abs path here */
    if(strSystemFileName.length()>0){
//	cout << "abs path: " << QDir::absFilePath(strSystemFileName.c_str()) << endl;
	if(clsFESerializer!=NULL){
	    string strValidationResult = ClsFESystemManager::Instance()->validateConnections();
	    if(strValidationResult.size()>0){
		strValidationResult.append("\nThis might make the file unloadable");
		ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_WARNING, strValidationResult);
	    }
	    clsFESerializer->SaveSystem(strSystemFileName, false, false);
	    ClsFESettings::instance().addFileOpened(strSystemFileName);
	    ClsFESystemManager::Instance()->systemSaved();
	}
    }
    else {
	slotSaveSystemAs();
    }
}


void ClsIqrGUI::slotSaveSystemAs() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotSaveSystemAs()" << endl;
#endif

    string strValidationResult = ClsFESystemManager::Instance()->validateConnections();
    if(strValidationResult.size()>0){
	strValidationResult.append("\nThis might make the file unloadable");
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_WARNING, strValidationResult);
    }

    QString filename = "";
    bool bValidFile = false;
    while(!bValidFile){
	filename = QFileDialog::getSaveFileName(
	    QDir::homeDirPath (), 
	    "Systemfile (*.xml *.iqr *)",
	    this,
	    "Save File"
	    "Choose a filename to save under" );


	QFileInfo qFileInfo(filename );
	QString qstrPath = qFileInfo.dirPath(true);
	qFileInfo.setFile (qstrPath);
	cout << "qFileInfo.isWritable(): " << qFileInfo.isWritable() << endl;
	
	if ( !qFileInfo.isWritable() ) {
	    int iReturn = QMessageBox::critical( this, "iqr",
					    "No permission to write in this directory\n",
					    "Retry", "Cancel", 0, 0, 1 );
	    if(iReturn == 1){
		return;
	    }
	} else {
	    bValidFile = true;
	}

    }

    /* this check should not be necessay, because the dialog should always return a name... */
    if(filename.length()<=0){
	return;
    }


    if ( QFile::exists(filename) ) {
/*      warning on overwrite */
	int iReturn = QMessageBox::warning( this, "iqr",
					"A file with the seleteced name exists\n",
					"Overwrite", "Cancel", 0, 0, 1 );
	if(iReturn == 1){
#ifdef DEBUG_CLSIQRGUI
	    cout << "saving aborted" << endl;
#endif
	    return;
	}
    }

    if(clsFESerializer!=NULL){
	string strFilename = filename.latin1();
	strSystemFileName = strFilename;
	file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM, true);
	ClsFESettings::instance().addFileOpened(strFilename);
	clsFESerializer->SaveSystem(strFilename, false, false);
	ClsFESystemManager::Instance()->systemSaved();
    }
}

void ClsIqrGUI::slotDebugSaveSystem() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotDebugSaveSystem()" << endl;
#endif
    if(clsFESerializer!=NULL){
	clsFESerializer->SaveSystem("/home/ulysses/Code/iqr421_Redesign/CodeExec/WriteTest.xml", false, false);
    }
}

void ClsIqrGUI::slotAutoSaveSystem() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotAutoSaveSystem()" << endl;
#endif

    if(strSystemFileName.length()>0){
	string strAutoSaveFilename = strSystemFileName;
	strAutoSaveFilename.append(",autosave");
	if(clsFESerializer!=NULL){
	    clsFESerializer->SaveSystem(strAutoSaveFilename, false, false);
	}
    }
}

void ClsIqrGUI::slotExportSystemForAER(){
    QString filename;
    
    filename = QFileDialog::getSaveFileName(
	"",
	"XML file (*.xml)",
	this,
	"Export File"
	"Choose a filename to export to" );

    if(filename.length()<=0){
	return;
    }


    if ( QFile::exists(filename) ) {
	int iReturn;
	iReturn = QMessageBox::warning( this, "iqr",
					"A file with the seleteced name exists\n",
					"Overwrite", "Cancel", 0, 0, 1 );
	if(iReturn == 1){
	    return;
	}
    }
    if(clsFESerializer!=NULL){
	clsFESerializer->serializeToAER(filename.latin1());
    }
}

void ClsIqrGUI::slotSettings() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotSettings()" << endl;
#endif
    if(ClsFEDialogManager::Instance()!=NULL){
	ClsFEDialogManager::Instance()->createSettingsDialog();
    }
}


void ClsIqrGUI::advanceProgress(){
    int p = progress->progress();
    progress->setProgress( ++p );
}


void ClsIqrGUI::customEvent( QCustomEvent* e ) {
//    cout << "ClsIqrGUI::customEvent( QCustomEvent* e )" << endl;
    if (dynamic_cast<ClsQCPSCustomEvent*>(e)){
	double fCPS = dynamic_cast<ClsQCPSCustomEvent*>(e)->getCPS();
	QString qstrCPS;
	qstrCPS.sprintf("CPS: %.2f", fCPS);
	lblStatusBar->setText(qstrCPS);
    }
}

void ClsIqrGUI::slotRunControl(){
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

void ClsIqrGUI::autoStartSimulation(){
    qtoolbtnRunControl->setOn(true);
    startSimulation();
}

int ClsIqrGUI::startSimulation() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::startSimulation()" << endl;
#endif
    int iError = 0;

/*
  QProgressDialog progress( "Copying files...", "Abort Copy", numFiles,
  this, "progress", TRUE );
*/

    if(ClsFESystemManager::Instance()->isValidSystem(false)){
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
	    ClsFEComputeEngine::Instance()->prepare(); 
	    ClsFEDataManager::Instance()->start();
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
	       file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM,       false);
	       file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM_AS,    false);
	       ----------------------------------- */


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

void ClsIqrGUI::stopSimulation() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::stopSimulation()" << endl;
#endif

    ClsFEComputeEngine::Instance()->stop(); 
    cout << "ClsIqrGUI::waiting..." << endl;
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
       file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM,       true);
       file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM_AS,    true);
       ----------------------------------- */
    emit sigSimulationRunning(false);
}


void ClsIqrGUI::slotNewSystem(){
    if( slotCloseSystem()){
	newSystem();
	file->setItemEnabled (ClsIqrGUI::FILE_CLOSE_SYSTEM,      true);
	file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM,       false);
	file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM_AS,    true);
	file->setItemEnabled (ClsIqrGUI::FILE_SYSTEM_PROPERTIES, true);
	qtoolbtnFileSave->setDisabled(false);
    }
}


void ClsIqrGUI::slotSystemBuildt(){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotSystemBuildt()" << endl;
#endif
    ClsFEDiagramManager::Instance()->DiagramCreate(diagramTypes::DIAGRAM_BLOCK);
    ClsFEDiagramManager::Instance()->slotSystemChanged();
    clsQSystemBrowser->slotSystemChanged();
    qtoolbtnRunControl->setEnabled(true);
};


bool ClsIqrGUI::slotCloseSystem() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotCloseSystem()" << endl;
#endif

    int iReturn = 1;

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

	qtoolbtnRunControl->setEnabled(false);

	file->setItemEnabled (ClsIqrGUI::FILE_CLOSE_SYSTEM,      false);
	file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM,       false);
	file->setItemEnabled (ClsIqrGUI::FILE_SAVE_SYSTEM_AS,    false);
	file->setItemEnabled (ClsIqrGUI::FILE_SYSTEM_PROPERTIES, false);
	qtoolbtnFileSave->setDisabled(true);
	strSystemFileName = "";
    }

    return true;
}


bool ClsIqrGUI::slotQuitIQR() {
//    cout << "ClsIqrGUI::slotQuitIQR()" << endl;
    if( slotCloseSystem()){
	saveSettings();
	qApp->quit();
	return true;
    }
    return false;
}


void ClsIqrGUI::slotSaveDiagram(){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotSaveDiagram()" << endl;
#endif
    ClsFEDiagramManager::Instance()->saveBlockDiagram();
};

void ClsIqrGUI::slotPrintDiagram(){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotPrintDiagram()" << endl;
#endif
    ClsFEDiagramManager::Instance()->printBlockDiagram();
};


void ClsIqrGUI::slotDAVMenu(int iMenuID){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotDAVMenu(int iMenuID)" << endl;
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
    default:
	break;
    }
};


void ClsIqrGUI::copyItemsToClipboard(int iType, list<string> lstIDs){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::copyItemsToClipboard(int iType, list<string> lstIDs)" << endl;
#endif

    if(ClsFESystemManager::Instance()->isValidSystem(false)){

	QClipboard *cb = QApplication::clipboard();
	QString qstr = "";
	
	if(iType == ClsFESystemManager::ITEM_PROCESS){
	    qstr.append(clsFESerializer->getProcessesAsXML(lstIDs).c_str());
	}
	else if(iType == ClsFESystemManager::ITEM_GROUP){
	    qstr.append(clsFESerializer->getGroupsAsXML(lstIDs).c_str());
	}
	else if(iType == ClsFESystemManager::ITEM_CONNECTION){
	    qstr.append(clsFESerializer->getConnectionsAsXML(lstIDs).c_str());
	}
	
	cb->setText ( qstr, QClipboard::Clipboard) ;
    }
};

void ClsIqrGUI::cutItemsToClipboard(int /* iType */, list<string> /* lstIDs */){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::cutItemsToClipboard(int iType, list<string> lstIDs)" << endl;
#endif

};

void ClsIqrGUI::pasteItemsFromClipboard(int /* iType */, list<string> /* lstIDs */){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::pasteItemsFromClipboard(int iType, list<string> lstIDs)" << endl;
#endif

};

void ClsIqrGUI::slotPaste() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotPaste()" << endl;
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

void ClsIqrGUI::slotItemActivated(int iType, string strID) {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotItemActivated(int,string)" << endl;
    cout << "Type: " << iType << endl;
    cout << "ID: " << strID << endl;
#endif
    iActiveItemType = iType;
    strActiveItemID = strID;
    changeState();
}


void ClsIqrGUI::slotClipboardChanged() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotClipboardChanged()" << endl;
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
	} else {
	    iClipboardType = -99;
	}
    } else {
//	cout << "no match" << endl;
	iClipboardType = -99;
    }

    changeState();
}


void ClsIqrGUI::changeState() {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::changeState()" << endl;
#endif
    if(iActiveItemType == ClsFESystemManager::ITEM_SYSTEM){
	if(iClipboardType == ClsFESystemManager::ITEM_PROCESS){
	    edit->changeItem(ClsIqrGUI::ITEM_PASTE, QString("Paste Process(es)"));
	    edit->setItemEnabled (ClsIqrGUI::ITEM_PASTE, true );
	}
	else {
	    edit->changeItem(ClsIqrGUI::ITEM_PASTE, QString("Paste "));
	    edit->setItemEnabled (ClsIqrGUI::ITEM_PASTE, false );
	}
    }
    else if(iActiveItemType == ClsFESystemManager::ITEM_PROCESS){
	if(iClipboardType == ClsFESystemManager::ITEM_GROUP){
	    edit->changeItem(ClsIqrGUI::ITEM_PASTE, QString("Paste Group(s)"));
	    edit->setItemEnabled (ClsIqrGUI::ITEM_PASTE, true );
	}
	else if(iClipboardType == ClsFESystemManager::ITEM_CONNECTION){
	    edit->changeItem(ClsIqrGUI::ITEM_PASTE, QString("Paste Connection(s)"));
	    edit->setItemEnabled (ClsIqrGUI::ITEM_PASTE, true );
	}
	else {
	    edit->changeItem(ClsIqrGUI::ITEM_PASTE, QString("Paste "));
	    edit->setItemEnabled (ClsIqrGUI::ITEM_PASTE, false );
	}

    } else {
	edit->changeItem(ClsIqrGUI::ITEM_PASTE, QString("Paste "));
	edit->setItemEnabled (ClsIqrGUI::ITEM_PASTE, false );
    }
}


void ClsIqrGUI::slotValidateSystem(){
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::slotValidateSystem()" << endl;
#endif
    ClsFESystemManager::Instance()->isValidSystem(true);
}

void ClsIqrGUI::closeEvent( QCloseEvent *e) {
#ifdef DEBUG_CLSIQRGUI
    cout << "ClsIqrGUI::closeEvent( QCloseEvent *e)" << endl;
#endif
    if(slotQuitIQR()){
	e->accept();
    }
};


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
