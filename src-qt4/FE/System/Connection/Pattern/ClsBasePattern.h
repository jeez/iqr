/****************************************************************************
 ** $Filename: ClsBasePattern.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu May 29 14:39:06 2003
 **
 *****************************************************************************/


#ifndef CLSBASEPATTERN_H
#define CLSBASEPATTERN_H    /*+ To stop multiple inclusions. +*/

#include <iostream>
#include "item.hpp"

#include "ClsSysFileNode.h"
using namespace iqrcommon;
using namespace std;

class ClsBaseSubPopulation;
class ClsTuples;


class ClsBasePattern : public ClsItem {

public:
    ClsBasePattern();

    void setSourcePopulation(ClsBaseSubPopulation *clsSubPopulation);
    void setTargetPopulation(ClsBaseSubPopulation *clsSubPopulation);
    
    ClsBaseSubPopulation * getTargetPopulation();
    ClsBaseSubPopulation * getSourcePopulation();

    void setTuples(ClsTuples* _clsSubPopulation);
    ClsTuples* getTuples();


    void setSourcePopulationParameter(ClsSysFileNode &_clsSysFileNodeSubPopulation);
    void setTargetPopulationParameter(ClsSysFileNode &_clsSysFileNodeSubPopulation);
    void setTupleParameter(ClsSysFileNode &_clsSysFileNodeSubPopulation);


protected:
    

    ClsBaseSubPopulation *clsSubPopulationSource;
    ClsBaseSubPopulation *clsSubPopulationTarget;
    ClsTuples *clsTuples;

};

#endif /* CLSBASEPATTERN_H */

/// Local Variables: 
/// mode: c++
/// End: 
