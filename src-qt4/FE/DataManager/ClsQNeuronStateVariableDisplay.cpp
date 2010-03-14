/****************************************************************************
 ** $Filename: ClsQNeuronStateVariableDisplay.cpp
 **
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

#include <QLabel>
#include <QWidget>
#include <QMenu>

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
	qgly->addWidget(qpbtnClose, 0, 0, Qt::AlignRight | Qt::AlignTop);
	connect(qpbtnClose, SIGNAL(clicked()), SLOT(slotClose()));
    }

    ClsQDragHandle* clsQDragHandle = new ClsQDragHandle(this);
    qgly->addWidget(clsQDragHandle, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    connect(clsQDragHandle, SIGNAL(drag(bool)), this, SLOT(startDragging(bool)));


    lblCaption->setText( clsFEGroup->getGroupName().c_str());
    QToolTip::add( lblCaption, strInfo.c_str() );

    qgly->addWidget (lblCaption, 1, 0, Qt::AlignHCenter | Qt::AlignVCenter ) ;

//	  setTitle(_clsQlstItemGroup->getGroupID());


//-------------------------------
    qgrpbxStateVariables = new QGroupBox("states");

    qbtngrpStateVariables = new QButtonGroup();
    qlayoutStateVariables = new QVBoxLayout;
    qlayoutStateVariables->setSpacing ( 0 );
    qlayoutStateVariables->setContentsMargins ( 0,0,0, 0);
    qgrpbxStateVariables->setLayout(qlayoutStateVariables);


    int ii=0;

    StateVariableList lstNeuronStateVariables = clsFEGroup->getListNeuronStateVariables( );
    iqrcommon::StateVariableList::const_iterator itlstNeuronStateVariables;
    for (itlstNeuronStateVariables = lstNeuronStateVariables.begin();
	 itlstNeuronStateVariables != lstNeuronStateVariables.end(); ++itlstNeuronStateVariables){

	QAbstractButton *qrb;
	ClsStateVariable *pState = *itlstNeuronStateVariables;

	if(bExclusiveStates) {
	    qrb = new QRadioButton ( pState->getName().c_str());
	}
	else{
	    qrb = new QCheckBox ( pState->getName().c_str());
	}
 	qrb->show();
	qbtngrpStateVariables->addButton(qrb, ii );
	qlayoutStateVariables->addWidget(qrb);

	ii++;
    }



    connect(qbtngrpStateVariables, SIGNAL(buttonClicked(int)), SLOT(slotStateVariables(int)));
    qbtngrpStateVariables->setExclusive ( bExclusiveStates );

//--------------------------

    qgly->addWidget ( qgrpbxStateVariables, 2, 0, Qt::AlignHCenter) ;

    if(bMiniGroupDisplay){
	qLabelMiniGroup = new QLabel(this);

	QPixmap qpm;
	qpm = ClsQMiniGroup::getPixmap(clsFEGroup->getTopology(), strSelectedIndices, qgrpbxStateVariables->sizeHint().width() - 10, 100);
	qLabelMiniGroup->setPixmap(qpm);
	qgly->addWidget(qLabelMiniGroup, 3, 0, Qt::AlignCenter  );
	qgly->setRowStretch ( 3, 5 );
    } 

    if(bAverageEnabled){
	qchkbxAverage = new QCheckBox ( "average", this) ;
	qgly->addWidget ( qchkbxAverage, 4, 0, Qt::AlignLeft ) ;
	connect(qchkbxAverage, SIGNAL(toggled(bool)), SLOT(slotAverage(bool)));
    } else {
	qchkbxAverage = NULL;
    }

    qchkbxLiveData = new QCheckBox ( "live data", this) ;
    qgly->addWidget ( qchkbxLiveData, 5, 0, Qt::AlignLeft ) ;
    qchkbxLiveData->setChecked( true); /* should on initially.... */
    connect(qchkbxLiveData, SIGNAL(toggled(bool)), SLOT(slotLiveData(bool)));

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
   
   
    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;
   
    if(bToggled){
	for (it = lst.begin(); it != lst.end(); ++it) {
	    if((*it)->isChecked()){
		/* send request */
		string strSinkID = (*it)->text().latin1() + strID;
		clsFEDataClient->addDataSink(strSinkID, clsFEGroup, (*it)->text().latin1(), strSelectedIndices);
		clsFEDataClient->setSinkMath(strSinkID, ( bAverage ? ClsFEDataSink::MATH_AVERAGE : ClsFEDataSink::MATH_NONE));
		QColor qc;
		qc.setHsv(clsFEDataClient->getDataSinkColor(strSinkID), 255, 210);
		QPalette palette = ((*it))->palette();
		palette.setColor ( QColorGroup::Foreground, qc );
		((*it))->setPalette(palette);
		if(qbtngrpStateVariables->exclusive ( )){
		    iCurrentStateButton = qbtngrpStateVariables->id((*it)); 
		}
	    }
	}      
    } else {
	for (it = lst.begin(); it != lst.end(); ++it) {
	    if((*it)->isChecked()){
		string strSinkID = (*it)->text().latin1() + strID;
		clsFEDataClient->removeDataSink(strSinkID);
		QColor qc;
		qc.setRgb(0,0,0);
		QPalette palette = ((*it))->palette();
		palette.setColor ( QColorGroup::Foreground, qc );
		((*it))->setPalette(palette);
	    }
	}
    }
};

void ClsQNeuronStateVariableDisplay::slotStateVariables(int ii){
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "void ClsQNeuronStateVariableDisplay::slotStateVariables(int ii)" << endl;
#endif

    /*
    cout << "qbtngrpStateVariables->exclusive ( ): " << qbtngrpStateVariables->exclusive ( ) << endl;
    cout << "iCurrentStateButton: " << iCurrentStateButton << endl;
    cout << "ii: " << ii << endl;
    */

    QAbstractButton *btn = qbtngrpStateVariables->button ( ii );
    if(btn==NULL){
	return;
    } 
   

    if(qchkbxLiveData->isOn() && ii != iCurrentStateButton){
	if(btn->isChecked()){
	    //cout << "send request: " << btn->text().latin1() << endl;
	    /* send request */
	    string strSinkID = btn->text().latin1() + strID;

	    clsFEDataClient->addDataSink(strSinkID, clsFEGroup, btn->text().latin1(), strSelectedIndices);

	    QColor qc;
	    qc.setHsv(clsFEDataClient->getDataSinkColor(strSinkID), 255, 210);
	    QPalette palette = (btn)->palette();
	    palette.setColor ( QColorGroup::Foreground, qc );
	    (btn)->setPalette(palette);
	    if(bAverage){
		clsFEDataClient->setSinkMath(strSinkID, ClsFEDataSink::MATH_AVERAGE);
	    }

	    if(qbtngrpStateVariables->exclusive( ) && iCurrentStateButton >= 0){
		/* cancel rewquest of the last active checkbox (iCurrentStateButton) */
		QAbstractButton *btnCurrent =  qbtngrpStateVariables->button (iCurrentStateButton);
		
		if(btnCurrent!=NULL){
		    // cout << "cancel request: " << btnCurrent->text().latin1() << endl;
		    string strSinkID = btnCurrent->text().latin1() + strID;
		    clsFEDataClient->removeDataSink(strSinkID);
		    QColor qc;
		    qc.setRgb(0,0,0);

		    QPalette palette = btnCurrent->palette();
		    palette.setColor ( QColorGroup::Foreground, qc );
		    btnCurrent->setPalette(palette);
		}
	    }
	} else {
	    /* cancel request */
	    // cout << "cancel request: " << btn->text().latin1() << endl;
	    string strSinkID = btn->text().latin1() + strID;
	    clsFEDataClient->removeDataSink(strSinkID);
	    QColor qc;
	    qc.setRgb(0,0,0);
	    QPalette palette = (btn)->palette();
	    palette.setColor ( QColorGroup::Foreground, qc );
	    (btn)->setPalette(palette);
	}
    } 
    if(qbtngrpStateVariables->exclusive ( )){
	iCurrentStateButton = ii;
    }
};

