/****************************************************************************
 ** $Filename: ClsPatterns.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Thu May 29 15:03:20 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSPATTERNS_H
#define CLSPATTERNS_H    /*+ To stop multiple inclusions. +*/


#include "ClsBasePattern.h"
//#include "ClsSubPopulations.h"

class ClsBaseSubPopulation;
class ClsTuples;


using namespace iqrcommon;
using namespace std;

class ClsPatternMapped : public ClsBasePattern {
public:
    ClsPatternMapped();

//     void setSourcePopulation(ClsBaseSubPopulation *clsSubPopulation);

//     void setTargetPopulation(ClsBaseSubPopulation *clsSubPopulation);

private:
//    ClsBaseSubPopulation *clsSubPopulationSource;
//    ClsBaseSubPopulation *clsSubPopulationTarget;

};

class ClsPatternForeach : public ClsBasePattern {
public:
    ClsPatternForeach();
    string getTest();

private:
//    ClsBaseSubPopulation *clsSubPopulationSource;
//    ClsBaseSubPopulation *clsSubPopulationTarget;




};

class ClsPatternTuples : public ClsBasePattern {
public:
    ClsPatternTuples();

//    void setTuples(ClsBaseSubPopulation *_clsSubPopulation);
//    void setTuples(ClsTuples* _clsSubPopulation);


//    ClsBaseSubPopulation *getTuples();
//    ClsTuples* getTuples();


private:
//    ClsBaseSubPopulation *clsSubPopulation;
//    ClsTuples *clsTuples;

};




#endif /* CLSPATTERNS_H */

/// Local Variables:
/// mode: c++
/// End:
