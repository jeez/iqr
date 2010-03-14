/****************************************************************************
 ** $Filename: ClsProtocol.h
 **
 ** $Author: Ulysses Bernardet
 **
 *****************************************************************************/


#ifndef CLSPROTOCOL
#define CLSPROTOCOL 

#define LENGTH_BODYLENGTH 16
#define LENGTH_TYPE 2
#define LENGTH_ID 16
#define LENGTH_PARAMNAME 16

#define LENGTH_HEADER (LENGTH_BODYLENGTH + LENGTH_TYPE +  LENGTH_ID +  LENGTH_PARAMNAME)

namespace iqrcommon {
     
     class ClsProtocol {
     public:
	  enum MESSAGE_TYPES {
	       PROTO_UNKNOWN = 0,
	       /*command (FE -> Prc) The ID refers to a group*/
	       PROTO_CMD_CMD,               //cm 
	       PROTO_CMD_SET_PARAMETER,     //pa 
	       PROTO_CMD_REQUEST_PERMANENT, //rp
	       PROTO_CMD_REQUEST_VOLATILE,  //rv
	       PROTO_CMD_CANCEL_REQUEST,    //cr
	       PROTO_CMD_SYSTEMFILE,        //sy
	       PROTO_CMD_GO,        //go // NEW 20020815
	
/*message (Prc -> FE) The ID refers to the Prc that sends the message*/
	       PROTO_MESSAGE_ERROR,         //er
	       PROTO_MESSAGE_MESSAGE,       //ms
	       PROTO_MESSAGE_ACK,           //ak
	       PROTO_MESSAGE_ROUNDACK,      //rd

/*error (FE -> PrcDaemon) The ID refers to a special  Prc UID! (which is != PrcID */
	       PROTO_CMD_DAEMON,        //dc

/*error (PrcDaemon -> FE) he ID refers to a special  Prc UID! (which is != PrcID */
	       PROTO_ERROR_DAEMON,         //de
	       
/*data (normal) The ID refers to a group*/
	       PROTO_DATA,               //da
	       
/*data (perturb) The ID refers to a group*/
	       PROTO_DATA_PERTURB          //pt
	  };
	
	  static const unsigned short int  iHeaderLength = 50;
  
	  inline static const char* cmd_cmd()               { return "cm"; }; 
	  inline static const char* cmd_set_parameter()     { return "pa"; }; 
	  inline static const char* cmd_request_permanent() { return "rp"; };
	  inline static const char* cmd_request_volatile()  { return "rv"; };
	  inline static const char* cmd_cancel_request()    { return "cr"; };
	  inline static const char* cmd_systemfile()        { return "sy"; };
	  inline static const char* cmd_go()                { return "go"; };
	  inline static const char* message_error()         { return "er"; };
	  inline static const char* message_message()       { return "ms"; };
	  inline static const char* message_ack()           { return "ak"; };
	  inline static const char* message_roundack()      { return "rd"; };
	  inline static const char* data()                  { return "da"; };
	  inline static const char* data_perturb()          { return "pt"; };

	  inline static const char* error_daemon()          { return "de"; };
	  inline static const char* cmd_daemon()            { return "dc"; };



	  /* some message that are exchanged between FE and Prc ---------------------------- */
	  inline static const char* ackAlive()            { return "Alive"; };
	  inline static const char* msgSystemFile()       { return "SystemFile"; };
	  inline static const char* ackSystemFileParsed() { return "SystemFileParsed"; };
	  inline static const char* cmdBuildSystem()      { return "BuildSystem"; };
	  inline static const char* ackSystemBuildt()     { return "SystemBuildt"; };
	  inline static const char* cmdConnectToPrcs()    { return "ConnectToPrcs"; };
	  inline static const char* ackConnectedToPrcs()  { return "ConnectedToPrcs"; };
	  inline static const char* cmdEndWaiting()       { return "EndWaiting"; };
	  inline static const char* ackEndWaiting()       { return "EndedWaiting"; };
	  
/*  for performance reasons "go" will be it's own message type
 	  inline static const char* cmdGo()               { return "Go"; };
*/
	  inline static const char* cmdStop()             { return "Stop"; };

	  /* ------------------------------------------------------------------------------- */
	  
     };
}

#endif






