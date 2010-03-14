/****************************************************************************
 ** $Filename: ClsFEGroupPlot.cpp
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/DataManager/ClsFEGroupPlot.cpp,v 1.9 2004/02/04 12:05:00 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Oct 26 01:32:17 2001
 ** $Date: 2004/02/04 12:05:00 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <list>
#include <algorithm>

#include <qpopupmenu.h>
#include <qvgroupbox.h>
#include <qdragobject.h>
#include <qobjectlist.h>

#include <ClsFESystemManager.h>
#include "ClsFEGroupPlot.h"
#include <ClsTopologySparse.h>
#include <ClsTopologyRect.h>
#include <ClsFEDataSink.h>
#include <iqrUtils.h>
#include <ClsDragDropDeEncoder.h>

#include <icon_groupplot_32x32.xpm>


//#define DEBUG_CLSFEGROUPPLOT 

ClsFEGroupPlot::ClsFEGroupPlot(ClsFEDataManager *_clsFEDataManager, QMutex *_qmutexSysGUI, string _strDataClientID, ClsFEGroup* _clsFEGroup) : 
    ClsFEDataClient(_clsFEDataManager, _qmutexSysGUI, _strDataClientID) {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::ClsFEGroupPlot(...) [new]" << endl;
#endif
    bSparseTopology = false;   
    setClientType(CLIENT_SPACEPLOT);
    setAcceptDrops(TRUE);
    this->setIcon(QPixmap(icon_groupplot));
/* set up the WSpacePlot */
    wSpacePlot = new WSpacePlot(QSize(0,0),FALSE,this, this);
    wSpacePlot->setColor(QColor(0,0,0));
    wSpacePlot->showGrid(true);
    wSpacePlot->setMinimumWidth(200);

    (wSpacePlot->yAxis())->setDirection(-1);


    QBoxLayout* boxlayoutPlot = new QHBoxLayout();
    boxlayoutPlot->addWidget(wSpacePlot,1);
    boxlayoutPlot->addWidget(wSpacePlot->getColorBar());

    boxlayoutBase->addLayout(boxlayoutPlot, 0, 0);

    connect( wSpacePlot, SIGNAL(mouseEvent(QEvent::Type, QPoint, int, int)), this,
	     SLOT(slotPlotMouseClick(QEvent::Type, QPoint, int, int)) );

    connect( wSpacePlot, SIGNAL(wouldDrag()), this, SLOT(startDragging()) );
/* ----------------------- */
    if(_clsFEGroup != NULL){

	string strOutputStateName = _clsFEGroup->getNeuronOutput()->getName(); //NEW
	list<string> lstSelectedStates; // NEW
	lstSelectedStates.push_back(strOutputStateName); // NEW
	
	addStateVariableDisplay(_clsFEGroup, lstSelectedStates, "");
    }

    QWidget::show();
};

string ClsFEGroupPlot::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string /* strRange */) {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string /* strRange */)" << endl;
#endif
    strGroupID = _clsFEGroup->getGroupID();
//    cout << "addStateVariableDisplay(..):strGroupID: " << strGroupID << endl;


    string strDisplayID = _clsFEGroup->getGroupID() + ClsIDGenerator::Instance()->Next();

    strTitle = "Space Plot for \"" + _clsFEGroup->getGroupName() + "\"";
    setCaption(strTitle.c_str());


    if(clsQBaseStateVariableDisplay!=NULL){
	boxlayoutBase->remove(clsQBaseStateVariableDisplay); 
    }

    clsQBaseStateVariableDisplay = 
	new ClsQNeuronStateVariableDisplay(this, strDisplayID, this, _clsFEGroup, "",false, false, true, false, false);

    connect(dynamic_cast<ClsQNeuronStateVariableDisplay*>(clsQBaseStateVariableDisplay), 
	    SIGNAL(addDisplay(string, list<string>, string )),
	    this, SLOT(addStateVariableDisplay(string, list<string>, string)));


    iGroupWidth = _clsFEGroup->getNrCellsHorizontal();
    iGroupHeight = _clsFEGroup->getNrCellsVertical();
    getCellIndices(_clsFEGroup);
    wSpacePlot->setSize(QSize(iGroupWidth,iGroupHeight));

    boxlayoutBase->addWidget(clsQBaseStateVariableDisplay, 0, 2, Qt::AlignTop); 
    return strDisplayID;
};

