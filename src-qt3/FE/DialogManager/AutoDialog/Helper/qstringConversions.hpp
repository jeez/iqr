/****************************************************************************
 ** $Filename: qstringConversions.hpp
 ** $Header$
 **
 ** Copyright (C) 2002 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Fri Jan  4 18:55:26 2002
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Functions to convert QString into std::string and
 **               vice versa.
 **
 *****************************************************************************/

#ifndef QSTRINGCONVERSIONS_HPP
#define QSTRINGCONVERSIONS_HPP

#include <cstring>
#include <string>

#include <qstring.h>

namespace iqrfe {

    using namespace std;

    /** 
     * Converts a std::string into a QString.
     *
     * This function copies the contents of the specified std::string
     * by value into a QString.  The present implementation does not
     * support Unicode characters; this support could be provided by
     * using std::wstring.
     *
     * @return The new QString.  
     * @param _strOriginal string to convert.
     */
    inline QString string2QString(string _strOriginal) 
    {
	return _strOriginal.c_str();
    }

    /** 
     * Converts a QString into a std::string.
     *
     * This function copies the contents of the specified QString by
     * value into a std::string.  The present implementation does not
     * support Unicode characters; this support could be provided by
     * using std::wstring.
     *
     * @return string Copy of original QString.
     * @param _qstrOriginal QString to convert.
     */
    inline string QString2string(QString _qstrOriginal) 
    {	
	return _qstrOriginal.latin1();		
    }

    inline bool operator==(string _string, QString _qstring)
    {
	if (strcmp(_string.c_str(), _qstring.latin1()) == 0) {
	    return true;
	}
	return false;
    }
    inline bool operator==(QString _qstring, string _string)
    {
	if (strcmp(_qstring.latin1(), _string.c_str()) == 0) {
	    return true;
	}
	return false;
    }

};

#endif

