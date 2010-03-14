/****************************************************************************
 ** $Filename: ClsQSynapseStateVariableDisplay.cpp
 ** $Header:
 **
 ** Copyright (C) 2003- by Ulysses Bernardet
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
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qwidget.h>

#include "ClsQSynapseStateVariableDisplay.h"
#include <ClsParameter.h>
#include <stateVariableList.hpp>
#include <stateVariable.hpp>

#include "ClsTopologySparse.h"
#include "ClsTopologyHex.h"
#include "ClsTopologyRect.h"

#include "iqrUtils.h"


using namespace iqrcommon;

ClsExclusifier::ClsExclusifier(){
    iRecursion = 0;
}
void ClsExclusifier::addButton(QRadioButton* qrb, int iID) {
    connect(qrb, SIGNAL(stateChanged(int)), this, SLOT(buttonClicked(int)));
//    list.append(qrb);
    mapButtons[iID] = qrb;
}


void ClsExclusifier::checkButton(int iID){
    if(mapButtons.find(iID)!=mapButtons.end()){
	mapButtons.find(iID).data()->setChecked(true);
    }
}

void ClsExclusifier::toggleButton(int iID){
    if(mapButtons.find(iID)!=mapButtons.end()){
	mapButtons.find(iID).data()->toggle();
    }
}

bool ClsExclusifier::isChecked(int iID){
    if(mapButtons.find(iID)!=mapButtons.end()){
	return mapButtons.find(iID).data()->isChecked();
    }
    return false;
}


void ClsExclusifier::buttonClicked(int iState){
//    cout << "ClsExclusifier::buttonClicked(int iState)" << endl;

    if(iRecursion==0){
	iRecursion++;
	QRadioButton *qrb = (QRadioButton *)sender();
	int iSignal= -99;
	/* iState = new State */
//	cout << "State: " << iState << endl;
	if(iState==QButton::Off){
	    qrb->setChecked(true);
	} 
	else if(iState==QButton::On){
	    QMap<int, QRadioButton*>::Iterator it;
	    for ( it = mapButtons.begin(); it != mapButtons.end(); ++it ) {
		if(it.data()!=qrb ){
		    if(it.data()->isOn()){
			it.data()->setChecked(false);
		    }
		} else {
		    iSignal = it.key();
		}
	    }
	}
	if(iSignal>=0){
//	    cout << "Signal: " << iSignal << endl;
	    emit sigClicked(iSignal);
	}
    }
    iRecursion = 0;
}



//#define DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY

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

//    this->setBackgroundColor(QColor(0,0,0));
    QVBoxLayout *qgly = new QVBoxLayout ( this, 2, 0, "qgly");

    string strInfo;
//    strInfo = "Connection ID:\t" + clsFEConnection->getConnectionID();
    strInfo = strInfo + "Connection name:\t" + clsFEConnection->getConnectionName();
    strInfo = strInfo + "\nSynapse type:\t" + clsFEConnection->getConnectionSynapseType();


    lblCaption->setText( clsFEConnection->getConnectionName().c_str());
    QToolTip::add( lblCaption, strInfo.c_str() );

    qgly->addWidget (lblCaption) ;


    clsExclusifier = new ClsExclusifier();

    qgbDisplay = new QGroupBox(1, QGroupBox::Horizontal, "display", this);
    qgbDisplay->setInsideSpacing (2 );

    QFrame* qfmDummy = new QFrame(qgbDisplay);

    QGridLayout *qlyGB = new QGridLayout ( qfmDummy, 6, 2, 0 ,-1, "qlyGB");
//only whith QT>3.1.1    qlyGB->setColSpacing ( 0, 10 );

    QRadioButton *qrbDistance = new QRadioButton ("Distance", qfmDummy );
    QRadioButton *qrbDelay = new QRadioButton ("Delay", qfmDummy );
    QRadioButton *qrbAttenuation = new QRadioButton ("Attenuation", qfmDummy );
    QRadioButton *qrbSynapseState = new QRadioButton ("Synapse", qfmDummy );
    qrbDistance->setChecked( TRUE );
 
    clsExclusifier->addButton(qrbDistance, ClsQSynapseStateVariableDisplay::TYPE_DISTANCE);
    clsExclusifier->addButton(qrbDelay, ClsQSynapseStateVariableDisplay::TYPE_DELAY);
    clsExclusifier->addButton(qrbAttenuation, ClsQSynapseStateVariableDisplay::TYPE_ATTENUATION);
    clsExclusifier->addButton(qrbSynapseState, ClsQSynapseStateVariableDisplay::TYPE_SYNAPSESTATE);
    connect( clsExclusifier, SIGNAL(sigClicked(int)), SLOT(slotSetType(int)) );

    qlyGB->addRowSpacing (0, 5);
    qlyGB->addMultiCellWidget (qrbDistance, 0, 0, 0, 1);
    qlyGB->addMultiCellWidget (qrbDelay, 1, 1, 0, 1);
    qlyGB->addMultiCellWidget (qrbAttenuation, 2, 2, 0, 1);
    qlyGB->addMultiCellWidget (qrbSynapseState, 3, 3, 0, 1);

    qgly->addWidget (qgbDisplay);

    qbtngrpStateVariables = new QButtonGroup(1, QGroupBox::Horizontal, "states", qfmDummy, "qbtngrpStateVariables");
    qbtngrpStateVariables->setInsideMargin ( 3 );
    qbtngrpStateVariables->setInsideSpacing (1 );
    qlyGB->addWidget (qbtngrpStateVariables, 4, 1);

    int ii=0;
    StateVariableList lstSynapseStateVariables = clsFEConnection->getListSynapseStateVariables( );
    iqrcommon::StateVariableList::const_iterator itlstSynapseStateVariables;
    for (itlstSynapseStateVariables = lstSynapseStateVariables.begin();
 	 itlstSynapseStateVariables != lstSynapseStateVariables.end(); ++itlstSynapseStateVariables){
 	QButton *qrb;
 	ClsStateVariable *pState = *itlstSynapseStateVariables;
 	qrb = new QRadioButton ( pState->getName().c_str(), qbtngrpStateVariables, "");
 	qbtngrpStateVariables->insert(qrb, ii );
 	ii++;
    }
    connect(qbtngrpStateVariables, SIGNAL(clicked(int)), SLOT(slotStateVariables(int)));
    qbtngrpStateVariables->setRadioButtonExclusive ( true );

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
    if(bToggled){
	for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
	    if(qbtngrpStateVariables->find(ii)->state()==2){
		/* send request */
		string strSinkID = qbtngrpStateVariables->find(ii)->text().latin1() + strID;
//--		clsFEDataClient->addDataSink(strSinkID, clsFEConnection->getConnectionID(), qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);
		clsFEDataClient->addDataSink(strSinkID, clsFEConnection, qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);
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
    emit sigLiveData(bToggled);
};

void ClsQSynapseStateVariableDisplay::slotStateVariables(int ii){
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "void ClsQSynapseStateVariableDisplay::slotStateVariables(int ii)" << endl;
#endif

    if(qchkbxLiveData->isOn() && ii != iCurrentStateButton){
	int iState = qbtngrpStateVariables->find(ii)->state();
	if(iState==2){
//	       cout << "send request: " << qbtngrpStateVariables->find(ii)->text() << endl;
	    /* send request */
	    string strSinkID = qbtngrpStateVariables->find(ii)->text().latin1() + strID;

//	       clsFEDataClient->addDataSink(strSinkID, clsQlstItemConnection->getConnectionID(), qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);
//--	    clsFEDataClient->addDataSink(strSinkID, clsFEConnection->getConnectionID(), qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);
	    clsFEDataClient->addDataSink(strSinkID, clsFEConnection, qbtngrpStateVariables->find(ii)->text().latin1(), strSelectedIndices);

	    QColor qc;
	    qc.setHsv(clsFEDataClient->getDataSinkColor(strSinkID), 255, 210);
	    QPalette palette = (qbtngrpStateVariables->find(ii))->palette();
	    palette.setColor ( QColorGroup::Foreground, qc );
	    (qbtngrpStateVariables->find(ii))->setPalette(palette);

	    if(qbtngrpStateVariables->isRadioButtonExclusive ( ) && iCurrentStateButton >= 0){
		/* cancel rewquest of the last active checkbox (iCurrentStateButton) */
//		    cout << "cancel request: " << qbtngrpStateVariables->find(iCurrentStateButton)->text() << endl;
		string strSinkID = strID + qbtngrpStateVariables->find(iCurrentStateButton)->text().latin1();
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


list<string> ClsQSynapseStateVariableDisplay::getSelectedStates() {
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::getSelectedStates()" << endl;
#endif/*
	list<string>::iterator itlst;
	for (itlst = lstSelectedStates.begin(); itlst != lstSelectedStates.end(); ++itlst)
	cout << *itlst << endl;
      */
    list<string> lstSelectedStates;

    for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
//	  cout << qbtngrpStateVariables->find(ii)->text() << ":" << qbtngrpStateVariables->find(ii)->state() << endl;
	if(qbtngrpStateVariables->find(ii)->state()==2)
	    lstSelectedStates.push_back(qbtngrpStateVariables->find(ii)->text().latin1());
    }
    return lstSelectedStates;
};

string ClsQSynapseStateVariableDisplay::getSelectedStatesAsString() {
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::getSelectedStatesAsString()" << endl;
#endif
    string strParamList="";
    for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
	if(qbtngrpStateVariables->find(ii)->state()==2)
	    strParamList = (string)(qbtngrpStateVariables->find(ii)->text().latin1()) + ";" + strParamList;
    }
    return strParamList;
};

void ClsQSynapseStateVariableDisplay::setSelectedStates(list<string> lst) {
#ifdef DEBUG_CLSSYNAPSEQSTATEVARIABLEDISPLAY
    cout << "ClsQSynapseStateVariableDisplay::setSelectedStates(list<string> lst)" << endl;
#endif
    for(int ii = 0; ii< qbtngrpStateVariables->count() ; ii++){
	string str= (string)(qbtngrpStateVariables->find(ii)->text().latin1());
	if(std::find(lst.begin(), lst.end(), str) != lst.end()){
	    qbtngrpStateVariables->find(ii)->toggle();
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
    StateVariableList lstSynapseStateVariables = clsFEConnection->getListSynapseStateVariables( );
    iqrcommon::StateVariableList::const_iterator itlstSynapseStateVariables;
    for (itlstSynapseStateVariables = lstSynapseStateVariables.begin();
	 itlstSynapseStateVariables != lstSynapseStateVariables.end(); ++itlstSynapseStateVariables){
	QButton *qrb;
	ClsStateVariable *pState = *itlstSynapseStateVariables;
	qrb = new QRadioButton ( pState->getName().c_str(), qbtngrpStateVariables, "");
//	qrb->setGeometry( 5, ii*qrb->sizeHint().height()+15, qrb->sizeHint().width(), qrb->sizeHint().height() );
	qrb->show();
	qbtngrpStateVariables->insert(qrb, ii );
	ii++;
    }
    qbtngrpStateVariables->setFixedHeight(qbtngrpStateVariables->sizeHint().height());
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


//cout << __FILE__ << ":" << __LINE__ << endl;

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
