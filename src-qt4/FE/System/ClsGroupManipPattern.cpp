/****************************************************************************
 ** $Filename: ClsGroupManipPattern.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Oct 14 17:32:09 2003
 **
 *****************************************************************************/


#include <iostream>
#include <string>
using namespace std;

//#include <Common/Helper/deleteObject.hpp>

#include "ClsGroupManipPattern.h"

//#define DEBUG_CLSGROUPMANIPPATTERN

ClsGroupManipPattern::ClsGroupManipPattern(int _iSize, int _iLength) {
#ifdef DEBUG_CLSGROUPMANIPPATTERN
    cout << "ClsGroupManipPattern::ClsGroupManipPattern" << endl;
#endif

    iSize = _iSize;
    iMode = -1;
    iPlayBack = -1;
    iInterval = 1;
    iLoops = 1;
    iLoopCounter = 0;
    iAdvanceCounter = 0;
    iStepSize = 1;
    bDoUpdate = true;
    pPatternBuffer = new iqrcommon::StateArray(_iSize, _iLength);    
}

ClsGroupManipPattern::~ClsGroupManipPattern() {    
#ifdef DEBUG_CLSGROUPMANIPPATTERN
    cout << "ClsGroupManipPattern::~ClsGroupManipPattern:" << endl;
#endif
//    delete pPatternBuffer;
//    pPatternBuffer = NULL;
}

void ClsGroupManipPattern::setDepth(int _iDepth){
#ifdef DEBUG_CLSGROUPMANIPPATTERN
    cout << "ClsGroupManipPattern::setDepth(int _iDepth)" << endl;
#endif
    if(pPatternBuffer!=NULL){
	pPatternBuffer->setDepth(_iDepth);
    }
}

// void ClsGroupManipPattern::appendPattern(valarray<double> p){
// //#ifdef DEBUG_CLSGROUPMANIPPATTERN
//     cout << "ClsGroupManipPattern::appendPattern(valarray<double> p)" << endl;
// //#endif
    
//     int i = pPatternBuffer->getBufferLength();
//     pPatternBuffer->extend (i+1);
//     (*pPatternBuffer)[i] = p;

//     for(int ii=0; ii<pPatternBuffer->getBufferLength();ii++){
// 	cout << (*pPatternBuffer)[ii] << endl << "------------------------" << endl;
//     } 
// }

void ClsGroupManipPattern::insertPattern(unsigned int iIndex, valarray<double> p){
#ifdef DEBUG_CLSGROUPMANIPPATTERN
    cout << "ClsGroupManipPattern::insertPattern(int iIndex, valarray<double> p)" << endl;
#endif
//    if(iIndex < pPatternBuffer->getBufferLength()){
    if(iIndex < pPatternBuffer->getDepth()){
	(*pPatternBuffer)[iIndex] = p;
    }

#ifdef DEBUG_CLSGROUPMANIPPATTERN
	cout << "pPatternBuffer->getDepth(): " << pPatternBuffer->getDepth() << endl;
#endif
/*

     for(int ii=0; ii<pPatternBuffer->getBufferLength();ii++){
 	cout << (*pPatternBuffer)[ii] << endl << "------------------------" << endl;
     } 
*/

}


iqrcommon::StateArray& ClsGroupManipPattern::getPatternBuffer() {
#ifdef DEBUG_CLSGROUPMANIPPATTERN
    cout << "ClsGroupManipPattern::getPatternBuffer()" << endl;
#endif
    return *pPatternBuffer;
}

void ClsGroupManipPattern::advance() {
#ifdef DEBUG_CLSGROUPMANIPPATTERN
    cout << "ClsGroupManipPattern::advance()" << endl;
#endif

    for(int ii=0; ii<iStepSize; ii++){
//	cout << "pPatternBuffer->advance()" << endl;
//	cout << "pPatternBuffer->getDepth(): " << pPatternBuffer->getDepth() << endl;
	pPatternBuffer->advance();
    }

    if(iPlayBack==ClsGroupManipPattern::PLAYBACK_ITERATIONS){
	iAdvanceCounter+= iStepSize;
//	if(iAdvanceCounter % (pPatternBuffer->getBufferLength()) ==0){
	if(iAdvanceCounter % (pPatternBuffer->getDepth()) ==0){
	    iLoopCounter++;
#ifdef DEBUG_CLSGROUPMANIPPATTERN
	    cout << "loop completed: iAdvanceCounter, iLoopCounter: " << iAdvanceCounter << ", " << iLoopCounter << endl;
#endif
	    if(iLoopCounter>=iLoops){
		bDoUpdate = false;
	    }
	}
    }
}


// void ClsGroupManipPattern::deleteAllPatterns(){
//     delete pPatternBuffer;
//     pPatternBuffer = new iqrcommon::StateArray(iSize);

// }
