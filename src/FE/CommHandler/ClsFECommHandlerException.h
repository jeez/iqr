/****************************************************************************
 ** $Filename: ClsFECommHandlerException.h
 ** $Id: ClsFECommHandlerException.h,v 1.1 2002/02/20 22:27:19 ulysses Exp ulysses $
 **
 ** Copyright (C) 2002 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Feb 20 23:27:12 2002
 ** $Date: 2002/02/20 22:27:19 $
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFECOMMHANDLEREXCEPTION_H
#define CLSFECOMMHANDLEREXCEPTION_H    /*+ To stop multiple inclusions. +*/

#include <errno.h>     
#include <stdio.h>

#include <string>

using namespace std;


class ClsFECommHandlerException
{
public:
     ClsFECommHandlerException(int _iException, string _strError) {
	       iException = _iException;
	       bErrnoError = false;
	       strError = _strError;
	  };

     ClsFECommHandlerException(int i, int _iException, string _strError) {
	       iException = _iException;
	       bErrnoError = true;
	       strError = _strError;
	  };

     enum FECOMMHANDLER_ERROR {
	  SOCKET_ERROR,
	  SOCKET_TIMEOUT_READ,
     };

  
  
     string getMessage() {
	       string strMessage;

	       if(!bErrnoError){
		    switch (iException) {
		    case SOCKET_ERROR:
			 strMessage = "FECOMMHANDLER_ERROR:_socket_error:_" + strError;
			 break;
		    case SOCKET_TIMEOUT_READ:
			 strMessage = "FECOMMHANDLER_ERROR:_socket_timeout_read:_" + strError;
			 break;
		    default:
			 strMessage = "FECOMMHANDLER_ERROR:_unknown_Error:_" + strError;
			 break;
		    }
	       } else {
		    strMessage = "FECOMMHANDLER_ERROR:_" + (string)strerror(iException);
	       }
      
      
	       return strMessage;
	  };
  
private:
     string strError;
     bool bErrnoError;
     int iException;

};

#endif /* CLSFECOMMHANDLEREXCEPTION_H */

// Local Variables: 
/// mode: c++
/// End: 

