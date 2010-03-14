#ifndef CLSFECONNECTION_H
#define CLSFECONNECTION_H

#include <string>
#include <list>
//#include "item.hpp"

#include "ClsBaseConnection.h"
#include "ClsHyperLists.h"
#include "ClsTopologyRect.h"
#include "ClsTopologySparse.h"
#include "ClsTopologyHex.h"


using namespace iqrcommon;
using namespace std;

class ClsTuples;


//class ClsFEConnection : public ClsItem {
class ClsFEConnection : public ClsBaseConnection {

public:

//BFS     ClsFEConnection(ClsFESystemManager* _clsFESystemManager);
    ClsFEConnection();

//BFS     ClsFEConnection(ClsFESystemManager* _clsFESystemManager,
    ClsFEConnection(
		    string _strConnectionID,
		    string _strConnectionName,
		    string _strConnectionSourceID,
		    string _strConnectionTargetID );

    void setNotes(string _str);
    string getNotes();
    string validate();

    void calculateArbPattern();
    void init();

    const ListIndexQuadruples& getQuadrupleList(){
	return listIndexQuadruples;
    };




private:

    void checkNfixPatternBoundaries(ClsBaseTopology *clsBaseTopology, ClsBaseSubPopulation *clsBaseSubPopulation);
    void checkNfixPatternBoundaries(ClsBaseTopology *clsBaseTopologySource, ClsBaseTopology *clsBaseTopologyTarget, ClsBasePattern *clsPattern);


    void initSynapses();

    ListIndexQuadruples calculateArborization(const ListPointTriples& listPointTriples, ClsBaseTopology *clsBaseTopologySource, ClsBaseTopology *clsBaseTopologyTarget); 
    ListPointTriples expandXXX(const tListOfPairs &tListOfPairsSource , const tListOfPairs &tListOfPairsTarget);
    ListIndexQuadruples PointTriples2IndexQuadruples(const ListPointTriples& listPointTriples, ClsBaseTopology *clsBaseTopologySource, ClsBaseTopology *clsBaseTopologyTarget );

    ListIndexQuadruples listIndexQuadruples;

    // The connection holds the array which is used to sum the inputs
    // from the synapses for each postsynaptic neuron.  The neuron is
    // passed only a pointer to this array.  This array can be defined
    // as a member in connection.


};


#endif




/// Local Variables:
/// mode: c++
/// End:
