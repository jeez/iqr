/****************************************************************************
 ** $Filename: ClsBaseConnection.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Mon Aug  4 17:51:52 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSBASECONNECTION_H
#define CLSBASECONNECTION_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>
#include <valarray>
#include <map>

#include "item.hpp"
#include "pattern.hpp"

#include "ClsSysFileNode.h"
#include "ClsHyperLists.h"

#include "synapse.hpp"


using namespace iqrcommon;
using namespace std;

class ClsBaseArborization;
class ClsBaseFunction;
class ClsBasePattern;


namespace iqrcommon {
    struct ConnInType {
	unsigned int axonalDelay;	// Axonal delay is uniform
	valarray<size_t> maskPre;	// Mask for presynaptic output
	// No maskSyn is needed, all elements receive input
    };
    struct SynOutType {
	unsigned int dendriticDelay; // Dendritic delay between synapses
	                             // and postsynaptic cell in timesteps
	valarray<size_t> maskSyn; 	 // Mask for synaptic output
    };
    struct ConnOutType {
	unsigned int idPost;
	valarray<size_t> maskSyn; 	 // Mask for synaptic output
    };
    struct ConnFbType {
        unsigned int dendriticDelay;	// Delay 
	valarray<size_t> maskPost;	// Mask of postsynaptic neuron IDs
        valarray<size_t> maskSyn;       // Mask of synapse IDs
    };
    
    // Structs to store the arborization details of a single neuron
    // consisting of indices of synapses and the indices of connected
    // neurons.
    //
    // Valarrays are used to store the neuron and synapse IDs as this
    // allows the data to be extracted from state arrays on demand.
    struct Pre2PostType {
	int pre;			// Index of presynaptic neuron
	valarray<size_t> maskSyn;	// Mask of synapse indices
	valarray<size_t> maskPost;	// Mask of postsynaptic neuron indices
    };

    struct Post2PreType {
	int post;			// Index of postsynaptic neuron
	valarray<size_t> maskSyn;	// Mask of synapse indices
	valarray<size_t> maskPre;	// Mask of presynaptic neuron indices
    };

    struct FeedbackInputType {
	ClsStateVariable *pNeuronState;
	ClsStateVariable *pSynapseInput;
    };
    
}

typedef vector<tiPoint> tVectorOfPoints;


// namespace iqrcommon {
//     class ClsSynapse;
// //    class StateArray;
//}


class ClsBaseConnection : public ClsItem {

public:

    enum CONNECTION_TYPE {
	CONN_EXCITATORY,
	CONN_INHIBITORY,
	CONN_MODULATORY
    };
    
    ClsBaseConnection ( );
    ~ClsBaseConnection();


    void setConnectionID( string _strConnectionID );
    string getConnectionID( );
    
    void setConnectionTargetID(string _strConnectionTargetID );
    string getConnectionTargetID( );
    
    void setConnectionSourceID(string _strConnectionSourceID );
    string getConnectionSourceID( );

    void update();
    void cleanup();

    /* Synapse related */
    int createSynapse(string strSynapseType);
    int createSynapseByLabel(string strSynapseLabel);
    int setSynapseParameter(string strParamName, string strParamValue);
    void setSynapseParameter(ClsSysFileNode _clsSysFileNodeSynapseParameter);

    ClsSynapse* getSynapse( ){return pSynapse; };

//    virtual const ParameterList& getListSynapseParameters() const;    
    const ParameterList getListSynapseParameters() const;    
    string getConnectionSynapseType() { return strSynapseType; };
    string getConnectionSynapseLabel();
    StateVariableList getListSynapseStateVariables( );

    list<string> getListSynapseXRefHolders();
    const ClsXRefHolder* getSynapseXRefHolder(string strName) const;


/* ------------- */


/* Danny stuff */
    list<connection_pattern>* getTPattern();
    void setTPattern(list <connection_pattern> _pattern) {
	pattern = _pattern;
    };
/* -------------- */
    void setConnectionName(string _strConnectionName);
    string getConnectionName();

    string getConnectionLongName();

    void setConnectionType(string t);
    void setConnectionType(int t);


    string getConnectionTypeAsString();
    int getConnectionTypeAsInt();

    StateArray& getConnectionOut() { return connectionOut; };
    StateArray& getSynapseOutput() { return pSynapse->getOutputState()->getStateArray(); };

    void setConnectionSubNodes(ClsSysFileNode _clsSysFileNodeConnectionSubNodes);
    
    static string convertIntConnectiontype2StringConnectiontype(int t);
    static int convertStringConnectiontype2IntConnectiontype(string t);

    /* --------------------------- */
    list<string> getListOfMembers();
    ClsItem* getMemberByName(string strName);
    void setMemberTypeByName(string strName, string strValue);
    string getMemberTypeByName(string strName);
    list<string> getListOfMembersTypes(string strName);
    /* --------------------------- */

/* Arborization related */
    string ArborizationType()  const;
    ClsBaseArborization *getArborization() { return clsArborization; };
    void setArborizationParameter(ClsSysFileNode &_clsSysFileNodeArborizationParameter);
    list<string> ArborizationTypeList() const {return lstArborizationTypes;};
    void setArborizationType(const string &_strArborizationType);
    double DistMax();
