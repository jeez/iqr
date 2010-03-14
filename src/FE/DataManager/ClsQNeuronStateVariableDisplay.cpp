/****************************************************************************
 ** $Filename: ClsQNeuronStateVariableDisplay.cpp
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/DataManager/ClsQNeuronStateVariableDisplay.cpp,v 1.5 2003/10/06 15:08:16 ulysses Exp ulysses $
 **
 ** Copyright (C) 2002 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Sun Feb 10 14:22:08 2002
 ** $Date: 2003/10/06 15:08:16 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <algorithm>
#include <qdragobject.h>
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qwidget.h>

#include "ClsQBaseStateVariableDisplay.h"

#include "ClsQMiniGroup.h"
#include "ClsQNeuronStateVariableDisplay.h"
#include <ClsParameter.h>
#include <ClsQDragHandle.h>
#include <stateVariableList.hpp>
#include <stateVariable.hpp>

#include "ClsTopologySparse.h"
#include "ClsTopologyHex.h"
#include "ClsTopologyRect.h"

#include <iqrUtils.h>
#include <ClsFESystemManager.h>
#include <close_very_small_6x6.xpm>
#include <ClsDragDropDeEncoder.h>

using namespace iqrcommon;

//#define DEBUG_CLSQSTATEVARIABLEDISPLAY

ClsQNeuronStateVariableDisplay::ClsQNeuronStateVariableDisplay(ClsFEDataClient *_clsFEDataClient,
							       string _strID, QWidget *_parent,
							       ClsFEGroup* _clsFEGroup,
							       string _strSelectedIndices,
							       bool bCloseEnabled, bool bAverageEnabled,
							       bool _bExclusiveStates, bool _bAdditiveDisplay, bool bMiniGroupDisplay) :
    ClsQBaseStateVariableDisplay(_clsFEDataClient, _strID, _parent), clsFEGroup(_clsFEGroup),
    strSelectedIndices(_strSelectedIndices), bExclusiveStates(_bExclusiveStates), bAdditiveDisplay(_bAdditiveDisplay) {

    

#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::ClsQNeuronStateVariableDisplay(...)" << endl;
#endif

    qLabelMiniGroup = NULL;
    iCurrentStateButton = -99;
    bAverage = false;

    setAcceptDrops(TRUE);
//     this->setBackgroundColor(QColor(0,0,0));
    QGridLayout *qgly = new QGridLayout ( this, 5, 1, 2, 0);

    string strInfo;
    strInfo = "Group ID:\t" + clsFEGroup->getGroupID();
    strInfo = strInfo + "\nGroup name:\t" + clsFEGroup->getGroupName();
    strInfo = strInfo + "\nNeuron type:\t" + clsFEGroup->getGroupNeuronType();
    strInfo = strInfo + "\nGroup width:\t" + iqrUtils::int2string(clsFEGroup->getNrCellsHorizontal());
    strInfo = strInfo + "\nGroup height:\t" + iqrUtils::int2string(clsFEGroup->getNrCellsVertical());


    if(bCloseEnabled){
	qpbtnClose = new QPushButton ( this);
	qpbtnClose->setPixmap( QPixmap(close_very_small ));
	qpbtnClose->setFlat(true);
	qgly->addWidget(qpbtnClose, 0, 0, AlignRight | AlignTop);
	connect(qpbtnClose, SIGNAL(clicked()), SLOT(slotClose()));
    }

    ClsQDragHandle* clsQDragHandle = new ClsQDragHandle(this);
    qgly->addWidget(clsQDragHandle, 0, 0, AlignLeft | AlignTop);
    connect(clsQDragHandle, SIGNAL(drag(bool)), this, SLOT(startDragging(bool)));


    lblCaption->setText( clsFEGroup->getGroupName().c_str());
    QToolTip::add( lblCaption, strInfo.c_str() );

    qgly->addWidget (lblCaption, 1, 0, AlignHCenter | AlignVCenter ) ;

//	  setTitle(_clsQlstItemGroup->getGroupID());

    qbtngrpStateVariables = new QButtonGroup(1, QGroupBox::Horizontal, "states", this, "qbtngrpStateVariables");
    qbtngrpStateVariables->setInsideSpacing (1 );

    int ii=0;

    StateVariableList lstNeuronStateVariables = clsFEGroup->getListNeuronStateVariables( );
    iqrcommon::StateVariableList::const_iterator itlstNeuronStateVariables;
    for (itlstNeuronStateVariables = lstNeuronStateVariables.begin();
	 itlstNeuronStateVariables != lstNeuronStateVariables.end(); ++itlstNeuronStateVariables){
	QButton *qrb;

	ClsStateVariable *pState = *itlstNeuronStateVariables;

	if(bExclusiveStates) {
	    qrb = new QRadioButton ( pState->getName().c_str(), qbtngrpStateVariables, "");
	}
	else{
	    qrb = new QCheckBox ( pState->getName().c_str(), qbtngrpStateVariables, "");
	}
	qbtngrpStateVariables->insert(qrb, ii );
	ii++;
    }



    connect(qbtngrpStateVariables, SIGNAL(clicked(int)), SLOT(slotStateVariables(int)));

    qbtngrpStateVariables->setRadioButtonExclusive ( bExclusiveStates );
    qgly->addWidget ( qbtngrpStateVariables, 2, 0, AlignHCenter) ;

    if(bMiniGroupDisplay){
	qLabelMiniGroup = new QLabel(this);

	QPixmap qpm;
	qpm = ClsQMiniGroup::getPixmap(clsFEGroup->getTopology(), strSelectedIndices, qbtngrpStateVariables->sizeHint().width() - 10, 100);
	qLabelMiniGroup->setPixmap(qpm);
	qgly->addWidget(qLabelMiniGroup, 3, 0, AlignCenter  );
	qgly->setRowStretch ( 3, 5 );
    } 
/*
    else {
	qLabelMiniGroup = new QLabel(this);
	qgly->addWidget(qLabelMiniGroup, 3, 0, AlignCenter  );
	qgly->setRowStretch ( 3, 5 );
    }
*/

    if(bAverageEnabled){
	qchkbxAverage = new QCheckBox ( "average", this) ;
	qgly->addWidget ( qchkbxAverage, 4, 0, AlignLeft ) ;
	connect(qchkbxAverage, SIGNAL(toggled(bool)), SLOT(slotAverage(bool)));
    } else {
	qchkbxAverage = NULL;
    }

    qchkbxLiveData = new QCheckBox ( "live data", this) ;
    qgly->addWidget ( qchkbxLiveData, 5, 0, AlignLeft ) ;
    connect(qchkbxLiveData, SIGNAL(toggled(bool)), SLOT(slotLiveData(bool)));

//--    this->setFixedWidth(qbtngrpStateVariables->sizeHint().width());

//    parent->setFixedWidth(parent->width()+this->width());
    this->show();
};


ClsQNeuronStateVariableDisplay::~ClsQNeuronStateVariableDisplay(){
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::~ClsQNeuronStateVariableDisplay()" << endl;
#endif


}

void ClsQNeuronStateVariableDisplay::slotLiveData(bool bToggled) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
   cout << "ClsQNeuronStateVariableDisplay::slotLiveData(bool bToggled)" << endl;
#endif

    /* this can happen if this function is called from outside */
	if(qchkbxLiveData->isChecked() != bToggled){
	    qchkbxLiveData->toggle();
	}

    if(bToggled){
	for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
	    if(qbtngrpStateVariables->find(ii)->state()==2){
		/* send request */
		string strSinkID = qbtngrpStateVariables->find(ii)->text().latin1() + strID;


//--		clsFEDataClient->addDataSink(strSinkID, clsFEGroup->getGroupID(), qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);
		clsFEDataClient->addDataSink(strSinkID, clsFEGroup, qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);
		clsFEDataClient->setSinkMath(strSinkID, ( bAverage ? ClsFEDataSink::MATH_AVERAGE : ClsFEDataSink::MATH_NONE));

		QColor qc;
		qc.setHsv(clsFEDataClient->getDataSinkColor(strSinkID), 255, 210);
		QPalette palette = (qbtngrpStateVariables->find(ii))->palette();
		palette.setColor ( QColorGroup::Foreground, qc );
		(qbtngrpStateVariables->find(ii))->setPalette(palette);
	    }
	}      
    } else {
	for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
	    if(qbtngrpStateVariables->find(ii)->state() == 2){
		/* cancel rewquest */
		string strSinkID = qbtngrpStateVariables->find(ii)->text().latin1() + strID;
		clsFEDataClient->removeDataSink(strSinkID);
		QColor qc;
		qc.setRgb(0,0,0);
		QPalette palette = (qbtngrpStateVariables->find(ii))->palette();
		palette.setColor ( QColorGroup::Foreground, qc );
		(qbtngrpStateVariables->find(ii))->setPalette(palette);
	    }
	}
    }





};

