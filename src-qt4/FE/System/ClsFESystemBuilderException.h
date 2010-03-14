#ifndef CLSFESYSTEMBUILDEREXCEPTION_H
#define CLSFESYSTEMBUILDEREXCEPTION_H

#include <string>
//#include "ClsFEBaseException.h"

class ClsFESystemBuilderException
//class ClsFESystemBuilderException : public ClsFEBaseException
{
public:
    ClsFESystemBuilderException(int _iException) {
	    iException = _iException;
	    strError = "";
	};

    ClsFESystemBuilderException(int _iException, string _strError) {
	    iException = _iException;
	    strError = _strError;
	};

    ClsFESystemBuilderException(string _strError) {
	     iException = MESSAGE;
	    strError = _strError;
	};
    
    enum SYSTEMBUILDER_ERROR {
	 MESSAGE,
	 NO_SUCH_PARAMETER,
	 NO_FILE_SYSTEMFILE,
	 NO_FENAME,
	 NO_FEID,
	 NO_FETYPE,
	 NO_SYSTEMFILENAME,
	 SYSFILEPARSER_ERROR,
	 IMPORT_DUPLICATE_IDS,
	 IMPORT_FRAGMENT_NOT_FOUND
    };
    
    
    
    string getMessage() {
	string strMessage;
	
	switch (iException) {
	case MESSAGE:
	    strMessage = "SYSTEMBUILDER_ERROR:_" + strError;
	    break;
	case NO_SUCH_PARAMETER:
	    strMessage = "SYSTEMBUILDER_ERROR:_unknown_parameter" + strError;
	    break;
	case NO_FILE_SYSTEMFILE:
	    strMessage = "SYSTEMBUILDER_ERROR:_system_file_could_not_be_opened" + strError;
	    break;
	case NO_FENAME:
	    strMessage = "SYSTEMBUILDER_ERROR:_missing_FE_name" + strError;
	    break;
	case NO_FEID:
	    strMessage = "SYSTEMBUILDER_ERROR:_missing_FE_ID" + strError;
	    break;
	case NO_FETYPE:
	    strMessage = "SYSTEMBUILDER_ERROR:_missing_FE_type" + strError;
	    break;
	case NO_SYSTEMFILENAME:
	    strMessage = "SYSTEMBUILDER_ERROR: missing_system_file_name" + strError;
	    break;
	case SYSFILEPARSER_ERROR:
	    strMessage = "SYSTEMBUILDER_ERROR: error_in_ClsSysFileParser" + strError;
	    break;
	case IMPORT_DUPLICATE_IDS:
	    strMessage = "SYSTEMBUILDER_ERROR: Duplicate ID: item with same ID already in system\nItem cannot be imported" + strError;
	    break;
	case IMPORT_FRAGMENT_NOT_FOUND:
	    strMessage = "SYSTEMBUILDER_ERROR: External process not found" + strError;
	    break;
	default:
	    strMessage = "SYSTEMBUILDER_ERROR: unknown error" + strError;             
	    break;
	}
	return strMessage;
    };
    
private:
    int iException;
     string strError;
    
};



#endif



/// Local Variables: 
/// mode: c++
/// compile-command: "cd ../../ && make -k "
/// End: 
