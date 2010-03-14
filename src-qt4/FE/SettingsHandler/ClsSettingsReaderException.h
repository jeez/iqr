#ifndef CLSSETTINGSREADEREXCEPTION_H
#define CLSSETTINGSREADEREXCEPTION_H

#include <string>

using namespace std;



class ClsSettingsReaderException //: public ClsPrcBaseException
{
public:
    ClsSettingsReaderException(int _iException) {
	iException = _iException;
	strMessage = "";
	
    };

    ClsSettingsReaderException(string _strMessage) {
	iException = -99;
	strMessage = _strMessage;
    };

    enum SETTINGSREADER_ERROR {
	BUFFER_NOT_PARSED,
	NO_IQRSETTING_NODE,
	ENTITY_NOT_FOUND,
	PARSE_ERROR,
    };

  
  
    string getMessage() {
	if(iException>0){
	    switch (iException) {
	    case BUFFER_NOT_PARSED:
		strMessage = "SETTINGSREADER_ERROR:_buffer_not_parsed_yet:_call_parseBuffer()_first";
		break;
	    case NO_IQRSETTING_NODE:
		strMessage = "SETTINGSREADER_ERROR:_no_iqrSetting_node_found";
		break;
	    case ENTITY_NOT_FOUND:
		strMessage = "SETTINGSREADER_ERROR:_entity_not_found";
		break;
	    case PARSE_ERROR:
		strMessage = "SETTINGSREADER_ERROR:_error_parsing_system_file";
		break;
	    default:
		strMessage = "SETTINGSREADER_ERROR:_unknown_Error";
		break;
	    }
	}
	
	return strMessage;
    };
  
private:
    int iException;
    string strMessage;
    

};



#endif

/// Local Variables: 
/// mode: c++
/// End: 
