/****************************************************************************
 ** $Filename: ClsPortFinder.h
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Oct 17 22:03:22 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSPORTFINDER_H
#define CLSPORTFINDER_H    /*+ To stop multiple inclusions. +*/

#include <arpa/inet.h>
#include <errno.h>     
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <map>


#include "ClsPortFinderException.h"
class ClsPortFinder {

public:
     ClsPortFinder() { 
	  iPortLowFix = 49154;
	  iPortHiFix = 65535;
     };
     
/** 
 * set the range of ports to scan, default is 49152 ... 65535
 * @param _iPortLowFix lower search boundary
 * @param _iPortHiFix upper search boundary
 */
     void setScanRange(int _iPortLowFix, int _iPortHiFix) {
	  iPortLowFix = _iPortLowFix;
	  iPortHiFix = _iPortHiFix;
     };
     
//       void getPortByHostName(string strHostName) {
//  	  struct hostent *hostinfo;
//  	  struct in_addr addr_in;
//  	  hostinfo = gethostbyname (strHostName.c_str());
//  	  if (hostinfo == NULL) {
//  	       ClsPortFinderException clsPortFinderException(ClsPortFinderException::UNKNOWN_HOST);
//  	       throw clsPortFinderException;
//  	  }
//  	  /* converting the hostname to in_addr */
//  	  addr_in = *(struct in_addr *) hostinfo->h_addr_list[0];

//  	  cout << inet_ntoa ( addr_in)  << endl;

//  	  loopPorts(addr_in );
//       };

/** 
 * Get a port on a host
 * @return port found
 * @param strHostName Name of the host
 */
     int getPortByHostName(string strHostName) {
	  int iPort = -99;
	  struct hostent *hostinfo;
	  struct in_addr addr_in;
	  hostinfo = gethostbyname (strHostName.c_str());
	  if (hostinfo == NULL) {
	       ClsPortFinderException clsPortFinderException(ClsPortFinderException::UNKNOWN_HOST, 
							     strHostName);
	       throw clsPortFinderException;
	  }
  	  addr_in = *(struct in_addr *) hostinfo->h_addr_list[0];
	  string strHostIP = inet_ntoa ( addr_in);
	  iPort = getPortByHostIP (strHostIP);
	  return iPort;
     };
     
/** 
 * Get a port on a host
 * @return port found
 * @param strHostIP IP of the host
 */
    int getPortByHostIP(string strHostIP) {
	  struct in_addr addr_in;
	  /* converting the IP to in_addr works only for  IPv4 ! */
	  if( (inet_aton (strHostIP.c_str(),  &addr_in)) < 1){
	       ClsPortFinderException clsPortFinderException(ClsPortFinderException::WRONG_IP_FORMAT,
		    strHostIP);
	       throw clsPortFinderException;
	  }
//	  loopPorts(addr_in );

	  int iPortLowTemp;
    	  if(mapHosts.find(strHostIP) != mapHosts.end()){
    	       iPortLowTemp = mapHosts.find(strHostIP)->second +1;
    	  } else {
  	       iPortLowTemp = iPortLowFix;
  	  }

	  
	  for(int iCurrentPort = iPortLowTemp; iCurrentPort <= iPortHiFix; iCurrentPort++){
	       if(scanPorts(iCurrentPort,addr_in)){
  		    if(mapHosts.find(strHostIP) != mapHosts.end()){
  			 mapHosts.find(strHostIP)->second = iCurrentPort;
  		    } else {
  			 pair<string, int> pairTemp(strHostIP, iCurrentPort);
  			 mapHosts.insert(pairTemp);
  		    }
	       }
	       return iCurrentPort;
	  } 
	  ClsPortFinderException clsPortFinderException(ClsPortFinderException::MAX_PORT_REACHED,"");
	  throw clsPortFinderException;
     };
     
private:
     struct sockaddr_in sock_addr;
//     map<struct in_addr, int> mapHosts;
     map<string, int> mapHosts;
     int iPortLowFix;
     int iPortHiFix;

//       void loopPorts (struct in_addr addr_in) {
//  	  int iPortLowTemp;
//  //    	  if(mapHosts.find(addr_in) != mapHosts.end()){
//  //    	       iPortLowTemp = mapHosts.find(addr_in)->second;
//  //    	  } else {
//  //  	       iPortLowTemp = iPortLowFix;
//  //  	  }
	  
//  	  for(int iCurrentPort = iPortLowTemp; iCurrentPort <= iPortHiFix; iCurrentPort++){
//  	       if(scanPorts(iCurrentPort,addr_in)){
//  //  		    if(mapHosts.find(addr_in) != mapHosts.end()){
//  //  			 mapHosts.find(addr_in)->second = iCurrentPort;
//  //  		    } else {
//  //  			 pair<struct in_addr, int> pairTemp(addr_in, iCurrentPort);
//  //  			 mapHosts.insert(pairTemp);
//  //  		    }
//  		    break;
//  //		    cout << "port free: " << iCurrentPort << endl;
//  	       } 
//  //	       else {
//  //		    cout << "\tport used1: " << iCurrentPort << endl;
//  //	       }
//  	  }
//  	  ClsPortFinderException clsPortFinderException(ClsPortFinderException::MAX_PORT_REACHED);
//  	  throw clsPortFinderException;
//       };
     

     int scanPorts(int port, struct in_addr _addr_in){
	  struct sockaddr_in sock_addrTemp;
	  int sockT = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


	  /* ok... after all the hassle with this it doesn't do the job... */
	     struct linger l;
	     l.l_linger = 10;
	     l.l_onoff = -1;
	     if((setsockopt (sockT, SOL_SOCKET, SO_LINGER , &l, sizeof(l))) < 0)
	     cout << strerror(errno) << endl;
	     /* */

	  sock_addrTemp.sin_port = htons(port);
	  sock_addrTemp.sin_family = PF_INET;
	  sock_addrTemp.sin_addr = _addr_in;

	  if(connect(sockT,(struct sockaddr *) &sock_addrTemp, sizeof(sock_addrTemp)) == 0){
	       close(sockT);
	       return 0;
	  } else {
	       if(errno!=ECONNREFUSED){
		    string strHostIP = inet_ntoa ( _addr_in);		    
		    ClsPortFinderException clsPortFinderException(0, errno, strHostIP);
		    throw clsPortFinderException;
	       }
	  }
	  return -1;
     };
     
};

#endif /* CLSPORTFINDER_H */


//// Local Variables: 
//// mode: c++
//// End: 

