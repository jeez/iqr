/****************************************************************************
 ** $Filename: ClsPatterns.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sat Jun 28 01:21:40 2003
 **
 *****************************************************************************/


#include "ClsPatterns.h"
#include "tagLibrary.hpp"
//#include "item.hpp"
#include "ClsSubPopulations.h"

#include "optionsParameter.hpp"



ClsPatternMapped::ClsPatternMapped(){
//    cout << "ClsPatternMapped::ClsPatternMapped()" << endl;
	
    strType = ClsTagLibrary::ConnectionPatternMapped();
//	clsSubPopulationSource = NULL;
//	clsSubPopulationSource = new ClsBaseSubPopulation();
//	clsSubPopulationTarget = NULL;
//	clsSubPopulationTarget = new ClsBaseSubPopulation();

	
    addOptionsParameter(ClsTagLibrary::ConnectionTypeTag(),// string _strName
			"mapping type",
			false,             //bool   _bReadOnly,
			"mapping type", //_strDescription,
			"Properties");
    ClsOptionsParameter *op = static_cast<ClsOptionsParameter*>(getParameter(ClsTagLibrary::ConnectionPatternMappedType()));

    op->addOption(ClsTagLibrary::ConnectionPatternMappedTypeAll());
    op->addOption(ClsTagLibrary::ConnectionPatternMappedTypeCenter());
    
    setParameter(ClsTagLibrary::ConnectionPatternMappedType(), ClsTagLibrary::ConnectionPatternMappedTypeCenter());

/* defaults */
	clsSubPopulationSource = new ClsAll();
	clsSubPopulationTarget = new ClsAll();

    
};

//     void ClsPatternMapped::setSourcePopulation(ClsBaseSubPopulation *clsSubPopulation) {
// 	*clsSubPopulationSource = *clsSubPopulation;
	
//     }

//     void ClsPatternMapped::setTargetPopulation(ClsBaseSubPopulation *clsSubPopulation) {
// 	*clsSubPopulationTarget = *clsSubPopulation;
//     }
    

ClsPatternForeach::ClsPatternForeach(){
    strType = ClsTagLibrary::ConnectionPatternForeach();
//	clsSubPopulationSource = NULL;
//	clsSubPopulationTarget = NULL;

//	clsSubPopulationSource = new ClsBaseSubPopulation();
//	clsSubPopulationTarget = new ClsBaseSubPopulation();

/* defaults */
	clsSubPopulationSource = new ClsAll();
	clsSubPopulationTarget = new ClsAll();

};

string ClsPatternForeach::getTest() { return "WORKS"; };
    
    
    
ClsPatternTuples::ClsPatternTuples(){
    strType = ClsTagLibrary::ConnectionPatternTuples();
//	clsSubPopulation = NULL;
//    clsSubPopulation = new ClsBaseSubPopulation();
//    clsTuples = NULL;
    
};

// //void ClsPatternTuples::setTuples(ClsBaseSubPopulation *_clsSubPopulation) {
// void ClsPatternTuples::setTuples(ClsTuples *_clsTuples) {
//     cout << "ClsPatternTuples::setTuples(ClsTuples *_clsTuples)" << endl;
    
//     cout << "_clsTuples is NULL: " << (_clsTuples == NULL ? "yes" : "no" ) << endl;
// //    *clsSubPopulation = *_clsSubPopulation;
//     *clsTuples = *_clsTuples;
// };

// //ClsBaseSubPopulation* ClsPatternTuples::getTuples() {
// ClsTuples* ClsPatternTuples::getTuples() {
// //    return clsSubPopulation;
//     return clsTuples;
// };
    
