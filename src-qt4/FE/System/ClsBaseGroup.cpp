#include "ClsBaseGroup.h"

#ifdef OLDITEM
#include <Item.20021216/Neuron/neuronManager.hpp>
#else
#include "neuronManager.hpp"
using iqrfe::initializeNeuronManager;
using iqrfe::NeuronManager;
#endif


#include "ClsTopologyHex.h"
#include "ClsTopologyRect.h"
#include "ClsTopologySparse.h"

#include "ClsQLogWindow.h"
#include "tagLibrary.hpp"
#include "neuronManager.hpp"

//#include "optionsParameter.hpp"
#include "stringParameter.hpp"

#ifdef DEBUG_CLSFEGROUP
static const bool bDebugBaseGroup = true;
#else
static const bool bDebugBaseGroup = false;
#endif

static const int DEFAULT_MAX_WIDTH  = 100;
static const int DEFAULT_MAX_HEIGHT = 100;

ClsBaseGroup::ClsBaseGroup(string _strGroupID, string _strGroupName, string _strPrcID) :
    strGroupID(_strGroupID), strPrcID(_strPrcID) {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::ClsBaseGroup(string _strGroupID, string _strGroupName, string _strPrcID)" << endl;
    }


//    strNeuronType = ClsTagLibrary::LinearThresholdNeuronTag();
    pNeuron = NULL;
    strNeuronType = "";
    clsTopology = NULL;
    iRoundCounter = 0;

    createListOfTopologyTypes();

    // Literal name strings should be replaced with constants here.
    addStringParameter(ClsTagLibrary::NameTag(), "Group Name",
		       _strGroupName,
		       true, false,
		       "Name",
		       "Properties");

    setTopologyType(ClsTagLibrary::RectTopologyTag());

    iGroupWidth = 1;
    iGroupHeight = 1;

};


ClsBaseGroup::~ClsBaseGroup() {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::~ClsBaseGroup" << endl;
    }

    // Delete topologies.
    for (TTopologyMap::iterator tmapitTopology = tmapTopologies.begin();
	 tmapitTopology != tmapTopologies.end(); tmapitTopology++) {

	// Delete dynamically allocated topology objects.
//	delete tmapitTopology.data();
//don't think we need this:	tmapTopologies.erase(tmapitTopology);

//	delete &(tmapitTopology->second);
	delete tmapitTopology->second;
    }

    // Clear pointers from map.
    tmapTopologies.clear();

    cleanup();

    try {
//	NeuronManager::instance().destroy(pNeuron);
	delete pNeuron;
	pNeuron = NULL;
    }
    catch(...){
	cerr << "could not destroy Neuron" << endl;
    }

}

void ClsBaseGroup::init(){
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::init()" << endl;
    }

    if(pNeuron!=NULL){
	pNeuron->resize(clsTopology->Size());
	pNeuron->initialize();
    }
    iRoundCounter = 0;
}


void ClsBaseGroup::cleanup(){
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::cleanup()" << endl;
    }

//    cout << "groupName: " << getGroupName() << endl;
    if(pNeuron!=NULL){
	pNeuron->removeAllInputs();
	/* superflous, as we resize at init ... pNeuron->resetSize(); */
	pNeuron->resetSize();
    }
}


void ClsBaseGroup::createListOfTopologyTypes() {
    lstTopologyTypes.push_back(ClsTagLibrary::RectTopologyTag());
/* we'll diable this for now; all the rest of the code is left in place...
   lstTopologyTypes.push_back(ClsTagLibrary::HexTopologyTag());
*/
    lstTopologyTypes.push_back(ClsTagLibrary::SparseTopologyTag());
};


string ClsBaseGroup::getGroupName() {//const {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::getGroupName()" << endl;
    }


    ClsParameter* p = getParameter(ClsTagLibrary::NameTag());
    string strGroupName = (dynamic_cast<ClsStringParameter*>(p))->getValue();
    return strGroupName;
}

void ClsBaseGroup::setGroupName(string _strName) {
//    pclsName->setValue(_strName);
    setParameter(ClsTagLibrary::NameTag(),  _strName);

}

int ClsBaseGroup::getNumberOfNeurons() {
    return clsTopology->Size();
};

int ClsBaseGroup::getNrCellsHorizontal() {
    return clsTopology->nrCellsHorizontal();
}

int ClsBaseGroup::getNrCellsVertical()  {
    return clsTopology->nrCellsVertical();
}

