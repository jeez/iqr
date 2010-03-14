/****************************************************************************
 ** $Filename: idGenerator.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Jan 16 22:09:04 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: generates a unique ID number.
 **
 **               ID format:
 **                   OS identifier (different OS store time differently)
 **                   PID (on UNIX, in hex capitals)
 **                   current time (since 1 Jan 1970 on UNIX as decimal)
 **                   counter (in lowercase hex) (allows MAX_INT IDs to 
 **                                               be created each second)
 ** 
 **               This class will need to be reimplemented for each OS.
 **
 *****************************************************************************/

#ifndef IDGENERATOR_HPP
#define IDGENERATOR_HPP

// Are there more C++/cross-platform ways to find the time and process
// ID?
#include <ctime>
#include <unistd.h>

//#include <qstring.h>
#include <string>

using namespace std;

class ClsIDGenerator {

public:
    static ClsIDGenerator* Instance();

//    ~ClsIDGenerator() {}

    string Next();

private:

    ClsIDGenerator ();
    static ClsIDGenerator* _instanceIDGenerator;

    
    time_t  tCurrent, tPrevious;

    int     iCount, iPID;

};

#endif
