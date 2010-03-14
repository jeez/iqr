/****************************************************************************
 ** $Filename: stringConversions.hpp
 **
 ** $Author: Mark Blanchard
 ** 
 ** $Keywords:
 ** $Description: Functions to convert bool, int and double values to
 **               and from standard library strings.
 **
 *****************************************************************************/

#ifndef STRINGCONVERSIONS_HPP
#define STRINGCONVERSIONS_HPP

#include <sstream>
#include <string>

namespace iqrcommon {

    using namespace std;
    
    /** 
     * Extracts an integer from a string.
     * 
     * @return Value of the integer found, or 0 if no value was found.
     * @param _strValue String containing the value.
     */
    inline int string2int(const string& _strValue)
    {
	istringstream buffer(_strValue);
	int iValue = 0;
	buffer >> iValue;
	return iValue;
    }

    /** 
     * Creates a string from an integer.
     *
     * @return String containing the value.
     * @param _iValue Integer value to convert.
     */
    inline string int2string(int _iValue)
    {
	ostringstream buffer;
	buffer << _iValue;
	return buffer.str();
    }

    /** 
     * Extracts a double from a string.
     * 
     * @return Value of the double found, or 0.0 if no value was found.
     * @param _strValue String containing the value.
     */
    inline double string2double(const string& _strValue)
    {
	istringstream buffer(_strValue);
	double dValue = 0.0;
	buffer >> dValue;
	return dValue;
    }

    /** 
     * Creates a string from a double.
     *
     * @return String containing the value.
     * @param _dValue Double value to convert.
     */
    inline string double2string(double _dValue)
    {
	ostringstream buffer;
	buffer << _dValue;
	return buffer.str();
    }

    inline bool string2bool(const string& _strValue)
    {
	if (_strValue == "true") {
	    return true;
	}
	return false;
    }

    inline string bool2string(bool _bValue)
    {
	return _bValue ? "true" : "false";
    }

};

#endif
