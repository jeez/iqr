/****************************************************************************
 ** $Filename: ClsFECommHandler.cpp
 ** $Id: ClsFECommHandler.cpp,v 1.7 2002/12/08 17:33:16 ulysses Exp ulysses $
 **
 ** Copyright (C) 2002 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Feb 21 14:44:12 2002
 ** $Date: 2002/12/08 17:33:16 $
 **
 ** $Log: ClsFECommHandler.cpp,v $
 ** Revision 1.7  2002/12/08 17:33:16  ulysses
 ** After separating system building, managing and serializing
 **
 ** Revision 1.6  2002/08/15 09:54:05  ulysses
 ** new protocol largey implemented
 ** switching to CMD_GO message type
 **
 ** Revision 1.5  2002/08/02 16:32:47  ulysses
 ** before applying the changes ClsFEMessageOut and ClsSocketOut* classes
 **
 ** Revision 1.4  2002/03/06 13:35:53  ulysses
 ** intermediate check in, before cleaning up some more (delete what's moved to startup, and
 ** launch prcs)
 **
 ** Revision 1.3  2002/02/25 12:03:09  ulysses
 ** before framing out the launching to the ClsFEPrcManager, and using the new communication schema
 **
 ** Revision 1.2  2002/02/21 13:44:38  ulysses
 ** *** empty log message ***    
 **  
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

/*
  reactions:
  if(alive) -> openPortOut, send systemFile 
  if(sysFileParsed) -> only change status
  if(SystemBuildt) -> change state, wait until all prcs are in this state

  2 phase start up
  1. alive, systemfilem, systembuild (this can be done asynchronously)
  2. prcs connects to other prcs (has to be done in sequence)

  consequences:
  separate startup procedures from runtime procedures
*/

//#include <ClsSimControl.h>
#include <ClsFECommHandler.h>
#include <ClsProtocol.h>
#include "ClsInterThreadMessage.h"
#include "ClsFEProcess.h"

#include "ClsFESystemManager.h"

//const int iReportingStatus = 1; // 0: be quiet ... 2: loads of blabla

/** 
 * establishing communication with and between Prcs
 *  if(alive) -> openPortOut, send systemFile
 *  if(sysFileParsed) -> only change status
 *  if(SystemBuildt) -> change state, wait until all prcs are in this state
 */

#define COUT cout << "[01;34m"
#define ENDL "[00m" << endl


//ClsFECommHandler::ClsFECommHandler(ClsSimControl *_parent) : parent(_parent){
ClsFECommHandler::ClsFECommHandler( ){

     
    iTimeOutSec = 2;	  
    iReportingStatus = 1; // 0: be quiet ... 2: loads of blabla
//     bManualLaunch = false;

    /* this is a global read time out: it's not specific to a single prc, but to the socketIn in general */
    iTimeOutRead = 15;
    bInternalError = false;

};

     
ClsFECommHandler::~ClsFECommHandler() {
//     if(clsSocketIn)
//	  clsSocketIn->closeSocket();
};
     
int ClsFECommHandler::open(void*) {
    COUT << "ClsFECommHandler::open(void*)" << ENDL;
    int iError = 0;
    iRun = 0;

    try{
	openSocketIn();
    }
    catch (ClsSocketException &e){
	COUT << e.getMessage() << ENDL;
	iError = -1;
	ClsFECommHandlerException clsFECommHandlerException(ClsFECommHandlerException:: SOCKET_ERROR,  e.getMessage());
	throw clsFECommHandlerException;
    }
     
    if(!iError){
	activate();
    }
    return iError;
};
     
     
void ClsFECommHandler::setTimeOutSec(int _iTimeOutSec) { 
    iTimeOutSec = _iTimeOutSec; 
};

void ClsFECommHandler::closeSocketIn() {
    if(clsSocketIn)
	clsSocketIn->closeSocket();
};
     
void ClsFECommHandler::openSocketIn() {
    COUT << "ClsFECommHandler::openSocketIn()" << ENDL;

/*
    ClsInfoSystem clsInfoSystem = clsFESystemManager->getInfoSystem();
    string strFEHostname =  clsInfoSystem.getHostname( ); 
    int iPortFE = clsInfoSystem.getPort( );
*/


    string strFEHostname =  clsFESystemManager->getSystemHostName();
    int iPortFE = clsFESystemManager->getSystemPort();



//@@1    ClsInfoPrc clsInfoPrcFE = clsFESystemManager->getInfoPrcFE();
//@@1    string strFEHostname = clsInfoPrcFE.getHostname();
//@@1    int iPortFE = clsInfoPrcFE.getPort();

//     clsSocketIn = new ClsSocketInPipe();
    clsSocketIn = new ClsSocketInTcp(strFEHostname, iPortFE);

//       clsSocketIn->setHostName(strFEHostname);
//       clsSocketIn->setPortNumber(iPortFE);
//     clsSocketIn->setTimeOutSec(3); // not used anymore
//     clsSocketIn->setTimeOutUSec(0);  // not used anymore

    if(iReportingStatus>0){
	sendMessage2Parent("",ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, "Opening SocketIn");
    }
     
    try{
	clsSocketIn->openSocket(); // error handling!
    }
    catch (ClsSocketException &e){
	COUT << e.getMessage() << ENDL;;
	throw;
    }

    COUT << "SOCKET OPEN" << ENDL;
    if(iReportingStatus>0){
	sendMessage2Parent("",ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, "SocketIn opened");
    }
};
     
void ClsFECommHandler::addDataRequest(string strPrcID, ClsFEMessageOut clsFEMessageOut) {
    COUT << "ClsFECommHandler::addDataRequest" << ENDL;
//     pair<string, ClsFEMessageOut > pairTemp(clsFEMessageOut.getPrcID(), clsFEMessageOut);
    pair<string, ClsFEMessageOut > pairTemp(strPrcID, clsFEMessageOut);
    mapMessagesOut.insert(pairTemp);

    /*
      - check if there's already a message in for the data
      - if not create a new message in
      - if yes, add the pointer of the sink to the message in
    */

    string strTitle = strPrcID + ":" + clsFEMessageOut.getParamName();
    ClsFEMessageIn clsFEMessageIn(strTitle);
    clsFEMessageIn.addDataSinkPointer(clsFEMessageOut.getDataSinkPointer());
    pair<string, ClsFEMessageIn> pairTempMessage(strTitle, clsFEMessageIn); 
    mapMessages.insert(pairTempMessage);

    COUT << clsFEMessageOut << ENDL;
};


void ClsFECommHandler::startup( ) {
/* needs doing:
   - error handling:
   - try again if no responed
   - use timeouts
   - check that systemfile is set
*/
    COUT << "ClsFECommHandler::startup()" << ENDL;
    int iState;

    /* first off we need to create the map of Prcs */
    createPrcMap();

    ace_condReady->signal();
    /* wait for Prcs to connect */
    iState = ClsInfoPrc::PRC_ALIVE;
    iTimeOutRead = 15;
    while(!allPrcsInState (iState)){
	COUT << "waiting for Prcs to connect" << ENDL;
	clsSocketIn->acceptPeerBlocking();
	try{
	    checkForPrcState(ClsProtocol::ackAlive(), iState);
	}
	catch( ClsFECommHandlerException &e){
	    throw;
	}
    }
        
    // NOW THAT THE PRCS SHOULD BE LAUNCHED... 
//     COUT << "WE'RE GETTING HERE: " << __LINE__ << ENDL;
    connectToPrcs();

// to all prcs: 
    /* send the system file to all Prcs */
//     COUT << "WE'RE GETTING HERE: " << __LINE__ << ENDL;
    map<string, pair<ClsInfoPrc, ClsSocketOutTcp> >::iterator mapIteratorPrcs;
//24032002     map<string, pair<ClsInfoPrc*, ClsBaseSocketOut*> >::iterator mapIteratorPrcs; 

    for(mapIteratorPrcs = mapPrcs.begin(); mapIteratorPrcs!=mapPrcs.end(); ++mapIteratorPrcs){
	string strPrcID = mapIteratorPrcs->second.first.getPrcID();
//	  COUT << "WE'RE GETTING HERE: " << __LINE__ << " strPrcID: " << strPrcID << ENDL;
	sendSystemFile2Prc(strPrcID);  
    }
//     COUT << "WE'RE GETTING HERE: " << __LINE__ << ENDL;
    /* wait for Prc's systemfile is parsed */
    iState = ClsInfoPrc::PRC_SYSTEMFILEPARSED;
    iTimeOutRead = 20;
    while(!allPrcsInState(iState)){
	try{
	    checkForPrcState(ClsProtocol::ackSystemFileParsed(), iState);
	}
	catch( ClsFECommHandlerException &e){
	    throw;
	}
    }
     
//     sleep(1); /* hmmm */
    /* to all prcs: send message to prcs: "BuildSystem" */
    for(mapIteratorPrcs = mapPrcs.begin(); mapIteratorPrcs!=mapPrcs.end(); ++mapIteratorPrcs){
	string strPrcID = mapIteratorPrcs->second.first.getPrcID();
	sendCommand2Prc(strPrcID, ClsProtocol::cmdBuildSystem());
    }

    /* wait for Prc's system is buildt */
    iState = ClsInfoPrc::PRC_SYSTEMBUILDT;
    iTimeOutRead =0; /* Prcs might take a while to build the system */
    while(!allPrcsInState( iState )){
	try{
	    checkForPrcState(ClsProtocol::ackSystemBuildt(), iState);
	}
	catch( ClsFECommHandlerException &e){
	    throw;
	}
    }
     
    /* important, at this point we send, and wait for each Prc in turn, to avoid dead locks */
    for(mapIteratorPrcs = mapPrcs.begin(); mapIteratorPrcs!=mapPrcs.end(); ++mapIteratorPrcs){
	string strPrcID = mapIteratorPrcs->second.first.getPrcID();
	sendCommand2Prc(strPrcID, ClsProtocol::cmdConnectToPrcs());
	/* send message to prc: "ConnectToPrcs" */
	waitForPrcState(strPrcID, ClsProtocol::ackConnectedToPrcs(),  ClsInfoPrc::PRC_CONNECTED2PRCS);  // wait for Prc to Prc connections
    }

//     sleep(1); /* hmmm this is unfortunate, but we seem to have to wait until the Prc started ready the socket... */
    /* finally we tell all the Prcs to end waiting for connections */
    for(mapIteratorPrcs = mapPrcs.begin(); mapIteratorPrcs!=mapPrcs.end(); ++mapIteratorPrcs){
	string strPrcID = mapIteratorPrcs->second.first.getPrcID();
	sendCommand2Prc(strPrcID, ClsProtocol::cmdEndWaiting());
	mapIteratorPrcs->second.first.setStatus(ClsInfoPrc::PRC_READY); /* I think it is save to assume this... */
	reportStatus2Parent(strPrcID, ClsInfoPrc::PRC_READY);
    }

};

void ClsFECommHandler::connectToPrcs() {
    COUT << "ClsFECommHandler::connectToPrcs()" << ENDL;
    map<string, pair<ClsInfoPrc, ClsSocketOutTcp> >::iterator mapIteratorPrcs;
// 24032002     map<string, pair<ClsInfoPrc*, ClsBaseSocketOut*> >::iterator mapIteratorPrcs;
    for(mapIteratorPrcs = mapPrcs.begin(); mapIteratorPrcs!=mapPrcs.end(); ++mapIteratorPrcs){
//24032002	  ClsBaseSocketOut *clsSocketOut = mapIteratorPrcs->second.second;
	try{
//24032002	       clsSocketOut->openSocket();
	    mapIteratorPrcs->second.second.openSocket();
	}
	catch (ClsSocketException &e){
	    COUT << e.getMessage() << ENDL;;
	    throw;
	}
    }
}; 

void ClsFECommHandler::setSystemFile(string _strSystemFile) { 
    COUT << "ClsFECommHandler::setSystemFile(string _strSystemFile)" << ENDL;
    strSystemFile = _strSystemFile; 
};

/** 
 * this is the function that actually is threaded off...
 * 2002/08/02 THIS FUNCTION NEEDS A MAJOR OVERHAUL AFTER FOR USING THE NEW PROTOCOL
 */
int ClsFECommHandler::svc(void) { // needed by ACE_Task
    COUT << "ClsFECommHandler::svc(void)" << ENDL;

    bool bError = false;
     
    try{
	startup();
    }
    catch( ClsFECommHandlerException &e){
//	 close(0);
//	 throw;

	bError = true;
	bInternalError = true;
	sendMessage2Parent(" Communication Handler ", ClsInterThreadMessage::MESSAGE_CH_FATAL_ERROR, "read timeout in start up"); 
	
    }//     sleep(2);

    if(!bError){
	 
//	timeval tv_run; // TIMING

	string strText = "" ;
	string strMessageIn = "";
	string strENDL = "";
	int iMessageType;
	string strPrcID;
	string strParameter;

	for(;;){ // NEW FOR ACE TASK
//	  gettimeofday(&tv_run, NULL); COUT << __FILE__ << ":" << __LINE__ << "\t" << tv_run.tv_usec << ENDL; // TIMING
	    if(ACE_Thread_Manager::instance()->testcancel(ACE_Thread::self())){
		COUT << "CANCELING!!!!" << ENDL;
		break;    
	    }

	    iRun ++;

	    strText = "" ;
	    strMessageIn = "";
	    strENDL = "";
	  
	  
	    /* 2002/08/02 new schema:
	       - first we just read the header of a prespecified length (as defined in ClsProtocol.h)
	       - the be read the body of the message */
	  
	  
	    clsSocketIn->readSocket(ClsProtocol::iHeaderLength, iTimeOutRead);
	  
	    /* first off we just read the header of which we know the length */
	    strMessageIn = clsSocketIn->getMessageIn();
	  
	    clsMessageParser.parseHeader(strMessageIn);
	  
	    unsigned short int iBodyLength = clsMessageParser.getBodyLength();
	    if(iBodyLength > 0){
		clsSocketIn->readSocket(iBodyLength, iTimeOutRead);
		strMessageIn = clsSocketIn->getMessageIn();
		clsMessageParser.parseBody(strMessageIn);
	    }


//  
//  ClsProtocol::PROTO_MESSAGE_ACK

/* ---------------------------------------- */


//  	  clsSocketIn->readSocket();


//  	  while(clsSocketIn->size()){
//  	       strMessageIn = clsSocketIn->getMessage();

//  	       clsMessageParser.setMessage(strMessageIn);
//  	       clsMessageParser.parseMessage();
		    
	    iMessageType = clsMessageParser.getType();
	    strPrcID = clsMessageParser.getID();
	    strParameter = clsMessageParser.getParameterName();

	    if(iMessageType==ClsProtocol::PROTO_MESSAGE_ERROR) {
		// Prc -> FE: ERROR
		string strError = clsMessageParser.getBody();
		strText = (string)"error from " + strPrcID + (string)" " + strError + strENDL;	
		sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_ERROR, strText); 
		//  @#@ we should assuem that this is fatal!!!!
	    } else if(iMessageType==ClsProtocol::PROTO_MESSAGE_MESSAGE) {
		//1     Prc -> FE: message
		string strMessage = clsMessageParser.getBody();
			 
		if(iReportingStatus>0){
		    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, "M");
		}
		     
		if(iReportingStatus>1){
		    strText = (string)"mesage from " + strPrcID + (string)" " + strMessage + strENDL;	
		    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, strText); 
		}
	    }  else if(iMessageType==ClsProtocol::PROTO_MESSAGE_ROUNDACK) {
		//1     Prc -> FE: round counter
		string strRound = clsMessageParser.getBody(); /* don't this this will return anything... */
		if(iReportingStatus>1){
		    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, "R");
		}
		    
		if(iReportingStatus>1){
		    strText = (string)"mesage from " + strPrcID + (string)" " + strRound + strENDL;	
		    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, strText); 
		}
		mapPrcs.find(strPrcID)->second.first.setStatus(ClsInfoPrc::PRC_SEND);


		/* NEEDS MAJOR OVER DOING */
	    } else if(iMessageType==ClsProtocol::PROTO_DATA) {
//@@			/* 20010912: 
//@@			   this needs some major reworking for working in the Qt diagram framework 
//@@			*/
//@@			//3     Prc -> PE: data of type XX and size SS, for Group YY
//@@			if(iReportingStatus>1){
//@@			     sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, "DA");
//@@			}
//@@			
//@@			string strData = clsMessageParser.getValue();
//@@			if(iReportingStatus>1){
//@@			     strText =  (string)"from " + strPrcID +  (string)" got " + strParameter  + strENDL;
//@@			     sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, strText); 
//@@			}
//@@			
//@@
//@@			string strTitle = strPrcID + ":" + strParameter;
//@@			/* we check to see if a message for the incoming data exists, if not (the case which is handeled below),
//@@			   - we create a new message
//@@			   - we create a new plot
//@@			   - handover the pointer to the plot to the message
//@@			*/
//@@
//@@			if(mapMessages.find(strTitle) != mapMessages.end()){
//@@			     /* if there is a message, set the data to it */
//@@			     mapMessages.find(strTitle)->second.setData(strData);
//@@			}
//@@			else if(mapMessages.find(strTitle) == mapMessages.end()){
//@@			     /* if there's no message, create it, set the data, create a plot, and set the pointer */
//@@
//@@/*************************************
//@@					  ClsFEMessageIn clsFEMessageIn(strTitle);
//@@					  clsFEMessageIn.setData(strData);
//@@					  ClsFEBasePlot * clsFEPlotTemp = new ClsPlPlot(clsFEMessageIn.xPoints(), clsFEMessageIn.yPoints(), clsFEMessageIn.getID());
//@@					  clsFEMessageIn.setPlotPointer(clsFEPlotTemp);
//@@					  pair<string, ClsFEMessageIn> pairTempMessage(strTitle, clsFEMessageIn); 
//@@					  mapMessages.insert(pairTempMessage);
//@@					  lstPlots.push_back(clsFEPlotTemp);
//@@********************************/
//@@			}
//@@			    
	    } 
	    else {
		COUT << "unknown message type: " << iMessageType << ENDL;
		if(iReportingStatus>1){
		    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, "unknown message");
		}
		    
	    }
