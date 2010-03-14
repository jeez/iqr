#ifndef CLSBASEGROUP_H
#define CLSBASEGROUP_H

#include <string>
#include <list>

#include "item.hpp"
#include "ClsSysFileNode.h"

#include "neuron.hpp"

class ClsBaseTopology;

using namespace iqrcommon;
using namespace std;


class ClsBaseGroup : public ClsItem {

public:

    ClsBaseGroup ( ){};

    ClsBaseGroup(string _strGroupID, string _strGroupName, string _strPrcID);
    virtual ~ClsBaseGroup ( );

    void init();
    void cleanup();
    virtual void update();

    void setGroupID( string _strGroupID){ strGroupID = _strGroupID;};
    string getGroupID() { return strGroupID; };

    void setGroupName(string strName);
    string getGroupName();// const;

    void setProcessID( string _strPrcID ) { strPrcID = _strPrcID ; };
    string getProcessID() { return strPrcID; };

    void setGroupSubNodes(ClsSysFileNode _clsSysFileNodeGroupSubNodes);

    /* --------------------------- */
    list<string> getListOfMembers();
    ClsItem* getMemberByName(string strName);
    void setMemberTypeByName(string strName, string strValue);
    string getMemberTypeByName(string strName);
    list<string> getListOfMembersTypes(string strName);
    /* --------------------------- */

/* topology related */
    virtual string TopologyType() const;
    virtual ClsBaseTopology* getTopology();
    virtual ClsBaseTopology* getTopology(const string &_strTopologyType) const;
    virtual list<string> TopologyTypeList() const {return lstTopologyTypes;};
    virtual void setTopologyType(const string &_strTopologyType);
/* end topology related */

/* neuron related */

    ClsNeuron* getNeuron ( ) { return pNeuron; };
    ClsStateVariable* getNeuronOutput ();


    virtual int setNeuronParameter(string strParamName, string strParamValue);
    virtual const ParameterList getListNeuronParameters() const;
    int createNeuron(string strNeuronType);
    int createNeuronByLabel(string strNeuronLabel);
    string getGroupNeuronType() { return strNeuronType; };
    string getGroupNeuronLabel();
    virtual StateVariableList getListNeuronStateVariables( );
    virtual int getNumberOfNeurons();
    virtual int getNrCellsHorizontal();
    virtual int getNrCellsVertical();
/* end neuron related */

protected:

    string strGroupID;
    string strPrcID;

    ClsSysFileNode clsSysFileNodeGroupSubNodes;


/* neuron related */
//    ClsSysFileNode clsSysFileNodeNeuronParameter;
    virtual void setNeuronParameter(ClsSysFileNode _clsSysFileNodeNeuronParameter);
    iqrcommon::ClsNeuron *pNeuron;
    string strNeuronType;
/* end neuron related */


/* Topology related */
//    ClsSysFileNode clsSysFileNodeTopologyParameter;
    virtual void setTopologyParameter(ClsSysFileNode _clsSysFileNodeTopologyParameter);

    ClsBaseTopology *clsTopology;
    typedef map<string,ClsBaseTopology*> TTopologyMap;
    TTopologyMap tmapTopologies;
    void createListOfTopologyTypes();
    list<string> lstTopologyTypes;
/* end Topology related */

    unsigned int iGroupWidth;
    unsigned int iGroupHeight;

    int iRoundCounter;
};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
