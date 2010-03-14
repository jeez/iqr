#include "ClsFEProcess.h"
#include "tagLibrary.hpp"
#include "iqrUtils.h"

#include "optionsParameter.hpp"
#include "stringParameter.hpp"
#include "intParameter.hpp"

#include "moduleManager.hpp"
#include "ClsFESystemManager.h"
#include "ClsFEGroup.h"
#include "ClsQLogWindow.h"

using iqrfe::ModuleManager;

//#define DEBUG_CLSFEPROCESS

ClsFEProcess::ClsFEProcess() {
    strProcessID = "";
    iLastUpdateTime = 0;
    iProcessStatus = 0;
    iTimeDiff = 0;
    pModule = NULL;

    addStringParameter(ClsTagLibrary::NameTag(), "Process Name",
		       "",
		       true, false,
 		       "Name",
		       "Properties");
    defineParameters();
};


ClsFEProcess::ClsFEProcess(string _strProcessID) : strProcessID(_strProcessID ) {
    iLastUpdateTime = 0;
    iProcessStatus = 0;
    iTimeDiff = 0;
    pModule = NULL;

    addStringParameter(ClsTagLibrary::NameTag(), "Process Name",
		       "",
		       true, false,
 		       "Name",
		       "Properties");
    defineParameters();
};



ClsFEProcess::ClsFEProcess(string _strProcessID, string _strProcessName, string /*_strHostname*/, int /*_iPort*/) : strProcessID(_strProcessID ) {
    iLastUpdateTime = 0;
    iProcessStatus = 0;
    iTimeDiff = 0;
    pModule = NULL;

    addStringParameter(ClsTagLibrary::NameTag(), "Process Name",
		       _strProcessName,
		       true, false,
 		       "Name",
		       "Properties");
    defineParameters();
};

void ClsFEProcess::defineParameters(  ) {


    addStringParameter(ClsTagLibrary::NotesTag(), "Notes",
		       "",
		       true, true,
		       "",
		       "Notes");


    clsBoolParameterEnableModule = addBoolParameter(ClsTagLibrary::EnableModuleTag(), /* _strName */
						  "Enable Module", /* _strLabel */
						  true, /* _bValue */
						  "Should the Module be enabled?", /* _strDescription */
						  "Properties" /* _strCategory */);

    addStringParameter(ClsTagLibrary::PathTag(), /* _strName */ 
						 "External Path", /* _strLabel */
						 "", /* _strValue */
						 false, /* _bEditable */
						 true, /* _bLong */
						 "Path to external processes", /* _strDescription */
						 "Properties"/* _strCategory */);


};


void ClsFEProcess::setProcessID( string _str ) { strProcessID = _str ; };
string ClsFEProcess::getProcessID(){ return strProcessID; };

void ClsFEProcess::setProcessName( string _str ){ setParameter(ClsTagLibrary::NameTag(),  _str); };
string ClsFEProcess::getProcessName(){
    return getParameter(ClsTagLibrary::NameTag())->getValueAsString();
};


void ClsFEProcess::setNotes(string _str){ setParameter(ClsTagLibrary::NotesTag(),  _str); };
string ClsFEProcess::getNotes() { return getParameter(ClsTagLibrary::NotesTag())->getValueAsString(); };


void ClsFEProcess::setHostname( string _str) { setParameter(ClsTagLibrary::HostnameTag(),  _str); };
string ClsFEProcess::getHostname( ){
    return getParameter(ClsTagLibrary::HostnameTag())->getValueAsString();
};


string ClsFEProcess::getPath( ){
    return getParameter(ClsTagLibrary::PathTag())->getValueAsString();
};


void ClsFEProcess::setPort( int _iPort){ setParameter(ClsTagLibrary::PortTag(),  iqrUtils::int2string(_iPort)); };
int ClsFEProcess::getPort ( ){
    return iqrUtils::string2int(getParameter(ClsTagLibrary::PortTag())->getValueAsString());
};
string ClsFEProcess::getPortAsString ( ){
    return getParameter(ClsTagLibrary::PortTag())->getValueAsString();
};

bool ClsFEProcess::getEnableModules( ){
    return clsBoolParameterEnableModule->getValue();
};

string ClsFEProcess::getEnableModulesAsString( ){
    return getParameter(ClsTagLibrary::EnableModuleTag())->getValueAsString();
};

void ClsFEProcess::setLastUpdateTime ( int _iLastUpdateTime ){
    iTimeDiff = (abs(iLastUpdateTime - _iLastUpdateTime) + iTimeDiff) / 2; // average over last two times
    //	  cout << "iTimeDiff: " << iTimeDiff << endl;
    iLastUpdateTime = _iLastUpdateTime;
};

int ClsFEProcess::getLastUpdateTime ( ){ return iLastUpdateTime;};

void ClsFEProcess::setProcessUID( string _strProcessUID){ strProcessUID = _strProcessUID; };
string ClsFEProcess::getProcessUID( ){ return strProcessUID;};

void ClsFEProcess::setStatus ( int _iProcessStatus ){ iProcessStatus = _iProcessStatus;};
int ClsFEProcess::getStatus ( ){ return iProcessStatus;};


void ClsFEProcess::setProcessSubNodes(ClsSysFileNode _clsSysFileNodeProcessParameter){
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::setProcessSubNodes(ClsSysFileNode _clsSysFileNodeProcessParameter)" << endl;
#endif

    clsSysFileNodeProcessParameter = _clsSysFileNodeProcessParameter;
    while (_clsSysFileNodeProcessParameter.countNodes()){
	ClsSysFileNode F1 = _clsSysFileNodeProcessParameter.popNode();
	string strNodeName = F1.getName();
#ifdef DEBUG_CLSFEPROCESS
	cout << "strNodeName: " << strNodeName << endl;
#endif
	if(!strNodeName.compare(ClsTagLibrary::ModuleTag())) {
	    setModuleParameter(F1);
	}
    }
}


list<string> ClsFEProcess::getListOfMembers() {
    list<string> lst;
    lst.push_back(ClsTagLibrary::ModuleTag());
    return lst;
}

ClsItem* ClsFEProcess::getMemberByName(string strName) {
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::getMemberByName(string strName)" << endl;
#endif

    if(!strName.compare(ClsTagLibrary::ModuleTag())){
	return pModule;
    }
    return NULL;
}

void ClsFEProcess::setMemberTypeByName(string strName, string strValue) {
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::setMemberTypeByName(string strName, string strValue)" << endl;
#endif
    if(strValue.size()>0){
	if(!strName.compare(ClsTagLibrary::ModuleTag())){
	    createModuleByLabel(strValue);
	}
    }
    else {
	try {
	    ModuleManager::instance().destroy(pModule);
	    pModule = NULL;
	}
	catch(...){
	    cerr << "could not destroy Module" << endl;
	}
	pModule = NULL;
	strModuleType = "";
    }
}

string ClsFEProcess::getMemberTypeByName(string strName){
    if(!strName.compare(ClsTagLibrary::ModuleTag())){
	return getProcessModuleLabel();
    }
    return "";
}


list<string> ClsFEProcess::getListOfMembersTypes(string strName) {
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::getListOfMembersTypes(string strName)" << endl;
#endif

    list<string> lst;
    if(!strName.compare(ClsTagLibrary::ModuleTag())){
	lst = ModuleManager::instance().getListLabels();
	lst.push_back(""); /* no module option!!! */
    }
    return lst;
}

int ClsFEProcess::createModule(string _strModuleType) {
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::createModule(string _strModuleType)" << endl;
#endif

    strModuleType = _strModuleType;
    int iReturn = 0;
#ifdef DEBUG_CLSFEPROCESS
    cout << "ModuleType: " << _strModuleType << endl;
#endif
    try{
	pModule = ModuleManager::instance().createByType(_strModuleType);
	pModule->setProcessID(strProcessID);
    }
    catch (iqrcommon::UnknownTypeError &e) {
	string strError = "Process \"" + getProcessName() + "\" Couldn't create Unknown type module: ";
	strError.append(e.what());
	cerr << strError << endl;
	strModuleType = "";
	iReturn = -1;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
    }

    return iReturn;
}

int ClsFEProcess::createModuleByLabel(string strModuleLabel) {
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::createModuleByLabel(string strModuleLabel)" << endl;
#endif
    int iReturn = 0;
    try{
	pModule = ModuleManager::instance().createByLabel(strModuleLabel);
	pModule->setProcessID(strProcessID);
	strModuleType =ModuleManager::instance().label2type(strModuleLabel);
    }
    catch (iqrcommon::UnknownTypeError &e) {
	string strError = "Process \"" + getProcessName() + "\" Couldn't create Unknown type module: ";
	strError.append(e.what());
	cerr << strError << endl;
	strModuleType = "";
	iReturn = -1;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
    }
    return iReturn;
}

void ClsFEProcess::setModuleParameter(ClsSysFileNode clsSysFileNodeModuleParameter) {
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::setModuleParameter( ClsSysFileNode _clsSysFileNodeModuleParameter )" << endl;
#endif
    while (clsSysFileNodeModuleParameter.countParameters()){
	iqrcommon_old::ClsParameter clsParameterTemp = clsSysFileNodeModuleParameter.popParameter();
	string strParamName = clsParameterTemp.getName();
	string strParamValue = clsParameterTemp.getValueAsString();

	if(!strParamName.compare(ClsTagLibrary::NameTag())) {
	    createModule(strParamValue);
	}
    }

    if(pModule!=NULL){
	while(clsSysFileNodeModuleParameter.countNodes()>0){
	    ClsSysFileNode F1 = clsSysFileNodeModuleParameter.popNode();
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
//		cout << "strParamName, strParamValue: " << strParamName << ", " << strParamValue << endl;
		pModule->setParameter(strParamName, strParamValue);
	    }
	    else if(!strNodeName.compare(ClsTagLibrary::XRefHolderTag())) {
		string strXRefHolderName = F1.popParameter().getValueAsString();
//		cout << "== strXRefHolderName: " << strXRefHolderName << endl;
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
		    pModule->setXRefTargetByName( strXRefHolderName,  strXRefName,  strXRefTarget);
		}
	    }
	}
    }
};

const ParameterList ClsFEProcess::getListModuleParameters() const  {
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::getListModuleParameters() const" << endl;
#endif

    if(pModule!=NULL){
	return pModule->getListParameters();
    } else {
	cerr << "pModule is NULL" << endl;
    }
    ParameterList mt;
    return mt;
}

list<string> ClsFEProcess::getListModuleXRefHolders(){
    if(pModule!=NULL){
	return pModule->getListOfXRefHolders();
    } else {
	cerr << "pModule is NULL" << endl;
	list<string> lst;
	return lst;
    }
}

const ClsXRefHolder* ClsFEProcess::getModuleXRefHolder(string strName) const {
    if(pModule!=NULL){
	return pModule->getXRefHolder(strName);
    } else {
	cerr << "pModule is NULL" << endl;
	return NULL;
    }
};

string ClsFEProcess::getProcessModuleLabel() {
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::getProcessModuleLabel()" << endl;
#endif

//    cout << "strModuleType: " << strModuleType << endl;
    
    if(strModuleType.length()>0){
	try {
	    return ModuleManager::instance().type2label(strModuleType);
	}
	catch (iqrcommon::UnknownTypeError &e) {
	    return "";
	}
    }
    else {
	return "";
    }
};

void ClsFEProcess::setModuleParameter(string strParamName, string strParamValue) {
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::setModuleParameter(string strParamName, string strParamValue)" << endl;
#endif
    pModule->setParameter(strParamName, strParamValue);
}

moduleIcon ClsFEProcess::getModuleIcon() {
    if(pModule!=NULL){
	return pModule->getIcon();
    }
    moduleIcon mi(NULL, 0);
    return mi;
}

bool ClsFEProcess::hasModule(){
    if(pModule!=NULL){
	return true;
    }
    return false;
}


list<string> ClsFEProcess::getListGroupsInputFromModule(){
    list<string> lst;
    if(pModule!=NULL){
	const XRefList xRefList = pModule->getListOutputsToGroups();
	XRefList::const_iterator itXRef;
	for(itXRef=xRefList.begin(); itXRef!=xRefList.end(); itXRef++){
	    string strXRefTarget = (*itXRef)->getTarget();
	    lst.push_back(strXRefTarget);
	}
    }
    return lst;
}

list<string> ClsFEProcess::getListGroupsOutputToModule(){
    list<string> lst;
    if(pModule!=NULL){
	const XRefList xRefList = pModule->getListInputsFromGroups();
	XRefList::const_iterator itXRef;
	for(itXRef=xRefList.begin(); itXRef!=xRefList.end(); itXRef++){
	    string strXRefTarget = (*itXRef)->getTarget();
	    lst.push_back(strXRefTarget);
	}
    }
    return lst;
}

string ClsFEProcess::validate(){
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::validate()" << endl;
#endif
    string strMessage = "";
    string strModuleMessage = "";

    if(pModule!=NULL){
	XRefList::iterator itXRef;


	/* check xrefs */
	XRefList xRefListOTG = pModule->getListOutputsToGroups();
	for(itXRef=xRefListOTG.begin(); itXRef!=xRefListOTG.end(); itXRef++){
	    string strXRefTarget = (*itXRef)->getTarget();
	    string strXRefLabel = (*itXRef)->getLabel();
	    if( ClsFESystemManager::Instance()->getFEGroup(strXRefTarget)==NULL ){
		strModuleMessage.append("\n\tModule cannot set output for reference: \"" + strXRefLabel + "\"");
	    } 
	}
	
	XRefList xRefListIFG = pModule->getListInputsFromGroups();
	for(itXRef=xRefListIFG.begin(); itXRef!=xRefListIFG.end(); itXRef++){
	    string strXRefTarget = (*itXRef)->getTarget();
	    string strXRefLabel = (*itXRef)->getLabel();
	    if( ClsFESystemManager::Instance()->getFEGroup(strXRefTarget) == NULL ){
		strModuleMessage.append("\n\tModule cannot set input for reference: \"" + strXRefLabel + "\"");
	    } 
	}

	if(strModuleMessage.size()>0){
	    strMessage = "Process \"" + getProcessName() + "\": ";
	    strMessage.append(strModuleMessage);
	}


    }
    return strMessage;
}


void ClsFEProcess::init(){
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::init()" << endl;
#endif

    bModuleEnabled = clsBoolParameterEnableModule->getValue();

    if(bModuleEnabled && pModule!=NULL){
	initModule();
    }

}

void ClsFEProcess::initModule(){
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::initModule()" << endl;
#endif



    if(pModule!=NULL){
	if(dynamic_cast<ClsThreadModule*>(pModule)){
//	    cout << "--CAST OK: dynamic_cast<ClsThreadModule*>(pModule)" << endl;
	    bModuleThreaded = true;
	} else {
//	    cout << "--cast not OK" << endl;
	    bModuleThreaded = false;
	}
//bModuleThreaded = false;
	cout << "\t" << getProcessName() << "::Threaded Module: " << (bModuleThreaded ? "yes" : "no") << endl;


	XRefList::iterator itXRef;

/* Module output to groups */
	XRefList xRefList = pModule->getListOutputsToGroups();
	for(itXRef=xRefList.begin(); itXRef!=xRefList.end(); itXRef++){
	    string strXRefName = (*itXRef)->getName();
	    string strXRefTarget = (*itXRef)->getTarget();

	    ClsStateVariable* clsStateVariable = pModule->getState(strXRefName);
	    unsigned int nNeuronsPost = ClsFESystemManager::Instance()->getFEGroup(strXRefTarget)->getNumberOfNeurons();
/* NEW */
	    clsStateVariable->setNrCellsHorizontal(ClsFESystemManager::Instance()->getFEGroup(strXRefTarget)->getNrCellsHorizontal());
	    clsStateVariable->setNrCellsVertical(ClsFESystemManager::Instance()->getFEGroup(strXRefTarget)->getNrCellsVertical());
/* ---- */	    
	    
	    StateArray *pStateArray = &(clsStateVariable->getStateArray());
	    if(pStateArray!=NULL){
		pStateArray->setWidth(nNeuronsPost);
		ClsFESystemManager::Instance()->getFEGroup(strXRefTarget)->getNeuron()->addExcitatoryInput(pStateArray);
	    }
	}

/*  ------------------- */


/* Module input from groups (StateVariablePtrList inputFromGroupPtrList) */
	StateVariablePtrList::iterator itStateVariablePtrList;
	for(itStateVariablePtrList = pModule->getListInputFromGroupPtrs().begin();
	    itStateVariablePtrList != pModule->getListInputFromGroupPtrs().end();
	    itStateVariablePtrList++){

	    string strGroupName = (*itStateVariablePtrList)->getName();
	    string strXRefTarget = pModule->getInputFromGroupRefByName(strGroupName)->getTarget();

	    ClsStateVariable* clsStateVariable = ClsFESystemManager::Instance()->getFEGroup(strXRefTarget)->getNeuron()->getOutputState();
	    if(clsStateVariable!=NULL){
		(*itStateVariablePtrList)->setTarget(clsStateVariable);

/* NEW */
	    clsStateVariable->setNrCellsHorizontal(ClsFESystemManager::Instance()->getFEGroup(strXRefTarget)->getNrCellsHorizontal());
	    clsStateVariable->setNrCellsVertical(ClsFESystemManager::Instance()->getFEGroup(strXRefTarget)->getNrCellsVertical());
/* ---- */



	    } else {
		/* CABOUM!!!!!!!!!!!!!! */
	    }
	}
	try{
	    pModule->init();
	}
	catch(runtime_error &e){
	    cerr << "ModuleError: " << e.what() << endl;
	    throw;
	}
    }
}

void ClsFEProcess::startModule(){
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::startModule()" << endl;
#endif

    if(bModuleEnabled && pModule!=NULL && bModuleThreaded){
/*
	cout << "\tpModule->isRunning(): " <<
	    dynamic_cast<ost::Thread*>(pModule)->isRunning() << endl;

	if(dynamic_cast<ost::Thread*>(pModule)->isRunning()){
	    	dynamic_cast<ost::Thread*>(pModule)->resume();
	} else {
	    dynamic_cast<ost::Thread*>(pModule)->start();
	}
*/

//	cout << "(pModule)->start()" << endl;
	(pModule)->start();
    }
}


void ClsFEProcess::stopModule(){
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::stopModule()" << endl;
#endif
    if(bModuleEnabled && pModule!=NULL && bModuleThreaded){
	pModule->stop();
/*	dynamic_cast<ost::Thread*>(pModule)->suspend(); */



    }
}

void ClsFEProcess::update(){
    if(bModuleEnabled && pModule!=NULL && !bModuleThreaded){
	updateModule();
    }
}

void ClsFEProcess::updateModule(){
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::updateModule()" << endl;
#endif
	pModule->update();
}

void ClsFEProcess::cleanup(){
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::cleanup()" << endl;
#endif
    if(bModuleEnabled && pModule!=NULL){
	cleanupModule();
    }
}

void
ClsFEProcess::cleanupModule(){
#ifdef DEBUG_CLSFEPROCESS
    cout << "ClsFEProcess::cleanupModule()" << endl;
#endif

    if(pModule!=NULL){
	pModule->cleanup();

	pModule->resetOutputToGroupStateHolder();

	StateVariablePtrList::iterator itStateVariablePtrList;
	for(itStateVariablePtrList = pModule->getListInputFromGroupPtrs().begin();
	    itStateVariablePtrList != pModule->getListInputFromGroupPtrs().end();
	    itStateVariablePtrList++){
	    (*itStateVariablePtrList)->resetTarget();
	}
    }
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
