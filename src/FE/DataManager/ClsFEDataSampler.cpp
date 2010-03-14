#include <algorithm>
#include <list>
#include <map>

#include <qdragobject.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qfiledialog.h>
#include <qobjectlist.h>
#include <qlabel.h>
#include <qmessagebox.h>

namespace {
#include <fileopen_16x14.xpm>
#include <icon_datasampler_32x32.xpm>
}

#include "iqrUtils.h"
#include "ClsFEDataSampler.h"
#include <ClsFEComputeEngine.h>
#include <ClsFESystemManager.h>
#include <ClsQDivider.h>
#include <ClsDragDropDeEncoder.h>

//#include "/home/ulysses/Code/iqr421_Redesign/FE/icons/run_animation/Two_bullets_2.cpp"
#include "../icons/run_animation/Two_bullets_2.cpp"

//#define DEBUG_CLSFEDATASAMPLER


ClsQProgress::ClsQProgress(QWidget *parent, int _iGaugeType) : QFrame(parent, "", Qt::WDestructiveClose), iGaugeType(_iGaugeType) {
//**	 qwtThermo = new QwtThermo (this , "speed");
//**	 qwtThermo->setOrientation ( QwtThermo::Horizontal, QwtThermo::Bottom);

    qProgressBar = new QProgressBar (this , "speed");

    qLCD = new QLCDNumber ( this );

    if(iGaugeType == GAUGE_THERMO){
//**	  qwtThermo->show();
	qProgressBar->show();
	qLCD->hide();
    }
    else {
//**	  qwtThermo->hide();
	qProgressBar->hide();
	qLCD->show();
    }
    qLCD->setFont(QFont("Helvetica", 7));
};

void ClsQProgress::setGaugeType(int _iGaugeType){
    iGaugeType = _iGaugeType;
    if(iGaugeType == GAUGE_THERMO){
//**	  qwtThermo->show();
	qProgressBar->show();
	qLCD->hide();
    }
    else {
//**	  qwtThermo->hide();
	qProgressBar->hide();
	qLCD->show();
    }
};

void ClsQProgress::resize(QSize qsize) {
//**     qwtThermo->setFixedSize(qsize);
//     qwtThermo->setFixedWidth(parent->sizeHint().width() - 20);

    qProgressBar->setFixedSize(qsize);


    qLCD->setFixedSize(qsize);
//     qLCD->setFixedWidth(parent->sizeHint().width() - 10);
    this->setFixedSize(qsize);
};

void ClsQProgress::setValue( int ii) {
    if(iGaugeType == GAUGE_THERMO)
//**	  qwtThermo->setValue((double)ii);
	qProgressBar->setProgress(ii);
    else
	qLCD->display(ii);
};

void ClsQProgress::setMaxValue(int iMax){
    if(iGaugeType == GAUGE_THERMO){
//**	  qwtThermo->setRange (0., (double)iMax);
	qProgressBar->setTotalSteps(iMax);
    }
};


/* ******************************************************************************************* */

ClsFEDataSampler::ClsFEDataSampler(ClsFEDataManager *_clsFEDataManager, QMutex *_qmutexSysGUI, string _strDataClientID) : 
    /*QWidget(0,0,Qt::WDestructiveClose),*/ ClsFEDataClient(_clsFEDataManager, _qmutexSysGUI, _strDataClientID) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::ClsFEDataSampler(...)" << endl;
#endif

//    this->setBackgroundColor(QColor(204,204,204));
//--    strTitle = _strDataClientID;
    strTitle = "Data Sampler";
    bAutoStart = false;
    strFileName = "";
    bFirstRound = true;
    bSampling = false;
/* general gui stuff */
    setCaption(strTitle.c_str());
    this->setIcon(QPixmap(icon_datasampler));
    setClientType(CLIENT_DATASAMPLER);
    setAcceptDrops(TRUE);

/* initialize values */
    fSamplingRate = 1;
    iCOUNTING_MODE = COUNTING_CONTINUOUS;
    iTARGET_MODE = TARGET_OVERWRITE;
    iFileSequenceCounter= 0;


    qtimerStop = new QTimer( this );
    connect( qtimerStop, SIGNAL(timeout()), SLOT(slotStopSampling()) );

/* widget setup */
    QLabel *lbl;
    QButton *qrb;

    QGridLayout* gridlayoutControls = new QGridLayout (5, 3, -1, "gridlayoutControls");
//    QGridLayout* gridlayoutControls = new QGridLayout (this,  5, 3, 2, -1, "gridlayoutControls");
//    QGridLayout* gridlayoutControls = new QGridLayout (boxlayoutBase, 5, 3, -1, "gridlayoutControls");
    boxlayoutBase->addLayout(gridlayoutControls, 0, 0);

    /* -------------------------------------- */
    qbtngrpSampling = new QButtonGroup ( 2, Horizontal, "Sampling", this, "" ) ;
    gridlayoutControls->addMultiCellWidget(qbtngrpSampling, 0, 0, 0, 2);
//    connect(qbtngrpSampling, SIGNAL(clicked(int)), this, SLOT(slotSettingsLocking(int)));

/*
    qrb = new QRadioButton ( "Every x Cycle", qbtngrpSampling, "");
    qrb->toggle();
    qbtngrpSampling->insert(qrb, LOCKING_CYCLES );
*/

    lbl = new QLabel ( "Every x Cycle", qbtngrpSampling, "");
//    qrb->toggle();
//    qbtngrpSampling->insert(qlbl, LOCKING_CYCLES );


    qspinSamplePerCycle = new QSpinBox(qbtngrpSampling);
    connect(qspinSamplePerCycle, SIGNAL(valueChanged ( const QString& ) ), this, SLOT(slotSpinLocking (const QString&)));
    qspinSamplePerCycle->setMaxValue(1000000);
    qspinSamplePerCycle->setEnabled(true);
    

    /* -------------------------------------- */

    /* -------------------------------------- */
    qbtngrpAcquisition = new QButtonGroup ( 2, Horizontal, "Acquisition", this, "" ) ;
    gridlayoutControls->addMultiCellWidget(qbtngrpAcquisition, 1, 1, 0, 2);
    connect(qbtngrpAcquisition, SIGNAL(clicked(int)), this, SLOT(slotSettingsCounting(int)));

    qrb = new QRadioButton ( "Continuous", qbtngrpAcquisition, "");
//    qrb->toggle();
    iAmount = 1; // needed for checking on valid entries
    qbtngrpAcquisition->insert(qrb, COUNTING_CONTINUOUS );
    lbl = new QLabel(qbtngrpAcquisition); /* needed as a space filler, so the other elements fall into the right position */

    qrb = new QRadioButton ( "Steps", qbtngrpAcquisition, "");
    qbtngrpAcquisition->insert(qrb, COUNTING_STEPS );

    qspinSteps = new QSpinBox(qbtngrpAcquisition);
    connect(qspinSteps, SIGNAL(valueChanged ( const QString& ) ), this, SLOT(slotSpinCounting (const QString&)));
    qspinSteps->setMaxValue(1000000);
    qspinSteps->setEnabled(false);


/* temporarily disable -> init issue:
   we should not start sampling data unless the compute engine is running -> only after that the state arrays
   will have the right size...

    qrb = new QRadioButton ( "Time (sec)", qbtngrpAcquisition, "");
    qbtngrpAcquisition->insert(qrb, COUNTING_TIME );

    qspinTime = new QSpinBox(qbtngrpAcquisition);
    connect(qspinTime, SIGNAL(valueChanged ( const QString&) ), this, SLOT(slotSpinCounting (const QString&)));
    qspinTime->setMaxValue(1000000);
    qspinTime->setEnabled(false);
*/

    qbtngrpAcquisition->setButton( COUNTING_CONTINUOUS );
    /* -------------------------------------- */

    /* -------------------------------------- */
    qbtngrpSampleTarget = new QButtonGroup ( 2, Horizontal, "Target", this, "" ) ;
    gridlayoutControls->addMultiCellWidget(qbtngrpSampleTarget, 2, 2, 0, 2);
    connect(qbtngrpSampleTarget, SIGNAL(clicked(int)), this, SLOT(slotSettingsTarget(int)));

    lbl = new QLabel(qbtngrpSampleTarget); /* needed as a space filler, so the other elements fall into the right position */
    lbl->setText("Save to:");
    lbl = new QLabel(qbtngrpSampleTarget); /* needed as a space filler, so the other elements fall into the right position */

    leditFileName = new QLineEdit(qbtngrpSampleTarget);
    leditFileName->setReadOnly ( TRUE);
    connect(leditFileName, SIGNAL(textChanged ( const QString& ) ), this, SLOT(slotFilenameChanged (const QString&)));

    QPushButton *qbtnFileName = new QPushButton(qbtngrpSampleTarget);
    connect(qbtnFileName, SIGNAL(clicked()), this, SLOT(slotFileDlg()));
    qbtnFileName->setPixmap(QPixmap(fileopen));
    qbtnFileName->setFlat(true);

    qrb = new QRadioButton ( "Overwrite", qbtngrpSampleTarget, "");
//    qrb->toggle();
    qbtngrpSampleTarget->insert(qrb, TARGET_OVERWRITE );

    lbl = new QLabel(qbtngrpSampleTarget); /* needed as a space filler, so the other elements fall into the right position */

    qrb = new QRadioButton ( "Append", qbtngrpSampleTarget, "");
    qbtngrpSampleTarget->insert(qrb, TARGET_APPEND );

    lbl = new QLabel(qbtngrpSampleTarget); /* needed as a space filler, so the other elements fall into the right position */

    qrb = new QRadioButton ( "Sequence", qbtngrpSampleTarget, "");
    qbtngrpSampleTarget->insert(qrb, TARGET_SEQUENCE );
    qbtngrpSampleTarget->setButton(TARGET_OVERWRITE);


    qbtngrpMiscOptions = new QButtonGroup ( 2, Horizontal, "Misc", this, "" ) ;
    gridlayoutControls->addMultiCellWidget(qbtngrpMiscOptions, 3, 3, 0, 2);
    QCheckBox *qchkAutoStart = new QCheckBox ( "auto start/stop", qbtngrpMiscOptions, "");
    connect(qbtngrpMiscOptions, SIGNAL(clicked(int)), this, SLOT(slotSettingsMisc(int)));
    qbtngrpMiscOptions->insert(qchkAutoStart, OPT_AUTOSTART);

    /* -------------------------------------- */

/* ----------------------- */

