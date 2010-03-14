#include <iostream>
#include <list>

#include <QLayout>
#include <QPainter>
#include <QFrame>
#include <QButtonGroup>
#include <QPushButton>
#include <QCheckBox>
#include <QCloseEvent>

#include "ClsFEConnectionDiagram.h"

#include <ClsFESystemManager.h>
#include <ClsFEConnection.h>
#include <ClsFEGroup.h>
#include <ClsTopologyRect.h>
#include <ClsTopologySparse.h>
#include <ClsTopologyHex.h>

#include "ClsQStateArrayViewRect.h"
#include "ClsQStateArrayViewSparse.h"
#include "ClsQStateArrayViewHex.h"

#include "ClsArbRandom.h"
#include "ClsArbAll.h"

#include <iqrUtils.h>

namespace{
#include <icon_connection_plot_32x32.xpm>
}

//#define DEBUG_CLSFECONNECTIONDIAGRAM

using namespace std;

ClsFEConnectionDiagram::ClsFEConnectionDiagram (ClsFEDataManager *_clsFEDataManager, QMutex *_qmutexSysGUI, string _strMyID, string _strConnectionID = ""):
//    QFrame( 0, "", Qt::WDestructiveClose), strMyID(_strMyID), strConnectionID(_strConnectionID) {
    ClsFEDataClient(_clsFEDataManager, _qmutexSysGUI, _strMyID),
    strMyID(_strMyID), strConnectionID(_strConnectionID) {

#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram (string _strMyID, string _strConnectionID)" << endl;
#endif

    clsQStateArrayViewSource = NULL;
    clsQStateArrayViewTarget = NULL;

    fDistMaxSource = 0;
    fDistMaxTarget = 0;
    iDelayMax = 1;

    iIndexSourceCell = iIndexTargetCell = -1;

    iType = ClsQSynapseStateVariableDisplay::TYPE_DISTANCE;

    bLiveData = false;

    this->setIcon(QPixmap(icon_connection_plot));

//    QGridLayout* layoutStateArrays = new QGridLayout(this, /*rows*/ 3, /*cols*/ 2, /*spacing*/ 2, -1, "layoutStateArrays" );
    QGridLayout* layoutStateArrays = new QGridLayout(/*rows*/ 3, /*cols*/ 2, /*spacing*/ 2, "layoutStateArrays" );
    boxlayoutBase->addLayout(layoutStateArrays, 0, 0);


    qlblSource = new QLabel(this);
    layoutStateArrays->addWidget(qlblSource, 0, 0, 0);

    qfmSASource = new QFrame(this);
    QHBoxLayout *qlayoutQfmSASource = new QHBoxLayout( qfmSASource /*, 0, -1, "qlayoutQfmSASource"*/);

    qlayoutQfmSASource->setAutoAdd ( true);
    layoutStateArrays->addWidget(qfmSASource, 1, 0, 0);


    qlblTarget = new QLabel(this);
    layoutStateArrays->addWidget(qlblTarget, 0, 1, 0);

    qfmSATarget = new QFrame(this);
    QHBoxLayout *qlayoutQfmSATarget = new QHBoxLayout( qfmSATarget /*zzz, 0, -1, "qlayoutQfmSATarget"*/);
    qlayoutQfmSATarget->setAutoAdd ( true);
    layoutStateArrays->addWidget(qfmSATarget, 1, 1, 0);



///////////////

    clsRangeGradient = new ClsRangeGradient(this);
    layoutStateArrays->addMultiCellWidget(clsRangeGradient, 2, 2, 0, 1, Qt::AlignHCenter);



//    layoutStateArrays->setResizeMode (QLayout::Fixed);
    boxlayoutBase->setResizeMode (QLayout::Fixed);
};