//	  gettimeofday(&tv_run, NULL); COUT << __FILE__ << ":" << __LINE__ << "\t" << tv_run.tv_usec << ENDL; // TIMING
//     } // end of if(strMessageIn.size()>0)

	    /* apply() on the message is plotting the data 
	       probably not needed if using Qt plots...
	       map<string, ClsFEMessageIn >::iterator mapIteratorMessages;
	       for(mapIteratorMessages = mapMessages.begin(); mapIteratorMessages!=mapMessages.end(); ++mapIteratorMessages){
	       mapIteratorMessages->second.apply();
	       string strText = string("MessageInID: ") +  mapIteratorMessages->second.getID();
	       sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, strText);	
	       }
	    */
	      
/* sending messages to the Prcs and sending the "go" signal */
	    map<string, pair<ClsInfoPrc, ClsSocketOutTcp> >::iterator mapIteratorPrcs;
	    for(mapIteratorPrcs = mapPrcs.begin(); mapIteratorPrcs!=mapPrcs.end(); ++mapIteratorPrcs){
		string strPrcID = mapIteratorPrcs->first;
//@@		int iStatus = mapIteratorPrcs->second.first->getStatus();
		int iStatus = mapIteratorPrcs->second.first.getStatus();
	       
		if(iReportingStatus>2){
		    strText = (string)"sending to: " + strPrcID;
		    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, strText);
		}
	       
	       
		/* getting the current time */
		timeval tv;
		gettimeofday(&tv,  NULL); 
		int iuSec = tv.tv_usec;
		int iLastUpdateTime = mapIteratorPrcs->second.first.getLastUpdateTime();
		/* we calculate the time difference from the last update */
		int iTimeDiff = abs(iuSec - iLastUpdateTime);
		cerr << "fps: " << 1000000.0 / (double)iTimeDiff << endl;

		/* axioms:
		   - we check for a valid port at very run, as soon as Prcs acked ALIVE
		   - timeouts start counting from the last update
		*/
		if(iStatus != ClsInfoPrc::PRC_DIED){ // not really necessary....
//24032002		    ClsBaseSocketOut *clsSocketOut = mapPrcs.find(strPrcID)->second.second; //NEW 20020308
//24032002		      if(clsSocketOut->socketBrocken() ) {
		    if(mapIteratorPrcs->second.second.socketBrocken() ) {
			string strErrorMsg = (string)"socket broken: " + strPrcID;
			mapIteratorPrcs->second.first.setStatus(ClsInfoPrc::PRC_DIED); // we alternate
//24032002			   mapIteratorPrcs->second.second->closeSocket(); /* we need to close the ports, so that it can 
//24032002									     be reopened for a reconnecting Prc */
			mapIteratorPrcs->second.second.closeSocket(); /* we need to close the ports, so that it can 
									 be reopened for a reconnecting Prc */
			reportStatus2Parent(strPrcID, ClsInfoPrc::PRC_DIED); 
//		    mapPrcs.erase(mapIteratorPrcs);
			string strText = (string)"BROKEN PIPE for Prc: " + strPrcID;
			sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_ERROR, strText);	
		    } else { /* the socket in OK */
			if(iStatus == ClsInfoPrc::PRC_READY || iStatus == ClsInfoPrc::PRC_SEND){ /* Prcs reacted */

			      
			    map<string, ClsFEMessageOut >::iterator mapIteratorMessageOut;
			    for (mapIteratorMessageOut = mapMessagesOut.lower_bound(strPrcID); 
				 mapIteratorMessageOut != mapMessagesOut.upper_bound(strPrcID); 
				 ++mapIteratorMessageOut){
//				   *clsSocketOut <<  mapIteratorMessageOut->second.getMessage() << ENDL;
//24032002				   clsSocketOut->sendStream(mapIteratorMessageOut->second.getMessage());
				mapIteratorPrcs->second.second.sendStream(mapIteratorMessageOut->second.getMessage());
				mapMessagesOut.erase(mapIteratorMessageOut);
			    }
			      
/* temporarily diabled
   to do:
   - go through queueMessagesOut
   - send message
   - create new ClsFEMessageIn
   while(!queueMessagesOut.empty()){
   *clsSocketOut << queueMessagesOut.front() << ENDL;
   queueMessagesOut.pop();
   }
*/

			    ClsFEMessageOut clsFEMessageOut ( ClsProtocol::PROTO_CMD_GO, strPrcID, "", "" );
			    mapIteratorPrcs->second.second.sendStream(clsFEMessageOut.getMessage());
			    if(iReportingStatus>2){
				string strText = (string)"sendt: go to " + strPrcID;
				sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, strText);	
			    }

			    mapIteratorPrcs->second.first.setStatus(ClsInfoPrc::PRC_SLEEP); // we alternate
			      
			    /* set the time of the last update to the InfoPrc (used for timeouts) */
			    timeval tv; gettimeofday(&tv,  NULL); 
			    mapIteratorPrcs->second.first.setLastUpdateTime(tv.tv_usec); // 
			      
			} else if (iStatus == ClsInfoPrc::PRC_SLEEP && iTimeDiff>(iTimeOutSec*1000000)) { /* Prcs has been 
													     updated (i.e. PRC_SLEEP), 
													     we wait for a reaction */
			    /* checking for timeouts 
			       right now Prcs that have a timeout are being removed... this isn't absolutely necessary...
			    */
			    mapIteratorPrcs->second.first.setStatus(ClsInfoPrc::PRC_DIED);
			    reportStatus2Parent(strPrcID, ClsInfoPrc::PRC_DIED); 
			    if(iReportingStatus>-1){
				string strText = "TIME OUT";
				sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, strText);	
			    }
			    /* in the case of a timeout also, we can or cannot assume, that the Prc has died... The best solution would actually be, to  
			       check the PID of the Prc, and kill it if it is still alive... 
			       to be fixed later */
			}
		    }
		}

		/* if we work based on a prespecified list of Prcs, dead Prcs should not be removed from the list, 
		   if(mapIteratorPrcs->second.first->getStatus() == ClsInfoPrc::PRC_DIED){
		   other wise a reconnecting is not possible... 
		   mapPrcs.erase(mapIteratorPrcs); 
		   } */
//	  gettimeofday(&tv_run, NULL); COUT << __FILE__ << ":" << __LINE__ << "\t" << tv_run.tv_usec << ENDL; // TIMING
	    } // end of for(mapIteratorPrcs = mapPrcs.begin(); mapIteratorPrcs!=mapPrcs.end(); ++mapIteratorPrcs){
//---------------------------------------------------
//	  gettimeofday(&tv_run, NULL); COUT << __FILE__ << ":" << __LINE__ << "\t" << tv_run.tv_usec << ENDL; // TIMING
//	  usleep(1000);
	} // END OF ETERNAL FOR LOOP
    }
    return 0;
};




int ClsFECommHandler::close(u_long) {
    COUT << "ClsFECommHandler::close(u_long)" << ENDL;

/* 
   - send stop signal to all Prcs!
   - remove all Prcs from the list of receivers
*/
 


       ClsFEMessageOut clsFEMessageOut ( ClsProtocol::PROTO_CMD_CMD, "", "", ClsProtocol::cmdStop() );


       map<string, pair<ClsInfoPrc, ClsSocketOutTcp> >::iterator mapIteratorPrcs;
       for(mapIteratorPrcs = mapPrcs.begin(); mapIteratorPrcs!=mapPrcs.end(); ++mapIteratorPrcs){

// @#@ this crashed some times...
//	   mapIteratorPrcs->second.second.sendStream(clsFEMessageOut.getMessage()); // send stop
//  	  mapIteratorPrcs->second.second.closeSocket();     // close socket

       } 

       mapPrcs.clear(); // clear the map, as we'll refill it at a new launch
       closeSocketIn(); 

       /* clear the flag on the mutex, so SimControl knows we're done closing 
	BUT: we should only do this, if the call to close came from the outside!
	Otherwise the whole thing (like: SimControl) will collapse */
       if(!bInternalError){
	   ace_condReady->signal();  
       }
       
       return 0;
       
};


/* 
 * helper functions 
 * 
 */


void ClsFECommHandler::createPrcMap() {
    COUT << "ClsFECommHandler::createPrcMap()" << ENDL;

//+++    map<string, ClsInfoPrc> mapProcesses =  clsFESystemManager->getMapProcesses();
//+++    map<string, ClsInfoPrc>::iterator mapIteratorProcesses;
//+++    for(mapIteratorProcesses = mapProcesses.begin(); mapIteratorProcesses!=mapProcesses.end(); ++mapIteratorProcesses){
//+++	ClsSocketOutTcp clsSocketOut ( mapIteratorProcesses->second.getHostname(),  mapIteratorProcesses->second.getPort(), ios::out);
//+++	ClsInfoPrc clsInfoPrcTemp = mapIteratorProcesses->second;
//+++	pair<ClsInfoPrc, ClsSocketOutTcp> pairTemp( clsInfoPrcTemp, clsSocketOut);
//+++	pair<string, pair<ClsInfoPrc, ClsSocketOutTcp> > pairTemp2(mapIteratorProcesses->first, pairTemp);
//+++	mapPrcs.insert(pairTemp2);
//+++    }
//+++

    map<string, ClsFEProcess*> mapProcesses =  clsFESystemManager->getMapFEProcesses();
    map<string, ClsFEProcess*>::iterator mapIteratorProcesses;
    for(mapIteratorProcesses = mapProcesses.begin(); mapIteratorProcesses!=mapProcesses.end(); ++mapIteratorProcesses){
	ClsSocketOutTcp clsSocketOut ( mapIteratorProcesses->second->getHostname(),  mapIteratorProcesses->second->getPort(), ios::out);

	ClsInfoPrc clsInfoPrcTemp;
	clsInfoPrcTemp.setPrcID(mapIteratorProcesses->second->getProcessID());
	clsInfoPrcTemp.setHostname( mapIteratorProcesses->second->getHostname());
	clsInfoPrcTemp.setPort( mapIteratorProcesses->second->getPort());
	

	pair<ClsInfoPrc, ClsSocketOutTcp> pairTemp( clsInfoPrcTemp, clsSocketOut);
	pair<string, pair<ClsInfoPrc, ClsSocketOutTcp> > pairTemp2(mapIteratorProcesses->first, pairTemp);
	mapPrcs.insert(pairTemp2);
    }



};

void ClsFECommHandler::checkForPrcState(string strPrcMessage, int iState) { 
    COUT << "ClsFECommHandler::checkForPrcState(string strPrcMessage, int iState)" << ENDL;

    string strText;
    bool fDone = false;
    while(!fDone){

	COUT << __FILE__ << ":" << __LINE__ << ":iTimeOutRead:: " << iTimeOutRead << ENDL;
	ssize_t  r_bytes = clsSocketIn->readSocket(ClsProtocol::iHeaderLength, iTimeOutRead);
	COUT << __FILE__ << ":" << __LINE__ << ":ClsProtocol::iHeaderLength: " << ClsProtocol::iHeaderLength << ENDL;


 	if(r_bytes <= 0){
 	    COUT << "WE HAVE A TIMEOUT" << endl;
 	    ClsFECommHandlerException clsFECommHandlerException(ClsFECommHandlerException:: SOCKET_TIMEOUT_READ, "");
 	    throw clsFECommHandlerException;   
 	} 

	/* first off we just read the header of which we know the length */
	string strMessageIn = clsSocketIn->getMessageIn();

	COUT << __FILE__ << ":" << __LINE__ << ":" << strMessageIn  << "|"<< ENDL;
	COUT << __FILE__ << ":" << __LINE__ << ":strMessageIn.length(): " << strMessageIn.length()  << ENDL;
	   
	clsMessageParser.parseHeader(strMessageIn);
  	  
	/* then we can read the body (which is of variable length(*/
	unsigned short int iBodyLength = clsMessageParser.getBodyLength();  
 	  COUT << __FILE__ << ":" << __LINE__ << ":iBodyLength: " << iBodyLength << ENDL;
	if(iBodyLength > 0){
	    clsSocketIn->readSocket(iBodyLength, iTimeOutRead);
	    strMessageIn = clsSocketIn->getMessageIn();
	    clsMessageParser.parseBody(strMessageIn);
	    COUT << clsMessageParser.getValue() << ENDL;
	}
	   
	int iMessageType = clsMessageParser.getType();
	
	string strPrcID = clsMessageParser.getID();

 

 	  COUT << __FILE__ << ":" << __LINE__ << ":clsMessageParser.getValue(): " << clsMessageParser.getValue() << ENDL;
 	  COUT << __FILE__ << ":" << __LINE__ << ":clsMessageParser.getBody(): " << clsMessageParser.getBody() << "*" <<  ENDL;
 	  COUT << __FILE__ << ":" << __LINE__ << ":strPrcMessage: " << strPrcMessage << ENDL;
	  
 	  COUT << __FILE__ << ":" << __LINE__ << "strPrcMessage.compare(clsMessageParser.getBody()): " << strPrcMessage.compare(clsMessageParser.getBody()) << ENDL;

	if(iMessageType==ClsProtocol::PROTO_MESSAGE_ACK){
	    COUT << "\tACK" << ENDL;
	    if(!strPrcMessage.compare(clsMessageParser.getBody())){ 
		COUT << "checkForPrcState: matching states:" << ENDL;
		fDone = true;


		    if(mapPrcs.find(strPrcID)== mapPrcs.end()){
			COUT << "checkForPrcState: Prc not found in map!" << ENDL;
		    } else {
			COUT << "checkForPrcState: Prc FOUND in map!" << ENDL;
			mapPrcs.find(strPrcID)->second.first.setStatus(iState);
		    }
		

		strText="Prc acknowledged: " + strPrcMessage;
		if(iReportingStatus>0){
		    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, strText);
		}
	    }
	    else {
		COUT << "checkForPrcState: non matching states:" << strPrcMessage <<"!=" << clsMessageParser.getBody() << ENDL;
	    }
	    
	} else if(iMessageType==ClsProtocol::PROTO_MESSAGE_ERROR){
	    string strError = (string)"Error: " + clsMessageParser.getBody() + (string)" in: " + strPrcID;
	    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_ERROR, strError); 
	}
    }
};