/* setting up the runtime tab */

    QLabel *qlblStatus = new QLabel(this, "animation");
    gridlayoutControls->addWidget(qlblStatus, 4, 0);
    QByteArray  qbaMoview;
    qbaMoview.setRawData( movie_data, sizeof(movie_data) );

    qlblStatus->setMovie(QMovie(qbaMoview));

    qlblStatus->setFixedWidth(38);
    qlblStatus->setFixedHeight(19);
    qlblStatus->show();

    qmovieAnimation = qlblStatus->movie();
    qmovieAnimation->pause();
    qmovieAnimation->step();


    qbtnAction = new QPushButton("Sample", this);
    gridlayoutControls->addWidget(qbtnAction, 4, 1);
    connect(qbtnAction,SIGNAL(clicked()), this, SLOT(slotRunControl()));
    qbtnAction->setEnabled(false);

    QPushButton *qpbtnClose = new QPushButton ("Close", this, "Close");
    gridlayoutControls->addWidget(qpbtnClose, 4, 2);
    connect(qpbtnClose, SIGNAL(clicked()), this, SLOT(close()));

    qframeStateVariableDisplays = new QFrame(this);
    boxlayoutStateFrames = new QHBoxLayout(qframeStateVariableDisplays,0,0);
    boxlayoutBase->addWidget(qframeStateVariableDisplays, 0, 2); 

    boxlayoutBase->setResizeMode (QLayout::Fixed);
    QWidget::show();

/* has to be down here, otherwise iqr crashes... */
    qspinSamplePerCycle->setValue(1);

}

string ClsFEDataSampler::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange)" << endl;
    cout << "strRange: " << strRange << endl;
#endif

    /* 2004/11/25:
       prefix the random id by the group name: this should preserve the order in which the data is written to disk */
    string strDisplayID = _clsFEGroup->getGroupID() + ClsIDGenerator::Instance()->Next();//.latin1();
//    string strDisplayID = ClsIDGenerator::Instance()->Next();//.latin1();
//    cout << "@@strDisplayID: " << strDisplayID << endl;

    

    ClsQNeuronStateVariableDisplay * clsQNeuronStateVariableDisplay = new ClsQNeuronStateVariableDisplay(this, strDisplayID, qframeStateVariableDisplays, _clsFEGroup, strRange, true, true, false, true, true);
    connect(clsQNeuronStateVariableDisplay,SIGNAL(addDisplay(string, list<string>, string )),this, SLOT(addStateVariableDisplay(string, list<string>, string)));
    connect(clsQNeuronStateVariableDisplay,SIGNAL(sigClose(string)),this, SLOT(removeStateVariableDisplay(string)));

//     connect(clsQNeuronStateVariableDisplay,SIGNAL(addDataSink(string, string, string, string)), SLOT(addDataSink(string, string, string, string)));

    pair<string, ClsQNeuronStateVariableDisplay*> pairTemp(strDisplayID, clsQNeuronStateVariableDisplay);
    mapStateVariableDisplays.insert(pairTemp);
    boxlayoutStateFrames->addWidget(clsQNeuronStateVariableDisplay);




//--    qspinSamplePerCycle->setValue(10);
//--    qspinSamplePerCycle->setValue((int)fSamplingRate);

//--    if(leditFileName->text().length() > 0 && iAmount > 0 && fSamplingRate > 0){
//--	qbtnAction->setEnabled(true);
//--    }
    checkState();

    return strDisplayID;
};

string ClsFEDataSampler::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string strRange) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string strRange)" << endl;
#endif

    /* 2004/11/25:
       prefix the random id by the group name: this should preserve the order in which the data is written to disk */
    string strDisplayID = _clsFEGroup->getGroupID() + ClsIDGenerator::Instance()->Next();//.latin1();



    ClsQNeuronStateVariableDisplay * clsQNeuronStateVariableDisplay = new ClsQNeuronStateVariableDisplay(this, strDisplayID, qframeStateVariableDisplays, _clsFEGroup, strRange, true, true, false, true, true);
    clsQNeuronStateVariableDisplay->setSelectedStates(lstSelectedStates);
    connect(clsQNeuronStateVariableDisplay,SIGNAL(addDisplay(string, list<string>, string )),this, SLOT(addStateVariableDisplay(string, list<string>, string)));
    connect(clsQNeuronStateVariableDisplay,SIGNAL(sigClose(string)),this, SLOT(removeStateVariableDisplay(string)));

//     connect(clsQNeuronStateVariableDisplay,SIGNAL(addDataSink(string, string, string, string)),  SLOT(addDataSink(string, string, string, string)));

    pair<string, ClsQNeuronStateVariableDisplay*> pairTemp(strDisplayID, clsQNeuronStateVariableDisplay);
    mapStateVariableDisplays.insert(pairTemp);
    boxlayoutStateFrames->addWidget(clsQNeuronStateVariableDisplay);

//--    if(leditFileName->text().length() > 0 && iAmount > 0 && fSamplingRate > 0){
//--	qbtnAction->setEnabled(true);
//--    }
    checkState();
    return strDisplayID;
};

string ClsFEDataSampler::addStateVariableDisplay(string strGroupID, list<string> lstSelectedStates, string strRange) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::addStateVariableDisplay(string strGroupID, list<string> lstSelectedStates, string strRange)" << endl;
#endif
    return addStateVariableDisplay( ClsFESystemManager::Instance()->getFEGroup( strGroupID ), lstSelectedStates, strRange);
};

string ClsFEDataSampler::addStateVariableDisplay(string strGroupID, string strRange){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::addStateVariableDisplay(string strGroupID, string strRange)" << endl;
#endif
    return addStateVariableDisplay(ClsFESystemManager::Instance()->getFEGroup( strGroupID ), strRange);
}

void ClsFEDataSampler::removeStateVariableDisplay(string strDisplayID) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::removeStateVariableDisplay(string strDisplayID)" << endl;
#endif
    if(mapStateVariableDisplays.find(strDisplayID) != mapStateVariableDisplays.end()){
	boxlayoutStateFrames->remove(mapStateVariableDisplays.find(strDisplayID)->second); 
	mapStateVariableDisplays.erase(strDisplayID);
    }

//--    if(mapStateVariableDisplays.size() == 0){
//--	qbtnAction->setEnabled(false);
//--    }
    checkState();
};


void ClsFEDataSampler::init(){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::init()" << endl;
#endif
    map<string, ClsDataSinkCopying* >::iterator it;
    for(it = mapDataSinks.begin(); it!=mapDataSinks.end(); ++it){
	it->second->changeSize();
    }
}


void ClsFEDataSampler::groupDeleted(string strID){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::groupDeleted(string strID)" << endl;
#endif
    map<string, ClsQNeuronStateVariableDisplay*>::iterator it;
    for(it=mapStateVariableDisplays.begin(); it!=mapStateVariableDisplays.end(); it++){
	if(!strID.compare(it->second->getGroupID())){
	    it->second->close();
	}
    }
}



void ClsFEDataSampler::dragEnterEvent(QDragEnterEvent* event) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::dragEnterEvent(QDragEnterEvent* event)" << endl;
#endif
    if(!bSampling){
	event->accept( QTextDrag::canDecode(event) );
    }
}

void ClsFEDataSampler::dropEvent(QDropEvent* event) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::dropEvent(QDropEvent* event)" << endl;
#endif

    QString text;
    if ( QTextDrag::decode(event, text) ) {
	if(event->provides("text/iqr-plot")){

	    dropInfo di = ClsDragDropDeEncoder::decode(text.latin1());
	    unsigned int iItemType = di.ItemType;
	    if(iItemType == ClsFESystemManager::ITEM_GROUP){
		string strGroupID = di.ItemID;
		string strParams = di.Params;
		string strRange = di.Range;
		
//--	    int ii;
//--	    ii = text.find("|", 0);
//--	    string strGroupID = text.mid(0, ii).latin1();
//--	    text = text.mid(ii+1, text.length());
//--	    ii = text.find("|", 0);
//--	    string strParams =  text.mid(0, ii).latin1();
//--	    text = text.mid(ii+1, text.length());
//--	    ii = text.find("|", 0);
//--	    string strRange =   text.mid(0, ii).latin1();
//--
//	       cout << "ClsFEDataSampler::strRange: " << strRange << endl;
		list<string> lst;
		string::size_type pos;
		while((pos = strParams.find(";")) != string::npos){
		    lst.push_back(strParams.substr(0, pos));
		    strParams.erase(0, pos+1);
		}
		
//	       ClsQlstItemGroup* _clsQlstItemGroup = (ClsQlstItemGroup*)(clsFEDataManager->getQBaseItem( strGroupID ));
//	       addStateVariableDisplay(_clsQlstItemGroup, lst, strRange);
		
		addStateVariableDisplay(ClsFESystemManager::Instance()->getFEGroup( strGroupID ), lst, strRange);
	    }
	}
    }
};


void ClsFEDataSampler::slotSettingsCounting( int iBtn ) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotSettingsCounting( int iBtn )" << endl;
#endif
    bool ok;

    iCOUNTING_MODE = iBtn;
    switch  (iBtn){
    case COUNTING_CONTINUOUS:
	iAmount = 1; // needed for checking on valid entries
	qspinSteps->setEnabled(false);
//TEMP	qspinTime->setEnabled(false);
	ok = true;
//--	if( mapStateVariableDisplays.size() > 0){
//--	    qbtnAction->setEnabled(true);
//--	}
	
	break;
    case COUNTING_STEPS:
	qspinSteps->setEnabled(true);
//TEMP	qspinTime->setEnabled(false);
	iAmount = qspinSteps->text().toInt(&ok, 10);

//--	if(ok && iAmount > 0  && mapStateVariableDisplays.size() > 0) {
//--	    qbtnAction->setEnabled(true);
//--	} else{
//--	    qbtnAction->setEnabled(false);
//--	}
	break;
    case COUNTING_TIME:
	qspinSteps->setEnabled(false);
//TEMP	qspinTime->setEnabled(true);
//TEMP	iAmount = qspinTime->text().toInt(&ok, 10);

//--	if(ok && iAmount > 0  && mapStateVariableDisplays.size() > 0) {
//--	    qbtnAction->setEnabled(true);
//--	}else{
//--	    qbtnAction->setEnabled(false);
//--	}

	break;
    }
    checkState(ok);
};

//--void ClsFEDataSampler::slotSettingsLocking( int iBtn ) {
//--//#ifdef DEBUG_CLSFEDATASAMPLER
//--    cout << "ClsFEDataSampler::slotSettingsLocking( int iBtn )" << endl;
//--//#endif
//--    bool ok;
//--
//--    iLOCKING_MODE = iBtn;
//--    switch  (iBtn){
//--    case LOCKING_CYCLES:
//--	qspinSamplePerCycle->setEnabled(true);
//--	fSamplingRate = qspinSamplePerCycle->text().toInt(&ok, 10);
//--	break;
//--    }
//--
//--
//--//--    if(ok && fSamplingRate > 0  && mapStateVariableDisplays.size() > 0 ) {
//--//--	qbtnAction->setEnabled(true);
//--//--    } else{
//--//--	qbtnAction->setEnabled(false);
//--//--    }
//--
//--    checkState(ok);
//--};
//--
void ClsFEDataSampler::slotSettingsTarget (int iBtn){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotSettingsTarget (int iBtn)" << endl;
#endif
    iTARGET_MODE = iBtn;
};

void ClsFEDataSampler::slotSpinCounting (const QString& qstr) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotSpinCounting (const QString& qstr)" << endl;
#endif
    bool ok;
    iAmount = qstr.toInt(&ok, 10);

