#ifndef CLSPORTFINDEREXCEPTION_H
#define CLSPORTFINDEREXCEPTION_H

#include <errno.h>     
#include <stdio.h>

#include <string>


class ClsPortFinderException //: public ClsPrcBaseException
{
public:
     ClsPortFinderException(int _iException, string _strHost) {
	       iException = _iException;
	       bErrnoError = false;
	       strHost = _strHost;
	  };

     ClsPortFinderException(int i, int _iException, string _strHost) {
	       iException = _iException;
	       bErrnoError = true;
	       strHost = _strHost;
	  };

     enum PORTFINDER_ERROR {
	  WRONG_IP_FORMAT,
	  UNKNOWN_HOST,
	  MAX_PORT_REACHED
     };

  
  
     string getMessage()
	  {
	       string strMessage;

	       if(!bErrnoError){
		    switch (iException) {
		    case WRONG_IP_FORMAT:
			 strMessage = "PORTFINDER_ERROR:_wrong_IP_format:_" + strHost;
			 break;
		    case UNKNOWN_HOST:
			 strMessage = "PORTFINDER_ERROR:_unknown_host:_" + strHost;
			 break;
		    case MAX_PORT_REACHED:
			 strMessage = "PORTFINDER_ERROR:_no_port_found_in_range";
			 break;
		    default:
			 strMessage = "PORTFINDER_ERROR:_unknown_Error:_" + strHost;
			 break;
		    }
	       } else {
		    strMessage = "PORTFINDER_ERROR:_" + (string)strerror(iException);
	       }
      
      
	       return strMessage;
	  };
  
private:
     string strHost;
     bool bErrnoError;
     int iException;

};



#endif

/// Local Variables: 
/// mode: c++
/// End: 