ClsStateVariable* ClsBaseGroup::getNeuronOutput() {
    if(pNeuron == NULL) {
	cerr << "NO NEURON OBJECT" << endl;
	return NULL;
    };

    return pNeuron->getOutputState();
};


void ClsBaseGroup::setGroupSubNodes(ClsSysFileNode _clsSysFileNodeGroupSubNodes){
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::setGroupSubNodes(ClsSysFileNode _clsSysFileNodeGroupSubNodes)" << endl;
    }

    clsSysFileNodeGroupSubNodes = _clsSysFileNodeGroupSubNodes;
    while (_clsSysFileNodeGroupSubNodes.countNodes()){
	ClsSysFileNode F1 = _clsSysFileNodeGroupSubNodes.popNode();
	string strNodeName = F1.getName();
	if(!strNodeName.compare(ClsTagLibrary::NeuronTag())) {
	    setNeuronParameter(F1);
	}
	else if(!strNodeName.compare(ClsTagLibrary::TopologyTag())) {
	    setTopologyParameter(F1);
	}
    }
};


void ClsBaseGroup::setNeuronParameter( ClsSysFileNode _clsSysFileNodeNeuronParameter ){
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::setNeuronParameter( ClsSysFileNode _clsSysFileNodeNeuronParameter )" << endl;
    }

    ClsSysFileNode clsSysFileNodeNeuronParameter = _clsSysFileNodeNeuronParameter;
    while (_clsSysFileNodeNeuronParameter.countParameters()){
	iqrcommon_old::ClsParameter clsParameterTemp = _clsSysFileNodeNeuronParameter.popParameter();
	string strParamName = clsParameterTemp.getName();
	string strParamValue = clsParameterTemp.getValueAsString();

	if(!strParamName.compare(ClsTagLibrary::NameTag())) {
//	    strNeuronType = strParamValue;
//	    cout << "------\tNeuronType: " << strNeuronType << endl;
//	    createNeuron(strNeuronType);
	    createNeuron(strParamValue);
	}
    }

    if(pNeuron!=NULL){
	while(_clsSysFileNodeNeuronParameter.countNodes()>0){
	    ClsSysFileNode F1 = _clsSysFileNodeNeuronParameter.popNode();
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
//	    cout << "strParamName, strParamValue: " << strParamName << ", " << strParamValue << endl;
		pNeuron->setParameter(strParamName, strParamValue);
	    }
	}
    }
};


int ClsBaseGroup::setNeuronParameter(string strParamName, string strParamValue) {
    if(pNeuron->getParameter(strParamName)!=NULL){
	pNeuron->setParameter(strParamName, strParamValue);
	return 0;
    }
    return -1;
}


int ClsBaseGroup::createNeuron(string _strNeuronType) {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::createNeuron(string _strNeuronType)" << endl;
    }

    strNeuronType = _strNeuronType;
    int iReturn = 0;

    if (bDebugBaseGroup) {
	cout << "NeuronType: " << _strNeuronType << endl;
    }
    try{
	pNeuron = NeuronManager::instance().createByType(_strNeuronType);
    }
    catch (iqrcommon::UnknownTypeError &e) {
	string strError = "Group \"" + getGroupName() + "\" Couldn't create Unknown type neuron: ";
	strError.append(e.what());
	cerr << strError << endl;
	strNeuronType = "";
	iReturn = -1;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	iReturn = -1;
    }
    return iReturn;
}

int ClsBaseGroup::createNeuronByLabel(string strNeuronLabel) {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::createNeuronByLabel(string strNeuronLabel)" << endl;
    }

    int iReturn = 0;
    try{
	pNeuron = NeuronManager::instance().createByLabel(strNeuronLabel);
	strNeuronType =NeuronManager::instance().label2type(strNeuronLabel);
    }
    catch (iqrcommon::UnknownTypeError &e) {
	string strError = "Group \"" + getGroupName() + "\" Couldn't create Unknown type neuron: ";
	strError.append(e.what());
	cerr << strError << endl;
	strNeuronType = "";
	iReturn = -1;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	iReturn = -1;
    }
    return iReturn;
}



void ClsBaseGroup::setTopologyParameter(ClsSysFileNode _clsSysFileNodeTopologyParameter){
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::setTopologyParameter(ClsSysFileNode _clsSysFileNodeTopologyParameter)" << endl;
    }

    if(_clsSysFileNodeTopologyParameter.countNodes()>0){
	string strGroupTopology;

	ClsSysFileNode clsSysFileNodeTopologyParameter = _clsSysFileNodeTopologyParameter;
	ClsSysFileNode F1 = _clsSysFileNodeTopologyParameter.popNode();
	strGroupTopology =  F1.getName();
//	cout << "strGroupTopology: " << strGroupTopology << endl;
	setTopologyType(strGroupTopology);

	while (F1.countParameters()){
	    iqrcommon_old::ClsParameter clsParameterTemp = F1.popParameter();
	    string strParamName = clsParameterTemp.getName();
	    string strParamValue = clsParameterTemp.getValueAsString();

//	    cout << "strParamName: " << strParamName << endl;
//	    cout << "strParamValue: " << strParamValue << endl;

	    clsTopology->setParameter(strParamName, strParamValue);
	    if(!strParamName.compare("width")){
		iGroupWidth =  clsParameterTemp.getValueAsInt();
	    } else if (!strParamName.compare("height")){
		iGroupHeight =  clsParameterTemp.getValueAsInt();
	    }
	}

	/* taking care of sparse topologies */
	if(dynamic_cast<ClsTopologySparse*>(clsTopology)){
	    list<pair<int, int> > lst;
	    while(F1.countNodes()){
		ClsSysFileNode F2 = F1.popNode();
		int iX = -99;
		int iY = -99;

		while (F2.countParameters()){
		    iqrcommon_old::ClsParameter clsParameterTemp = F2.popParameter();
		    string strParamName = clsParameterTemp.getName();
		    string strParamValue = clsParameterTemp.getValueAsString();
//		    cout << "strParamName: " << strParamName << endl;
//		    cout << "strParamValue: " << strParamValue << endl;
		    if(!strParamName.compare(ClsTagLibrary::PointX())){
			iX = iqrUtils::string2int(strParamValue);
		    }
		    else if(!strParamName.compare(ClsTagLibrary::PointY())){
			iY = iqrUtils::string2int(strParamValue);
		    }
		}
		if(iX>0 && iY>0){
		    pair<int, int> pairPoint(iX, iY);
		    lst.push_back(pairPoint);
		}
	    }
	    (dynamic_cast<ClsTopologySparse*>(clsTopology))->setList(lst);
	}
	/* ---------------------- */
    }
};


//const ParameterList& ClsBaseGroup::getListNeuronParameters() const  {
const ParameterList ClsBaseGroup::getListNeuronParameters() const  {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::getListNeuronParameters() const" << endl;
    }


    if(pNeuron!=NULL){
	return pNeuron->getListParameters();
    } else {
	cerr << "pNeuron is NULL" << endl;

    }

    ParameterList mt;
    return mt;
}



//////////////////////////////
// Topology-related functions.
//////////////////////////////

/**
 * Get a pointer to the current topology object.
 *
 * The pointer is returned without a check for a valid object.  This
 * is left to the caller.
 *
 * @return Pointer to the current topology object.
 */
ClsBaseTopology* ClsBaseGroup::getTopology() {
    return clsTopology;
}

/**
 * Get a pointer to the specified topology object.
 *
 * If the specified type is not found in the gemoetries map, a pointer
 * to the current topology is returned.  This is to minimize the risk
 * of NULL pointers being used.
 *
 * @return Pointer to specified topology object, or the current
 * topology object if the specified type is not found.
 * @param _strTopologyType Name of the desired topology type.
 */
ClsBaseTopology* ClsBaseGroup::getTopology(const string &_strTopologyType) const {
    // Find the specified topology in the topologies map.
    TTopologyMap::const_iterator tmapitTopology;

    // Is topology type in map?
    if ((tmapitTopology = tmapTopologies.find(_strTopologyType))
	!= tmapTopologies.end()) {
	return tmapitTopology->second;
    } else {
	// Topology type is not in topologies map.  Report the error.
/*
  qWarning("WARNING: ClsBaseGroup::setTopologyType:"
  " topology type %s not found, keeping type %s",
  _strTopologyType.c_str(), clsTopology->Type().c_str());
*/
    }

    // Return the pointer to the current topology in order to minimize
    // NULL pointer errors.
    return clsTopology;
}

/**
 * Get the name of the current topology type
 *
 * @return Current topology type name.
 */
string ClsBaseGroup::TopologyType() const {
    if (clsTopology == NULL) {
	return "";
    }
    return clsTopology->Type();
}



void ClsBaseGroup::setTopologyType(const string &_strTopologyType) {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::setTopologyType(const string &_strTopologyType)" << endl;
    }



    TTopologyMap::iterator tmapitTopology;

    if ((tmapitTopology = tmapTopologies.find(_strTopologyType)) != tmapTopologies.end()) {
	if (bDebugBaseGroup) {
	    cout << "reusing topology" << endl;
	}
	clsTopology = tmapitTopology->second;
    } else {
	if(find(lstTopologyTypes.begin(), lstTopologyTypes.end(), _strTopologyType) != lstTopologyTypes.end()){
	    if (bDebugBaseGroup) {
		cout << "instantiating new topology" << endl;
	    }
	    if(!_strTopologyType.compare(ClsTagLibrary::RectTopologyTag())){
		clsTopology = new ClsTopologyRect();
	    }
/*
  else if(!_strTopologyType.compare(ClsTagLibrary::HexTopologyTag())){
  clsTopology = new ClsTopologyHex();
  }
*/

	    else if(!_strTopologyType.compare(ClsTagLibrary::SparseTopologyTag())){
		clsTopology = new ClsTopologySparse();
	    }

	    else {
		clsTopology = new ClsTopologyRect();
	    }
	    pair <string,ClsBaseTopology*> pairTemp(clsTopology->Type(), clsTopology);
	    tmapTopologies.insert(pairTemp);
	} else {
	    cerr << "Type of Topology not valid" << endl;
	}
    }
}






//---  int ClsBaseGroup::getGroupWidth() { return iGroupWidth; };
//--- int ClsBaseGroup::getGroupHeight() { return iGroupHeight; };

string ClsBaseGroup::getGroupNeuronLabel() {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::getGroupNeuronLabel()" << endl;
    }
    
//    cout << "strNeuronType: " << strNeuronType << endl;

    if(strNeuronType.size()>0){
	try {
	    return NeuronManager::instance().type2label(strNeuronType);
	}
	catch (iqrcommon::UnknownTypeError &e) {
	    return "";
	}
    } else {
	return "";
    }
};

StateVariableList ClsBaseGroup::getListNeuronStateVariables( ){
    if (bDebugBaseGroup) {
	cout << "StateVariableList ClsBaseGroup::getListNeuronStateVariables( )" << endl;
    }

    return (pNeuron->getListStates());

};


/* --------------------------
   Functions for member items
   -------------------------- */

list<string> ClsBaseGroup::getListOfMembers() {
    list<string> lst;
    lst.push_back(ClsTagLibrary::NeuronTag());
    lst.push_back(ClsTagLibrary::TopologyTag());
    return lst;
}

ClsItem* ClsBaseGroup::getMemberByName(string strName) {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::getMemberByName(string strName)" << endl;
    }

//    cout << "strName: " << strName << endl;
    if(!strName.compare(ClsTagLibrary::TopologyTag())){
	return clsTopology;
    }
    else if(!strName.compare(ClsTagLibrary::NeuronTag())){
//	cout << "getNeuron" << endl;
//	cout << "pNeuron is NULL: " << (pNeuron == NULL ? "yes" : "no" ) << endl;

	return pNeuron;
    }

    return NULL;
}

void ClsBaseGroup::setMemberTypeByName(string strName, string strValue) {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::setMemberTypeByName(string strName, string strValue)" << endl;
    }

    if(!strName.compare(ClsTagLibrary::TopologyTag())){
	setTopologyType(strValue);
    }
    else if(!strName.compare(ClsTagLibrary::NeuronTag())){
	createNeuronByLabel(strValue);
    }
}

string ClsBaseGroup::getMemberTypeByName(string strName){
    if(!strName.compare(ClsTagLibrary::TopologyTag())){
	return TopologyType();
    }
    else if(!strName.compare(ClsTagLibrary::NeuronTag())){
	return getGroupNeuronLabel();
    }
    return "";
}


list<string> ClsBaseGroup::getListOfMembersTypes(string strName) {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::getListOfMembersTypes(string strName)" << endl;
    }

    list<string> lst;
    if(!strName.compare(ClsTagLibrary::TopologyTag())){
	lst = TopologyTypeList();
    }
    else if(!strName.compare(ClsTagLibrary::NeuronTag())){
	lst = NeuronManager::instance().getListLabels();
    }
    return lst;
}


void ClsBaseGroup::update() {
    if (bDebugBaseGroup) {
	cout << "ClsBaseGroup::update()" << endl;
    }
    iRoundCounter++;
    pNeuron->advance();
    pNeuron->update();
}

//    cout << __FILE__ << ":" << __LINE__ << endl;


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
