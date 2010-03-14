/****************************************************************************
 ** $Filename: ClsDataClientConfigReaderException.h
 ** $Id: ClsDataClientConfigReaderException.h,v 1.1 2003/10/04 10:08:15 ulysses Exp $
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Thu Oct  2 14:33:48 2003
 ** $Date: 2003/10/04 10:08:15 $
 **
 ** $Log: ClsDataClientConfigReaderException.h,v $
 ** Revision 1.1  2003/10/04 10:08:15  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSDATACLIENTCONFIGREADEREXCEPTION_H
#define CLSDATACLIENTCONFIGREADEREXCEPTION_H    /*+ To stop multiple inclusions. +*/

#include <string>

using namespace std;

class ClsDataClientConfigReaderException {
public:
    ClsDataClientConfigReaderException(int _iException) {
	iException = _iException;
	strMessage = "";

    };

    ClsDataClientConfigReaderException(string _strMessage) {
	iException = -99;
	strMessage = _strMessage;
    };

    enum CONFIGREADER_ERROR {
	BUFFER_NOT_PARSED,
	NO_IQRCONFIG_NODE,
	ENTITY_NOT_FOUND,
	PARSE_ERROR,
    };

    string getMessage() {
	if(iException>0){
	    switch (iException) {
	    case BUFFER_NOT_PARSED:
		strMessage = "CONFIGREADER_ERROR:_buffer_not_parsed_yet:_call_parseBuffer()_first";
		break;
	    case NO_IQRCONFIG_NODE:
		strMessage = "CONFIGREADER_ERROR:_no_iqrConfig_node_found";
		break;
	    case ENTITY_NOT_FOUND:
		strMessage = "CONFIGREADER_ERROR:_entity_not_found";
		break;
	    case PARSE_ERROR:
		strMessage = "CONFIGREADER_ERROR:_error_parsing_config_file";
		break;
	    default:
		strMessage = "CONFIGREADER_ERROR:_unknown_Error";
		break;
	    }
	}

	return strMessage;
    };

private:
    int iException;
    string strMessage;

};

#endif /* CLSDATACLIENTCONFIGREADEREXCEPTION_H */

/// Local Variables:
/// mode: c++
/// End:
