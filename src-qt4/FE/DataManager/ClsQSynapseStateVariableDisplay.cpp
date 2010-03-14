/****************************************************************************
 ** $Filename: ClsQSynapseStateVariableDisplay.cpp
 ** $Header:
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate:
 ** $Date:
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <algorithm>

#include <QLabel>
#include <QWidget>
#include <QCloseEvent>

#include "ClsQSynapseStateVariableDisplay.h"
#include <ClsParameter.h>
#include <stateVariableList.hpp>
#include <stateVariable.hpp>

#include "ClsTopologySparse.h"
#include "ClsTopologyHex.h"
#include "ClsTopologyRect.h"

#include "iqrUtils.h"

//#define DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY


using namespace iqrcommon;

//--ClsExclusifier::ClsExclusifier(){
//--    iRecursion = 0;
//--}
//--void ClsExclusifier::addButton(QRadioButton* qrb, int iID) {
//--    connect(qrb, SIGNAL(toggled(bool)), this, SLOT(buttonClicked(bool)));
//--//    list.append(qrb);
//--    mapButtons[iID] = qrb;
//--}
//--
//--
//--void ClsExclusifier::checkButton(int iID){
//--    if(mapButtons.find(iID)!=mapButtons.end()){
//--	mapButtons.find(iID).data()->setChecked(true);
//--    }
//--}
//--
//--void ClsExclusifier::toggleButton(int iID){
//--    if(mapButtons.find(iID)!=mapButtons.end()){
//--	mapButtons.find(iID).data()->toggle();
//--    }
//--}
//--
//--bool ClsExclusifier::isChecked(int iID){
//--    if(mapButtons.find(iID)!=mapButtons.end()){
//--	return mapButtons.find(iID).data()->isChecked();
//--    }
//--    return false;
//--}
//--
//--
//--void ClsExclusifier::buttonClicked(bool bToggled){
//--//    cout << "ClsExclusifier::buttonClicked(int iState)" << endl;
//--
//--    if(iRecursion==0){
//--	iRecursion++;
//--	QRadioButton *qrb = (QRadioButton *)sender();
//--	int iSignal= -99;
//--	/* iState = new State */
//--//	cout << "State: " << iState << endl;
//--
//--//--	if(!qrb->isChecked()){
//--//--	    qrb->setChecked(true);
//--//--	} 
//--	else /*zzzif(iState==QButton::On)*/{
//--	    QMap<int, QRadioButton*>::Iterator it;
//--	    for ( it = mapButtons.begin(); it != mapButtons.end(); ++it ) {
//--//--		if(it.data()!=qrb ){
//--//--		    if(it.data()->isOn()){
//--//--			it.data()->setChecked(false);
//--//--		    }
//--//--		} else {
//--		if(it.data()==qrb ){
//--		    iSignal = it.key();
//--		}
//--	    }
//--	}
//--	if(iSignal>=0){
//--//	    cout << "Signal: " << iSignal << endl;
//--	    emit sigClicked(iSignal);
//--	}
//--    }
//--    iRecursion = 0;
//--}




ClsQSynapseStateVariableDisplay::ClsQSynapseStateVariableDisplay(ClsFEDataClient *_clsFEDataClient,
								 string _strID, QWidget *_parent,
								 ClsFEConnection* _clsFEConnection,
								 string _strSelectedIndices) :
    ClsQBaseStateVariableDisplay(_clsFEDataClient, _strID, _parent), clsFEConnection(_clsFEConnection),
    strSelectedIndices(_strSelectedIndices) {



#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::ClsQSynapseStateVariableDisplay(...)" << endl;
#endif

//    cout << "ClsQSynapseStateVariableDisplay::strID: " << strID << endl;

    iCurrentStateButton = -99;
    setAcceptDrops(TRUE);

    QVBoxLayout *qgly = new QVBoxLayout ( this, 2, 0, "qgly");
    qgly->setContentsMargins ( 0,0,0, 0);


    string strInfo;
//    strInfo = "Connection ID:\t" + clsFEConnection->getConnectionID();
    strInfo = strInfo + "Connection name:\t" + clsFEConnection->getConnectionName();
    strInfo = strInfo + "\nSynapse type:\t" + clsFEConnection->getConnectionSynapseType();


    lblCaption->setText( clsFEConnection->getConnectionName().c_str());
    QToolTip::add( lblCaption, strInfo.c_str() );

    qgly->addWidget (lblCaption) ;



    qgrpbxDisplay = new QGroupBox("display");
    QVBoxLayout *qlayoutDisplay = new QVBoxLayout;
    qlayoutDisplay->setSpacing ( 0 );
    qlayoutDisplay->setContentsMargins ( 0,0,0, 0);
    qgrpbxDisplay->setLayout(qlayoutDisplay);


    QFrame* qfmDummy = new QFrame();
    qlayoutDisplay->addWidget(qfmDummy);



    QGridLayout *qlyGB = new QGridLayout ( qfmDummy, 6, 2, 0 ,-1, "qlyGB");
//only whith QT>3.1.1    qlyGB->setColSpacing ( 0, 10 );

    QRadioButton *qrbDistance = new QRadioButton ("Distance", qfmDummy );
    QRadioButton *qrbDelay = new QRadioButton ("Delay", qfmDummy );
    QRadioButton *qrbAttenuation = new QRadioButton ("Attenuation", qfmDummy );
    QRadioButton *qrbSynapseState = new QRadioButton ("Synapse", qfmDummy );
    qrbDistance->setChecked( TRUE );
 
    QButtonGroup* qbtngrpStateVariable = new QButtonGroup();
    qbtngrpStateVariable->addButton(qrbDistance, ClsQSynapseStateVariableDisplay::TYPE_DISTANCE);
    qbtngrpStateVariable->addButton(qrbDelay, ClsQSynapseStateVariableDisplay::TYPE_DELAY);
    qbtngrpStateVariable->addButton(qrbAttenuation, ClsQSynapseStateVariableDisplay::TYPE_ATTENUATION);
    qbtngrpStateVariable->addButton(qrbSynapseState, ClsQSynapseStateVariableDisplay::TYPE_SYNAPSESTATE);
    connect( qbtngrpStateVariable, SIGNAL(buttonClicked(int)), SLOT(slotSetType(int)) );

    qlyGB->addRowSpacing (0, 5);
    qlyGB->addMultiCellWidget (qrbDistance, 0, 0, 0, 1);
    qlyGB->addMultiCellWidget (qrbDelay, 1, 1, 0, 1);
    qlyGB->addMultiCellWidget (qrbAttenuation, 2, 2, 0, 1);
    qlyGB->addMultiCellWidget (qrbSynapseState, 3, 3, 0, 1);

    qgly->addWidget (qgrpbxDisplay);

    qgrpbxStateVariables = new QGroupBox("states");
    qbtngrpStateVariables = new QButtonGroup();

    qlayoutStateVariables = new QVBoxLayout;
    qlayoutStateVariables->setSpacing ( 0 );
    qlayoutStateVariables->setContentsMargins ( 0,0,0, 0);

    qgrpbxStateVariables->setLayout(qlayoutStateVariables);

    qlyGB->addWidget (qgrpbxStateVariables, 4, 1);

    int ii=0;
    StateVariableList lstSynapseStateVariables = clsFEConnection->getListSynapseStateVariables( );
    iqrcommon::StateVariableList::const_iterator itlstSynapseStateVariables;
    for (itlstSynapseStateVariables = lstSynapseStateVariables.begin();
 	 itlstSynapseStateVariables != lstSynapseStateVariables.end(); ++itlstSynapseStateVariables){
 	QAbstractButton *qrb;
 	ClsStateVariable *pState = *itlstSynapseStateVariables;
 	qrb = new QRadioButton ( pState->getName().c_str());
	qbtngrpStateVariables->addButton(qrb, ii );
	qlayoutStateVariables->addWidget(qrb);

 	ii++;
    }
    connect(qbtngrpStateVariables, SIGNAL(buttonClicked(int)), SLOT(slotStateVariables(int)));

    qbtngrpStateVariables->setExclusive ( true );

    qchkbxLiveData = new QCheckBox ( "live data", qfmDummy) ;
    qchkbxLiveData->setEnabled(false);
    qlyGB->addWidget (qchkbxLiveData, 5, 1);
    connect(qchkbxLiveData, SIGNAL(toggled(bool)), SLOT(slotLiveData(bool)));

    this->show();
    };


ClsQSynapseStateVariableDisplay::~ClsQSynapseStateVariableDisplay(){
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::~ClsQSynapseStateVariableDisplay()" << endl;
#endif
}



void ClsQSynapseStateVariableDisplay::slotLiveData(bool bToggled) {
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::slotLiveData(bool bToggled)" << endl;
#endif

    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;

    if(bToggled){
	for (it = lst.begin(); it != lst.end(); ++it) {
	    if((*it)->isChecked()/*state()==2*/){
		/* send request */
		string strSinkID = (*it)->text().latin1() + strID;
		clsFEDataClient->addDataSink(strSinkID, clsFEConnection, (*it)->text().latin1(), strSelectedIndices);

		QColor qc;
		qc.setHsv(clsFEDataClient->getDataSinkColor(strSinkID), 255, 210);
		QPalette palette = (*it)->palette();
		palette.setColor ( QColorGroup::Foreground, qc );
		(*it)->setPalette(palette);
		if(qbtngrpStateVariables->exclusive ( )){
		    iCurrentStateButton = qbtngrpStateVariables->id((*it)); 
		}
	    }
	}      
    } else {
	for (it = lst.begin(); it != lst.end(); ++it) {

	    if((*it)->isChecked()/*state() == 2*/){
		/* cancel rewquest */
		string strSinkID = (*it)->text().latin1() + strID;
		clsFEDataClient->removeDataSink(strSinkID);
		QColor qc;
		qc.setRgb(0,0,0);
		QPalette palette = (*it)->palette();
		palette.setColor ( QColorGroup::Foreground, qc );
		(*it)->setPalette(palette);
	    }
	}
    }
    emit sigLiveData(bToggled);
};

void ClsQSynapseStateVariableDisplay::slotStateVariables(int ii){
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "void ClsQSynapseStateVariableDisplay::slotStateVariables(int ii)" << endl;
#endif

    QAbstractButton * btn = qbtngrpStateVariables->button ( ii );
    if(btn==NULL){
	return;
    }
    
    if(qchkbxLiveData->isOn() && ii != iCurrentStateButton){
	if(btn->isChecked()){
//	       cout << "send request: " << qbtngrpStateVariables->find(ii)->text() << endl;
	    /* send request */
	    string strSinkID = btn->text().latin1() + strID;

	    clsFEDataClient->addDataSink(strSinkID, clsFEConnection, btn->text().latin1(), strSelectedIndices);

	    QColor qc;
	    qc.setHsv(clsFEDataClient->getDataSinkColor(strSinkID), 255, 210);
	    QPalette palette = (btn)->palette();
	    palette.setColor ( QColorGroup::Foreground, qc );
	    (btn)->setPalette(palette);

	    if(qbtngrpStateVariables->exclusive ( ) && iCurrentStateButton >= 0){
		/* cancel rewquest of the last active checkbox (iCurrentStateButton) */
//		    cout << "cancel request: " << qbtngrpStateVariables->find(iCurrentStateButton)->text() << endl;
		QAbstractButton *btnCurrent =  qbtngrpStateVariables->button (iCurrentStateButton);
		if(btnCurrent!=NULL){
		    string strSinkID = strID + btnCurrent->text().latin1();
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
//	       cout << "cancel request: " << btn->text() << endl;
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



void ClsQSynapseStateVariableDisplay::slotClose(){
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::slotClose()" << endl;
#endif

    this->close();
};


void ClsQSynapseStateVariableDisplay::closeEvent( QCloseEvent* ce ) {
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::closeEvent( QCloseEvent* ce )" << endl;
#endif

    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;
    for (it = lst.begin(); it != lst.end(); ++it) {
	if((*it)->isChecked()/*state() == 2*/){
	    /* cancel rewquest */
	    string strSinkID = (*it)->text().latin1() + strID;
	    clsFEDataClient->removeDataSink(strSinkID);
	}
    }
    emit sigClose(strID);
    ce->accept();
}


list<string> ClsQSynapseStateVariableDisplay::getSelectedStates() {
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::getSelectedStates()" << endl;
#endif/*
	list<string>::iterator itlst;
	for (itlst = lstSelectedStates.begin(); itlst != lstSelectedStates.end(); ++itlst)
	cout << *itlst << endl;
      */
    list<string> lstSelectedStates;

    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;
    for (it = lst.begin(); it != lst.end(); ++it) {
	if((*it)->isChecked()/*state()==2*/)
	    lstSelectedStates.push_back((*it)->text().latin1());
    }
    return lstSelectedStates;
};

string ClsQSynapseStateVariableDisplay::getSelectedStatesAsString() {
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::getSelectedStatesAsString()" << endl;
#endif
    string strParamList="";
    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;
    for (it = lst.begin(); it != lst.end(); ++it) {
	if((*it)->isChecked()/*state()==2*/)
	    strParamList = (string)((*it)->text().latin1()) + ";" + strParamList;
    }
    return strParamList;
};

void ClsQSynapseStateVariableDisplay::setSelectedStates(list<string> lstStates) {
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::setSelectedStates(list<string> lst)" << endl;
#endif
    QList<QAbstractButton *> lst = qbtngrpStateVariables->buttons ();
    QList<QAbstractButton*>::iterator it;
    for (it = lst.begin(); it != lst.end(); ++it) {
	string str= (string)((*it)->text().latin1());
	if(std::find(lstStates.begin(), lstStates.end(), str) != lstStates.end()){
	    (*it)->toggle();
	}
    }
};

void ClsQSynapseStateVariableDisplay::setSelectedStates(string strStates){
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::setSelectedStates(string strStates)" << endl;
#endif
    list<string> lst;
    string::size_type pos;
    while((pos = strStates.find(";")) != string::npos){
	lst.push_back(strStates.substr(0, pos));
	strStates.erase(0, pos+1);
    }
    setSelectedStates(lst);

};


ClsStateVariableDisplayConfig ClsQSynapseStateVariableDisplay::getConfig(){
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::getConfig()" << endl;
#endif

    /* caution: this is order dependent! first set the selected states, _then_ set live */

    ClsStateVariableDisplayConfig clsStateVariableDisplayConfig(/*ConfigTagLibrary::ConnectionType(), */strID, clsFEConnection->getConnectionID(), strSelectedIndices);

    pair<string,string> p04("SelectedStates", getSelectedStatesAsString());
    clsStateVariableDisplayConfig.addParameter(p04);
    pair<string,string> p05("Live", iqrUtils::bool2string(qchkbxLiveData->isChecked ()));
    clsStateVariableDisplayConfig.addParameter(p05);

    return clsStateVariableDisplayConfig;
}

void ClsQSynapseStateVariableDisplay::setConfig(ClsStateVariableDisplayConfig clsStateVariableDisplayConfig){
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::setConfig(ClsStateVariableDisplayConfig clsStateVariableDisplayConfig)" << endl;
#endif

    /* caution: this is order dependent! first read the selected states, _then_ check for live */

    list<pair<string, string> > lstParameters = clsStateVariableDisplayConfig.getListParameters();
    list<pair<string, string> >::iterator itLstParameters;
    for(itLstParameters = lstParameters.begin(); itLstParameters != lstParameters.end(); itLstParameters++){
	string strParamName = (*itLstParameters).first;
	string strParamValue = (*itLstParameters).second;
	
	if(!strParamName.compare("SelectedStates")){
	    setSelectedStates(strParamValue);
	}
	else if(!strParamName.compare("Live")){
	    slotLiveData(iqrUtils::string2bool(strParamValue));
	    qchkbxLiveData->setChecked(iqrUtils::string2bool(strParamValue));
	}

    }
}

void ClsQSynapseStateVariableDisplay::connectionChanged(){
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::connectionChanged()" << endl;
#endif

/* change label */
    string strInfo;
    strInfo = "Connection ID:\t" + clsFEConnection->getConnectionID();
    strInfo = strInfo + "\nConnection name:\t" + clsFEConnection->getConnectionName();
    strInfo = strInfo + "\nSynapse type:\t" + clsFEConnection->getConnectionSynapseType();

    lblCaption->setText( clsFEConnection->getConnectionName().c_str());
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
    StateVariableList lstSynapseStateVariables = clsFEConnection->getListSynapseStateVariables( );
    iqrcommon::StateVariableList::const_iterator itlstSynapseStateVariables;
    for (itlstSynapseStateVariables = lstSynapseStateVariables.begin();
	 itlstSynapseStateVariables != lstSynapseStateVariables.end(); ++itlstSynapseStateVariables){
	QAbstractButton *qrb;
	ClsStateVariable *pState = *itlstSynapseStateVariables;
	qrb = new QRadioButton ( pState->getName().c_str());
//	qrb->setGeometry( 5, ii*qrb->sizeHint().height()+15, qrb->sizeHint().width(), qrb->sizeHint().height() );
	qrb->show();
	qbtngrpStateVariables->addButton(qrb, ii );
	qlayoutStateVariables->addWidget(qrb);
	ii++;
    }
    qgrpbxStateVariables->setFixedHeight(qgrpbxStateVariables->sizeHint().height());
    setSelectedStates(lstSelectedState);
}


void ClsQSynapseStateVariableDisplay::slotSetType(int ii){
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::slotSetType(int ii)" << endl;
#endif

    if(ii!=ClsQSynapseStateVariableDisplay::TYPE_SYNAPSESTATE){
	if(qchkbxLiveData->isChecked()){
	    qchkbxLiveData->toggle();
	}
	qchkbxLiveData->setEnabled(false);
    } else {
	qchkbxLiveData->setEnabled(true);
    }


    emit sigChangeType(ii);
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
