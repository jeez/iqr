#include <string>
#include <list>

#include "ClsPortFinder.h"


int main() {
     string strHostName = "zonk.ini.unizh.ch";
     string strHostIP = "192.168.0.2";
     int iPort = -99;

     ClsPortFinder clsPortFinder;
//     clsPortFinder.setScanRange(100,120);
     clsPortFinder.setScanRange(49152,65534);


     list<string> lstHostNames;
     lstHostNames.push_back("ggbii");
     lstHostNames.push_back("zonk.ini.unizh.ch");
     lstHostNames.push_back("ulysses1121");
     lstHostNames.push_back("localhost");
     lstHostNames.push_back("zonk");
     lstHostNames.push_back("localhostsdsd");
     lstHostNames.push_back("localhost");

     
     while (lstHostNames.size())
     {
	  strHostName = lstHostNames.front();
	  try{
	       iPort = clsPortFinder.getPortByHostName(strHostName);
	  }
	  catch(ClsPortFinderException& e) {
	       cout << e.getMessage() << endl;
	       iPort = -99;
	  }
	  cout << "HostName: " << strHostName << " gets: " << iPort << endl;
	  lstHostNames.pop_front();
     }

     cout << "===============" << endl;

     list<string> lstHostIPs;
     lstHostIPs.push_back("127.0.0.1");
     lstHostIPs.push_back("192.168.0.2");
     lstHostIPs.push_back("192.168.0.2");
     lstHostIPs.push_back("localhost");
     lstHostIPs.push_back("192.168.0.1");
     lstHostIPs.push_back("192.168.0.2");

     
     while (lstHostIPs.size())
     {
	  strHostIP = lstHostIPs.front();
	  try{
	       iPort = clsPortFinder.getPortByHostIP(strHostIP);
	  }
	  catch(ClsPortFinderException& e) {
	       cout << e.getMessage() << endl;
	       iPort = -99;
	  }
	  cout << "HostIP: " << strHostIP << " gets: " << iPort << endl;
	  lstHostIPs.pop_front();
     }

     
};


 

