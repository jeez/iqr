/****************************************************************************
 ** $Filename: ClsFEMessageOut.h
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Sep 14 14:05:15 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEMESSAGEOUT_H
#define CLSFEMESSAGEOUT_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <sstream>
#include <iomanip>

#include <ClsFEDataSink.h>
#include <ClsProtocol.h>

using namespace iqrcommon;
using namespace std;

class ClsFEMessageOut {
public:
//     ClsFEMessageOut( int _iMessageType, string _strID, string _strParamName, ClsFEDataSink *_clsFEDataSink) :
    ClsFEMessageOut( int _iMessageType, string _strID, string _strParamName, ClsBaseDataSink *_clsDataSink) :
	iMessageType(_iMessageType), strID(_strID), strParamName(_strParamName), clsDataSink(_clsDataSink) {
	fValue = -99;
	strBody = "";
    };

    ClsFEMessageOut( int _iMessageType, string _strID, string _strParamName, string _strBody) :
	iMessageType(_iMessageType), strID(_strID), strParamName(_strParamName), strBody(_strBody) {
	fValue = -99;
    };

    ClsFEMessageOut( int _iMessageType, string _strID, string _strParamName, float _fValue) :
	iMessageType(_iMessageType), strID(_strID), strParamName(_strParamName), fValue(_fValue) {
	strBody = "";
    };

//       /* header related */
    void setID (string _strID) { };

    void setParameterName(string _strParamName) { strParamName = _strParamName; };
    void setMessageType(int _iMessageType) { iMessageType = _iMessageType; };


//       /* body related */
    void setBody(string _strBody) { strBody = _strBody; };
    void setBody(float _fValue) { fValue = _fValue; };
    void setDataSink(ClsBaseDataSink *_clsDataSink) { clsDataSink = _clsDataSink; };



    int getMessageType() { return iMessageType; };
    string getID() { return strID; };
    string getParamName() { return strParamName; };
    ClsBaseDataSink *getDataSinkPointer() { return clsDataSink; };

    string getMessage() {

	if(strID.length()==0){ strID = "- "; }
	if(strParamName.length()==0){ strParamName = "-"; }

	string strMessageType;
	ostringstream osMessageBody;
	unsigned short int iBodyLength = 0;


	switch (iMessageType) {
	case ClsProtocol::PROTO_CMD_SET_PARAMETER:
	    strMessageType = ClsProtocol::cmd_set_parameter();
	    osMessageBody << setw(32) << fValue;
	    break;

	case ClsProtocol::PROTO_CMD_REQUEST_PERMANENT:
	    strMessageType = ClsProtocol::cmd_request_permanent();
	    break;

	case ClsProtocol::PROTO_CMD_REQUEST_VOLATILE:
	    strMessageType = ClsProtocol::cmd_request_volatile();
	    break;

	case ClsProtocol::PROTO_CMD_CANCEL_REQUEST:
	    strMessageType = ClsProtocol::cmd_cancel_request();
	    break;

	case ClsProtocol::PROTO_CMD_SYSTEMFILE:
	    strMessageType = ClsProtocol::cmd_systemfile();
	    osMessageBody  << strBody;
	    break;

	case ClsProtocol::PROTO_MESSAGE_ERROR:
	    strMessageType = ClsProtocol::message_error();
	    osMessageBody << strBody;
	    break;

	case ClsProtocol::PROTO_MESSAGE_MESSAGE:
	    strMessageType = ClsProtocol::message_message();
	    osMessageBody  << strBody;
	    break;

	case ClsProtocol::PROTO_CMD_CMD:
	    strMessageType = ClsProtocol::cmd_cmd();
	    osMessageBody  << strBody;
	    break;

	case ClsProtocol::PROTO_CMD_GO:
	    strMessageType = ClsProtocol::cmd_go();
	    break;

/* relating to the PrcDaemon */
	case ClsProtocol::PROTO_CMD_DAEMON:
	    strMessageType = ClsProtocol::cmd_daemon();
	    osMessageBody  << strBody;
	    break;
/* -------------------------- */

	    /* not needed in the current context
	       case ClsProtocol::PROTO_MESSAGE_ROUNDACK:
	       strMessageType = ClsProtocol::message_roundack();
	       break;


	       case ClsProtocol::PROTO_DATA_PERTURB:
	       strMessageType = ClsProtocol::data_perturb();
	       break;
	    */

	default:
	    exit(0); //!!!! to prevent the usage of non existing types
	    break;
//@#@ we should have some error handling here!!!!!

	}

	ostringstream osHeadFixed;
//	  char p = osHeadFixed.fill('*');
	osHeadFixed << std::setw(2) << strMessageType;
	osHeadFixed << std::setw(15) << strID << " ";
	osHeadFixed << std::setw(15) << strParamName << " ";

// format < 20030113		  iBodyLength = (osMessageBody.str()).size();
// format < 20030113		  ostringstream osMessage;
// format < 20030113		  osMessage << osHeadFixed.str();
// format < 20030113		  osMessage << std::setw(16) << iBodyLength;
// format < 20030113		  osMessage << osMessageBody.str();

/* format > 20030113: body length as first field */
	iBodyLength = (osMessageBody.str()).size();
	ostringstream osMessage;
	osMessage << std::setw(16) << iBodyLength;
	osMessage << osHeadFixed.str();
	osMessage << osMessageBody.str();




	/* here the body needs to be handled: do we have and fValue, strBody, or ClsFEDataSink??? */

	return osMessage.str();
    }



private:
    int iMessageType;
    string strID;
    string strParamName;
    string strBody;
    float fValue;
    ClsBaseDataSink *clsDataSink;

    friend ostream &operator<<(ostream &stream, ClsFEMessageOut clsFEMessageOut);
    friend ostream &operator<<(ostream &stream, ClsFEMessageOut *clsFEMessageOut);

};



/// Local Variables:
/// mode: c++
/// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ && make -k"
/// End:

#endif /* CLSFEMESSAGEOUT_H */
