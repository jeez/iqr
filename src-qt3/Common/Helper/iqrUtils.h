/****************************************************************************
 ** $Filename: iqrUtils.h
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Aug 30 19:00:24 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef IQRUTILS_H
#define IQRUTILS_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

namespace iqrUtils {
     
     inline double string2double(string str) {
	  double fReturn;
	  fReturn = atof(str.c_str());
	  return fReturn;
     };

     inline int string2int(string str) {
	  int iReturn;
	  int iLength;
	  iLength = str.length();
	  iReturn = 0;
	  for (int j = 0; j < iLength; ++j)
	       iReturn = 10 * iReturn + str[j] - '0';
	  return iReturn;
     };

     inline string int2string(int ii)  {
	  char a[8];
	  sprintf(a,"%d", ii);
	  string strText = a;
	  return strText;
     };

     inline string bool2string(bool b) {
	 if(b){
	     return "true";
	 }
	 return "false";
     };

     inline long string2long(string str) {
	  long lReturn;
	  int iLength;
	  iLength = str.length();
	  lReturn = 0;
	  for (int j = 0; j < iLength; ++j)
	       lReturn = 10 * lReturn + str[j] - '0';
	  return lReturn;
     };

     inline bool string2bool(string str) {
	  bool bReturn = false;
	  if(!str.compare("true") || !str.compare("TRUE"))
	       bReturn = true;
	  else if(!str.compare("false") || !str.compare("FALSE"))
	       bReturn = false;
	  return bReturn;
     };

     inline short string2short(string str) {
	  short sReturn;
	  sReturn = (short)atof(str.c_str());
	  return sReturn;
     };

    static string getDateTimeSuffix(){
	time_t curr;
	tm local;
	time(&curr); // get current time_t value
	local=*(localtime(&curr)); // dereference and assign
	std::stringstream ssDate;    
	ssDate << local.tm_year+1900; 
	ssDate << setw(2) << setfill('0') << local.tm_mon+1;
	ssDate << setw(2) << setfill('0') << local.tm_mday;
	ssDate << "+";
	ssDate << setw(2) << setfill('0') << local.tm_hour;
	ssDate << setw(2) << setfill('0') << local.tm_min;
	ssDate << setw(2) << setfill('0') << local.tm_sec;
	return ssDate.str();
    }

    static string getEpoch(){
	std::stringstream ssEpoch; 
	time_t curr;
	tm local;
	time(&curr); // get current time_t value
	ssEpoch << curr;
	return ssEpoch.str();
    }


}

#endif /* IQRUTILS_H */

/// Local Variables: 
/// mode: c++
/// End: 