//--    if(ok && iAmount > 0){
//--	if(leditFileName->text().length() > 0  && mapStateVariableDisplays.size() > 0){
//--	    qbtnAction->setEnabled(true);
//--	}
//--    }
//--    else {
//--	qbtnAction->setEnabled(false);
//--    }

    checkState(ok);

};

void ClsFEDataSampler::slotSpinLocking (const QString& qstr) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotSpinLocking (const QString& qstr)" << endl;
#endif
    bool ok;
    fSamplingRate = qstr.toInt(&ok, 10);

//--     ClsFEComputeEngine::Instance()->setDataSamplerInterval((int)fSamplingRate);

//--    if(ok && fSamplingRate > 0){
//--	if(leditFileName->text().length() > 0  && mapStateVariableDisplays.size() > 0){
//--	    qbtnAction->setEnabled(true);
//--	}
//--    }
//--    else {
//--	qbtnAction->setEnabled(false);
//--    }

    checkState(ok);

};

void ClsFEDataSampler::slotFilenameChanged (const QString& /*qstr*/) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotFilenameChanged (const QString& qstr)" << endl;
#endif
//--    if(qstr.length() > 0 ){
//--	iFileSequenceCounter= 0;
//--	if(iAmount > 0 && fSamplingRate > 0  && mapStateVariableDisplays.size() > 0){
//--	    qbtnAction->setEnabled(true);
//--	}
//--
//--    } else {
//--	qbtnAction->setEnabled(false);
//--    }
    checkState();
};

void ClsFEDataSampler::slotRunControl ( ){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotRunControl ( )" << endl;
#endif
    if(!bSampling){
	slotStartSampling();
    }
    else {
	slotStopSampling();
    }
};

void ClsFEDataSampler::slotStartSampling(){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotStartSampling()" << endl;
    cout << "iCOUNTING_MODE: " << iCOUNTING_MODE << endl;
#endif
    qbtngrpSampling->setEnabled(false);
    qbtngrpAcquisition->setEnabled(false);
    qbtngrpSampleTarget->setEnabled(false);
    qbtngrpMiscOptions->setEnabled(false);


    qbtnAction->setText("Stop");
    qmovieAnimation->unpause();
    qframeStateVariableDisplays->setEnabled(false);
    iStepCounter = 0;
    bSampling = true;
    /* none of the below is thread save!!! */

    if(iCOUNTING_MODE == COUNTING_CONTINUOUS){
#ifdef DEBUG_CLSFEDATASAMPLER
	cout << "iCOUNTING_MODE == COUNTING_CONTINUOUS" << endl;
#endif
	ClsFEComputeEngine::Instance()->setDataSampler(this, (int)fSamplingRate);
    }
    else if(iCOUNTING_MODE == COUNTING_STEPS){
#ifdef DEBUG_CLSFEDATASAMPLER
	cout << "iCOUNTING_MODE == COUNTING_STEPS" << endl;
#endif
	ClsFEComputeEngine::Instance()->setDataSampler(this, (int)fSamplingRate);
    }
    else if(iCOUNTING_MODE == COUNTING_TIME){
#ifdef DEBUG_CLSFEDATASAMPLER
	cout << "iCOUNTING_MODE == COUNTING_TIME" << endl;
#endif
	ClsFEComputeEngine::Instance()->setDataSampler(this, (int)fSamplingRate);
	qtimerStop->start(iAmount*1000, TRUE); 
    }

}

void ClsFEDataSampler::prepareOutStream(){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::prepareOutStream()" << endl;
    cout << "iTARGET_MODE: " << iTARGET_MODE << endl;
#endif

    if(iTARGET_MODE == TARGET_SEQUENCE){
	strFileName = leditFileName->text().latin1() + (string)("_") + iqrUtils::getDateTimeSuffix()  + string(".dat");
//	strFileName = leditFileName->text().latin1() + (string)(".") + iqrUtils::int2string(iFileSequenceCounter);
	iFileSequenceCounter++;
    } else {
	strFileName = leditFileName->text().latin1();
    }

    if(iTARGET_MODE == TARGET_APPEND) {
	ofstreamFile.open(strFileName.c_str(), ios::out | ios::app);
    }
    else {
	ofstreamFile.open(strFileName.c_str(), ios::out);
    }

#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ofstreamFile.bad(): " << ofstreamFile.bad() << endl;
    cout << "ofstreamFile.is_open(): " << ofstreamFile.is_open() << endl;
//    cout << "ClsFEDataSampler::mapDataSinks.size(): " << mapDataSinks.size() << endl << endl;
#endif

    if(mapDataSinks.size()>0){
	map<string, ClsDataSinkCopying* >::iterator it;
	/* write  header: ID to name association */
	for(it = mapDataSinks.begin(); it!=mapDataSinks.end(); ++it){
	    string strGroupID = it->second->getItemID();
	    string strGroupName = ClsFESystemManager::Instance()->getGroupName(strGroupID);
	    ofstreamFile << "# \"" << strGroupName << "\"=" << strGroupID << endl;
	}

	ofstreamFile << "cycle;";


//	map<string, ClsBaseDataSink* >::iterator it;
	for(it = mapDataSinks.begin(); it!=mapDataSinks.end(); ++it){
	    string strHead = it->second->getItemID() +  "_" + it->second->getParamName() + "_";
//	    cout << "strHead: " << strHead << endl;
	    /* we're only saving the average of this datasink */
	    if(it->second->getMaths() == ClsBaseDataSink::MATH_AVERAGE){
//		cout << "ClsBaseDataSink::MATH_AVERAGE" << endl;
		if((it->second->getRange()).size()>0){
		    ofstreamFile << strHead << "RangeAverage;";
		}
		else {
		    ofstreamFile << strHead << "GroupAverage;";
		}
	    }


	    /* no average: get the indices of all the cells to save */
	    else {
//		cout << "No Maths" << endl;
		if((it->second->getRange()).size()>0){
//		    cout << "/* if a range if defined, we'll get all the indices out */" << endl;
		    string::size_type pos;
		    string strRange = it->second->getRange();
		    while((pos = strRange.find(";")) != string::npos){
			ofstreamFile << strHead << strRange.substr(0, pos) << ";";
			strRange.erase(0, pos+1);
		    }
		}
		else {
//		    cout << "/* if there's no range, we'll just do all the cell, i.e. loop over the size of the group */" << endl;
//		    cout << "it->second->size(): " << it->second->size() << endl;
		    for(unsigned int ii=0; ii< it->second->size(); ii++){
				   ofstreamFile << strHead << ii << ";";
		    }
		}
	    }
	}
	ofstreamFile << endl;
    }
//    cout << "exit:ClsFEDataSampler::prepareOutStream()" << endl;
}

void ClsFEDataSampler::slotStopSampling(){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotStopSampling()" << endl;
#endif


    ClsFEComputeEngine::Instance()->setDataSampler(NULL, -1/*, 0*/); 

    ofstreamFile.flush();
    ofstreamFile.close();
    ofstreamFile.clear();
    bSampling = false;
    qbtngrpSampling->setEnabled(true);
    qbtngrpAcquisition->setEnabled(true);
    qbtngrpSampleTarget->setEnabled(true);
    qbtngrpMiscOptions->setEnabled(true);


    qbtnAction->setText("Sample");
    qmovieAnimation->pause();
    qframeStateVariableDisplays->setEnabled(true);
    bFirstRound = true;
    iStepCounter = 0;
}

void ClsFEDataSampler::slotFileDlg() {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotFileDlg()" << endl;
#endif
    
    bool bCont = true;
    QString qstrFN;
    while(bCont){
	qstrFN = QFileDialog::getSaveFileName( "data.dat", "All files (*.*)", this ).latin1();
	if(qstrFN.length()>0){
	    ofstream ofstr;
	    ofstr.open(qstrFN.latin1(), ios::out); 
	    if(ofstr.fail()) {
		ofstr.clear(ios::failbit);
		int iReturn = QMessageBox::critical( this, "iqr",
						     "No permission to write file \"" + qstrFN + "\"\n",
						     "Select other file", "Cancel", 0, 0, 1 );
		if(iReturn==1){
		    ofstr.close();
		    return;
		}
	    } else {
		bCont = false;
	    }
	    ofstr.close();
	} else {
	    return;
	}
	
    }

    leditFileName->setText(qstrFN);
    strFileName = qstrFN.latin1();
    slotFilenameChanged (qstrFN);


};

void ClsFEDataSampler::slotHideControls ( bool bIsRight ) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotHideControls ( bool bIsRight )" << endl;
#endif
    if (bIsRight && qframeStateVariableDisplays!=NULL){
	qframeStateVariableDisplays->hide();
    } else{
	qframeStateVariableDisplays->show();
    }
};

void ClsFEDataSampler::slotSettingsMisc(int ii){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotSettingsMisc(int ii)" << endl;
#endif
    if(ii == OPT_AUTOSTART ){
	bAutoStart = (bAutoStart == true ? false : true);
    }
}

void ClsFEDataSampler::customEvent(QCustomEvent *e){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "Event: " << e->type() << endl;
#endif
    if(e->type() == EVENT_SAVEDATA && bSampling){
	saveData();
    } else if(e->type() == EVENT_STOP){
	slotStopSampling();
    }
//	EVENT_SAVEDATA
//	EVENT_STOP

}

//--void ClsFEDataSampler::update() {
//--#ifdef DEBUG_CLSFEDATASAMPLER
//--    cout << "ClsFEDataSampler::update()" << endl;
//--#endif
//-- 
//--    if(iCOUNTING_MODE == COUNTING_STEPS){
//--	if(iStepCounter >= iAmount){
//--	    slotStopSampling();
//--	    return;
//--	}
//--    }
//--    saveData();
//--/*
//--  cout << "ClsFEDataSampler::update()" << endl;
//--  cout << "iStepCounter: " << iStepCounter << endl;
//--  cout << "iAmount: " << iAmount << endl;
//--  cout << "fSamplingRate: " << fSamplingRate << endl;
//--  cout << "mapSinks.size(): " << mapDataSinks.size() << endl;
//--  cout << "=======================" << endl;
//--*/
//--};
//--
inline void ClsFEDataSampler::saveData(  ) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::saveData(  )" << endl;
#endif


    if(bFirstRound){
	init();
	prepareOutStream();
	bFirstRound = false;
    }


    ofstreamFile << iStepCounter*fSamplingRate << ";"; 



#ifdef DEBUG_CLSFEDATASAMPLER
    cout << iStepCounter*fSamplingRate << ";"; 
#endif