void ClsQNeuronStateVariableDisplay::slotAverage(bool bToggled) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "void ClsQNeuronStateVariableDisplay::slotAverage()" << endl;
#endif

    bAverage = bToggled;
    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;

    if(bToggled){
//	cerr << "TRUE" << endl;

	for (it = lst.begin(); it != lst.end(); ++it) {
//	    cout << "(*it)->state():" << (*it)->state() << endl;
	    if((*it)->isChecked()/*state() == 2*/){ /* WHAT IS THIS CHECK DOING??? 
						       does this mean, that we can only set average, if one state is selected?*/
//		cout << "clsFEDataClient->setSinkMath(strSinkID, ClsFEDataSink::MATH_AVERAGE)" << endl;
		string strSinkID = (*it)->text().latin1() + strID;
//		cout << "strSinkID: " << strSinkID << endl;
		clsFEDataClient->setSinkMath(strSinkID, ClsFEDataSink::MATH_AVERAGE);
	    }
	}
    } else {
//	cerr << "FALSE" << endl;

	for (it = lst.begin(); it != lst.end(); ++it) {
//	    cout << "(*it)->state():" << (*it)->state() << endl;
	    if((*it)->isChecked()/*zzzstate() == 2*/){  /* WHAT IS THIS CHECK DOING??? -> 
							   does this mean, that we can only set average, if one state is selected?*/
//		cout << "clsFEDataClient->setSinkMath(strSinkID, ClsFEDataSink::MATH_NONE)" << endl;
		string strSinkID = (*it)->text().latin1() + strID;
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

    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;

    for (it = lst.begin(); it != lst.end(); ++it) {
	if((*it)->isChecked()/*zzzstate() == 2*/){
	    /* cancel rewquest */
	    string strSinkID = (*it)->text().latin1() + strID;
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

    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;

    for (it = lst.begin(); it != lst.end(); ++it) {
//	  cout  << (*it)->text() << ":" << (*it)->state() << endl;
	if((*it)->/*state()==2*/isChecked())
	    lstSelectedStates.push_back((*it)->text().latin1());
    }
    return lstSelectedStates;
};

string ClsQNeuronStateVariableDisplay::getSelectedStatesAsString() {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::getSelectedStatesAsString()" << endl;
#endif
    string strParamList="";
    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;

    for (it = lst.begin(); it != lst.end(); ++it) {
	if((*it)->/*state()==2*/isChecked())
	    strParamList = (string)((*it)->text().latin1()) + ";" + strParamList;
    }
    return strParamList;
};

void ClsQNeuronStateVariableDisplay::setSelectedStates(list<string> lstStates) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::setSelectedStates(list<string> lst)" << endl;
#endif

    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;

    for (it = lst.begin(); it != lst.end(); ++it) {
	string str= (string)((*it)->text().latin1());
	if(std::find(lstStates.begin(), lstStates.end(), str) != lstStates.end()){
	    QAbstractButton *qrb = (*it);
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

    QDrag *drag = new QDrag(this);

    string strParamList=getSelectedStatesAsString();
    string str = "";
    str = ClsDragDropDeEncoder::encode(ClsFESystemManager::ITEM_GROUP, clsFEGroup->getGroupID(), strParamList, strSelectedIndices);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("text/iqr-plot", str.c_str());

    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec();

};


void ClsQNeuronStateVariableDisplay::dragEnterEvent(QDragEnterEvent* event) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::dragEnterEvent(QDragEnterEvent* event)" << endl;
#endif
    if(event->mimeData()->hasFormat ("text/iqr-plot" )){
	event->acceptProposedAction();
    }
}

void ClsQNeuronStateVariableDisplay::dropEvent(QDropEvent* event) {
//#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::dropEvent(QDropEvent* event)" << endl;
//#endif

    if(event->mimeData()->hasFormat ("text/iqr-plot" )){
	qstrDropString = event->mimeData()->data ("text/iqr-plot");
    } else {
	qstrDropString ="";
    }

    if(bAdditiveDisplay){
	QMenu* qmenu = new QMenu(this);

	QAction *qactAdd = new QAction("add", this);
	connect(qactAdd, SIGNAL(triggered()), this, SLOT(slotDropActionAdd()));
	qmenu->addAction(qactAdd);

	QAction *qactReplace = new QAction("replace", this);
	connect(qactReplace, SIGNAL(triggered()), this, SLOT(slotDropActionReplace()));
	qmenu->addAction(qactReplace);

	qmenu->popup( this->cursor().pos() );
    } else {
	slotDropActionReplace();
    }
};

void ClsQNeuronStateVariableDisplay::slotDropActionAdd(){
//#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::slotDropActionAdd()" << endl;
//#endif
    if(qstrDropString.length() > 0){
	dropInfo di = ClsDragDropDeEncoder::decode(qstrDropString.toStdString());
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
	    emit addDisplay(strGroupID , lst, strRange);
	}
    }
}

void ClsQNeuronStateVariableDisplay::slotDropActionReplace(){
//#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsQNeuronStateVariableDisplay::slotDropActionReplace()" << endl;
//#endif

    if(qstrDropString.length() > 0){
	slotDropActionAdd();
	slotClose();
    }
}



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
	qpm = ClsQMiniGroup::getPixmap(clsFEGroup->getTopology(), strSelectedIndices, qgrpbxStateVariables->sizeHint().width() - 10, 100);
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
    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;


    for (it = lst.begin(); it != lst.end(); ++it) {
	QAbstractButton* bb = (*it);
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

	QAbstractButton *qrb;
	ClsStateVariable *pState = *itlstNeuronStateVariables;

	if(bExclusiveStates) {
	    qrb = new QRadioButton ( pState->getName().c_str());
	}
	else{
	    qrb = new QCheckBox ( pState->getName().c_str());
	}
	
 	qrb->show();
	qbtngrpStateVariables->addButton(qrb, ii );
	qlayoutStateVariables->addWidget(qrb);

	ii++;
    }

//    qgrpbxStateVariables->setFixedHeight(qgrpbxStateVariables->sizeHint().height());
    setSelectedStates(lstSelectedState);
}





//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:







