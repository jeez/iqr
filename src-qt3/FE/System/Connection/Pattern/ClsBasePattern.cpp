/****************************************************************************
 ** $Filename: ClsBasePattern.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Jun 27 04:30:41 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include "ClsBasePattern.h"
//#include "tagLibrary.hpp"
#include "ClsSubPopulations.h"

ClsBasePattern::ClsBasePattern(){
    clsSubPopulationSource = NULL;
    clsSubPopulationTarget = NULL;
    clsTuples = NULL;
    bRuntimeEditable = false;

};

void ClsBasePattern::setSourcePopulation(ClsBaseSubPopulation *clsSubPopulation) {
//    cout << "*ClsBasePattern::setSourcePopulation(ClsBaseSubPopulation *clsSubPopulation)" << endl;
    if(dynamic_cast<ClsRegion*>(clsSubPopulation)){
	clsSubPopulationSource = new ClsRegion();
	*((ClsRegion*)clsSubPopulationSource) =  *(dynamic_cast<ClsRegion*>(clsSubPopulation));;
//	cout << "cast to ClsRegion" << endl;
    }
    else if(dynamic_cast<ClsList*>(clsSubPopulation)){
	clsSubPopulationSource = new ClsList();
	*((ClsList*)clsSubPopulationSource) = *(dynamic_cast<ClsList*>(clsSubPopulation));
//	cout << "cast to ClsList" << endl;
    }
    else if(dynamic_cast<ClsAll*>(clsSubPopulation)){
	clsSubPopulationSource = new ClsAll();
	*((ClsAll*)clsSubPopulationSource) = *(dynamic_cast<ClsAll*>(clsSubPopulation));
//	cout << "cast to ClsAll" << endl;
    }
    else if(dynamic_cast<ClsTuples*>(clsSubPopulation)){
	clsSubPopulationSource = new ClsTuples();
	*((ClsTuples*)clsSubPopulationSource) = *(dynamic_cast<ClsTuples*>(clsSubPopulation));
//	cout << "cast to ClsTuples" << endl;
    }
    else {
	clsSubPopulationSource = new ClsBaseSubPopulation();
//	cout << "cast to ClsBaseSubPopulation" << endl;
    }
}
    

void ClsBasePattern::setTargetPopulation(ClsBaseSubPopulation *clsSubPopulation) {
//    cout << "ClsBasePattern::setTargetPopulation(ClsBaseSubPopulation *clsSubPopulation)" << endl;
    if(dynamic_cast<ClsRegion*>(clsSubPopulation)){
	clsSubPopulationTarget = new ClsRegion();
	*((ClsRegion*)clsSubPopulationTarget) =  *(dynamic_cast<ClsRegion*>(clsSubPopulation));;
//	cout << "cast to ClsRegion" << endl;
    }
    else if(dynamic_cast<ClsList*>(clsSubPopulation)){
	clsSubPopulationTarget = new ClsList();
	*((ClsList*)clsSubPopulationTarget) = *(dynamic_cast<ClsList*>(clsSubPopulation));
//	cout << "cast to ClsList" << endl;
    }
    else if(dynamic_cast<ClsAll*>(clsSubPopulation)){
	clsSubPopulationTarget = new ClsAll();
	*((ClsAll*)clsSubPopulationTarget) = *(dynamic_cast<ClsAll*>(clsSubPopulation));
//	cout << "cast to ClsAll" << endl;
    }
    else if(dynamic_cast<ClsTuples*>(clsSubPopulation)){
	clsSubPopulationTarget = new ClsTuples();
	*((ClsTuples*)clsSubPopulationTarget) = *(dynamic_cast<ClsTuples*>(clsSubPopulation));
//	cout << "cast to ClsTuples" << endl;
    }
    else {
	clsSubPopulationTarget = new ClsBaseSubPopulation();
//	cout << "cast to ClsBaseSubPopulation" << endl;
    }
}

ClsBaseSubPopulation* ClsBasePattern::getSourcePopulation() {
//    cout << "ClsBasePattern::getSourcePopulation()" << endl;
    return clsSubPopulationSource;
}

ClsBaseSubPopulation* ClsBasePattern::getTargetPopulation() {
//    cout << "ClsBasePattern::getTargetPopulation()" << endl;
    return clsSubPopulationTarget;
}


void ClsBasePattern::setTuples(ClsTuples *_clsTuples) {
//    cout << "ClsBasePattern::setTuples(ClsTuples *_clsTuples)" << endl;
//    cout << "_clsTuples is NULL: " << (_clsTuples == NULL ? "yes" : "no" ) << endl;
    clsTuples = new ClsTuples();
    *clsTuples = *_clsTuples;
};

ClsTuples* ClsBasePattern::getTuples() {
    return clsTuples;
};


void ClsBasePattern::setSourcePopulationParameter(ClsSysFileNode &_clsSysFileNodeSubPopulation){
//    cout << "ClsBasePattern::setSourcePopulationParameter(ClsSysFileNode &_clsSysFileNodeSubPopulation)" << endl;
    string strSubPopulation = _clsSysFileNodeSubPopulation.getName();
    if(!strSubPopulation.compare(ClsTagLibrary::SelectorRegion())){
	clsSubPopulationSource = new ClsRegion();
//	cout << "create SubPopulation:  ClsRegion" << endl;
    }
    else if(!strSubPopulation.compare(ClsTagLibrary::SelectorList())){
	clsSubPopulationSource = new ClsList();
//	cout << "create SubPopulation:  ClsList" << endl;
    }
    else if(!strSubPopulation.compare(ClsTagLibrary::SelectorAll())){
	clsSubPopulationSource = new ClsAll();
//	cout << "create SubPopulation:  ClsAll" << endl;
    }
    else {
	clsSubPopulationSource = NULL;
    }
    
    if(clsSubPopulationSource!=NULL){
//	cout << "#########################" << endl;
	clsSubPopulationSource->setParameter(_clsSysFileNodeSubPopulation);
    } 
    
};
void ClsBasePattern::setTargetPopulationParameter(ClsSysFileNode &_clsSysFileNodeSubPopulation){
//    cout << "ClsBasePattern::setTargetPopulationParameter(ClsSysFileNode &_clsSysFileNodeSubPopulation)" << endl;
    string strSubPopulation = _clsSysFileNodeSubPopulation.getName();
    if(!strSubPopulation.compare(ClsTagLibrary::SelectorRegion())){
	clsSubPopulationTarget = new ClsRegion();
//	cout << "create SubPopulation:  ClsRegion" << endl;
    }
    else if(!strSubPopulation.compare(ClsTagLibrary::SelectorList())){
	clsSubPopulationTarget = new ClsList();
//	cout << "create SubPopulation:  ClsList" << endl;
    }
    else if(!strSubPopulation.compare(ClsTagLibrary::SelectorAll())){
	clsSubPopulationTarget = new ClsAll();
//	cout << "create SubPopulation:  ClsAll" << endl;
    }
    else {
	clsSubPopulationTarget = NULL;
    }
    
	
    if(clsSubPopulationTarget!=NULL){
	clsSubPopulationTarget->setParameter(_clsSysFileNodeSubPopulation);
    } 
};




void ClsBasePattern::setTupleParameter(ClsSysFileNode &_clsSysFileNodeSubPopulation){
//    cout << "ClsBasePattern::setTupleParameter(ClsSysFileNode &_clsSysFileNodeSubPopulation)" << endl;

    string strSubPopulation = _clsSysFileNodeSubPopulation.getName();

    if(clsTuples==NULL){
	clsTuples = new ClsTuples();
//	cout << "create SubPopulation:  ClsTuples" << endl;
    }
    
    if(clsTuples!=NULL){    
	clsTuples->setParameter(_clsSysFileNodeSubPopulation);
    }
};