string ClsFEGroupPlot::addStateVariableDisplay( ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string /* strRange */) {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::addStateVariableDisplay(ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string strRange )" << endl;
#endif
    strGroupID = _clsFEGroup->getGroupID();
    string strDisplayID = _clsFEGroup->getGroupID() + ClsIDGenerator::Instance()->Next();

    strTitle = "Space Plot for \"" + _clsFEGroup->getGroupName() + "\"";
    setCaption(strTitle.c_str());

    if(clsQBaseStateVariableDisplay!=NULL){ 
	boxlayoutBase->remove(clsQBaseStateVariableDisplay); 
    }

    clsQBaseStateVariableDisplay = new ClsQNeuronStateVariableDisplay(this, strDisplayID, this, _clsFEGroup, "", false, false, true, false, false);
    dynamic_cast<ClsQNeuronStateVariableDisplay*>(clsQBaseStateVariableDisplay)->setSelectedStates(lstSelectedStates);
    connect(dynamic_cast<ClsQNeuronStateVariableDisplay*>(clsQBaseStateVariableDisplay), 
	    SIGNAL(addDisplay(string, list<string>, string )),
	    this, SLOT(addStateVariableDisplay(string, list<string>, string)));

    clsQBaseStateVariableDisplay->slotLiveData(true);

    iGroupWidth = _clsFEGroup->getNrCellsHorizontal();
    iGroupHeight = _clsFEGroup->getNrCellsVertical();
    getCellIndices(_clsFEGroup);

    wSpacePlot->setSize(QSize(iGroupWidth,iGroupHeight));

    boxlayoutBase->addWidget(clsQBaseStateVariableDisplay, 0, 2, Qt::AlignTop); 
    return strDisplayID;
};

string ClsFEGroupPlot::addStateVariableDisplay(string _strGroupID, list<string> lstSelectedStates, string strRange) {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::addStateVariableDisplay(string _strGroupID, list<string> lstSelectedStates, string strRange)" << endl;
#endif
    strGroupID = _strGroupID;
    strRange = "";
    if(ClsFESystemManager::Instance()->getFEGroup( _strGroupID )!=NULL) {
	return addStateVariableDisplay( ClsFESystemManager::Instance()->getFEGroup( _strGroupID ), lstSelectedStates, "");
    } 
    return "";
};

void ClsFEGroupPlot::plotData() {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::plotData()" << endl;
#endif

    if(mapDataSinks.size() > 0){
	ClsDataSinkCopying *clsDataSink =  mapDataSinks.begin()->second;

	qmutexSysGUI->lock(); 
	clsDataSink->update();
	qmutexSysGUI->unlock();

	unsigned int iIndex = 0;

	if(bSparseTopology){
	    QArray<WSpacePlot::SpaceCell> qaCells = wSpacePlot->getCellData();
	    for(iIndex=0; iIndex<vectCellIndices.size(); iIndex++){
//		cout << "iIndex: " << iIndex << ", index: " << vectCellIndices[iIndex] << endl;
		qaCells[vectCellIndices[iIndex]].setValue((*clsDataSink)[iIndex]);
	    }
	} else {
//	    cout << "RECT" << endl;
	    QArray<WSpacePlot::SpaceCell>::Iterator it;
	    for (it=wSpacePlot->getCellData().begin(); it!=wSpacePlot->getCellData().end();++it) {
		it->setValue((*clsDataSink)[iIndex]);
		iIndex++;
	    }
	}


	wSpacePlot->updateGraph();

    }
};

void ClsFEGroupPlot::slotPlotMouseClick(QEvent::Type /* qeventtype */, QPoint /* qpointPos */, int iButton, int /* iState */){
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::slotPlotMouseClick(QEvent::Type qeventtype, QPoint qpointPos, int iButton, int iState)" << endl;
#endif

    if(iButton == 2){
	QPopupMenu *menuBrowser = new QPopupMenu( this );
	menuBrowser->insertItem( "Time Plot", MENU_TIME_PLOT, -1 );

	menuBrowser->popup( this->cursor().pos() );

	connect( menuBrowser, SIGNAL(activated(int)), SLOT(slotPlotMenu(int)) );
    }
};

void ClsFEGroupPlot::slotPlotMenu(int iMenuID){
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::slotPlotMenu(int iMenuID)" << endl;
#endif
    string strParamList = "";
    string strRange = "";
    QArray<int> qarraySelected;

    switch (iMenuID){
    case MENU_SPACE_PLOT:
	strParamList = dynamic_cast<ClsQNeuronStateVariableDisplay*>(clsQBaseStateVariableDisplay)->getSelectedStatesAsString();
	clsFEDataManager->DataClientCreate(ClsFEDataClient::CLIENT_SPACEPLOT, 
					   clsQBaseStateVariableDisplay->getItemID(), strParamList, "");
	
	break;
    case MENU_TIME_PLOT:
	strParamList = dynamic_cast<ClsQNeuronStateVariableDisplay*>(clsQBaseStateVariableDisplay)->getSelectedStatesAsString();


//$$$$$$$$$$$$$44
/* NEED CHECKING AGAINST THE TOPOLOGY IF SPARSE.... */
	qarraySelected =  wSpacePlot->getSelecIndexList ();

	strRange = wSpacePlotSelected2Range(qarraySelected);

/*
  for(unsigned int ii = 0; ii< qarraySelected.size(); ii++){
  strRange = iqrUtils::int2string(qarraySelected[ii]) + ";"  +  strRange ;
  }
*/

	clsFEDataManager->DataClientCreate(ClsFEDataClient::CLIENT_TIMEPLOT, 
					   clsQBaseStateVariableDisplay->getItemID(), strParamList, strRange);

	break;
    default:
	break;
    }
};

