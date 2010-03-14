#ifndef CLSSYSFILEPARSEREXCEPTION_H
#define CLSSYSFILEPARSEREXCEPTION_H

#include <string>
//#include "ClsPrcBaseException.h"

using namespace std;
namespace iqrcommon {

    class ClsSysFileParserException //: public ClsPrcBaseException
    {
    public:
	ClsSysFileParserException(int _iException) {
	    cout << "EXCEPTION" << endl;
	    iException = _iException;
	    strMsg = "";
	  };
	
	ClsSysFileParserException(int _iException, string _strMsg) {
	    cout << "EXCEPTION" << endl;
	    iException = _iException;
	    strMsg = _strMsg;
	};
	
	enum SYSFILEPARSER_ERROR {
	    BUFFER_NOT_SET,
	    PARSER_NOT_INITIALIZED,
	    BUFFER_NOT_PARSED,
	    UNKNOWN_CONNECTION_TYPE,
	    OUT_OF_GROUP_RANGE,
	    NO_SYSTEM_NODE,
	    SYSTEM_ID_NOT_FOUND,
	    ATTRIBUTE_NOT_FOUND,
	    PARSE_ERROR,
	    PRC_TYPE_NOT_FOUND
	};
  
	string getMessage()
	{
	    string strMessage;
	    
	    switch (iException) {
	    case BUFFER_NOT_SET:
		strMessage = "SYSFILEPARSER_ERROR:_no_input_buffer_set_yet:_call_setInputBuffer(string_strBuffer)_first";
		break;
	    case PARSER_NOT_INITIALIZED:
		strMessage = "SYSFILEPARSER_ERROR:_parser_not_initilaized_yet:_call_XMLPlatformUtilsInitialize()_first";
		break;
	    case BUFFER_NOT_PARSED:
		strMessage = "SYSFILEPARSER_ERROR:_buffer_not_parsed_yet:_call_parseBuffer()_first";
		break;
	    case PRC_TYPE_NOT_FOUND:
		strMessage = "SYSFILEPARSER_ERROR:_unknown_Prc_type";
		break;
	    case UNKNOWN_CONNECTION_TYPE:
		strMessage = "SYSFILEPARSER_ERROR:_unknown_connection_type";
		break;
	    case OUT_OF_GROUP_RANGE:
		strMessage = "SYSFILEPARSER_ERROR:_group_index_out_of_range";
		break;
	    case NO_SYSTEM_NODE:
		strMessage = "SYSFILEPARSER_ERROR:_no_System_node_found";
		break;
	    case SYSTEM_ID_NOT_FOUND:
		strMessage = "SYSFILEPARSER_ERROR:_system_ID_not_found";
		break;
	    case ATTRIBUTE_NOT_FOUND:
		strMessage = "SYSFILEPARSER_ERROR:_required_attribute_not_found:_" + strMsg;
		break;
	    case PARSE_ERROR:
		strMessage = "SYSFILEPARSER_ERROR:_error_parsing_system_file:_" + strMsg;
		break;
	    default:
		strMessage = "SYSFILEPARSER_ERROR:_unknown_Error";
		break;
	    }
	    
	    return strMessage;
	};
	
    private:
	int iException;
	string strMsg;
    };
}

#endif

/// Local Variables: 
/// mode: c++
/// End: 