void ClsFECommHandler::waitForPrcState(string _strPrcID, string strPrcMessage, int iState) { 
    COUT << "ClsFECommHandler::waitForPrcState(string _strPrcID, string strPrcMessage, int iState)" << ENDL;
    COUT << "\tstrPrcMessage: " << strPrcMessage << ENDL;
    bool fDone = false;
    string strMessageIn;
    string strText;
    while(!fDone){
	clsSocketIn->readSocket(ClsProtocol::iHeaderLength, iTimeOutRead);
	  
	/* first off we just read the header of which we know the length */
	strMessageIn = clsSocketIn->getMessageIn();
	  
	clsMessageParser.parseHeader(strMessageIn);
	  
	/* then we can read the body (which is of variable length(*/
	unsigned short int iBodyLength = clsMessageParser.getBodyLength();
	if(iBodyLength > 0){
	    clsSocketIn->readSocket(iBodyLength, iTimeOutRead);
	    strMessageIn = clsSocketIn->getMessageIn();
	    clsMessageParser.parseBody(strMessageIn);
	    COUT << clsMessageParser.getValue() << ENDL;
	}
	  
	int iMessageType = clsMessageParser.getType();
	string strPrcID = clsMessageParser.getID();

	if(iMessageType==ClsProtocol::PROTO_MESSAGE_ACK && !strPrcID.compare(_strPrcID)){
	    if(!strPrcMessage.compare(clsMessageParser.getBody())){ 
		fDone = true;
		mapPrcs.find(strPrcID)->second.first.setStatus(iState); 
		strText="Prc acknowledged: " + strPrcMessage;
		if(iReportingStatus>0){
		    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_PROGRESS, strText);
		}
		reportStatus2Parent(strPrcID, iState);
	    }
	} else if(iMessageType==ClsProtocol::PROTO_MESSAGE_ERROR){
	    string strError = (string)"Error: " + clsMessageParser.getBody() + (string)" in: " + strPrcID;
	    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_ERROR, strError); 
	}
    }
};

bool ClsFECommHandler::allPrcsInState(int iState) {
    COUT << "ClsFECommHandler::allPrcsInState" << ENDL;

    bool fStateAchieved = true;

/* now lets see if all prcs are in the desired state */
//24032002     map<string, pair<ClsInfoPrc*, ClsBaseSocketOut*> >::iterator mapIteratorPrcs;
    map<string, pair<ClsInfoPrc, ClsSocketOutTcp> >::iterator mapIteratorPrcs;

//     COUT << "mapsize: " << mapPrcs.size() << ENDL;
//     COUT << "looking for state: " <<iState << ENDL;
    for(mapIteratorPrcs = mapPrcs.begin(); mapIteratorPrcs!=mapPrcs.end(); ++mapIteratorPrcs){
// 	COUT << "state found: " << mapIteratorPrcs->second.first->getStatus() << ENDL; 
	                                       
	if(mapIteratorPrcs->second.first.getStatus() !=  iState){
	    fStateAchieved = false;
	    break;
	} 
    }
    return fStateAchieved;
};


void ClsFECommHandler::sendCommand2Prc(string strPrcID, string _strMessage) {
    COUT << "ClsFECommHandler::sendCommand2Prc(string strPrcID, string strMessage)" << ENDL;
     
    ClsFEMessageOut clsFEMessageOut ( ClsProtocol::PROTO_CMD_CMD, strPrcID, "", _strMessage );

    COUT << "\t\tMessage to Prc: " << _strMessage << ENDL;


    if(!mapPrcs.find(strPrcID)->second.second.socketBrocken()) {
	mapPrcs.find(strPrcID)->second.second.sendStream( clsFEMessageOut.getMessage());
	COUT << "SENDING: " << clsFEMessageOut.getMessage() << ENDL;
    } else {
	ClsSocketException clsSocketException(ClsSocketException::SOCKET_BROKEN);
	COUT << "NOT SENDING" << ENDL;
	throw clsSocketException; // don't think this should be as radical as throwin an exception (?)
    }
};

//  void ClsFECommHandler::sendSystemFile2Prc(string strPrcID) {
//       COUT << "ClsFECommHandler::sendCommand2Prc(string strPrcID, string strMessage)" << ENDL;
//       string strMessage = ClsProtocol::msgSystemFile();
//       strMessage.append("|");
//       strMessage.append(strSystemFile);
//  //     COUT << "MESSAGE TO PRC: " << strMessage << ENDL;
//       COUT << "WE'RE GETTING HERE: " << __LINE__ << "mapPrcs.size(): " << mapPrcs.size() << ENDL;

//       if(mapPrcs.find(strPrcID)!=mapPrcs.end()){
//  //24032002	  ClsBaseSocketOut *clsSocketOutTemp = mapPrcs.find(strPrcID)->second.second;
//  	  COUT << "WE'RE GETTING HERE: " << __LINE__ << " brokensocket:" << mapPrcs.find(strPrcID)->second.second.socketBrocken() << ENDL;
//  	  if(!mapPrcs.find(strPrcID)->second.second.socketBrocken()) {
//  	        mapPrcs.find(strPrcID)->second.second.sendStream( strMessage);
//  	       COUT << "WE'RE GETTING HERE: " << __LINE__ << ENDL;
//  	  } else {
//  	       COUT << "WE'RE GETTING HERE: " << __LINE__ << ENDL;
//  	       ClsSocketException clsSocketException(ClsSocketException::SOCKET_BROKEN);
//  	       COUT << "WE'RE GETTING HERE: " << __LINE__ << ENDL;
//  	       throw clsSocketException;
//  	  }
//  	  COUT << "WE'RE GETTING HERE: " << __LINE__ << ENDL;
//       }
//       else {
//  	  COUT << "PRC NOT FOUND" << ENDL;
//       }
//  };

void ClsFECommHandler::sendSystemFile2Prc(string strPrcID) {
    COUT << "ClsFECommHandler::sendCommand2Prc(string strPrcID, string strMessage)" << ENDL;


    ClsFEMessageOut clsFEMessageOut ( ClsProtocol::PROTO_CMD_SYSTEMFILE, strPrcID, "", strSystemFile );


    if(mapPrcs.find(strPrcID)!=mapPrcs.end()){
	if(!mapPrcs.find(strPrcID)->second.second.socketBrocken()) {
	    mapPrcs.find(strPrcID)->second.second.sendStream( clsFEMessageOut.getMessage() );
	} else {
	    ClsSocketException clsSocketException(ClsSocketException::SOCKET_BROKEN);
	    throw clsSocketException;
	}
    }
    else {
	COUT << "PRC NOT FOUND" << ENDL;
    }
};


int ClsFECommHandler::sendMessage2Parent(string strPrcID, int iType, string strText){
    COUT << "ClsFECommHandler::sendMessage2Parent(string strText)" << ENDL;

/* format: 
   <e|p|s>|<ID>|<text>
*/

/* Old variable length messing protocoll
    string strMessage = "";

    switch(iType){
    case ClsInterThreadMessage::MESSAGE_PRC_ERROR:
	strMessage = ClsFECommHandler::prefixMessagePrcError();
	break;
    case ClsInterThreadMessage::MESSAGE_PRC_PROGRESS:
	strMessage = ClsFECommHandler::prefixMessagePrcProgress();
	break;
    case ClsInterThreadMessage::MESSAGE_PRC_STATUS:
	strMessage = ClsFECommHandler::prefixMessagePrcStatus();
	break;
    case ClsInterThreadMessage::MESSAGE_CH_FATAL_ERROR:
	strMessage = ClsFECommHandler::prefixMessageCommHandlerFatalError();
	break;
    default:
	break;
    }
     
    strMessage.append("|");
    strMessage.append(strPrcID);
    strMessage.append("|");
    strMessage.append(strText);
    strMessage.append("|");
     */



    ClsInterThreadMessage clsInterThreadMessage;
    clsInterThreadMessage.setType( iType );
    clsInterThreadMessage.setPrcID(strPrcID );
    clsInterThreadMessage.setContents( strText );
    string strMessage = clsInterThreadMessage.getMessage();    

    

    ACE_Time_Value timeout (0); 
    ACE_Message_Block *mb;

    ACE_NEW_RETURN (mb, ACE_Message_Block (BUFSIZ + 1), -1);
    ACE_OS::sprintf(mb->wr_ptr(), strMessage.c_str());
     
    mb->wr_ptr(ACE_OS::strlen(strMessage.c_str()));
    cout << "ERROR MESSAGE: " << strMessage << endl;
     
    if (putq(mb, &timeout) == -1){
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "put_next"));
    }
    return 0;
};


int ClsFECommHandler::reportStatus2Parent(string strPrcID, int iState){
    sendMessage2Parent(strPrcID, ClsInterThreadMessage::MESSAGE_PRC_STATUS, iqrUtils::int2string(iState));
    return 0;
    
};

//////////////////////////////////

ostream &operator<<(ostream &stream, ClsFEMessageOut clsFEMessageOut) {
    COUT << "MessageType:\t" << clsFEMessageOut.iMessageType << ENDL;
    COUT << "ID:\t\t" << 	    clsFEMessageOut.strID << ENDL;    
    COUT << "ParamName:\t" <<   clsFEMessageOut.strParamName << ENDL;
    return stream;
};

ostream &operator<<(ostream &stream, ClsFEMessageOut *clsFEMessageOut) {
    COUT << "MessageType:\t" << clsFEMessageOut->iMessageType << ENDL;
    COUT << "ID:\t\t" << 	    clsFEMessageOut->strID << ENDL;    
    COUT << "ParamName:\t" <<   clsFEMessageOut->strParamName << ENDL;
    return stream;
};




/// Local Variables: 
/// mode: c++
/// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ && make -k"
/// End: 