//    map<string, ClsBaseDataSink* >::iterator it;
    map<string, ClsDataSinkCopying* >::iterator it;
    qmutexSysGUI->lock();
    for(it = mapDataSinks.begin(); it!=mapDataSinks.end(); ++it){
	it->second->update();
//	cout << "DATASINKS" << endl;
	if(it->second->getMaths() == ClsBaseDataSink::MATH_AVERAGE){
	    ofstreamFile << it->second->average() << ";";
	} else {
//	    ofstreamFile << *(it->second);

	    /* the dynamic casting is here to prevent virtual calls to the [] operator! */
	    /* is this used anymore??? */
	    if(dynamic_cast<ClsDataSinkStateArrayPointer*>(it->second)){
		ofstreamFile << *(dynamic_cast<ClsDataSinkStateArrayPointer*>(it->second));
#ifdef DEBUG_CLSFEDATASAMPLER
		cout << *(dynamic_cast<ClsDataSinkStateArrayPointer*>(it->second));
#endif
	    }
	    /* is this used anymore??? */
	    else if(dynamic_cast<ClsDataSinkStateArrayRange*>(it->second)){
		ofstreamFile << *(dynamic_cast<ClsDataSinkStateArrayRange*>(it->second));
#ifdef DEBUG_CLSFEDATASAMPLER
		cout << *(dynamic_cast<ClsDataSinkStateArrayRange*>(it->second));
#endif
	    }
	    else if(dynamic_cast<ClsDataSinkCopying*>(it->second)){
		ofstreamFile << *(dynamic_cast<ClsDataSinkCopying*>(it->second));
#ifdef DEBUG_CLSFEDATASAMPLER
		cout << *(dynamic_cast<ClsDataSinkCopying*>(it->second));
#endif
	    }
	}
    }
    qmutexSysGUI->unlock(); 
    ofstreamFile << endl;
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << endl;
#endif
    ofstreamFile.flush();

    iStepCounter++;
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "iStepCounter: " << iStepCounter << endl;
#endif
    if(iCOUNTING_MODE == COUNTING_STEPS && iStepCounter>=iAmount){
	slotStopSampling();
    }
}

void ClsFEDataSampler::closeEvent( QCloseEvent* ce ) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::closeEvent( QCloseEvent* ce )" << endl;
#endif


    const QObjectList *qObjectList = qframeStateVariableDisplays->queryList( "ClsQNeuronStateVariableDisplay" );
    QObjectListIt iter( *qObjectList );
    QObject *tmp;
    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;
    while( (tmp = iter.current()) ) {
	++iter;
	if((clsQNeuronStateVariableDisplay = dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp))){
	    dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp)->close();
	}
    }

    ClsFEComputeEngine::Instance()->setDataSampler(NULL, -1/*, 0*/); 
    emit sigPlotClosed(strDataClientID);
    ce->accept();
}

ClsDataClientConfig ClsFEDataSampler::getConfig(){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::getConfig()" << endl;
#endif
    ClsDataClientConfig clsDataClientConfig(this->caption().latin1());
    clsDataClientConfig.setPosition(this->pos().x(), this->pos().y() );
    clsDataClientConfig.setGeometry(this->size().width(),  this->size().height());

    pair<string,string> p01("AutoStart", iqrUtils::bool2string(bAutoStart));
    clsDataClientConfig.addParameter(p01);

    pair<string,string> p03("ACQ_MODE", iqrUtils::int2string(iCOUNTING_MODE));
    clsDataClientConfig.addParameter(p03);

    pair<string,string> p04("TARGET_MODE", iqrUtils::int2string(iTARGET_MODE));
    clsDataClientConfig.addParameter(p04);

    pair<string,string> p05("SamplingRate", iqrUtils::int2string((int)fSamplingRate));
    clsDataClientConfig.addParameter(p05);

    pair<string,string> p06("Amount", iqrUtils::int2string(iAmount));
    clsDataClientConfig.addParameter(p06);

    pair<string,string> p07("FileName", strFileName);
    clsDataClientConfig.addParameter(p07);


    const QObjectList *qObjectList = qframeStateVariableDisplays->queryList( "ClsQNeuronStateVariableDisplay" );
    QObjectListIt iter( *qObjectList );
    QObject *tmp;
    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;
    while( (tmp = iter.current()) ) {
	++iter;
	if((clsQNeuronStateVariableDisplay = dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp))){
	    ClsStateVariableDisplayConfig clsStateVariableDisplayConfig = clsQNeuronStateVariableDisplay->getConfig();
	    clsDataClientConfig.addStateVariableDisplayConfig(clsStateVariableDisplayConfig);
	}
    }

    return clsDataClientConfig;
};

void ClsFEDataSampler::setConfig(ClsDataClientConfig clsDataClientConfig){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::setConfig(ClsDataClientConfig clsDataClientConfig)" << endl;
#endif

    list<pair<string, string> > lstParameters = clsDataClientConfig.getListParameters();
    list<pair<string, string> >::iterator itLstParameters;
    for(itLstParameters = lstParameters.begin(); itLstParameters != lstParameters.end(); itLstParameters++){
	string strParamName = (*itLstParameters).first;
	string strParamValue = (*itLstParameters).second;

	if(!strParamName.compare("AutoStart")){
	    setSettingAutoStart(iqrUtils::string2bool(strParamValue));
	}
//--	else if(!strParamName.compare("SAMPLE_MODE")){
//--	    setSettingsSampling(iqrUtils::string2int(strParamValue));
//--	}
	else if(!strParamName.compare("ACQ_MODE")){
	    setSettingsAcquistion(iqrUtils::string2int(strParamValue));
	}
	else if(!strParamName.compare("TARGET_MODE")){
	    setSettingTarget(iqrUtils::string2int(strParamValue));
	}
	else if(!strParamName.compare("SamplingRate")){
	    setSampling(iqrUtils::string2int(strParamValue));
	}
	else if(!strParamName.compare("Amount")){
	    setAcquisition (iqrUtils::string2int(strParamValue));
	}
	else if(!strParamName.compare("FileName")){
	    setFileName(strParamValue);
	}
    }

    list<ClsStateVariableDisplayConfig>lstSVDConfigs =  clsDataClientConfig.getListStateVariableDisplayConfig();
    list<ClsStateVariableDisplayConfig>::iterator itSVD;
    for(itSVD = lstSVDConfigs.begin(); itSVD != lstSVDConfigs.end(); itSVD++){
	string strSVDID =  (*itSVD).getID();
	string strGroupID = (*itSVD).getItemID();
	string strSelectedIndices = (*itSVD).getSelectedIndices();
/*
  cout << "strSVDID: " << strSVDID << endl;
  cout << "strGroupID: " << strGroupID << endl;
  cout << "strSelectedIndices: " << strSelectedIndices << endl;
*/
	if(ClsFESystemManager::Instance()->getFEGroup( strGroupID )!=NULL){
	    string strDisplayID = addStateVariableDisplay(ClsFESystemManager::Instance()->getFEGroup( strGroupID ), strSelectedIndices);
	    map<string, ClsQNeuronStateVariableDisplay*>::iterator it =  mapStateVariableDisplays.find(strDisplayID);
	    if(it != mapStateVariableDisplays.end()){
		it->second->setConfig((*itSVD));
	    }
	}
    }
};

//--void ClsFEDataSampler::setSettingsSampling( int i ){
//--#ifdef DEBUG_CLSFEDATASAMPLER
//--    cout << "ClsFEDataSampler::setSettingsSampling( int i )" << endl;
//--#endif
//--    qbtngrpSampling->setButton ( i );
//--//    slotSettingsLocking( i );
//--}
//--
void ClsFEDataSampler::setSettingsAcquistion( int i ){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::setSettingsAcquistion( int i )" << endl;
#endif
    iCOUNTING_MODE = i;
    qbtngrpAcquisition->setButton ( i );
    slotSettingsCounting( i );
}

void ClsFEDataSampler::setSettingTarget ( int i ){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::setSettingTarget ( int i )" << endl;
#endif
    iTARGET_MODE = i; 
    qbtngrpSampleTarget->setButton ( i );
    slotSettingsTarget( i );
}

void ClsFEDataSampler::setAcquisition ( int i ){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::setAcquisition ( int i )" << endl;
#endif
    iAmount = i;
    if(iCOUNTING_MODE == COUNTING_STEPS){
	qspinSteps->setValue(i);
    } else if(iCOUNTING_MODE == COUNTING_TIME){
//TEMP	qspinTime->setValue(i);
    }
}

void ClsFEDataSampler::setSampling ( int i ){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::setSampling ( int i )" << endl;
#endif
    fSamplingRate = (double)i;

    qspinSamplePerCycle->setValue(i);

}

void ClsFEDataSampler::setFileName ( string str ){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::setFileName ( string str )" << endl;
#endif
    strFileName = str;
    leditFileName->setText(strFileName.c_str());
}


void ClsFEDataSampler::setSettingAutoStart ( bool b ){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::setSettingAutoStart ( bool b )" << endl;
#endif

    bAutoStart = b;
    QCheckBox* qckb;
    if((qckb = dynamic_cast<QCheckBox*>(qbtngrpMiscOptions->find( OPT_AUTOSTART )))){
	qckb->setChecked (b);
    }
}


void ClsFEDataSampler::groupChanged(string _strGroupID){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::groupChanged(string strGroupID)" << endl;
#endif

    const QObjectList *qObjectList = qframeStateVariableDisplays->queryList( "ClsQNeuronStateVariableDisplay" );
    QObjectListIt iter( *qObjectList );
    QObject *tmp;
    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;
    while( (tmp = iter.current()) ) {
	++iter;
	if((clsQNeuronStateVariableDisplay = dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp))){
	    if(!_strGroupID.compare(clsQNeuronStateVariableDisplay->getGroupID())){
		clsQNeuronStateVariableDisplay->groupChanged();
	    }
	}
    }
    ClsFEDataClient::groupChanged(_strGroupID);
};



void ClsFEDataSampler::checkState(bool ok){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::checkState()" << endl;
#endif
    if(ok && 
       iAmount > 0  && 
       mapStateVariableDisplays.size() > 0 && 
       leditFileName->text().length() > 0 &&
       fSamplingRate > 0) {
	qbtnAction->setEnabled(true);
    }else{
	qbtnAction->setEnabled(false);
    }
}



ostream &operator<<(ostream &stream, ClsFEDataSink &clsDataSink) {
//    cout << "ostream &operator<<(ostream &stream, ClsFEDataSink &clsDataSink)" << endl;
    for(unsigned int ii=0; ii<clsDataSink.size(); ii++){
	stream << clsDataSink[ii] << ";";
    }
    return stream;
};

ostream &operator<<(ostream &stream, ClsDataSinkCopying &clsDataSink) {
//    cout << "ostream &operator<<(ostream &stream, ClsDataSinkCopying &clsDataSink)" << endl;
    for(unsigned int ii=0; ii<clsDataSink.size(); ii++){
	stream << clsDataSink[ii] << ";";
//	stream << ii << ":" << clsDataSink[ii] << ";";
    }
    return stream;
};

ostream &operator<<(ostream &stream,  ClsDataSinkStateArrayPointer &clsDataSink){
//    cout << "ostream &operator<<(ostream &stream,  ClsDataSinkStateArrayPointer &clsDataSink)" << endl;
    for(unsigned int ii=0; ii<clsDataSink.size(); ii++){
	stream << clsDataSink[ii] << ";";
    }
    return stream;
}

ostream &operator<<(ostream &stream, ClsDataSinkStateArrayRange &clsDataSink){
//    cout << "ostream &operator<<(ostream &stream, ClsDataSinkStateArrayRange &clsDataSink)" << endl;
    for(unsigned int ii=0; ii<clsDataSink.size(); ii++){
	stream << clsDataSink[ii] << ";";
    }
    return stream;
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