void ClsFEGroupPlot::slotLiveData(bool /*bToggled*/) {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::slotLiveData(bool bToggled)" << endl;
#endif
//     cout << "void ClsFEGroupPlot::slotLiveData()" << endl;
//     cout << "bToggled: " << bToggled << endl;
};

void ClsFEGroupPlot::startDragging() {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::startDragging()" << endl;
#endif
    QTextDrag *d = new QTextDrag( "", this );
    d->setSubtype("iqr-plot");

    string strParamList=dynamic_cast<ClsQNeuronStateVariableDisplay*>(clsQBaseStateVariableDisplay)->getSelectedStatesAsString();

    string strRange = "";

    QArray<int> qarraySelected =  wSpacePlot->getSelecIndexList ();
     
    strRange = wSpacePlotSelected2Range(qarraySelected);

    string str = "";
    str = ClsDragDropDeEncoder::encode(ClsFESystemManager::ITEM_GROUP, clsQBaseStateVariableDisplay->getItemID(), strParamList, strRange);

    d->setText(str.c_str());

    d->dragMove();
};


string ClsFEGroupPlot::wSpacePlotSelected2Range(QArray<int> &qarraySelected){
    string strRange;

 
    if(dynamic_cast<ClsTopologySparse*>(ClsFESystemManager::Instance()->getFEGroup( strGroupID )->getTopology())){
	ClsTopologySparse *clsTopologySparse = dynamic_cast<ClsTopologySparse*>(ClsFESystemManager::Instance()->getFEGroup( strGroupID )->getTopology());
	iGroupWidth = clsTopologySparse->nrCellsHorizontal();
	iGroupHeight = clsTopologySparse->nrCellsVertical();

	for(unsigned int ii = 0; ii< qarraySelected.size(); ii++){
	    int iy = int(qarraySelected[ii] / iGroupWidth) + 1;
	    int ix = qarraySelected[ii] - int(qarraySelected[ii] / iGroupWidth) * iGroupWidth + 1;
	    if(clsTopologySparse->isValidCell(ix, iy)){
//		cout << "clsTopologySparse->pos2index(ix, iy): " << clsTopologySparse->pos2index(ix, iy) << endl;
//		cout << __FILE__ << ": ix, iy: " << ix << ", " << iy << endl;
		strRange = iqrUtils::int2string(clsTopologySparse->pos2index(ix, iy)) + ";"  +  strRange ;
	    }
	}	      
    } else {
	for(unsigned int ii = 0; ii< qarraySelected.size(); ii++){
	    strRange = iqrUtils::int2string(qarraySelected[ii]) + ";"  +  strRange ;
	}
    }
    return strRange;
}

void ClsFEGroupPlot::dragEnterEvent(QDragEnterEvent* event) {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::dragEnterEvent(QDragEnterEvent* event)" << endl;
#endif
    event->accept( QTextDrag::canDecode(event) );
}

void ClsFEGroupPlot::dropEvent(QDropEvent* event) {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::dropEvent(QDropEvent* event)" << endl;
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


		list<string> lst;
		string::size_type pos;
		while((pos = strParams.find(";")) != string::npos){
		    lst.push_back(strParams.substr(0, pos));
		    strParams.erase(0, pos+1);
		}
		clsQBaseStateVariableDisplay->slotClose();
		addStateVariableDisplay( ClsFESystemManager::Instance()->getFEGroup( strGroupID ), lst, "");
	    }
	}
    }
};

void ClsFEGroupPlot::init(){
    ClsFEDataClient::init();
}

void ClsFEGroupPlot::update() {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::update()" << endl;
#endif
     
    plotData();
};

void ClsFEGroupPlot::DataSinkAdded(string strID, int iColor){
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::DataSinkAdded(string strID, int iColor)" << endl;
#endif
    strID = "";

//     cout << "ClsFEGroupPlot::DataSinkAdded(string strID)" << endl;
    QColor qc;
    qc.setHsv(iColor*2, 255, 210); // try to multiply by some factor, so the differences become larger
    wSpacePlot->setColor(qc);

};

void ClsFEGroupPlot::closeEvent( QCloseEvent* ce ) {
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::closeEvent( QCloseEvent* ce )" << endl;
#endif

    if(clsQBaseStateVariableDisplay!=NULL){
	clsQBaseStateVariableDisplay->close();
	delete clsQBaseStateVariableDisplay;
	clsQBaseStateVariableDisplay = NULL;
    }

    bSparseTopology = false;
    emit sigPlotClosed(strDataClientID);
    ce->accept();
}

void ClsFEGroupPlot::setConfig(ClsDataClientConfig clsDataClientConfig){
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::setConfig(ClsDataClientConfig clsDataClientConfig)" << endl;
#endif

    list<ClsStateVariableDisplayConfig>lstSVDConfigs =  clsDataClientConfig.getListStateVariableDisplayConfig();
    list<ClsStateVariableDisplayConfig>::iterator itSVD;
    for(itSVD = lstSVDConfigs.begin(); itSVD != lstSVDConfigs.end(); itSVD++){

	string strSVDID =  (*itSVD).getID();
	string _strGroupID = (*itSVD).getItemID();
	string strSelectedIndices = (*itSVD).getSelectedIndices();
/*
	cout << "strSVDID: " << strSVDID << endl;
	cout << "_strGroupID: " << _strGroupID << endl;
	cout << "strSelectedIndices: " << strSelectedIndices << endl;
*/

	if(ClsFESystemManager::Instance()->getFEGroup( _strGroupID )!=NULL){
	    addStateVariableDisplay(ClsFESystemManager::Instance()->getFEGroup( _strGroupID ), strSelectedIndices);
	    if(clsQBaseStateVariableDisplay!=NULL){
		clsQBaseStateVariableDisplay->setConfig((*itSVD));
	    }
	}
    }
}



ClsDataClientConfig ClsFEGroupPlot::getConfig(){
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::getConfig()" << endl;
#endif
    ClsDataClientConfig clsDataClientConfig(strDataClientID);
    clsDataClientConfig.setPosition(this->pos().x(), this->pos().y() );
    clsDataClientConfig.setGeometry(this->size().width(),  this->size().height());
    
    if(clsQBaseStateVariableDisplay!=NULL){
	ClsStateVariableDisplayConfig clsStateVariableDisplayConfig = clsQBaseStateVariableDisplay->getConfig();
	clsDataClientConfig.addStateVariableDisplayConfig(clsStateVariableDisplayConfig);
    }
    return clsDataClientConfig;
}


void ClsFEGroupPlot::groupChanged(string _strGroupID){
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::groupChanged(string strGroupID)" << endl;
#endif

    if(!strGroupID.compare(_strGroupID)){
	ClsFEGroup* _clsFEGroup = ClsFESystemManager::Instance()->getFEGroup( _strGroupID );
	iGroupWidth = _clsFEGroup->getNrCellsHorizontal();
	iGroupHeight = _clsFEGroup->getNrCellsVertical();
	
	getCellIndices(_clsFEGroup);
	
	wSpacePlot->setSize(QSize(iGroupWidth,iGroupHeight));
	wSpacePlot->updateGraph ( );
	
	if(!_strGroupID.compare(clsQBaseStateVariableDisplay->getItemID())){
	    dynamic_cast<ClsQNeuronStateVariableDisplay*>(clsQBaseStateVariableDisplay)->groupChanged();
	}
	ClsFEDataClient::groupChanged(_strGroupID);
    }
};


void ClsFEGroupPlot::groupDeleted(string strID){
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "ClsFEGroupPlot::groupDeleted(string strID)" << endl;
#endif

    if(!strGroupID.compare(strID)){
//	this->close();
	bSparseTopology = false;
	QWidget::close();
    }
}


void ClsFEGroupPlot::getCellIndices(ClsFEGroup* _clsFEGroup){
#ifdef DEBUG_CLSFEGROUPPLOT
    cout << "getCellIndices(ClsFEGroup* _clsFEGroup)" << endl;
#endif

    ClsBaseTopology *clsBaseTopology = _clsFEGroup->getTopology();
    if(dynamic_cast<ClsTopologySparse*>(clsBaseTopology)){
	vectCellIndices.resize(0);
	vector<tiPoint> lstPoints = dynamic_cast<ClsTopologySparse*>(clsBaseTopology)->getList();
	vector<tiPoint>::iterator it;
	for(it=lstPoints.begin(); it!=lstPoints.end(); it++){
//	    cout << "INDEX RECT: " << ClsTopologyRect::pos2index(iGroupWidth, (*it).first, (*it).second) << endl;
	    vectCellIndices.push_back(ClsTopologyRect::pos2index(iGroupWidth, (*it).first, (*it).second));
	}
	bSparseTopology = true;
//	cout << "vectCellIndices.size(): " << vectCellIndices.size() << endl;
    } else {
	bSparseTopology = false;
    }




}


//cout << __FILE__ << ":" << __LINE__ << endl;

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../
//// End:

