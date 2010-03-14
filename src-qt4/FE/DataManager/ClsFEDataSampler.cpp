#include <algorithm>
#include <list>
#include <map>


#include <QRadioButton>
#include <QCheckBox>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QAbstractButton>
#include <QGridLayout>

namespace {
#include <fileOpen.xpm>
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


ClsQProgress::ClsQProgress(QWidget *parent, int _iGaugeType) : QFrame(parent/*, Qt::WDestructiveClose*/), iGaugeType(_iGaugeType) {
    qProgressBar = new QProgressBar (this);

    qLCD = new QLCDNumber ( this );

    if(iGaugeType == GAUGE_THERMO){
	qProgressBar->show();
	qLCD->hide();
    }
    else {
	qProgressBar->hide();
	qLCD->show();
    }
    qLCD->setFont(QFont("Helvetica", 7));
};

void ClsQProgress::setGaugeType(int _iGaugeType){
    iGaugeType = _iGaugeType;
    if(iGaugeType == GAUGE_THERMO){
	qProgressBar->show();
	qLCD->hide();
    }
    else {
	qProgressBar->hide();
	qLCD->show();
    }
};

void ClsQProgress::resize(QSize qsize) {
    qProgressBar->setFixedSize(qsize);


    qLCD->setFixedSize(qsize);
    this->setFixedSize(qsize);
};

void ClsQProgress::setValue( int ii) {
    if(iGaugeType == GAUGE_THERMO)
//zzz	qProgressBar->setProgress(ii);
	qProgressBar->setValue(ii);
    else
	qLCD->display(ii);
};

void ClsQProgress::setMaxValue(int iMax){
    if(iGaugeType == GAUGE_THERMO){
//zzz	qProgressBar->setTotalSteps(iMax);
	qProgressBar->setMaximum(iMax);
    }
};


/* ******************************************************************************************* */

ClsFEDataSampler::ClsFEDataSampler(ClsFEDataManager *_clsFEDataManager, QMutex *_qmutexSysGUI, string _strDataClientID) : 
    /*QWidget(0,0,Qt::WDestructiveClose),*/ ClsFEDataClient(_clsFEDataManager, _qmutexSysGUI, _strDataClientID) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::ClsFEDataSampler(...)" << endl;
#endif

    strTitle = "Data Sampler";
    bAutoStart = false;
    strFileName = "";
    bFirstRound = true;
    bSampling = false;
/* general gui stuff */
    setWindowTitle(strTitle.c_str());
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

    QGridLayout* qglayoutControls = new QGridLayout ();
    boxlayoutBase->addLayout(qglayoutControls, 0, 0);

    /* -------------------------------------- */
    qgrpbxSampling = new QGroupBox ( "Sampling", this) ;
    qgrpbxSampling->setFlat(false);
    qglayoutControls->addMultiCellWidget(qgrpbxSampling, 0, 0, 0, 2);
    QLabel *lblExC = new QLabel ( "Every x Cycle");

    qspinSamplePerCycle = new QSpinBox();
    connect(qspinSamplePerCycle, SIGNAL(valueChanged ( const QString& ) ), this, SLOT(slotSpinLocking (const QString&)));
    qspinSamplePerCycle->setMaximum(1000000);
    qspinSamplePerCycle->setEnabled(true);
    
    QHBoxLayout *qlayoutSampling = new QHBoxLayout;
    qlayoutSampling->setSpacing ( 0 );

    qlayoutSampling->addWidget(lblExC);
    qlayoutSampling->addWidget(qspinSamplePerCycle);
    qgrpbxSampling->setLayout(qlayoutSampling);
    /* -------------------------------------- */

    /* -------------------------------------- */
    qgrpbxAcquisition = new QGroupBox ( "Acquisition", this) ;
    qgrpbxAcquisition->setFlat(false);

    qglayoutControls->addMultiCellWidget(qgrpbxAcquisition, 1, 1, 0, 2);


    QRadioButton* qrbContinuous = new QRadioButton ( "Continuous");
    iAmount = 1; // needed for checking on valid entries

    QRadioButton* qrbSteps = new QRadioButton ( "Steps");

    qbtngrpAcquisition = new QButtonGroup();
    connect(qbtngrpAcquisition, SIGNAL(buttonClicked(int)), this, SLOT(slotSettingsCounting(int)));
    qbtngrpAcquisition->addButton(qrbContinuous, COUNTING_CONTINUOUS );
    qbtngrpAcquisition->addButton(qrbSteps, COUNTING_STEPS );
    


    qspinSteps = new QSpinBox();
    connect(qspinSteps, SIGNAL(valueChanged ( const QString& ) ), this, SLOT(slotSpinCounting (const QString&)));
    qspinSteps->setMaximum(1000000);
    qspinSteps->setEnabled(false);

    qrbContinuous->setChecked(true);

    QGridLayout *qlayoutAcquisition = new QGridLayout;
    qlayoutAcquisition->setVerticalSpacing ( 0 );
    qlayoutAcquisition->addWidget(qrbContinuous,1,1);
    qlayoutAcquisition->addWidget(qrbSteps,2,1);
    qlayoutAcquisition->addWidget(qspinSteps,2,2);
    qgrpbxAcquisition->setLayout(qlayoutAcquisition);
    


    /* -------------------------------------- */

    /* -------------------------------------- */
    qgrpbxSampleTarget = new QGroupBox ( "Target", this) ;
    qgrpbxSampleTarget->setFlat(false);

    qglayoutControls->addMultiCellWidget(qgrpbxSampleTarget, 2, 2, 0, 2);


    QLabel* lblSaveTo = new QLabel(); 
    lblSaveTo->setText("Save to:");

    leditFileName = new QLineEdit();
    leditFileName->setReadOnly ( TRUE);
    connect(leditFileName, SIGNAL(textChanged ( const QString& ) ), this, SLOT(slotFilenameChanged (const QString&)));

    QPushButton *qbtnFileName = new QPushButton();
    connect(qbtnFileName, SIGNAL(clicked()), this, SLOT(slotFileDlg()));
    qbtnFileName->setPixmap(QPixmap(fileOpen_xpm));
    qbtnFileName->setFlat(false);

    QRadioButton* qrbOverwrite = new QRadioButton ( "Overwrite");
    QRadioButton* qrbAppend = new QRadioButton ( "Append");
    QRadioButton* qrbSequence = new QRadioButton ( "Sequence");

    qrbOverwrite->setChecked(true);


    qbtngrpSampleTarget = new QButtonGroup();
    connect(qbtngrpSampleTarget, SIGNAL(buttonClicked(int)), this, SLOT(slotSettingsTarget(int)));
    qbtngrpSampleTarget->addButton(qrbOverwrite, TARGET_OVERWRITE );
    qbtngrpSampleTarget->addButton(qrbAppend, TARGET_APPEND );
    qbtngrpSampleTarget->addButton(qrbSequence, TARGET_SEQUENCE );



    QGridLayout *qlayoutTarget = new QGridLayout;
    qlayoutTarget->setVerticalSpacing ( 0 );
    qlayoutTarget->addWidget(lblSaveTo,1,1,1,2 );
    qlayoutTarget->addWidget(leditFileName,2,1);
    qlayoutTarget->addWidget(qbtnFileName,2,2);
    qlayoutTarget->addWidget(qrbOverwrite,3,1,1,2);
    qlayoutTarget->addWidget(qrbAppend,4,1,1,2);
    qlayoutTarget->addWidget(qrbSequence, 5,1,1,2);
    qgrpbxSampleTarget->setLayout(qlayoutTarget);
    /* -------------------------------------- */

/* ----------------------- */


    qgrpbxMiscOptions = new QGroupBox ( "Misc", this) ;
    qgrpbxMiscOptions->setFlat(false);

    qglayoutControls->addMultiCellWidget(qgrpbxMiscOptions, 3, 3, 0, 2);
    qchkAutoStart = new QCheckBox ( "auto start/stop");

    connect(qchkAutoStart, SIGNAL(stateChanged (int)), this, SLOT(slotToggleAutoStart(int)));
    
    QHBoxLayout *qlayoutMisc = new QHBoxLayout;
    qlayoutMisc->setSpacing ( 0 );
    qlayoutMisc->addWidget(qchkAutoStart);
    qgrpbxMiscOptions->setLayout(qlayoutMisc);
    /* -------------------------------------- */

/* ----------------------- */

/* setting up the runtime tab */
    qlblStatus = new QLabel(this);

/*
    qbufferMoview = new QBuffer();
    qbufferMoview->setData( movie_data, sizeof(movie_data) );
    qlblStatus->setMovie(new QMovie(qbufferMoview));
*/
    
    QString filename = "/usr/lib/iqr/iqr-logo.gif";
    qlblStatus->setMovie(new QMovie(filename));


    if(qlblStatus->movie()!=NULL){
	qlblStatus->movie()->pause();
	qlblStatus->movie()->step();
    }

//--    qlblStatus->setFixedWidth(38);
//--    qlblStatus->setFixedHeight(19);
//--    qlblStatus->show();

    qglayoutControls->addWidget(qlblStatus, 4, 0);

    qbtnAction = new QPushButton("Sample", this);
    qglayoutControls->addWidget(qbtnAction, 4, 1);
    connect(qbtnAction,SIGNAL(clicked()), this, SLOT(slotRunControl()));
    qbtnAction->setEnabled(false);

    QPushButton *qpbtnClose = new QPushButton ("Close", this/*, "Close"*/);
    qglayoutControls->addWidget(qpbtnClose, 4, 2);
    connect(qpbtnClose, SIGNAL(clicked()), this, SLOT(close()));

    qframeStateVariableDisplays = new QFrame(this);
    boxlayoutStateFrames = new QHBoxLayout(qframeStateVariableDisplays);
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

    pair<string, ClsQNeuronStateVariableDisplay*> pairTemp(strDisplayID, clsQNeuronStateVariableDisplay);
    mapStateVariableDisplays.insert(pairTemp);
    boxlayoutStateFrames->addWidget(clsQNeuronStateVariableDisplay);


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


    pair<string, ClsQNeuronStateVariableDisplay*> pairTemp(strDisplayID, clsQNeuronStateVariableDisplay);
    mapStateVariableDisplays.insert(pairTemp);
    boxlayoutStateFrames->addWidget(clsQNeuronStateVariableDisplay);

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
	if(event->mimeData()->hasFormat ("text/iqr-plot" )){
	    event->acceptProposedAction();
	}
    }
}

void ClsFEDataSampler::dropEvent(QDropEvent* event) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::dropEvent(QDropEvent* event)" << endl;
#endif
    
    QString text;
//--    if(event->mimeData()->hasText()){
    if(event->mimeData()->hasFormat ("text/iqr-plot" )){
	text = event->mimeData()->data ("text/iqr-plot");
//	cout << "text: " << text.toStdString() << endl;
	
	
	dropInfo di = ClsDragDropDeEncoder::decode(text.latin1());
	unsigned int iItemType = di.ItemType;
	if(iItemType == ClsFESystemManager::ITEM_GROUP){
	    string strGroupID = di.ItemID;
	    string strParams = di.Params;
	    string strRange = di.Range;
	    
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
	ok = true;
	
	break;
    case COUNTING_STEPS:
	qspinSteps->setEnabled(true);
	iAmount = qspinSteps->text().toInt(&ok, 10);

	break;
    case COUNTING_TIME:
	qspinSteps->setEnabled(false);
	break;
    }
    checkState(ok);
};

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


    checkState(ok);

};

void ClsFEDataSampler::slotSpinLocking (const QString& qstr) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotSpinLocking (const QString& qstr)" << endl;
#endif
    bool ok;
    fSamplingRate = qstr.toInt(&ok, 10);


    checkState(ok);

};

void ClsFEDataSampler::slotFilenameChanged (const QString& /*qstr*/) {
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotFilenameChanged (const QString& qstr)" << endl;
#endif
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
    qgrpbxSampling->setEnabled(false);
    qgrpbxAcquisition->setEnabled(false);
    qgrpbxSampleTarget->setEnabled(false);
    qgrpbxMiscOptions->setEnabled(false);


    qbtnAction->setText("Stop");

//++++    qmovieAnimation->unpause();
    if(qlblStatus->movie()!=NULL){
	qlblStatus->movie()->unpause();
    } else {cout << "NO MOVIE" << endl;}



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
    qgrpbxSampling->setEnabled(true);
    qgrpbxAcquisition->setEnabled(true);
    qgrpbxSampleTarget->setEnabled(true);
    qgrpbxMiscOptions->setEnabled(true);


    qbtnAction->setText("Sample");

//++++    qmovieAnimation->pause();
    if(qlblStatus->movie()!=NULL){
	qlblStatus->movie()->unpause();
    } else {cout << "NO MOVIE" << endl;}
		


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

	qstrFN = QFileDialog::getSaveFileName( 
	    this, 
	    "Choose file name",
	    "data.dat",
	    "All files (*.*)"
	    );
	
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


void ClsFEDataSampler::slotToggleAutoStart(int i){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::slotToggleAutoStart(int i)" << endl;
#endif
    if(i == Qt::Checked) {
	bAutoStart = true;
    } else {
	bAutoStart = false;
    }
}


void ClsFEDataSampler::customEvent(QEvent *e){
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


    QObjectList qObjectList = qframeStateVariableDisplays->queryList( "ClsQNeuronStateVariableDisplay" );

    const QList<QObject *>::iterator it;
    QObject *tmp;
    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;

    for(it=qObjectList.begin();it!=qObjectList.end();it++ ) {
	QObject *tmp = *it;
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


    QObjectList qObjectList = qframeStateVariableDisplays->queryList( "ClsQNeuronStateVariableDisplay" );

    const QList<QObject *>::iterator it;

    QObject *tmp;
    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;
    for(it=qObjectList.begin();it!=qObjectList.end();it++ ) {
	QObject *tmp = *it;
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

void ClsFEDataSampler::setSettingsAcquistion( int i ){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::setSettingsAcquistion( int i )" << endl;
#endif
    iCOUNTING_MODE = i;
    if(qbtngrpAcquisition->button( i )!=NULL){
	qbtngrpAcquisition->button( i )->setChecked(true);
    }


    slotSettingsCounting( i );
}

void ClsFEDataSampler::setSettingTarget ( int i ){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::setSettingTarget ( int i )" << endl;
#endif
    iTARGET_MODE = i; 
    if(qbtngrpSampleTarget->button( i )!=NULL){
	qbtngrpSampleTarget->button( i )->setChecked(true);
    }

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
// 	qspinTime->setValue(i);
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
    qchkAutoStart->setChecked (b);

}


void ClsFEDataSampler::groupChanged(string _strGroupID){
#ifdef DEBUG_CLSFEDATASAMPLER
    cout << "ClsFEDataSampler::groupChanged(string strGroupID)" << endl;
#endif

    QObjectList qObjectList = qframeStateVariableDisplays->queryList( "ClsQNeuronStateVariableDisplay" );
    const QList<QObject *>::iterator it;
//--    QObject *tmp;
    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;

    for(it=qObjectList.begin();it!=qObjectList.end();it++ ) {
	QObject *tmp = *it;
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