void ClsFEConnectionDiagram::appendSourceStateArray(string strGroupID) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::appendSourceStateArray(string strGroupID)" << endl;
#endif

    string strSourceGroupName = ClsFESystemManager::Instance()->getGroupName(strGroupID).c_str();
    qlblSource->setText(strSourceGroupName.c_str());

    this->setCaption("Connection Plot for " + qlblSource->text() + "->" + qlblTarget->text());

    clsQStateArrayViewSource = createStateArray(strGroupID, qfmSASource);
    clsQStateArrayViewSource->setColorMode(ClsBaseQStateArrayView::BLUE2RED);
//    clsQStateArrayViewSource->setColorMode(ClsBaseQStateArrayView::GRAY);
    connect(clsQStateArrayViewSource, SIGNAL(cellClicked( int, int)), this, SLOT(slotSourceCellClicked( int, int)));

    fDistMaxSource = getDistMax(strGroupID);
    clsQStateArrayViewSource->setMaxValue(fDistMaxSource);

/* Marking the concerned cells */
    list<int> lst;
    ListIndexQuadruples::iterator it;
    for(it=listIndexQuadruple.begin();it!=listIndexQuadruple.end();it++){
	lst.push_back((*it).first);
    }
    lst.unique();
    clsQStateArrayViewSource->markCell(iMarkColor, lst);
/* -------------------- */

    clsQStateArrayViewSource->show();
}

void ClsFEConnectionDiagram::appendTargetStateArray(string strGroupID) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::appendTargetStateArray(string strGroupID)" << endl;
#endif

    string strTargetGroupName = ClsFESystemManager::Instance()->getGroupName(strGroupID).c_str();
    qlblTarget->setText(strTargetGroupName.c_str());

    this->setCaption("Connection Plot for " + qlblSource->text() + "->" + qlblTarget->text());

    clsQStateArrayViewTarget = createStateArray( strGroupID, qfmSATarget);
    clsQStateArrayViewTarget->setColorMode(ClsBaseQStateArrayView::BLUE2RED);
//    clsQStateArrayViewTarget->setColorMode(ClsBaseQStateArrayView::GRAY);
    connect(clsQStateArrayViewTarget, SIGNAL(cellClicked( int, int)), this, SLOT(slotTargetCellClicked( int, int)));

    fDistMaxTarget = getDistMax(strGroupID);
    clsQStateArrayViewTarget->setMaxValue(fDistMaxTarget);

/* Marking the concerned cells */
    list<int> lst;
    ListIndexQuadruples::iterator it;
    for(it=listIndexQuadruple.begin();it!=listIndexQuadruple.end();it++){
	lst.push_back((*it).second);
    }
    lst.unique();
    clsQStateArrayViewTarget->markCell(iMarkColor, lst);
/* -------------------- */
    clsQStateArrayViewTarget->show();
}

ClsBaseQStateArrayView* ClsFEConnectionDiagram::createStateArray(string strGroupID, QFrame* qfmParent) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::createStateArray(string strGroupID, QFrame* qfmParent)" << endl;
#endif

    ClsBaseQStateArrayView* clsQStateArrayView = NULL;
    ClsBaseTopology* clsBaseTopology = ClsFESystemManager::Instance()->getFEGroup(strGroupID)->getTopology();

    if(dynamic_cast<ClsTopologyRect*>(clsBaseTopology)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyRect" << endl;
	clsQStateArrayView = new ClsQStateArrayViewRect(qfmParent, clsBaseTopology, "source", ClsBaseQStateArrayView::SINGLE);
    }
    else if(dynamic_cast<ClsTopologyHex*>(clsBaseTopology)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyHex" << endl;
//***	clsQStateArrayView = new ClsQStateArrayViewHex(qfmParent, clsBaseTopology, "source", ClsBaseQStateArrayView::SINGLE);
    }

    else if(dynamic_cast<ClsTopologySparse*>(clsBaseTopology)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologySparse" << endl;
//***	clsQStateArrayView = new ClsQStateArrayViewSparse(qfmParent, clsBaseTopology, "source", ClsBaseQStateArrayView::SINGLE);
    }

