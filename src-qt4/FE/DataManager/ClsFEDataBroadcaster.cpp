/****************************************************************************
 ** $Filename: ClsFEDataBroadcaster.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <algorithm>

#include <numeric>
#include <sstream>

#include "ClsFEDataBroadcaster.h"

#include <ClsFESystemManager.h>
#include <ClsDragDropDeEncoder.h>

#define DEBUG_CLSFEDATABROADCASTER

ClsFEDataBroadcaster::ClsFEDataBroadcaster(ClsFEDataManager *_clsFEDataManager, QMutex *_qmutexSysGUI, string _strDataClientID) :
    ClsFEDataClient(_clsFEDataManager, _qmutexSysGUI, _strDataClientID) {
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::ClsFEDataBroadcaster(...)" << endl;
#endif
    strTitle = "Data Broadcaster";

    setCaption(strTitle.c_str());

    setClientType(CLIENT_DATABROADCASTER);
    setAcceptDrops(TRUE);




    QLabel *qlblIPReceiver = new QLabel(tr("IP address:"));
    qleIPReceiver = new QLineEdit("127.0.0.1");

    QLabel *qlblPort = new QLabel(tr("Port:"));
    qspinPort = new QSpinBox();
    qspinPort->setRange (32768 ,61000);
    qspinPort->setValue (61000);    


    QLabel *qlblInterval = new QLabel(tr("Interval (ms):"));
    qspinInterval = new QSpinBox();
    qspinInterval->setRange (40 ,10000);
    qspinInterval->setValue (500);    



    QGridLayout *layoutGridParameters = new QGridLayout();
    layoutGridParameters->addWidget(qlblIPReceiver, 0, 0);
    layoutGridParameters->addWidget(qleIPReceiver, 0, 1);
    layoutGridParameters->addWidget(qlblPort, 1, 0);
    layoutGridParameters->addWidget(qspinPort, 1, 1);
    layoutGridParameters->addWidget(qlblInterval, 2, 0);
    layoutGridParameters->addWidget(qspinInterval, 2, 1);


    qpbAddAll = new QPushButton("ON", this);
    qpbAddAll->setCheckable (true);
    connect(qpbAddAll,SIGNAL(toggled(bool)),this, SLOT(toggleAllGroups(bool)));
    QLabel *qlblAddAll = new QLabel(tr("Add all groups:"));

    qpbControl = new QPushButton("OFF", this);
    qpbControl->setCheckable (true);
    connect(qpbControl,SIGNAL(toggled(bool)),this, SLOT(broadcast(bool)));
    QLabel *qlblSendData = new QLabel(tr("Send data:"));

    


    QGridLayout *layoutGridControls = new QGridLayout();
    layoutGridControls->addWidget(qlblAddAll, 0, 0);
    layoutGridControls->addWidget(qpbAddAll, 0, 1);
    layoutGridControls->addWidget(qlblSendData, 1, 0);
    layoutGridControls->addWidget(qpbControl, 1, 1);

    QVBoxLayout *layoutBControls = new QVBoxLayout();
    layoutBControls->addLayout(layoutGridParameters);
    layoutBControls->addStretch(1);
    layoutBControls->addLayout(layoutGridControls);

    layoutBControls->setSizeConstraint (QLayout::SetMinAndMaxSize);
    boxlayoutBase->addLayout(layoutBControls, 0, 0);

    

//-----------------------

    qframeStateVariableDisplays = new QFrame(this);
    boxlayoutStateFrames = new QHBoxLayout(qframeStateVariableDisplays/*zzz,0,0*/);
    boxlayoutBase->addWidget(qframeStateVariableDisplays, 0, 2); 

    qtimer = new QTimer(this);
    connect(qtimer, SIGNAL(timeout ()), SLOT(update()));

    qlblAllGroups = new QLabel(tr("SENDING ALL GROUPS"));
    boxlayoutStateFrames->addWidget(qlblAllGroups);
    qlblAllGroups->hide();

    qs = new QUdpSocket (NULL);

    QWidget::show();
};


void ClsFEDataBroadcaster::addAllGroups(){
    cout << "ClsFEDataBroadcaster::addAllGroups()" << endl;
    /* remove all data sinks
       remove all statevariable displays
    */

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
    mapDataSinks.clear();

    map<string, ClsFEGroup*> mapGroups =  ClsFESystemManager::Instance()->getMapFEGroups();
    map<string, ClsFEGroup*>::iterator itGroups;
    for(itGroups=mapGroups.begin(); itGroups!=mapGroups.end(); itGroups++){
	string strGroupID = itGroups->first;
	ClsFEGroup* clsFEGroup = itGroups->second;
	string strOutputStateName = clsFEGroup->getNeuronOutput()->getName(); 
	string strSinkID = strOutputStateName + strGroupID + ClsIDGenerator::Instance()->Next();
	cout << "ADDING GROUP:" <<  strGroupID << endl;
	addDataSink(strSinkID, clsFEGroup, strOutputStateName, "");
	setSinkMath(strSinkID, ClsFEDataSink::MATH_AVERAGE);
    }

    

    qlblAllGroups->show();
    bAllGroups = true;
};


void ClsFEDataBroadcaster::removeAllGroups(){
    cout << "ClsFEDataBroadcaster::removeAllGroups()" << endl;

    mapDataSinks.clear();
    qlblAllGroups->hide();
    bAllGroups = false;
};



string ClsFEDataBroadcaster::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange) {
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange)" << endl;
#endif

    string strDisplayID = _clsFEGroup->getGroupID() + ClsIDGenerator::Instance()->Next();

    ClsQNeuronStateVariableDisplay * clsQNeuronStateVariableDisplay = new ClsQNeuronStateVariableDisplay(this, strDisplayID, qframeStateVariableDisplays, _clsFEGroup, strRange, true, false, false, true, true);
    connect(clsQNeuronStateVariableDisplay,SIGNAL(addDisplay(string, list<string>, string )),this, SLOT(addStateVariableDisplay(string, list<string>, string)));
    connect(clsQNeuronStateVariableDisplay,SIGNAL(sigClose(string)),this, SLOT(removeStateVariableDisplay(string)));

    pair<string, ClsQNeuronStateVariableDisplay*> pairTemp(strDisplayID, clsQNeuronStateVariableDisplay);
    mapStateVariableDisplays.insert(pairTemp);

    boxlayoutStateFrames->addWidget(clsQNeuronStateVariableDisplay);
    return strDisplayID;
};

string ClsFEDataBroadcaster::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string strRange) {
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::addStateVariableDisplay(string strDisplayID, ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates)" << endl;
#endif

    string strDisplayID = _clsFEGroup->getGroupID() + ClsIDGenerator::Instance()->Next();

    ClsQNeuronStateVariableDisplay * clsQNeuronStateVariableDisplay = new ClsQNeuronStateVariableDisplay(this, strDisplayID, qframeStateVariableDisplays, _clsFEGroup, strRange, true, false, false, true, true);
    clsQNeuronStateVariableDisplay->setSelectedStates(lstSelectedStates);
    connect(clsQNeuronStateVariableDisplay,SIGNAL(addDisplay(string, list<string>, string )),this, SLOT(addStateVariableDisplay(string, list<string>, string)));
    connect(clsQNeuronStateVariableDisplay,SIGNAL(sigClose(string)),this, SLOT(removeStateVariableDisplay(string)));

    clsQNeuronStateVariableDisplay->slotLiveData(true);


    pair<string, ClsQNeuronStateVariableDisplay*> pairTemp(strDisplayID, clsQNeuronStateVariableDisplay);
    mapStateVariableDisplays.insert(pairTemp);

    boxlayoutStateFrames->addWidget(clsQNeuronStateVariableDisplay);
    return strDisplayID;
};

string ClsFEDataBroadcaster::addStateVariableDisplay(string strGroupID, list<string> lstSelectedStates, string strRange) {
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::addStateVariableDisplay(string strGroupID, list<string> lstSelectedStates)" << endl;
#endif
    return addStateVariableDisplay(ClsFESystemManager::Instance()->getFEGroup( strGroupID ), lstSelectedStates, strRange);
};

void ClsFEDataBroadcaster::removeStateVariableDisplay(string strDisplayID) {
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::removeStateVariableDisplay(string strDisplayID)" << endl;
#endif

    if(mapStateVariableDisplays.find(strDisplayID) != mapStateVariableDisplays.end()){
	boxlayoutStateFrames->remove(mapStateVariableDisplays.find(strDisplayID)->second); 
	mapStateVariableDisplays.erase(strDisplayID);
    }
};


void ClsFEDataBroadcaster::groupDeleted(string strID){
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::groupDeleted(string strID)" << endl;
#endif
    map<string, ClsQNeuronStateVariableDisplay*>::iterator it;
    for(it=mapStateVariableDisplays.begin(); it!=mapStateVariableDisplays.end(); it++){
	if(!strID.compare(it->second->getGroupID())){
	    it->second->close();
	}
    }
}

void ClsFEDataBroadcaster::toggleAllGroups(bool b){
    if(b){
	addAllGroups();
	qpbAddAll->setText("ON");
    } else {
	removeAllGroups();
	qpbAddAll->setText("OFF");
    }
    
}

void ClsFEDataBroadcaster::broadcast(bool b) {
//#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::broadcast(bool b):" << b << endl;
//#endif


    int iIntervallMsec = qspinInterval->value();

    if(b){
	iPort = qspinPort->value();
	qa.setAddress (qleIPReceiver->text() );
	qpbControl->setText("ON");
	qtimer->start(iIntervallMsec, FALSE);
    } else {
	qpbControl->setText("OFF");
	qtimer->stop();
    }

};

void ClsFEDataBroadcaster::dragEnterEvent(QDragEnterEvent* event) {
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::dragEnterEvent(QDragEnterEvent* event)" << endl;
#endif
    if(event->mimeData()->hasFormat ("text/iqr-plot" )){
	event->acceptProposedAction();
    }
}

void ClsFEDataBroadcaster::dropEvent(QDropEvent* event) {
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::dropEvent(QDropEvent* event)" << endl;
#endif

    if(event->mimeData()->hasFormat ("text/iqr-plot" )){
	QString text = event->mimeData()->data ("text/iqr-plot");
	
//	cout << "drop text: " << text.latin1() << endl;
	dropInfo di = ClsDragDropDeEncoder::decode(text.latin1());
	unsigned int iItemType = di.ItemType;
	if(iItemType == ClsFESystemManager::ITEM_GROUP){
	    string strGroupID = di.ItemID;
	    string strParams = di.Params;
	    string strRange = di.Range;
	    
	    list<string> lst;
	    string::size_type pos;
	    while((pos = strParams.find(";")) != string::npos){
		lst.push_back(strParams.substr(0, pos));
		strParams.erase(0, pos+1);
	    }
	    addStateVariableDisplay( ClsFESystemManager::Instance()->getFEGroup( strGroupID ), lst, strRange);
	}
    }
};

void ClsFEDataBroadcaster::init(){
    ClsFEDataClient::init();
}



void ClsFEDataBroadcaster::update() {
//#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::update()" << endl;
//#endif
    
    string strID;
    
    map<string, ClsDataSinkCopying* >::iterator it;
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::mapDataSinks.size(): " << mapDataSinks.size() << endl;
#endif

    stringstream ss;
    
    for(it = mapDataSinks.begin(); it!=mapDataSinks.end(); ++it){
	strID = it->first;
	ClsDataSinkCopying *clsDataSink =  it->second;
	qmutexSysGUI->lock(); 
	clsDataSink->update();
	double fAverage = clsDataSink->average();
	qmutexSysGUI->unlock();
	ss << clsDataSink->getItemID() << ":" << fAverage << ";";
    }
    
    
    cout << "send data:" << ss.str() << endl;
    QString qstrData = ss.str().c_str();
    qs->writeDatagram( qstrData.toLatin1(), qstrData.length(), qa, iPort );

};

    



void ClsFEDataBroadcaster::closeEvent( QCloseEvent* ce ) {
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::closeEvent( QCloseEvent* ce )" << endl;
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

    emit sigPlotClosed(strDataClientID);
    ce->accept();
}


//--void ClsFEDataBroadcaster::setConfig(ClsDataClientConfig clsDataClientConfig){
//--#ifdef DEBUG_CLSFEDATABROADCASTER
//--    cout << "ClsFEDataBroadcaster::setConfig(ClsDataClientConfig clsDataClientConfig)" << endl;
//--#endif
//--
//--    list<ClsStateVariableDisplayConfig>lstSVDConfigs =  clsDataClientConfig.getListStateVariableDisplayConfig();
//--    list<ClsStateVariableDisplayConfig>::iterator itSVD;
//--    for(itSVD = lstSVDConfigs.begin(); itSVD != lstSVDConfigs.end(); itSVD++){
//--	string strSVDID =  (*itSVD).getID();
//--	string strGroupID = (*itSVD).getItemID();
//--	string strSelectedIndices = (*itSVD).getSelectedIndices();
//--
//--	if(ClsFESystemManager::Instance()->getFEGroup( strGroupID )!=NULL){
//--	    string strDisplayID = addStateVariableDisplay(ClsFESystemManager::Instance()->getFEGroup( strGroupID ), strSelectedIndices);
//--	    map<string, ClsQNeuronStateVariableDisplay*>::iterator it =  mapStateVariableDisplays.find(strDisplayID);
//--	    if(it != mapStateVariableDisplays.end()){
//--		it->second->setConfig((*itSVD));
//--	    }
//--	}
//--    }
//--}

ClsDataClientConfig ClsFEDataBroadcaster::getConfig(){
   ClsDataClientConfig clsDataClientConfig(strDataClientID);
//--    clsDataClientConfig.setPosition(this->pos().x(), this->pos().y() );
//--    clsDataClientConfig.setGeometry(this->size().width(),  this->size().height());
//--
//--    QObjectList qObjectList = qframeStateVariableDisplays->queryList( "ClsQNeuronStateVariableDisplay" );
//--    const QList<QObject *>::iterator it;
//--
//--    QObject *tmp;
//--    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;
//--
//--    for(it=qObjectList.begin();it!=qObjectList.end();it++ ) {
//--	QObject *tmp = *it;
//--	if((clsQNeuronStateVariableDisplay = dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp))){
//--	    ClsStateVariableDisplayConfig clsStateVariableDisplayConfig = clsQNeuronStateVariableDisplay->getConfig();
//--	    clsDataClientConfig.addStateVariableDisplayConfig(clsStateVariableDisplayConfig);
//--	}
//--   }
//--    return clsDataClientConfig;
}


void ClsFEDataBroadcaster::groupChanged(string _strGroupID){
#ifdef DEBUG_CLSFEDATABROADCASTER
    cout << "ClsFEDataBroadcaster::groupChanged(string strGroupID)" << endl;
#endif

    QObjectList qObjectList = qframeStateVariableDisplays->queryList( "ClsQNeuronStateVariableDisplay" );

    const QList<QObject *>::iterator it;

    QObject *tmp;
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


void ClsFEDataBroadcaster::slotHideControls ( bool bIsRight ) {
    if (bIsRight && qframeStateVariableDisplays!=NULL){
	qframeStateVariableDisplays->hide();
    } else{
	qframeStateVariableDisplays->show();
    }
};


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
