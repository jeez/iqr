/****************************************************************************
 ** $Filename: ClsGroupManipPattern.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Oct 14 17:32:11 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSGROUPMANIPPATTERN_H
#define CLSGROUPMANIPPATTERN_H    /*+ To stop multiple inclusions. +*/

#include <list>
#include <stdexcept>
#include <string>
using namespace std;
//using namespace iqrcommon;

//#include "item.hpp"
#include "stateArray.hpp"
//#include "stateVariable.hpp"
//#include "stateVariableList.hpp"

//class ClsStateVariable;


class ClsGroupManipPattern {
public:
    ClsGroupManipPattern(int _iSize, int _iLength = 1);
    ~ClsGroupManipPattern();

   void setDepth(int _iDepth);
	
    iqrcommon::StateArray& getPatternBuffer();
	
    void advance();
    void blindStep();

    void insertPattern(unsigned int iIndex, valarray<double> p);




//    void deleteAllPatterns();


    const valarray<double>& operator[] (int ii) const {
	return (*pPatternBuffer)[ii];
    };

    void resetCounters(){
	iLoopCounter = 0;
	iAdvanceCounter = 0;
	bDoUpdate = true;
    }

    void setMode(int ii){iMode = ii;};
    int getMode(){return iMode;};

    void setPlayBack(int ii){iPlayBack = ii;};
    int getPlayBack(){return iPlayBack;};

    void setInterval(int ii){iInterval=ii; };
    int getInterval(){return iInterval;};

    void setStepSize(int ii){iStepSize=ii; };
    int getStepSize(){return iStepSize;};

    void setLoops(int ii){iLoops=ii;};
    int getLoops(){return iLoops;};

    int getLoopCounter(){return iLoopCounter;};

    enum MODE {
	MODE_CLAMP = 0,
	MODE_ADD,
	MODE_MULTIPLY
    };

    enum PLAYBACK {
	PLAYBACK_LOOP = 0,
	PLAYBACK_ITERATIONS
    };


	
private:
    iqrcommon::StateArray *pPatternBuffer;

    int iSize;
    int iMode;
    int iPlayBack;
    int iInterval;
    int iStepSize;
    int iLoops;
    int iLoopCounter;
    int iAdvanceCounter;
    bool bDoUpdate;


//    friend class ClsFEComputeEngine;
    friend class ClsFEGroup;
};
    

#endif /* CLSGROUPMANIPPATTERN_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:

