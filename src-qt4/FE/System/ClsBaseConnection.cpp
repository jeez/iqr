/****************************************************************************
 ** $Filename: ClsBaseConnection.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Mon Aug  4 17:52:45 2003
 **
 *****************************************************************************/


#include "ClsBaseConnection.h"

#include "ClsArbAll.h"
#include "ClsArbEllipse.h"
#include "ClsArbEllipseWindow.h"
#include "ClsArbRandom.h"
#include "ClsArbRect.h"
#include "ClsArbRectWindow.h"
#include "ClsBaseArborization.h"
#include "ClsFunctions.h"
#include "ClsPatterns.h"
#include "ClsSubPopulations.h"

#include "ClsTopologyRect.h"
#include "ClsTopologySparse.h"
#include "ClsTopologyHex.h"

#include "ClsFESystemManager.h"
#include "ClsFEGroup.h"

#include "ClsFunctions.h"
#include "ClsQLogWindow.h"

#include "optionsParameter.hpp"
#include "stringParameter.hpp"

#include "tagLibrary.hpp"
#include "synapseManager.hpp"
using iqrfe::initializeSynapseManager;
using iqrfe::SynapseManager;

//#define DEBUG_CLSBASECONNECTION

ClsBaseConnection::ClsBaseConnection ( ) : axonalDelay(0) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::ClsBaseConnection ( )" << endl;
#endif

    strConnectionID = "";
    strConnectionName = "";
    strConnectionTargetID = "";
    strConnectionSourceID = "";


    pSynapse = NULL;
    strSynapseType = "";

    clsArborization = NULL;
    clsDelayFunction = NULL;
    clsPattern = NULL;
    clsAttenuationFunction = NULL;
    fMaxAttenuation = 0;

    createListOfArborizationTypes();
    createListOfFunctionTypes();
    createListOfPatternTypes();


    addStringParameter(ClsTagLibrary::NameTag(), "Connection Name",
		       "",
		       true, false,
		       "Name",
		       "Properties");


    addOptionsParameter(ClsTagLibrary::ConnectionTypeTag(),// string _strName
			"Connection type",
			false,             //bool   _bReadOnly,
			"Connection type", //_strDescription,
			"Properties");

    ClsOptionsParameter *op = static_cast<ClsOptionsParameter*>(getParameter(ClsTagLibrary::ConnectionTypeTag()));
    op->addOption(ClsTagLibrary::ExcitatoryConnectionTag());
    op->addOption(ClsTagLibrary::InhibitoryConnectionTag());
    op->addOption(ClsTagLibrary::ModulatoryConnectionTag());


    setPatternType(ClsTagLibrary::ConnectionPatternMapped());
    setArborizationType(ClsTagLibrary::ConnectionArborizationRectangular());
    setDelayFunctionType(ClsTagLibrary::ConnectionFunctionUniform());
    setAttenuationFunctionType(ClsTagLibrary::ConnectionFunctionUniform());

};


ClsBaseConnection::~ClsBaseConnection(){
    try {
	    SynapseManager::instance().destroy(pSynapse);
	    pSynapse = NULL;
	}
	catch(...){
	    cerr << "could not destroy Synapse" << endl;
	}
}

void ClsBaseConnection::update(){
//    cout << "ClsBaseConnection::update()" << endl;

    // Update all synapses
    pSynapse->advance();
    
    /////////////////////
    // CONNECTION::UPDATE

    StateArray &neuronPreOutput  = pNeuronPreOutput->getStateArray();
    StateArray &synapseInput     = pSynapseInput->getStateArray();
    StateArray &synapseOutput    = pSynapseOutput->getStateArray();
        
    // Connection copies presynaptic output into synapse
    
//     cout << "synapseInput[0].size(): " << synapseInput[0].size() << endl;
//     cout << "axonalDelay: " << axonalDelay << endl;
//     cout << "neuronPreOutput[0].size(): " << neuronPreOutput[0].size() << endl;
//     cout << "connIn.axonalDelay: " << connIn.axonalDelay << endl;
//     cout << "connIn.maskPre: " << connIn.maskPre << endl;
//     cout << "neuronPreOutput[connIn.axonalDelay][connIn.maskPre]: " <<
//     	neuronPreOutput[connIn.axonalDelay] << endl;
	
    synapseInput[0] = neuronPreOutput[connIn.axonalDelay][connIn.maskPre];	
    
//    cout << "Synapse_input " << synapseInput[0] << endl;    
    
    // Copy feedback from postsynaptic neuron into synapse
    for (unsigned int i = 0; i < fbStates.size(); ++i) {
	StateArray &neuronFeedback = fbStates[i].pNeuronState->getStateArray();
	StateArray &synapseInput   = fbStates[i].pSynapseInput->getStateArray();
//	cout << "Neuron_feedback " << fbStates[i].pNeuronState->getLabel()
//	     << " " << neuronFeedback[0] << endl;
	
	for (unsigned int j = 0; j < connFb.size(); ++j) {
	    synapseInput[0][connFb[j].maskSyn] 
		= neuronFeedback[connFb[j].dendriticDelay][connFb[j].maskPost];
	}
//	cout << "Feedback_input[" << fbStates[i].pSynapseInput->getName()
//	     << "] " << synapseInput[0] << endl;
    }

    pSynapse->update();
    

    synapseOutput[0] *= (fMaxAttenuation - dendriticAttenuation);

//     for (int i = 0; i < synapseOutput.getDepth(); ++i) {
// 	cout << "Synapse_output[" << i << "] " << synapseOutput[i] << endl;
//     }

    // Connection copies the data out of the synapse output array into
    // the connection out array, transforming from number of synapses
    // to number of postsynaptic neurons.
    //
    // synOut handles the dendritic delay
    for (unsigned int i = 0; i < synOut.size(); ++i) {
	synapseOutDelayed[0][synOut[i].maskSyn]
	    = synapseOutput[synOut[i].dendriticDelay][synOut[i].maskSyn];
    }

//    cout << "Synapse_output_delayed " << synapseOutDelayed[0] << endl;

    // connOut handles the summing for postsynaptic neurons    
    for (unsigned int i = 0; i < connOut.size(); ++i) {
// 	cout << "Output_to_cell[" << connOut[i].idPost << "]" 
// 	     << static_cast<valarray<double> >(synapseOutDelayed[0][connOut[i].maskSyn]) 
// 	     << endl;
	connectionOut[0][connOut[i].idPost]
	    = static_cast<valarray<double> >(synapseOutDelayed[0][connOut[i].maskSyn]).sum();
    }

//     cout << "Connection_output " << connectionOut[0] << endl;

    // Write output of pre- and post-synaptic neurons and synapse into file.
//     cout << "Connection_output   " << connectionOut[0] << endl;
    // Postsynaptic neuron output is one timestep behind the input
    // buffer, as the neuron is updated before the synapse.
//     if (pNeuronPostOutput) {
// 	cout << "Neuron_post_output "  << (pNeuronPostOutput->getStateArray())[0] << endl;
//     }
}

void ClsBaseConnection::cleanup(){
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::cleanup()" << endl;
    cout << "ConnectionID: " << strConnectionID << endl;
    cout << "ConnectionName" << strConnectionName << endl;
#endif

    pattern.clear();
    
    connectionOut.resetSize();



    pNeuronPreOutput = NULL;
    pSynapseInput = NULL;
    pSynapseOutput = NULL;

//20040616    connIn.resize(0);
    connOut.resize(0);
    synOut.resize(0);

//20060704: we need this to clear the buffer of the synapse (why was this not always here?)
    pSynapse->resetSize();

}

void ClsBaseConnection::setConnectionID( string _strConnectionID ){
    strConnectionID = _strConnectionID;
};
string ClsBaseConnection::getConnectionID( ){ return strConnectionID;};

void ClsBaseConnection::setConnectionName(string _strConnectionName){
    setParameter(ClsTagLibrary::NameTag(), _strConnectionName);
}

string ClsBaseConnection::getConnectionName(){
//    cout << "ClsBaseConnection::getConnectionName()" << endl;
    string strName = getParameter(ClsTagLibrary::NameTag())->getValueAsString();
//    cout << __FILE__ << ":" << __LINE__ << endl;
    return strName;
//    return "empty";
};


string ClsBaseConnection::getConnectionLongName(){
    string strSourceGroupName = ClsFESystemManager::Instance()->getGroupName(strConnectionSourceID).c_str();
    string strTargetGroupName = ClsFESystemManager::Instance()->getGroupName(strConnectionTargetID).c_str();
    string strLongName = "\"" + getConnectionName() + "\"\n(\"" + strSourceGroupName + "\"->\"" + strTargetGroupName + "\")";
    return strLongName;

}

void ClsBaseConnection::setConnectionTargetID(string _strConnectionTargetID ){
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setConnectionTargetID(string _strConnectionTargetID )" << endl;
#endif
    strConnectionTargetID = _strConnectionTargetID;
     ClsFESystemManager::Instance()->itemChanged(ClsFESystemManager::ITEM_CONNECTION, strConnectionID);

};

string ClsBaseConnection::getConnectionTargetID( ){ return  strConnectionTargetID;};

void ClsBaseConnection::setConnectionSourceID(string _strConnectionSourceID ){
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setConnectionSourceID(string _strConnectionSourceID )" << endl;
#endif
    strConnectionSourceID = _strConnectionSourceID;
     ClsFESystemManager::Instance()->itemChanged(ClsFESystemManager::ITEM_CONNECTION, strConnectionID);
};

string ClsBaseConnection::getConnectionSourceID( ){ return  strConnectionSourceID;};

list<string> ClsBaseConnection::getListOfMembers() {
    list<string> lst;
    lst.push_back(ClsTagLibrary::ConnectionPattern());
    lst.push_back(ClsTagLibrary::ConnectionArborization());
    lst.push_back(ClsTagLibrary::ConnectionAttenuationFunction());
    lst.push_back(ClsTagLibrary::ConnectionDelayFunction());
    lst.push_back(ClsTagLibrary::SynapseTag());
    return lst;
}

ClsItem* ClsBaseConnection::getMemberByName(string strName) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::getMemberByName(string strName)" << endl;
#endif

    if(!strName.compare(ClsTagLibrary::SynapseTag())){
	return pSynapse;
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionArborization())){
	return clsArborization;
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionAttenuationFunction())){
	return clsAttenuationFunction;
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionDelayFunction())){
	return clsDelayFunction;
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionPattern())){
	return clsPattern;
    }
    return NULL;
}

void ClsBaseConnection::setMemberTypeByName(string strName, string strValue) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setMemberTypeByName(string strName, string strValue)" << endl;
#endif
//    cout << "strName: " << strName << endl;
//    cout << "strValue: " << strValue << endl;

    if(!strName.compare(ClsTagLibrary::SynapseTag())){
	createSynapseByLabel(strValue);
    }
    if(!strName.compare(ClsTagLibrary::ConnectionArborization())){
	setArborizationType(strValue);
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionAttenuationFunction())){
	setAttenuationFunctionType(strValue);
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionDelayFunction())){
	setDelayFunctionType(strValue);
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionPattern())){
	setPatternType(strValue);
    }
}

string ClsBaseConnection::getMemberTypeByName(string strName){
    if(!strName.compare(ClsTagLibrary::SynapseTag())){
	return getConnectionSynapseLabel();
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionArborization())){
	return ArborizationType();
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionAttenuationFunction())){
	return AttenuationFunctionType();
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionDelayFunction())){
	return DelayFunctionType();
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionPattern())){
	return PatternType();
    }

    return "";
}


list<string> ClsBaseConnection::getListOfMembersTypes(string strName) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::getListOfMembersTypes(string strName)" << endl;
#endif

    list<string> lst;
    if(!strName.compare(ClsTagLibrary::SynapseTag())){
	lst = SynapseManager::instance().getListLabels();
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionArborization())){
	lst = ArborizationTypeList();
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionAttenuationFunction())){
	lst = AttenuationFunctionTypeList();
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionDelayFunction())){
	lst = DelayFunctionTypeList();
    }
    else if(!strName.compare(ClsTagLibrary::ConnectionPattern())){
	lst = PatternTypeList();
    }

    return lst;
}

void ClsBaseConnection::createListOfArborizationTypes() {
    lstArborizationTypes.push_back(ClsTagLibrary::ConnectionArborizationRectangular());
    lstArborizationTypes.push_back(ClsTagLibrary::ConnectionArborizationEllipse());
    lstArborizationTypes.push_back(ClsTagLibrary::ConnectionArborizationRectangularWindow());
    lstArborizationTypes.push_back(ClsTagLibrary::ConnectionArborizationEllipseWindow());
    lstArborizationTypes.push_back(ClsTagLibrary::ConnectionArborizationRandom());
    lstArborizationTypes.push_back(ClsTagLibrary::ConnectionArborizationAll());
};

void ClsBaseConnection::createListOfFunctionTypes() {
    lstFunctionTypes.push_back(ClsTagLibrary::ConnectionFunctionGaussian());
    lstFunctionTypes.push_back(ClsTagLibrary::ConnectionFunctionLinear());
    lstFunctionTypes.push_back(ClsTagLibrary::ConnectionFunctionBlock());
    lstFunctionTypes.push_back(ClsTagLibrary::ConnectionFunctionRandom());
    lstFunctionTypes.push_back(ClsTagLibrary::ConnectionFunctionUniform());
/*    lstFunctionTypes.push_back(ClsTagLibrary::ConnectionFunctionKernel()); */
};


void ClsBaseConnection::createListOfPatternTypes() {
    lstPatternTypes.push_back(ClsTagLibrary::ConnectionPatternMapped());
    lstPatternTypes.push_back(ClsTagLibrary::ConnectionPatternForeach());
    lstPatternTypes.push_back(ClsTagLibrary::ConnectionPatternTuples());
};

void ClsBaseConnection::setConnectionSubNodes(ClsSysFileNode _clsSysFileNodeConnectionSubNodes){
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setConnectionSubNodes(ClsSysFileNode _clsSysFileNodeConnectionSubNodes)" << endl;
#endif

    clsSysFileNodeConnectionSubNodes = _clsSysFileNodeConnectionSubNodes;
    while (_clsSysFileNodeConnectionSubNodes.countNodes()){
	ClsSysFileNode F1 = _clsSysFileNodeConnectionSubNodes.popNode();
	string strNodeName = F1.getName();
	if(!strNodeName.compare(ClsTagLibrary::SynapseTag())) {
	    setSynapseParameter(F1);
	}
	else if(!strNodeName.compare(ClsTagLibrary::ConnectionPattern())) {
	    setPatternParameter(F1);
	}
	else if(!strNodeName.compare(ClsTagLibrary::ConnectionArborization())) {
	    setArborizationParameter(F1);
	}
	else if(!strNodeName.compare(ClsTagLibrary::ConnectionAttenuationFunction())) {
	    setAttenuationFunctionParameter(F1);
	}
	else if(!strNodeName.compare(ClsTagLibrary::ConnectionDelayFunction())) {
	    setDelayFunctionParameter(F1);
	}
    }
};

void ClsBaseConnection::setArborizationParameter(ClsSysFileNode &_clsSysFileNodeArborizationParameter){
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setArborizationParameter(ClsSysFileNode _clsSysFileNodeArborizationParameter)" << endl;
#endif

    ClsSysFileNode F3 = _clsSysFileNodeArborizationParameter.popNode();
    string strArbType = F3.getName();
    setArborizationType(strArbType);

    while (F3.countParameters()){
	iqrcommon_old::ClsParameter clsParameterTemp = F3.popParameter();
	string strParamName = clsParameterTemp.getName();
	string strParamValue = clsParameterTemp.getValueAsString();
//			cout << "strParamName: " << strParamName << endl;
//			cout << "strParamValue: " << strParamValue << endl;
	clsArborization->setParameter(strParamName, strParamValue);
    }
}

double ClsBaseConnection::DistMax(){
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::DistMax()" << endl;
#endif
    if(clsArborization!=NULL){
	return clsArborization->DistMax();
    }
    return 0;
}

int ClsBaseConnection::createSynapse(string _strSynapseType) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::createSynapse(string _strSynapseType)" << endl;
#endif

    strSynapseType = _strSynapseType;
    int iReturn = 0;

//    cout << "SynapseType: " << _strSynapseType << endl;
    try{
	pSynapse = SynapseManager::instance().createByType(_strSynapseType);
	pSynapse->setConnectionID(strConnectionID);
    }
    catch (iqrcommon::UnknownTypeError &e) {
	string strError = "Connection \"" + getConnectionName() + "\" Couldn't create Unknown type synapse: ";
	strError.append(e.what());
	cerr << strError << endl;
	strSynapseType = "";
	iReturn = -1;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	iReturn = -1;
    }

    return iReturn;
}

int ClsBaseConnection::createSynapseByLabel(string strSynapseLabel) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::createSynapseByLabel(string strSynapseLabel)" << endl;
#endif
    int iReturn = 0;
    try{
	pSynapse = SynapseManager::instance().createByLabel(strSynapseLabel);
	pSynapse->setConnectionID(strConnectionID);
	strSynapseType =SynapseManager::instance().label2type(strSynapseLabel);
    }
    catch (iqrcommon::UnknownTypeError &e) {
	string strError = "Connection \"" + getConnectionName() + "\" Couldn't create Unknown type synapse: ";
	strError.append(e.what());
	cerr << strError << endl;
	strSynapseType = "";
	iReturn = -1;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	iReturn = -1;
    }
    return iReturn;
}

void ClsBaseConnection::setSynapseParameter(ClsSysFileNode clsSysFileNodeSynapseParameter) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setSynapseParameter( ClsSysFileNode _clsSysFileNodeSynapseParameter )" << endl;
#endif

    while (clsSysFileNodeSynapseParameter.countParameters()){
	iqrcommon_old::ClsParameter clsParameterTemp = clsSysFileNodeSynapseParameter.popParameter();
	string strParamName = clsParameterTemp.getName();
	string strParamValue = clsParameterTemp.getValueAsString();

	if(!strParamName.compare(ClsTagLibrary::NameTag())) {
	    createSynapse(strParamValue);

	}
    }

    if(pSynapse!=NULL){
	while(clsSysFileNodeSynapseParameter.countNodes()>0){
	    ClsSysFileNode F1 = clsSysFileNodeSynapseParameter.popNode();
	    string strNodeName = F1.getName();
	    if(!strNodeName.compare(ClsTagLibrary::ParameterTag())) {
		string strParamName = "";
		string strParamValue = "";
		while (F1.countParameters()){
		    iqrcommon_old::ClsParameter clsParameterTemp = F1.popParameter();
		    if(!clsParameterTemp.getName().compare(ClsTagLibrary::NameTag())){
			strParamName = clsParameterTemp.getValueAsString();
		    }
		    if(!clsParameterTemp.getName().compare(ClsTagLibrary::ClsTagLibrary::ValueTag())){
			strParamValue = clsParameterTemp.getValueAsString();
		    }
		}
		pSynapse->setParameter(strParamName, strParamValue);
	    }

	    else if(!strNodeName.compare(ClsTagLibrary::XRefHolderTag())) {
		/*
		string strXRefHolderName = F1.popParameter().getValueAsString();
		cout << "== strXRefHolderName: " << strXRefHolderName << endl;
		*/
		while(F1.countNodes()>0){
		    ClsSysFileNode F2 = F1.popNode();
		    string strXRefName = "";
		    string strXRefTarget = "";
		    while (F2.countParameters()){
			iqrcommon_old::ClsParameter clsParameterTemp = F2.popParameter();
			if(!clsParameterTemp.getName().compare(ClsTagLibrary::NameTag())){
			    strXRefName = clsParameterTemp.getValueAsString();
			}
			if(!clsParameterTemp.getName().compare(ClsTagLibrary::ClsTagLibrary::TargetTag())){
			    strXRefTarget = clsParameterTemp.getValueAsString();
			}
		    }
//		    cout << "@@@ strXRefName, strXRefTarget: " << strXRefName << ", " << strXRefTarget << endl;
		    pSynapse->setFeedbackInputByName( strXRefName,  strXRefTarget);
		}
	    }
	}
    }
};

const ParameterList ClsBaseConnection::getListSynapseParameters() const  {
//const ParameterList& ClsBaseConnection::getListSynapseParameters() const  {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::getListSynapseParameters() const" << endl;
#endif

    if(pSynapse!=NULL){
	return pSynapse->getListParameters();
    } else {
	cerr << "pSynapse is NULL" << endl;

    }

    ParameterList mt;
    return mt;
}

string ClsBaseConnection::getConnectionSynapseLabel() {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::getConnectionSynapseLabel()" << endl;
#endif
//    cout << "strSynapseType: " << strSynapseType << endl;
    
    if(strSynapseType.length()>0){
	try {
	    return SynapseManager::instance().type2label(strSynapseType);
	}
	catch (iqrcommon::UnknownTypeError &e) {
	    return "";
	}
    }
    else {
	return "";
    }
};


StateVariableList ClsBaseConnection::getListSynapseStateVariables( ){
#ifdef DEBUG_CLSBASECONNECTION
	cout << "StateVariableList ClsBaseConnection::getListSynapseStateVariables( )" << endl;
#endif

	if(pSynapse!=NULL){
	    return (pSynapse->getListStates());
	}
	StateVariableList mt;
	return mt;
	
};

list<string> ClsBaseConnection::getListSynapseXRefHolders(){
    if(pSynapse!=NULL){
	return pSynapse->getListOfXRefHolders();
    } else {
	cerr << "pSynapse is NULL" << endl;
	list<string> lst;
	return lst;
    }
}
 
const ClsXRefHolder* ClsBaseConnection::getSynapseXRefHolder(string strName) const {
    if(pSynapse!=NULL){
	return pSynapse->getXRefHolder(strName);
    } else {
	cerr << "pSynapse is NULL" << endl;
	return NULL;
    }
};



int ClsBaseConnection::setSynapseParameter(string strParamName, string strParamValue) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setSynapseParameter(string strParamName, string strParamValue)" << endl;
#endif
    if(pSynapse->getParameter(strParamName)!=NULL){
	pSynapse->setParameter(strParamName, strParamValue);
	return 0;
    }
    return -1;
}

list<connection_pattern>* ClsBaseConnection::getTPattern() {
    return &pattern;
}


void ClsBaseConnection::setConnectionType(string strConnectionType){
    setParameter(ClsTagLibrary::ConnectionTypeTag(), strConnectionType);
};

void ClsBaseConnection::setConnectionType(int iConnectionType){
    string strConnectionType = convertIntConnectiontype2StringConnectiontype(iConnectionType);
    setParameter(ClsTagLibrary::ConnectionTypeTag(), strConnectionType);
//    cout << "strConnectionType: " << strConnectionType << endl;
//    exit(0);
};

string ClsBaseConnection::getConnectionTypeAsString() {
    return getParameter(ClsTagLibrary::ConnectionTypeTag())->getValueAsString();
}


int ClsBaseConnection::getConnectionTypeAsInt() {
    string strConnectionType = getParameter(ClsTagLibrary::ConnectionTypeTag())->getValueAsString();
    return convertStringConnectiontype2IntConnectiontype(strConnectionType);
}

string ClsBaseConnection::convertIntConnectiontype2StringConnectiontype(int iConnectionType){
    string strConnectionType;
    if(iConnectionType == ClsBaseConnection::CONN_EXCITATORY){
	strConnectionType = ClsTagLibrary::ExcitatoryConnectionTag();
    }
    else if(iConnectionType == ClsBaseConnection::CONN_INHIBITORY){
	strConnectionType = ClsTagLibrary::InhibitoryConnectionTag();
    }
    else if(iConnectionType == ClsBaseConnection::CONN_MODULATORY){
	strConnectionType = ClsTagLibrary::ModulatoryConnectionTag();
    }
    else{
	strConnectionType = ClsTagLibrary::ExcitatoryConnectionTag();
    }
    return strConnectionType;
};

int ClsBaseConnection::convertStringConnectiontype2IntConnectiontype(string strConnectionType){
    int iConnectionType;
    if(!strConnectionType.compare(ClsTagLibrary::ExcitatoryConnectionTag())){
	iConnectionType = ClsBaseConnection::CONN_EXCITATORY;
    }
    else if(!strConnectionType.compare(ClsTagLibrary::InhibitoryConnectionTag())){
	iConnectionType = ClsBaseConnection::CONN_INHIBITORY;
    }
    else if(!strConnectionType.compare(ClsTagLibrary::ModulatoryConnectionTag())){
	iConnectionType = ClsBaseConnection::CONN_MODULATORY;
    }
    else {
	iConnectionType = ClsBaseConnection::CONN_EXCITATORY;
    }

    return iConnectionType;
};

string ClsBaseConnection::ArborizationType() const {
    if (clsArborization == NULL) {
	return "";
    }
    return clsArborization->Type();
};

void ClsBaseConnection::setArborizationType(const string &_strArborizationType) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setArborizationType(const string &_strArborizationType)" << endl;
#endif
//    cout << "strArborizationType: " << _strArborizationType << endl;

    TArborizationMap::iterator tmapitArborization;
    if ((tmapitArborization = tmapArborizations.find(_strArborizationType)) != tmapArborizations.end()) {
#ifdef DEBUG_CLSBASECONNECTION
	cout << "reusing arborization" << endl;
#endif
	clsArborization = tmapitArborization->second;
    } else {
	if(find(lstArborizationTypes.begin(), lstArborizationTypes.end(), _strArborizationType) != lstArborizationTypes.end()){
#ifdef DEBUG_CLSBASECONNECTION
	    cout << "instantiating new arborization" << endl;
#endif
	    if(!_strArborizationType.compare(ClsTagLibrary::ConnectionArborizationRectangular())){
		clsArborization = new ClsArbRect();
	    }
	    else if(!_strArborizationType.compare(ClsTagLibrary::ConnectionArborizationEllipse())){
		clsArborization = new ClsArbEllipse();
	    }
	    else if(!_strArborizationType.compare(ClsTagLibrary::ConnectionArborizationRectangularWindow())){
		clsArborization = new ClsArbRectWindow();
	    }
	    else if(!_strArborizationType.compare(ClsTagLibrary::ConnectionArborizationEllipseWindow())){
		clsArborization = new ClsArbEllipseWindow();
	    }
	    else if(!_strArborizationType.compare(ClsTagLibrary::ConnectionArborizationAll())){
		clsArborization = new ClsArbAll();
	    }
	    else if(!_strArborizationType.compare(ClsTagLibrary::ConnectionArborizationRandom())){
		clsArborization = new ClsArbRandom();
	    }
	    else {
		clsArborization = new ClsArbRect();
	    }
	    pair <string,ClsBaseArborization*> pairTemp(clsArborization->Type(), clsArborization);
	    tmapArborizations.insert(pairTemp);
	} else {
	    cerr << "Type of Arborization not valid" << endl;
	}
    }
};

string ClsBaseConnection::AttenuationFunctionType() const {
    if (clsAttenuationFunction == NULL) {
	return "";
    }
    return clsAttenuationFunction->Type();
};

string ClsBaseConnection::DelayFunctionType() const {
    if (clsDelayFunction == NULL) {
	return "";
    }
    return clsDelayFunction->Type();
};


void ClsBaseConnection::setAttenuationFunctionParameter(ClsSysFileNode &_clsSysFileNodeFunctionParameter){
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setFunctionParameter(ClsSysFileNode _clsSysFileNodeFunctionParameter)" << endl;
#endif

    ClsSysFileNode F3 = _clsSysFileNodeFunctionParameter.popNode();
    string strFunctionType = F3.getName();
    setAttenuationFunctionType(strFunctionType);
    while (F3.countParameters()){
	iqrcommon_old::ClsParameter clsParameterTemp = F3.popParameter();
	string strParamName = clsParameterTemp.getName();
	string strParamValue = clsParameterTemp.getValueAsString();
//	cout << "strParamName: " << strParamName << endl;
//	cout << "strParamValue: " << strParamValue << endl;
	clsAttenuationFunction->setParameter(strParamName, strParamValue);
    }
}

void ClsBaseConnection::setDelayFunctionParameter(ClsSysFileNode &_clsSysFileNodeFunctionParameter){
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setFunctionParameter(ClsSysFileNode _clsSysFileNodeFunctionParameter)" << endl;
#endif

    ClsSysFileNode F3 = _clsSysFileNodeFunctionParameter.popNode();
    string strFunctionType = F3.getName();
    setDelayFunctionType(strFunctionType);
    while (F3.countParameters()){
	iqrcommon_old::ClsParameter clsParameterTemp = F3.popParameter();
	string strParamName = clsParameterTemp.getName();
	string strParamValue = clsParameterTemp.getValueAsString();
//	cout << "strParamName: " << strParamName << endl;
//	cout << "strParamValue: " << strParamValue << endl;
	clsDelayFunction->setParameter(strParamName, strParamValue);
    }
}

void ClsBaseConnection::setAttenuationFunctionType(const string &_strAttenuationFunctionType) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setAttenuationFunctionType(const string &_strAttenuationFunctionType)" << endl;
#endif
//    cout << "strAttenuationFunctionType: " << _strAttenuationFunctionType << endl;

    TFunctionMap::iterator tmapitAttenuationFunction;
    if ((tmapitAttenuationFunction = tmapAttenuationFunctions.find(_strAttenuationFunctionType)) != tmapAttenuationFunctions.end()) {
#ifdef DEBUG_CLSBASECONNECTION
	cout << "reusing AttenuationFunction" << endl;
#endif
	clsAttenuationFunction = tmapitAttenuationFunction->second;
    } else {
	if(find(lstFunctionTypes.begin(), lstFunctionTypes.end(), _strAttenuationFunctionType) != lstFunctionTypes.end()){
#ifdef DEBUG_CLSBASECONNECTION
	    cout << "instantiating new AttenuationFunction" << endl;
#endif
	    if(!_strAttenuationFunctionType.compare(ClsTagLibrary::ConnectionFunctionGaussian())){
		clsAttenuationFunction = new ClsFunGaussian(ClsBaseFunction::attenuation);
	    }
	    else if(!_strAttenuationFunctionType.compare(ClsTagLibrary::ConnectionFunctionLinear())){
		clsAttenuationFunction = new ClsFunLinear(ClsBaseFunction::attenuation);
	    }
	    else if(!_strAttenuationFunctionType.compare(ClsTagLibrary::ConnectionFunctionBlock())){
		clsAttenuationFunction = new ClsFunBlock(ClsBaseFunction::attenuation);
	    }
	    else if(!_strAttenuationFunctionType.compare(ClsTagLibrary::ConnectionFunctionRandom())){
		clsAttenuationFunction = new ClsFunRandom(ClsBaseFunction::attenuation);
	    }
	    else if(!_strAttenuationFunctionType.compare(ClsTagLibrary::ConnectionFunctionUniform())){
		clsAttenuationFunction = new ClsFunUniform(ClsBaseFunction::attenuation);
	    }
/*
  else if(!_strAttenuationFunctionType.compare(ClsTagLibrary::ConnectionFunctionKernel())){
  clsAttenuationFunction = new ClsFunKernel();
  }
*/
	    else {
		clsAttenuationFunction = new ClsFunGaussian(ClsBaseFunction::attenuation);
	    }
	    pair <string,ClsBaseFunction*> pairTemp(clsAttenuationFunction->Type(), clsAttenuationFunction);
	    tmapAttenuationFunctions.insert(pairTemp);
	} else {
	    cerr << "Type of AttenuationFunction not valid" << endl;
	}
    }
};

void ClsBaseConnection::setDelayFunctionType(const string &_strDelayFunctionType) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setDelayFunctionType(const string &_strDelayFunctionType)" << endl;
#endif
//    cout << "strDelayFunctionType: " << _strDelayFunctionType << endl;

    TFunctionMap::iterator tmapitDelayFunction;
    if ((tmapitDelayFunction = tmapDelayFunctions.find(_strDelayFunctionType)) != tmapDelayFunctions.end()) {
#ifdef DEBUG_CLSBASECONNECTION
	cout << "reusing DelayFunction" << endl;
#endif
	clsDelayFunction = tmapitDelayFunction->second;
    } else {
	if(find(lstFunctionTypes.begin(), lstFunctionTypes.end(), _strDelayFunctionType) != lstFunctionTypes.end()){
#ifdef DEBUG_CLSBASECONNECTION
	    cout << "instantiating new DelayFunction" << endl;
#endif
	    if(!_strDelayFunctionType.compare(ClsTagLibrary::ConnectionFunctionGaussian())){
		clsDelayFunction = new ClsFunGaussian(ClsBaseFunction::delay);
	    }
	    else if(!_strDelayFunctionType.compare(ClsTagLibrary::ConnectionFunctionLinear())){
		clsDelayFunction = new ClsFunLinear(ClsBaseFunction::delay);
	    }
	    else if(!_strDelayFunctionType.compare(ClsTagLibrary::ConnectionFunctionBlock())){
		clsDelayFunction = new ClsFunBlock(ClsBaseFunction::delay);
	    }
	    else if(!_strDelayFunctionType.compare(ClsTagLibrary::ConnectionFunctionRandom())){
		clsDelayFunction = new ClsFunRandom(ClsBaseFunction::delay);
	    }
	    else if(!_strDelayFunctionType.compare(ClsTagLibrary::ConnectionFunctionUniform())){
		clsDelayFunction = new ClsFunUniform(ClsBaseFunction::delay);
	    }
/*
  else if(!_strDelayFunctionType.compare(ClsTagLibrary::ConnectionFunctionKernel())){
  clsDelayFunction = new ClsFunKernel();
  }
*/
	    else {
		clsDelayFunction = new ClsFunGaussian(ClsBaseFunction::delay);
	    }
	    pair <string,ClsBaseFunction*> pairTemp(clsDelayFunction->Type(), clsDelayFunction);
	    tmapDelayFunctions.insert(pairTemp);
	} else {
	    cerr << "Type of DelayFunction not valid" << endl;
	}
    }
};

string ClsBaseConnection::PatternType() const {
    if (clsPattern == NULL) {
	return "";
    }
    return clsPattern->Type();
};

void ClsBaseConnection::setPatternParameter(ClsSysFileNode &_clsSysFileNodePatternParameter){
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setPatternParameter(ClsSysFileNode _clsSysFileNodePatternParameter)" << endl;
#endif


    ClsSysFileNode F2 = _clsSysFileNodePatternParameter.popNode();
    string strPatternType = F2.getName();

#ifdef DEBUG_CLSBASECONNECTION
    cout << "strPatternType: " << strPatternType << endl;
#endif


    setPatternType(strPatternType);
    if(!strPatternType.compare(ClsTagLibrary::ConnectionPatternMapped()) || !strPatternType.compare(ClsTagLibrary::ConnectionPatternForeach())){
	ClsSysFileNode clsSysFileNodeSubPopulationSource = F2.popNode();  // KABOUM
	clsPattern->setSourcePopulationParameter(clsSysFileNodeSubPopulationSource);

	ClsSysFileNode clsSysFileNodeSubPopulationTarget = F2.popNode();
	clsPattern->setTargetPopulationParameter(clsSysFileNodeSubPopulationTarget);

    }
    else if(!strPatternType.compare(ClsTagLibrary::ConnectionPatternTuples())){
	while (F2.countNodes()){
	    ClsSysFileNode clsSysFileNodeTuple = F2.popNode();
	    clsPattern->setTupleParameter(clsSysFileNodeTuple);
	}
    }

    while (F2.countParameters()){
	iqrcommon_old::ClsParameter clsParameterTemp = F2.popParameter();
	string strParamName = clsParameterTemp.getName();
	string strParamValue = clsParameterTemp.getValueAsString();
//	cout << "strParamName: " << strParamName << endl;
//	cout << "strParamValue: " << strParamValue << endl;
	clsPattern->setParameter(strParamName, strParamValue);
    }

//    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;


}

void ClsBaseConnection::setPatternType(const string &_strPatternType) {
#ifdef DEBUG_CLSBASECONNECTION
    cout << "ClsBaseConnection::setPatternType(const string &_strPatternType)" << endl;
#endif

    TPatternMap::iterator tmapitPattern;
    if ((tmapitPattern = tmapPatterns.find(_strPatternType)) != tmapPatterns.end()) {
#ifdef DEBUG_CLSBASECONNECTION
	cout << "reusing pattern" << endl;
#endif
	clsPattern = tmapitPattern->second;
    } else {
	if(find(lstPatternTypes.begin(), lstPatternTypes.end(), _strPatternType) != lstPatternTypes.end()){
#ifdef DEBUG_CLSBASECONNECTION
	    cout << "instantiating new pattern" << endl;
#endif
	    if(!_strPatternType.compare(ClsTagLibrary::ConnectionPatternMapped())){
		clsPattern = new ClsPatternMapped();
	    }
	    else if(!_strPatternType.compare(ClsTagLibrary::ConnectionPatternForeach())){
		clsPattern = new ClsPatternForeach();
	    }
	    else if(!_strPatternType.compare(ClsTagLibrary::ConnectionPatternTuples())){
		clsPattern = new ClsPatternTuples();
	    }
	    else {
		clsPattern = new ClsPatternMapped();
	    }
	    pair <string,ClsBasePattern*> pairTemp(clsPattern->Type(), clsPattern);
	    tmapPatterns.insert(pairTemp);
	} else {
	    cerr << "Type of Pattern not valid" << endl;
	}
    }
};



void ClsBaseConnection::printTupleList(list<pair<tListOfPairs, tListOfPairs> > lstPLP) {
    list<pair<tListOfPairs, tListOfPairs> >::iterator it0;
    for(it0=lstPLP.begin(); it0!=lstPLP.end(); it0++){

	tListOfPairs tListOfPairsSource = (*it0).first;
	tListOfPairs tListOfPairsTarget = (*it0).second;

//	    cout << "--------------" << endl;
	cout << "(";
	tListOfPairs::iterator itLOPSource;
	for(itLOPSource=tListOfPairsSource.begin();itLOPSource!=tListOfPairsSource.end();itLOPSource++){
	    int iXSource = (*itLOPSource).first;
	    int iYSource = (*itLOPSource).second;
//		int iIndexSource = clsBaseTopologySource->pos2index(iXSource, iYSource);
	    cout << "(" << iXSource << "," << iYSource << "), ";
	}
	cout << ")<->(";
	tListOfPairs::iterator itLOPTarget;

	for(itLOPTarget=tListOfPairsTarget.begin();itLOPTarget!=tListOfPairsTarget.end();itLOPTarget++){
	    int iXTarget = (*itLOPTarget).first;
	    int iYTarget = (*itLOPTarget).second;
//		int iIndexTarget = clsBaseTopologyTarget->pos2index(iXTarget, iYTarget);
//		cout << iIndexTarget << ", ";
	    cout << "(" << iXTarget << "," << iYTarget << "), ";
	}
	cout << ")" << endl;

    }
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