/*
  double fDistMax;
  ClsBaseArborization *clsBaseArborization = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getArborization();
  if(dynamic_cast<ClsArbRandom*>(clsBaseArborization) || dynamic_cast<ClsArbAll*>(clsBaseArborization)){
  fDistMax =  clsBaseTopology->DistMax();
  }else {
  fDistMax =  ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->DistMax();
  }

  clsQStateArrayView->setMaxValue(fDistMax);
*/
    return clsQStateArrayView;
}


double ClsFEConnectionDiagram::getDistMax(string strGroupID){
    ClsBaseTopology* clsBaseTopology = ClsFESystemManager::Instance()->getFEGroup(strGroupID)->getTopology();
    ClsBaseArborization *clsBaseArborization = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getArborization();
    if(dynamic_cast<ClsArbRandom*>(clsBaseArborization) || dynamic_cast<ClsArbAll*>(clsBaseArborization)){
	return clsBaseTopology->DistMax()+.1;
    }else {
	return ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->DistMax()+.1;
    }
}


string ClsFEConnectionDiagram::addStateVariableDisplay(ClsFEConnection* _clsFEConnection) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::addStateVariableDisplay(ClsFEConnection* _clsFEConnection, string strRange)" << endl;
#endif

    if(clsQBaseStateVariableDisplay!=NULL){
	boxlayoutBase->remove(clsQBaseStateVariableDisplay); 
	delete clsQBaseStateVariableDisplay;
    }

    string strDisplayID = ClsIDGenerator::Instance()->Next();//.latin1();


    clsQBaseStateVariableDisplay = new ClsQSynapseStateVariableDisplay(this, 
									  strDisplayID, 
									  this, /*clsQBaseStateVariableDisplay, */
									  _clsFEConnection, 
									  "");

    boxlayoutBase->addWidget(clsQBaseStateVariableDisplay, 0, 2, Qt::AlignTop);



    connect(dynamic_cast<ClsQSynapseStateVariableDisplay*>(clsQBaseStateVariableDisplay),
	    SIGNAL(sigChangeType(int )), SLOT(slotSetType(int)));
    connect(dynamic_cast<ClsQSynapseStateVariableDisplay*>(clsQBaseStateVariableDisplay), 
	    SIGNAL(sigLiveData(bool)), SLOT(slotLiveData(bool)));
    



//???    boxlayoutStateFrames->addWidget(clsQSynapseStateVariableDisplay);
//    boxlayoutStateFrames->setResizeMode (QLayout::Fixed);
    return strDisplayID;
};


void ClsFEConnectionDiagram::createDiagram( ) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::createDiagram( )" << endl;
#endif

    string strSourceID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionSourceID();
    string strTargetID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionTargetID();

//    ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->calculateArbPattern();
    listIndexQuadruple = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getQuadrupleList();
    iDelayMax = listIndexQuadruple.getMaxDelay()+1;
    fAttenuationMax = listIndexQuadruple.getMaxAttenuation();
//    cout << "fAttenuationMax: " << fAttenuationMax << endl;
    fAttenuationMax += fAttenuationMax/10.;

    appendSourceStateArray(strSourceID);
    appendTargetStateArray(strTargetID);

    createPre2Post();
    createPost2Pre();

};


void ClsFEConnectionDiagram::createPre2Post(){

    // Use vector resize here, we don't need to use push_back because we already
    // know the number of elements (= number of presynaptic neurons).

    string strSourceID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionSourceID();
    unsigned int _nNeuronsPre = ClsFESystemManager::Instance()->getFEGroup(strSourceID)->getNumberOfNeurons();

    pre2post.resize(_nNeuronsPre);

//    cout << "Building pre2post..." << endl;
    for (unsigned int pre = 0; pre < _nNeuronsPre; ++pre) {
        vector<Indices> indices = listIndexQuadruple.getIndicesByFirst(pre);
	// Length of index vector gives us the number of connections
	// the current neuron makes.
        int nConns = indices.size();
	pre2post[pre].maskSyn.resize(nConns);
	pre2post[pre].maskPost.resize(nConns);

//	cout << pre << " makes " << nConns << " connections" << endl;

	// Store connection data.
	pre2post[pre].pre = pre;
	for (int conn = 0; conn < nConns; ++conn) {
	    pre2post[pre].maskSyn[conn] = indices[conn].synapse;
	    pre2post[pre].maskPost[conn] = indices[conn].neuronPost;
/*
	    cout << pre << " " 
		 << pre2post[pre].maskSyn[conn] << " "
		 << pre2post[pre].maskPost[conn] << endl;
*/
	}
    }
}




void ClsFEConnectionDiagram::createPost2Pre(){

    // Use vector resize here, we don't need to use push_back because we already
    // know the number of elements (= number of postsynaptic neurons).

    string strTargetID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionTargetID();
    unsigned int _nNeuronsPost = ClsFESystemManager::Instance()->getFEGroup(strTargetID)->getNumberOfNeurons();
    post2pre.resize(_nNeuronsPost);

//    cout << "Building post2pre..." << endl;
    for (unsigned int post = 0; post < _nNeuronsPost; ++post) {
        vector<Indices> indices = listIndexQuadruple.getIndicesBySecond(post);
	// Length of index vector gives us the number of connections
	// the current neuron makes.
        int nConns = indices.size();
	post2pre[post].maskSyn.resize(nConns);
	post2pre[post].maskPre.resize(nConns);

//	cout << post << " receives " << nConns << " connections" << endl;

	// Store connection data.
	post2pre[post].post = post;
	for (int conn = 0; conn < nConns; ++conn) {
	    post2pre[post].maskSyn[conn] = indices[conn].synapse;
	    post2pre[post].maskPre[conn] = indices[conn].neuronPre;
/*
	    cout << post << " " 
		 << post2pre[post].maskSyn[conn] << " "
		 << post2pre[post].maskPre[conn] << endl;
*/
	}
    }    
}


void ClsFEConnectionDiagram::slotSourceCellClicked( int iX, int iY) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::slotSourceCellClicked( int iX, int iY)" << endl;
#endif

    clsQStateArrayViewTarget->clear();
    string strSourceID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionSourceID();
    iIndexSourceCell = ClsFESystemManager::Instance()->getFEGroup(strSourceID)->getTopology()->pos2index(iX, iY);



    if(iType == ClsQSynapseStateVariableDisplay::TYPE_DISTANCE){
	clsQStateArrayViewTarget->setMaxValue(fDistMaxTarget);
	clsRangeGradient->setMaxValue(fDistMaxTarget);
	vector<pair<int, double> > lstIndexDistSource = listIndexQuadruple.getDistanceByFirst(iIndexSourceCell);
	clsQStateArrayViewTarget->setValue(lstIndexDistSource);

    }
    else if(iType == ClsQSynapseStateVariableDisplay::TYPE_ATTENUATION){
	clsQStateArrayViewTarget->setMaxValue(fAttenuationMax);
	clsRangeGradient->setMaxValue(fAttenuationMax);
	vector<pair<int, double> > lstIndexDistSource = listIndexQuadruple.getAttenuationByFirst(iIndexSourceCell);
	clsQStateArrayViewTarget->setValue(lstIndexDistSource);
    } else if(iType == ClsQSynapseStateVariableDisplay::TYPE_DELAY){
	clsQStateArrayViewTarget->setMaxValue((double)iDelayMax);
	clsRangeGradient->setMaxValue((double)iDelayMax);
	vector<pair<int, int> > lstIndexDelaySource = listIndexQuadruple.getDelayByFirst(iIndexSourceCell);
	clsQStateArrayViewTarget->setValue(lstIndexDelaySource);
    }
    iIndexTargetCell = -1;

    clsRangeGradient->setPixmap(clsQStateArrayViewTarget->getGradientPixmap(GRADIENT_WIDTH, GRADIENT_HEIGHT, ClsBaseQStateArrayView::BLUE2RED));
}

void ClsFEConnectionDiagram::slotTargetCellClicked( int iX, int iY) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::slotTargetCellClicked( int iX, int iY)" << endl;
#endif

    clsQStateArrayViewSource->clear();
    string strTargetID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionTargetID();
    iIndexTargetCell = ClsFESystemManager::Instance()->getFEGroup(strTargetID)->getTopology()->pos2index(iX, iY);
    if(iType == ClsQSynapseStateVariableDisplay::TYPE_DISTANCE){
// 	cout << "iType == ClsQSynapseStateVariableDisplay::TYPE_DISTANCE" << endl;
	clsQStateArrayViewSource->setMaxValue(fDistMaxSource);
	clsRangeGradient->setMaxValue(fDistMaxSource);
	vector<pair<int, double> > lstIndexDistTarget = listIndexQuadruple.getDistanceBySecond(iIndexTargetCell);
	clsQStateArrayViewSource->setValue(lstIndexDistTarget);
    }
    else if(iType == ClsQSynapseStateVariableDisplay::TYPE_ATTENUATION){
// 	cout << "iType == ClsQSynapseStateVariableDisplay::TYPE_ATTENUATION" << endl;
	clsQStateArrayViewSource->setMaxValue(fAttenuationMax);
	clsRangeGradient->setMaxValue(fAttenuationMax);
	vector<pair<int, double> > lstIndexDistTarget = listIndexQuadruple.getAttenuationBySecond(iIndexTargetCell);
	clsQStateArrayViewSource->setValue(lstIndexDistTarget);
    } 
    else if(iType == ClsQSynapseStateVariableDisplay::TYPE_DELAY){
//	cout << "iType == ClsQSynapseStateVariableDisplay::TYPE_DELAY" << endl;
	clsQStateArrayViewSource->setMaxValue((double)iDelayMax);
	clsRangeGradient->setMaxValue((double)iDelayMax);
	vector<pair<int, int> > lstIndexDelayTarget = listIndexQuadruple.getDelayBySecond(iIndexTargetCell);
	clsQStateArrayViewSource->setValue(lstIndexDelayTarget);
    }

    iIndexSourceCell = -1;

    clsRangeGradient->setPixmap(clsQStateArrayViewSource->getGradientPixmap(GRADIENT_WIDTH, GRADIENT_HEIGHT, ClsBaseQStateArrayView::BLUE2RED));

}


void ClsFEConnectionDiagram::connectionChanged(string strID) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cerr << "ClsFEConnectionDiagram::connectionChanged()" << endl;
#endif

    if(!strConnectionID.compare(strID)){
	string strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
	string strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);
	if(strSourceID.size()>0 && strTargetID.size()>0){
	    clsQStateArrayViewSource->close(true);
	    clsQStateArrayViewSource = NULL;
	    
	    clsQStateArrayViewTarget->close(true);
	    clsQStateArrayViewTarget = NULL;
	    
	    createDiagram( );
	    dynamic_cast<ClsQSynapseStateVariableDisplay*>(clsQBaseStateVariableDisplay)->connectionChanged();
	    ClsFEDataClient::groupChanged(strID);
	} else {
//--	    QFrame::close();
	}
    }
}

void ClsFEConnectionDiagram::connectionDeleted(string strID) {
//#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cerr << "ClsFEConnectionDiagram::connectionDeleted()" << endl;
//#endif

    if(!strConnectionID.compare(strID)){
//--	QFrame::close();
    }
}


void ClsFEConnectionDiagram::groupChanged(string strID) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::groupChanged()" << endl;
#endif

    string strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
    string strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);

    if(!strSourceID.compare(strID) || !strTargetID.compare(strID)){
	
	clsQStateArrayViewSource->close(true);
	clsQStateArrayViewSource = NULL;
	
	clsQStateArrayViewTarget->close(true);
	clsQStateArrayViewTarget = NULL;
	
	createDiagram( );
	ClsFEDataClient::groupChanged(strID);
    }
}


void ClsFEConnectionDiagram::groupDeleted(string strID){
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::groupDeleted(string)" << endl;
#endif

    string strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
    string strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);

//    cout << "strSourceID: " << strSourceID << endl;
//    cout << "strTargetID: " << strTargetID << endl;
//    cout << "strID: " << strID << endl;

    if((!strSourceID.compare(strID) || !strTargetID.compare(strID)) || (strSourceID.size()<=0 || strTargetID.size()<=0)){
//--	QFrame::close();
    }
}

void ClsFEConnectionDiagram::closeEvent( QCloseEvent* ce ) {
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "iqrfe::ClsSubtypedAutoDialog::closeEvent( QCloseEvent* ce )" << endl;
#endif
    emit sigPlotClosed(strMyID);
    ce->accept();
}


void ClsFEConnectionDiagram::slotSetType(int ii){
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::slotSetType(int)" << endl;
#endif
    iType = ii;
}


void ClsFEConnectionDiagram::slotLiveData(bool bToggled) {
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsFEConnectionDiagram::slotLiveData(bool bToggled)" << endl;
#endif

    bLiveData = bToggled;
}

void ClsFEConnectionDiagram::init(){
    ClsFEDataClient::init();
}

void ClsFEConnectionDiagram::update(){
#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
    cout << "ClsFEConnectionDiagram::update()" << endl;
#endif
    if(bLiveData){

//	cout << "iIndexSourceCell: " << iIndexSourceCell << endl;
//	cout << "iIndexTargetCell: " << iIndexTargetCell << endl;

/*
	if(iIndexSourceCell>=0){
	    valarray<size_t> vaMaskNeuronsPost = 
		pre2post[iIndexSourceCell].maskPost;
	    valarray<double> vaData;
	    vaData.resize(pre2post[iIndexSourceCell].maskPost.size());
	    vaData = (ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionOut())[0][vaMaskNeuronsPost];
	    clsQStateArrayViewSource->setValue(vaMaskNeuronsPost, vaData);
	} else if(iIndexTargetCell>=0){
	    valarray<size_t> vaMaskNeuronsPre = 
		post2pre[iIndexTargetCell].maskPre;
	    valarray<double> vaData;
	    vaData.resize(post2pre[iIndexTargetCell].maskPre.size());
	    vaData = (ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionOut())[0][vaMaskNeuronsPre];
	    clsQStateArrayViewTarget->setValue(vaMaskNeuronsPre, vaData);
	}
*/

	if(iIndexSourceCell>=0){
	    valarray<size_t> vaMaskSyn = pre2post[iIndexSourceCell].maskSyn;
	    valarray<size_t> vaMaskPost = pre2post[iIndexSourceCell].maskPost;
	    ClsDataSinkCopying *clsDataSink = mapDataSinks.begin()->second;
	    /* 
	       not necessary, we access the state array pointer 
	       clsDataSink->update();
	    */

	    qmutexSysGUI->lock(); //NEW 2004/04/12
	    valarray<double> vaData = (*clsDataSink->getStateArray())[0][vaMaskSyn];
	    qmutexSysGUI->unlock();//NEW 2004/04/12

	    double fMaxVal =  vaData.max()*1.2 + .1;
	    clsQStateArrayViewTarget->setMaxValue(fMaxVal);
	    clsQStateArrayViewTarget->setValue(vaMaskPost, vaData);
	    clsRangeGradient->setMaxValue(fMaxVal);
/*
	    cout << "vaMaskPost: " << vaMaskPost << endl;
	    cout << "vaData: " << vaData << endl;
*/

	} else if(iIndexTargetCell>=0){ 
	    valarray<size_t> vaMaskSyn = post2pre[iIndexTargetCell].maskSyn;
	    valarray<size_t> vaMaskPre = post2pre[iIndexTargetCell].maskPre;
	    
	    ClsDataSinkCopying *clsDataSink = mapDataSinks.begin()->second;
	    /* 
	       not necessary, we access the state array pointer 
	       clsDataSink->update();
	    */

	    qmutexSysGUI->lock(); //NEW 2004/04/12
	    valarray<double> vaData = (*clsDataSink->getStateArray())[0][vaMaskSyn];
	    qmutexSysGUI->unlock();//NEW 2004/04/12

	    double fMaxVal =  vaData.max()*1.2 + .1;
	    clsQStateArrayViewSource->setMaxValue(fMaxVal);
	    clsQStateArrayViewSource->setValue(vaMaskPre, vaData);
	    clsRangeGradient->setMaxValue(fMaxVal);

#ifdef DEBUG_CLSQSTATEVARIABLEDISPLAY
	    cout << "vaMaskPre: " << vaMaskPre << endl;
	    cout << "vaData: " << vaData << endl;
	    cout << "fMaxVal: " << fMaxVal << endl;
#endif
	}
    }
}

void ClsFEConnectionDiagram::print(){
}

void ClsFEConnectionDiagram::setConfig(ClsDataClientConfig clsDataClientConfig){
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::setConfig(ClsDataClientConfig clsDataClientConfig)" << endl;
#endif
    list<pair<string, string> > lstParameters= clsDataClientConfig.getListParameters();
    list<pair<string, string> >::iterator it;
    for(it=lstParameters.begin();it!=lstParameters.end();it++){
	string strParamName = it->first;
	string strParamValue = it->second;
	if(!strParamName.compare("ConnectionID")){
	    strConnectionID= strParamValue;
	} else if(!strParamName.compare("Type")){
	    iType = iqrUtils::string2int(strParamValue);
	}
    }

    QString qstrCaption("Connection plot for ");
    qstrCaption.append(strConnectionID.c_str());
    this->setCaption(qstrCaption);


    list<ClsStateVariableDisplayConfig>lstSVDConfigs =  clsDataClientConfig.getListStateVariableDisplayConfig();
    list<ClsStateVariableDisplayConfig>::iterator itSVD;
    for(itSVD = lstSVDConfigs.begin(); itSVD != lstSVDConfigs.end(); itSVD++){
	string strSVDID =  (*itSVD).getID();

	if(ClsFESystemManager::Instance()->getFEConnection( strConnectionID )!=NULL){
	    string strDisplayID = addStateVariableDisplay(ClsFESystemManager::Instance()->getFEConnection( strConnectionID ));
	    clsQBaseStateVariableDisplay->setConfig((*itSVD));
	}
    }
    createDiagram();
    show();
}

ClsDataClientConfig ClsFEConnectionDiagram::getConfig(){
#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
    cout << "ClsFEConnectionDiagram::getConfig()" << endl;
#endif
    ClsDataClientConfig clsDataClientConfig(strDataClientID);
    clsDataClientConfig.setPosition(this->pos().x(), this->pos().y() );
    clsDataClientConfig.setGeometry(this->size().width(),  this->size().height());

    pair<string, string> p0("ConnectionID", strConnectionID);
    clsDataClientConfig.addParameter(p0);

    pair<string, string> p1("Type", iqrUtils::int2string(iType));
    clsDataClientConfig.addParameter(p1);

    ClsStateVariableDisplayConfig clsStateVariableDisplayConfig = clsQBaseStateVariableDisplay->getConfig();
    clsDataClientConfig.addStateVariableDisplayConfig(clsStateVariableDisplayConfig);
    return clsDataClientConfig;
}

//cout << __FILE__ << ":" << __LINE__ << endl;

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