/* end Arborization related */    

/* Function related */
    string AttenuationFunctionType()  const; //**
    string DelayFunctionType()  const; //**
    ClsBaseFunction *getAttenuationFunction() { return clsAttenuationFunction; }; //**
    ClsBaseFunction *getDelayFunction() { return clsDelayFunction; }; //**

 
    void setAttenuationFunctionParameter(ClsSysFileNode &_clsSysFileNodeFunctionParameter);
    void setDelayFunctionParameter(ClsSysFileNode &_clsSysFileNodeFunctionParameter);

    list<string> AttenuationFunctionTypeList() const {return lstFunctionTypes;}; //**
    list<string> DelayFunctionTypeList() const {return lstFunctionTypes;}; //**
    void setAttenuationFunctionType(const string &_strFunctionType); //**
    void setDelayFunctionType(const string &_strFunctionType); //** 
/* end Function related */    

/* Pattern related */
    string PatternType() const;
    ClsBasePattern *getPattern() { return clsPattern; };
    void setPatternParameter(ClsSysFileNode &_clsSysFileNodePatternParameter);
    list<string> PatternTypeList() const {return lstPatternTypes;}; 
    void setPatternType(const string &_strPatternType); 
/* end Pattern related */



protected:
    string strConnectionID;
    string strConnectionName;
    string strConnectionTargetID;
    string strConnectionSourceID;

    string strSynapseType;
    ClsSynapse *pSynapse;


/* 2 reset */
    list<connection_pattern> pattern;

    // The connection holds the array which is used to sum the inputs
    // from the synapses for each cell.  The postsynaptic neuron is
    // passed only a pointer to this array.
    StateArray connectionOut;
    valarray<double> dendriticAttenuation;
    StateArray synapseOutDelayed;

    ClsStateVariable *pNeuronPreOutput;
    ClsStateVariable *pSynapseInput;
    ClsStateVariable *pSynapseOutput;

    // Axonal delay is the time taken for activity in the presynaptic
    // group's output state to reach the synapse input
    unsigned int axonalDelay;

    // We'll store the delay, maskPre and maskSyn sets for synapse
    // input in connInMasks.
    //
    // connIn associates presynaptic cells with synapses with uniform
    // delay due to axonal propogation
    ConnInType connIn;

    // synOut extracts data from synapses after the dendritic delay
    // has elapsed. Dendritic propogation delays depend on the
    // distance between the synapse and the soma.
    //
    // One synapse mask per dendritic delay needed.
    vector<SynOutType>  synOut;
    // connOut associates postsynaptic cells with their incoming
    // synapses. One synapse mask per postsynaptic cell.
    vector<ConnOutType> connOut;
/* 2 reset */

    // connFb is the opposite, associating synapses with their
    // postsynaptic cells for feedback.
    //
    // One neuron ID mask and one synapse mask per dendritic delay
    // specify the postsynaptic neurons connected to each synapse.
    vector<ConnFbType> connFb;

    // Feedback inputs
    //
    // fbStates holds multiple pairs of pointers to connect states in postsynaptic neuron
    // to input variables in the synapse, 1 pair for each feedback state
    vector<FeedbackInputType> fbStates;

    // ### jmb 9.5.2005 Added pre2post and post2pre
    //
    // pre2post specifies the synapses and postsynaptic cells
    // associated with each presynaptic cell
    //
    // Presynaptic->postsynaptic, 1 element for each presynaptic neuron.
    vector<Pre2PostType> pre2post;

    // post2pre specifies the synapses and presynaptic cells
    // associated with each postsynaptic cell
    //
    // Postsynaptic<-presynaptic, 1 element for each postsynaptic neuron.
    vector<Post2PreType> post2pre;

    ClsSysFileNode clsSysFileNodeConnectionSubNodes;

/* Arborization related */
    ClsBaseArborization *clsArborization;
    typedef map<string,ClsBaseArborization*> TArborizationMap;
    TArborizationMap tmapArborizations;
    void createListOfArborizationTypes();
    list<string> lstArborizationTypes;
/* end Arborization related */


/* Function related */
    ClsBaseFunction *clsAttenuationFunction;
    double fMaxAttenuation;
    ClsBaseFunction *clsDelayFunction;
    typedef map<string,ClsBaseFunction*> TFunctionMap;
    TFunctionMap tmapAttenuationFunctions;
    TFunctionMap tmapDelayFunctions;

    void createListOfFunctionTypes();
    list<string> lstFunctionTypes;

/* end Function related */

/* Pattern related */
    ClsBasePattern *clsPattern;
    typedef map<string,ClsBasePattern*> TPatternMap;
    TPatternMap tmapPatterns;
    void createListOfPatternTypes();
    list<string> lstPatternTypes;
/* end Pattern related */
    
    
/* helpers */
    void printTupleList(list<pair<tVectorOfPoints, tVectorOfPoints> > lstPLP);
/* end helpers */

    
};

#endif /* CLSBASECONNECTION_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:

