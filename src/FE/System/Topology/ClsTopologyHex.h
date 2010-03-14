#ifndef TOPOLOGYHEX_H
#define TOPOLOGYHEX_H

#include "ClsBaseTopology.h"

//class ClsIntParameter;
//class ClsOptionsParameter;

class ClsTopologyHex : public ClsBaseTopology {

public:

    ClsTopologyHex();
    ~ClsTopologyHex() {}

    list<int> getCellsForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation);
    list<tIndexDist> getCellsForArborization(tiPoint tPointCenter, ClsBaseArborization *clsBaseArborization);
    int Size();
    double DistMax(){ 
	cerr << "NOT IMPLEMENTED " << __FILE__ << endl;
	exit(1);
	return 0;
    }
    tiPoint index2pos(int /* iIndex */){
	tiPoint p;
	cerr << "NOT IMPLEMENTED" << endl;
	exit(1);
	return p;
    };
    int pos2index(int /* iX */, int /* iY */){
	cerr << "NOT IMPLEMENTED" << endl;
	exit(1);
	return 0;
    };
    static const char *pcName;

private:

    ClsIntParameter *pclsWidth, *pclsHeight, *pclsOffset;

    ClsOptionsParameter *pclsOrientation;

};

#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
