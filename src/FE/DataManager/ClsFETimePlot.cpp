/****************************************************************************
 ** $Filename: ClsFETimePlot.cpp
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/DataManager/ClsFETimePlot.cpp,v 1.7 2004/02/05 10:57:33 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Oct 26 02:13:54 2001
 ** $Date: 2004/02/05 10:57:33 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <algorithm>

#include <numeric>

#include <qdragobject.h>
#include <qobjectlist.h>

#include "ClsFETimePlot.h"

#include <ClsFESystemManager.h>
#include <ClsDragDropDeEncoder.h>

namespace{
#include <icon_timeplot_32x32.xpm>
}

//#define DEBUG_CLSFETIMEPLOT

ClsFETimePlot::ClsFETimePlot(ClsFEDataManager *_clsFEDataManager, QMutex *_qmutexSysGUI, string _strDataClientID) :
    ClsFEDataClient(_clsFEDataManager, _qmutexSysGUI, _strDataClientID) {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::ClsFETimePlot(...)" << endl;
#endif
    strTitle = "Time Plot";

    setCaption(strTitle.c_str());

    setClientType(CLIENT_TIMEPLOT);
    iTraceLength = 1000;
    this->setIcon(QPixmap(icon_timeplot));
    setAcceptDrops(TRUE);

    wLinePlot = new WLinePlot(this,NULL);
    boxlayoutBase->addWidget(wLinePlot, 0, 0);

    wLinePlot->showGrid(true);

    wLinePlot->setXLabel("x-axis");
    wLinePlot->setYLabel("y-axis");

    wLinePlot->setZoomEnabled(TRUE);
    wLinePlot->setAutoDisableZoom(FALSE);

    wLinePlot->setWrapAround (false);

    wLinePlot->setTraceLength(iTraceLength);
    wLinePlot->setXDataRange(WRange(0., iTraceLength/2));
    wLinePlot->setXVisRange(WRange(iTraceLength/2 - iTraceLength/20, iTraceLength/2));
    wLinePlot->setMinimumWidth(200);

    qframeStateVariableDisplays = new QFrame(this);
    boxlayoutStateFrames = new QHBoxLayout(qframeStateVariableDisplays,0,0);
    boxlayoutBase->addWidget(qframeStateVariableDisplays, 0, 2); 



    QWidget::show();
};

string ClsFETimePlot::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange) {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange)" << endl;
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

string ClsFETimePlot::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string strRange) {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::addStateVariableDisplay(string strDisplayID, ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates)" << endl;
#endif

    string strDisplayID = _clsFEGroup->getGroupID() + ClsIDGenerator::Instance()->Next();

    ClsQNeuronStateVariableDisplay * clsQNeuronStateVariableDisplay = new ClsQNeuronStateVariableDisplay(this, strDisplayID, qframeStateVariableDisplays, _clsFEGroup, strRange, true, false, false, true, true);
    clsQNeuronStateVariableDisplay->setSelectedStates(lstSelectedStates);
    connect(clsQNeuronStateVariableDisplay,SIGNAL(addDisplay(string, list<string>, string )),this, SLOT(addStateVariableDisplay(string, list<string>, string)));
    connect(clsQNeuronStateVariableDisplay,SIGNAL(sigClose(string)),this, SLOT(removeStateVariableDisplay(string)));

    clsQNeuronStateVariableDisplay->slotLiveData(true); //NEW 20080420


    pair<string, ClsQNeuronStateVariableDisplay*> pairTemp(strDisplayID, clsQNeuronStateVariableDisplay);
    mapStateVariableDisplays.insert(pairTemp);

    boxlayoutStateFrames->addWidget(clsQNeuronStateVariableDisplay);
    return strDisplayID;
};

string ClsFETimePlot::addStateVariableDisplay(string strGroupID, list<string> lstSelectedStates, string strRange) {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::addStateVariableDisplay(string strGroupID, list<string> lstSelectedStates)" << endl;
#endif
    return addStateVariableDisplay(ClsFESystemManager::Instance()->getFEGroup( strGroupID ), lstSelectedStates, strRange);
};

void ClsFETimePlot::removeStateVariableDisplay(string strDisplayID) {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::removeStateVariableDisplay(string strDisplayID)" << endl;
#endif

    if(mapStateVariableDisplays.find(strDisplayID) != mapStateVariableDisplays.end()){
	boxlayoutStateFrames->remove(mapStateVariableDisplays.find(strDisplayID)->second); 
	mapStateVariableDisplays.erase(strDisplayID);
    }
};


void ClsFETimePlot::groupDeleted(string strID){
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::groupDeleted(string strID)" << endl;
#endif
    map<string, ClsQNeuronStateVariableDisplay*>::iterator it;
    for(it=mapStateVariableDisplays.begin(); it!=mapStateVariableDisplays.end(); it++){
	if(!strID.compare(it->second->getGroupID())){
	    it->second->close();
	}
    }
}

void ClsFETimePlot::plotData() {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::plotData()" << endl;
#endif

    /* to do here:
       - loop over mapDataSinks
       - find the right trace form mapTraces
       - shuffle data over
    */


    string strID;
    int iTrace;
    map<string, int>::iterator itMapTraces;
    map<string, ClsDataSinkCopying* >::iterator it;
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::mapDataSinks.size(): " << mapDataSinks.size() << endl;
#endif
    for(it = mapDataSinks.begin(); it!=mapDataSinks.end(); ++it){
	strID = it->first;
//	ClsBaseDataSink *clsDataSink =  mapDataSinks.begin()->second;
	ClsDataSinkCopying *clsDataSink =  it->second;

	qmutexSysGUI->lock(); //NEW 2004/04/12
	clsDataSink->update();
	double fAverage = clsDataSink->average();
	qmutexSysGUI->unlock();//NEW 2004/04/12

	itMapTraces = mapTraces.find(strID);
	if(itMapTraces != mapTraces.end()) {
	    iTrace = itMapTraces->second;
	    wLinePlot->setCurrentValue(iTrace, fAverage) ;
	}

    }
    wLinePlot->updateGraph();
    wLinePlot->incCursorPos();
};

void ClsFETimePlot::dragEnterEvent(QDragEnterEvent* event) {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::dragEnterEvent(QDragEnterEvent* event)" << endl;
#endif
    event->accept( QTextDrag::canDecode(event) );
}

void ClsFETimePlot::dropEvent(QDropEvent* event) {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::dropEvent(QDropEvent* event)" << endl;
#endif

    QString text;
    if ( QTextDrag::decode(event, text) ) {
	if(event->provides("text/iqr-plot")){
	    
	    cout << "drop text: " << text.latin1() << endl;
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

    }
};

void ClsFETimePlot::init(){
    ClsFEDataClient::init();
}

void ClsFETimePlot::update() {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::update()" << endl;
#endif
    plotData();
};

void ClsFETimePlot::DataSinkAdded(string strID, int iColor){
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::DataSinkAdded(string strID)" << endl;
#endif

//    cout << "strID: " << strID << endl;

    QColor qc;
    qc.setHsv(iColor, 255, 210);
    pair<string, int> pairTemp2(strID, wLinePlot->countTraces());
    mapTraces.insert(pairTemp2);
    wLinePlot->addTrace(QString(""), qc);

};

void ClsFETimePlot::DataSinkRemoved(string strID){
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::DataSinkRemoved(string strID)" << endl;
#endif

    wLinePlot->removeTraces(); // CRASH....
    mapTraces.erase(strID);

    /* since wLinePlot->removeTraces() is crashing the code below is 
       at the moment not useful. The idea would be to remove all traces, and the add all the
       traces from mapTraces again... */
    
    int iColor = 0;
    map<string, int>::iterator it;
    for(it=mapTraces.begin(); it!=mapTraces.end(); it++){
	strID = it->first;
	if(mapDataSinks.find(strID)!=mapDataSinks.end()){
	    iColor = mapDataSinks.find(strID)->second->getColor();
	}
	it->second = wLinePlot->countTraces();
	QColor qc;
	qc.setHsv(iColor, 255, 210);
	wLinePlot->addTrace(QString(""), qc);
    }

};


void ClsFETimePlot::closeEvent( QCloseEvent* ce ) {
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::closeEvent( QCloseEvent* ce )" << endl;
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

    emit sigPlotClosed(strDataClientID);
    ce->accept();
}


void ClsFETimePlot::setConfig(ClsDataClientConfig clsDataClientConfig){
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::setConfig(ClsDataClientConfig clsDataClientConfig)" << endl;
#endif

    list<ClsStateVariableDisplayConfig>lstSVDConfigs =  clsDataClientConfig.getListStateVariableDisplayConfig();
    list<ClsStateVariableDisplayConfig>::iterator itSVD;
    for(itSVD = lstSVDConfigs.begin(); itSVD != lstSVDConfigs.end(); itSVD++){
	string strSVDID =  (*itSVD).getID();
	string strGroupID = (*itSVD).getItemID();
	string strSelectedIndices = (*itSVD).getSelectedIndices();

	if(ClsFESystemManager::Instance()->getFEGroup( strGroupID )!=NULL){
	    string strDisplayID = addStateVariableDisplay(ClsFESystemManager::Instance()->getFEGroup( strGroupID ), strSelectedIndices);
	    map<string, ClsQNeuronStateVariableDisplay*>::iterator it =  mapStateVariableDisplays.find(strDisplayID);
	    if(it != mapStateVariableDisplays.end()){
		it->second->setConfig((*itSVD));
	    }
	}
    }
}

ClsDataClientConfig ClsFETimePlot::getConfig(){
    ClsDataClientConfig clsDataClientConfig(strDataClientID);
    clsDataClientConfig.setPosition(this->pos().x(), this->pos().y() );
    clsDataClientConfig.setGeometry(this->size().width(),  this->size().height());

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
}


void ClsFETimePlot::groupChanged(string _strGroupID){
#ifdef DEBUG_CLSFETIMEPLOT
    cout << "ClsFETimePlot::groupChanged(string strGroupID)" << endl;
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


void ClsFETimePlot::slotHideControls ( bool bIsRight ) {
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