void ClsQNeuronStateVariableDisplay::slotStateVariables(int ii){
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "void ClsQNeuronStateVariableDisplay::slotStateVariables(int ii)" << endl;
#endif

    if(qchkbxLiveData->isOn() && ii != iCurrentStateButton){
	int iState = qbtngrpStateVariables->find(ii)->state();
	if(iState==2){
//	       cout << "send request: " << qbtngrpStateVariables->find(ii)->text() << endl;
	    /* send request */
	    string strSinkID = qbtngrpStateVariables->find(ii)->text().latin1() + strID;

//	       clsFEDataClient->addDataSink(strSinkID, clsQlstItemGroup->getGroupID(), qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);
//--	    clsFEDataClient->addDataSink(strSinkID, clsFEGroup->getGroupID(), qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);
	    clsFEDataClient->addDataSink(strSinkID, clsFEGroup, qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);

	    QColor qc;
	    qc.setHsv(clsFEDataClient->getDataSinkColor(strSinkID), 255, 210);
	    QPalette palette = (qbtngrpStateVariables->find(ii))->palette();
	    palette.setColor ( QColorGroup::Foreground, qc );
	    (qbtngrpStateVariables->find(ii))->setPalette(palette);
	    if(bAverage){
		clsFEDataClient->setSinkMath(strSinkID, ClsFEDataSink::MATH_AVERAGE);
	    }

	    if(qbtngrpStateVariables->isRadioButtonExclusive ( ) && iCurrentStateButton >= 0){
		/* cancel rewquest of the last active checkbox (iCurrentStateButton) */
//		    cout << "cancel request: " << qbtngrpStateVariables->find(iCurrentStateButton)->text() << endl;
//		string strSinkID = strID + qbtngrpStateVariables->find(iCurrentStateButton)->text().latin1();
		string strSinkID = qbtngrpStateVariables->find(iCurrentStateButton)->text().latin1() + strID;
		clsFEDataClient->removeDataSink(strSinkID);
		QColor qc;
		qc.setRgb(0,0,0);
		QPalette palette = (qbtngrpStateVariables->find(iCurrentStateButton))->palette();
		palette.setColor ( QColorGroup::Foreground, qc );
		(qbtngrpStateVariables->find(iCurrentStateButton))->setPalette(palette);
	    }

	} else if(iState == 0){
	    /* cancel rewquest */
//	       cout << "cancel request: " << qbtngrpStateVariables->find(ii)->text() << endl;
	    string strSinkID = qbtngrpStateVariables->find(ii)->text().latin1() + strID;
	    clsFEDataClient->removeDataSink(strSinkID);
	    QColor qc;
	    qc.setRgb(0,0,0);
	    QPalette palette = (qbtngrpStateVariables->find(ii))->palette();
	    palette.setColor ( QColorGroup::Foreground, qc );
	    (qbtngrpStateVariables->find(ii))->setPalette(palette);
	}
    }
    if(qbtngrpStateVariables->isRadioButtonExclusive ( )){
	iCurrentStateButton = ii;
    }
};

void ClsQNeuronStateVariableDisplay::slotAverage(bool bToggled) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "void ClsQNeuronStateVariableDisplay::slotAverage()" << endl;
#endif

    bAverage = bToggled;
    if(bToggled){
//	cerr << "TRUE" << endl;
	for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
//	    cout << "qbtngrpStateVariables->find(ii)->state():" << qbtngrpStateVariables->find(ii)->state() << endl;
	    if(qbtngrpStateVariables->find(ii)->state() == 2){ /* WHAT IS THIS CHECK DOING??? 
								  does this mean, that we can only set average, if one state is selected?*/
//		cout << "clsFEDataClient->setSinkMath(strSinkID, ClsFEDataSink::MATH_AVERAGE)" << endl;
		string strSinkID = qbtngrpStateVariables->find(ii)->text().latin1() + strID;
//		cout << "strSinkID: " << strSinkID << endl;
		clsFEDataClient->setSinkMath(strSinkID, ClsFEDataSink::MATH_AVERAGE);
	    }
	}
    } else {
//	cerr << "FALSE" << endl;
	for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
//	    cout << "qbtngrpStateVariables->find(ii)->state():" << qbtngrpStateVariables->find(ii)->state() << endl;
	    if(qbtngrpStateVariables->find(ii)->state() == 2){  /* WHAT IS THIS CHECK DOING??? -> 
								   does this mean, that we can only set average, if one state is selected?*/
//		cout << "clsFEDataClient->setSinkMath(strSinkID, ClsFEDataSink::MATH_NONE)" << endl;
		string strSinkID = qbtngrpStateVariables->find(ii)->text().latin1() + strID;
//		cout << "strSinkID: " << strSinkID << endl;
		clsFEDataClient->setSinkMath(strSinkID, ClsFEDataSink::MATH_NONE);
	    }
	}
    }
};


void ClsQNeuronStateVariableDisplay::slotClose(){
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::slotClose()" << endl;
#endif

    this->close();
};


void ClsQNeuronStateVariableDisplay::closeEvent( QCloseEvent* ce ) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::closeEvent( QCloseEvent* ce )" << endl;
#endif

    for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
	if(qbtngrpStateVariables->find(ii)->state() == 2){
	    /* cancel rewquest */
	    string strSinkID = qbtngrpStateVariables->find(ii)->text().latin1() + strID;
	    clsFEDataClient->removeDataSink(strSinkID);
	}
    }

    emit sigClose(strID);
    ce->accept();
}


list<string> ClsQNeuronStateVariableDisplay::getSelectedStates() {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::getSelectedStates()" << endl;
#endif
/*
  list<string>::iterator itlst;
  for (itlst = lstSelectedStates.begin(); itlst != lstSelectedStates.end(); ++itlst)
  cout << *itlst << endl;
*/
    list<string> lstSelectedStates;

    for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
//	  cout  << qbtngrpStateVariables->find(ii)->text() << ":" << qbtngrpStateVariables->find(ii)->state() << endl;
	if(qbtngrpStateVariables->find(ii)->state()==2)
	    lstSelectedStates.push_back(qbtngrpStateVariables->find(ii)->text().latin1());
    }
    return lstSelectedStates;
};

string ClsQNeuronStateVariableDisplay::getSelectedStatesAsString() {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::getSelectedStatesAsString()" << endl;
#endif
    string strParamList="";
    for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
	if(qbtngrpStateVariables->find(ii)->state()==2)
	    strParamList = (string)(qbtngrpStateVariables->find(ii)->text().latin1()) + ";" + strParamList;
    }
    return strParamList;
};

void ClsQNeuronStateVariableDisplay::setSelectedStates(list<string> lst) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::setSelectedStates(list<string> lst)" << endl;
#endif
    for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
	string str= (string)(qbtngrpStateVariables->find(ii)->text().latin1());
	if(std::find(lst.begin(), lst.end(), str) != lst.end()){
	    QButton *qrb = qbtngrpStateVariables->find(ii);
	    qrb->toggle();
 
	    string strSinkID = qrb->text().latin1() + strID;
	    QColor qc;	    
	    qc.setHsv(clsFEDataClient->getDataSinkColor(strSinkID), 255, 210);
	    QPalette palette = qrb->palette();
	    palette.setColor ( QColorGroup::Foreground, qc );
//	    palette.setColor ( QColorGroup::ButtonText, qc );
	    qrb->setPalette(palette);
	}
    }
};

void ClsQNeuronStateVariableDisplay::setSelectedStates(string strStates){
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::setSelectedStates(string strStates)" << endl;
#endif
    list<string> lst;
    string::size_type pos;
    while((pos = strStates.find(";")) != string::npos){
	lst.push_back(strStates.substr(0, pos));
	strStates.erase(0, pos+1);
    }
    setSelectedStates(lst);

};

void ClsQNeuronStateVariableDisplay::startDragging(bool bCopy) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::startDragging()" << endl;
#endif
    QTextDrag *d = new QTextDrag( "", this );
    d->setSubtype("iqr-plot");

    string strParamList=getSelectedStatesAsString();
//     string strRange = "";

//    string str = clsFEGroup->getGroupID() + "|" + strParamList + "|" + strSelectedIndices + "|";
    string str = "";
    str = ClsDragDropDeEncoder::encode(ClsFESystemManager::ITEM_GROUP, clsFEGroup->getGroupID(), strParamList, strSelectedIndices);

    d->setText(str.c_str());

    if(bCopy){
	d->dragCopy();
    }
    else {
	d->dragMove();
    }
};

void ClsQNeuronStateVariableDisplay::dragEnterEvent(QDragEnterEvent* event) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::dragEnterEvent(QDragEnterEvent* event)" << endl;
#endif
    if(this->isEnabled())
	event->accept( QTextDrag::canDecode(event) );
}

void ClsQNeuronStateVariableDisplay::dropEvent(QDropEvent* event) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::dropEvent(QDropEvent* event)" << endl;
#endif

    if(event->provides("text/iqr-plot")){
	QTextDrag::decode(event, qstrDropString);
    } else {
	qstrDropString ="";
    }

    if(bAdditiveDisplay){
	QPopupMenu *menuBrowser = new QPopupMenu( this );
	menuBrowser->insertItem( "add", QDropEvent::Copy, -1 );
	menuBrowser->insertItem( "replace", QDropEvent::Move, -1 );

	menuBrowser->popup( this->cursor().pos() );
	connect( menuBrowser, SIGNAL(activated(int)), SLOT(slotDropAction(int)) );
    } else {
	slotDropAction(QDropEvent::Move);
    }
};


/* this is all a terrible hack because I could not get the drop type (Move/Copy) out of the QDropEvent::Action...
   i.e. this:
   if(event->action() == QDropEvent::Move){
   slotClose();
   }
   just doesn't work !!!! (F****) */
void ClsQNeuronStateVariableDisplay::slotDropAction(int iDropAction) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::slotDropAction(int iDropAction)" << endl;
#endif
    if(qstrDropString.length() > 0){

	dropInfo di = ClsDragDropDeEncoder::decode(qstrDropString);
	unsigned int iItemType = di.ItemType;
	if(iItemType == ClsFESystemManager::ITEM_GROUP){
	    string strGroupID = di.ItemID;
	    string strParams = di.Params;
	    string strRange = di.Range;

//--	int ii;
//--	ii = qstrDropString.find("|", 0);
//--	string strGroupID = qstrDropString.mid(0, ii).latin1();
//--	qstrDropString = qstrDropString.mid(ii+1, qstrDropString.length());
//--	ii = qstrDropString.find("|", 0);
//--	string strParams =  qstrDropString.mid(0, ii).latin1();
//--	qstrDropString = qstrDropString.mid(ii+1, qstrDropString.length());
//--	ii = qstrDropString.find("|", 0);
//--	string strRange =   qstrDropString.mid(0, ii).latin1(); //qstrDropString.latin1();
//--

	    list<string> lst;
	    string::size_type pos;
	    while((pos = strParams.find(";")) != string::npos){
		lst.push_back(strParams.substr(0, pos));
		strParams.erase(0, pos+1);
	    }

	    emit addDisplay(strGroupID , lst, strRange);

	    if(iDropAction == QDropEvent::Move){
		slotClose();
	    }
	}
    }
};

ClsStateVariableDisplayConfig ClsQNeuronStateVariableDisplay::getConfig(){
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::getConfig()" << endl;
#endif

    /* caution: this is order dependent! first set the selected states, _then_ set live */

    ClsStateVariableDisplayConfig clsStateVariableDisplayConfig(/*ConfigTagLibrary::GroupType(), */strID, clsFEGroup->getGroupID(), strSelectedIndices);
    pair<string,string> p03("Average", iqrUtils::bool2string(bAverage));
    clsStateVariableDisplayConfig.addParameter(p03);

    pair<string,string> p04("SelectedStates", getSelectedStatesAsString());
    clsStateVariableDisplayConfig.addParameter(p04);

    pair<string,string> p05("Live", iqrUtils::bool2string(qchkbxLiveData->isChecked ()));
    clsStateVariableDisplayConfig.addParameter(p05);


    return clsStateVariableDisplayConfig;
}

void ClsQNeuronStateVariableDisplay::setConfig(ClsStateVariableDisplayConfig clsStateVariableDisplayConfig){
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::setConfig(ClsStateVariableDisplayConfig clsStateVariableDisplayConfig)" << endl;
#endif

    /* caution: this is order dependent! first read the selected states, _then_ check for live */

    bool bConfigAverage = false;
    bool bConfigLiveData = false;

    list<pair<string, string> > lstParameters = clsStateVariableDisplayConfig.getListParameters();
    list<pair<string, string> >::iterator itLstParameters;
    for(itLstParameters = lstParameters.begin(); itLstParameters != lstParameters.end(); itLstParameters++){
 	string strParamName = (*itLstParameters).first;
 	string strParamValue = (*itLstParameters).second;
 	
 	if(!strParamName.compare("Average")){
 	    bConfigAverage = iqrUtils::string2bool(strParamValue);
 	}
 	else if(!strParamName.compare("SelectedStates")){
 	    setSelectedStates(strParamValue);
 	}
 	else if(!strParamName.compare("Live")){
	    bConfigLiveData = iqrUtils::string2bool(strParamValue);
 	}
 
    }


    if(bConfigLiveData){
	slotLiveData(bConfigLiveData);
	qchkbxLiveData->setChecked(bConfigLiveData);
    }

    if(qchkbxAverage!=NULL){
	/* needs to happen after setting the selected states */
	qchkbxAverage->setChecked(bConfigAverage);
	/* the above call triggers:     slotAverage(bConfigAverage);*/
    }
}

void ClsQNeuronStateVariableDisplay::groupChanged(){
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::groupChanged()" << endl;
#endif
    if(qLabelMiniGroup!=NULL){
	QPixmap qpm;
	qpm = ClsQMiniGroup::getPixmap(clsFEGroup->getTopology(), strSelectedIndices, qbtngrpStateVariables->sizeHint().width() - 10, 100);
	qLabelMiniGroup->setPixmap(qpm);
    }

/* change label */
    string strInfo;
    strInfo = "Group ID:\t" + clsFEGroup->getGroupID();
    strInfo = strInfo + "\nGroup name:\t" + clsFEGroup->getGroupName();
    strInfo = strInfo + "\nNeuron type:\t" + clsFEGroup->getGroupNeuronType();
    strInfo = strInfo + "\nGroup width:\t" + iqrUtils::int2string(clsFEGroup->getNrCellsHorizontal());
    strInfo = strInfo + "\nGroup height:\t" + iqrUtils::int2string(clsFEGroup->getNrCellsVertical());
    
    lblCaption->setText( clsFEGroup->getGroupName().c_str());
    QToolTip::add( lblCaption, strInfo.c_str() );

    /* save selections */
    list<string> lstSelectedState = getSelectedStates();

/* remove all buttons from the QButtonGroup */
    int iCount = qbtngrpStateVariables->count();
    for(int ii=0; ii<iCount; ii++){
	QButton* bb = qbtngrpStateVariables->find(ii);
	if(bb!=NULL){
	    qbtngrpStateVariables->remove(bb);
	    delete bb;
	}
    }

/* rebuild state list */
    int ii=0;

    StateVariableList lstNeuronStateVariables = clsFEGroup->getListNeuronStateVariables( );
    iqrcommon::StateVariableList::const_iterator itlstNeuronStateVariables;
    for (itlstNeuronStateVariables = lstNeuronStateVariables.begin();
	 itlstNeuronStateVariables != lstNeuronStateVariables.end(); ++itlstNeuronStateVariables){
	QButton *qrb;
	ClsStateVariable *pState = *itlstNeuronStateVariables;

	if(bExclusiveStates) {
	    qrb = new QRadioButton ( pState->getName().c_str(), qbtngrpStateVariables, "");
	}
	else{
	    qrb = new QCheckBox ( pState->getName().c_str(), qbtngrpStateVariables, "");
	}
	
 	qrb->show();
	qbtngrpStateVariables->insert(qrb, ii );
	ii++;
    }
    qbtngrpStateVariables->setFixedHeight(qbtngrpStateVariables->sizeHint().height());
    setSelectedStates(lstSelectedState);
}





//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:






