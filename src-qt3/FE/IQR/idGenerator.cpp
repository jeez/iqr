/****************************************************************************
 ** $Filename: idGenerator.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Jan 16 22:14:33 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: this class uses Linux/UNIX specific clock calls.  These 
 **               must be replaced on other platforms.
 **
 **               MS GUID format:
 **                   CPU serial no.
 **                   Network adaptor MAC address
 **                   time
 **
 *****************************************************************************/

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "idGenerator.hpp"

//using namespace std;

#ifdef LINUX
static const char *pcOS = "L";
#else
static const char *pcOS = "O";
#endif

//#define DEBUG_CLSIDGENERATOR

#ifdef DEBUG_CLSIDGENERATOR
static const bool bDebugIDGenerator = true;
#else
static const bool bDebugIDGenerator = false;
#endif

ClsIDGenerator*  ClsIDGenerator::_instanceIDGenerator = NULL;


ClsIDGenerator* ClsIDGenerator::Instance(){
    if(_instanceIDGenerator == NULL){
	_instanceIDGenerator = new ClsIDGenerator();
    } 
    return _instanceIDGenerator;
}


ClsIDGenerator::ClsIDGenerator() : iCount(0) {

    // Get current time.
    tPrevious = time(NULL);

    // Store process ID as an integer.
    iPID = int(getpid());

    if (bDebugIDGenerator) {
	// qDebug can only be used once the QApplication is running.
	//qDebug("ClsIDGenrator::ClsIDGenerator: created at %s in process %d",
	// 	 pcNow, iPID);
	cerr << "ClsIDGenerator::ClsIDGenerator: created at "
	     << ctime(&tPrevious) << " in process " << iPID << endl;
	cerr << "ClsIDGenerator::ClsIDGenerator: previous time set to "
	     << int(tPrevious) << endl;
    }

}

string ClsIDGenerator::Next() {
    
    // Get current time (in seconds since 1 Jan 1970, this will loop
    // in 2034).
    tCurrent = time(NULL);

    // Has the time changed since the last ID was generated?
    if (tCurrent == tPrevious) {

	// Time hasn't changed so increment the counter.  The counter
	// allows unique IDs to be created during the same second.
	// The limit is MAX_INT IDs during 1 second.
	iCount++;

    } else {

	// Time has changed, save for next check and
	tPrevious = tCurrent;
	iCount    = 0;

    }

    ostringstream oss;

    // Generate ID.
    string strID;

//    strID.sprintf("%s-%X-%x-%d-%X", pcOS, iPID, int(tCurrent), iCount, random());
    oss << pcOS << "-" << iPID << "-" << int(tCurrent) << "-" << random();
    
    strID = oss.str();


    if (bDebugIDGenerator) {
//	qDebug("ClsIDGenrator::Next: new ID %s created at %s", strID, ctime(&tCurrent));
	cout << "ClsIDGenrator::Next: new ID " << strID << " at " << ctime(&tCurrent) << endl;
    }

    return strID;

}
