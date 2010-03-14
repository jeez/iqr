/****************************************************************************
 ** $Filename: ClsFESystemBuilder.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 *****************************************************************************/


#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif

#include <qfileinfo.h>     

#include "ClsFESystemBuilder.h"
#include "ClsFESystemManager.h"

//#define DEBUG_CLSFESYSTEMBUILDER

//--#ifndef COUT
//--#define COUT cout << "[01;31m"
//--#endif
//--
//--#ifndef ENDL
//--#define ENDL "[00m" << endl
//--#endif

ClsFESystemBuilder::ClsFESystemBuilder(){ };

void ClsFESystemBuilder::readSystemFileFromDisk(string _strSystemFileName) {
#ifdef DEBUG_CLSFESYSTEMBUILDER
    cout << "ClsFESystemBuilder::readSystemFileFromDisk(string _strSystemFileName)" << ENDL;
    cout << "_strSystemFileName: " << _strSystemFileName << ENDL;
#endif
    strSystemFileCont = readFileFromDisk(_strSystemFileName);
};



string ClsFESystemBuilder::readFileFromDisk(string _strFileName) {
#ifdef DEBUG_CLSFESYSTEMBUILDER
    cout << "ClsFESystemBuilder::readFileFromDisk(string _strFileName)" << endl;
    cout << "_strFileName: " << _strFileName << endl;
#endif
    ifstream ifstr;
    ifstr.open(_strFileName.c_str(), ios::in); // @#@ chatch an open exception here...
    ostringstream os2;
    string strSF;
    os2 << ifstr.rdbuf();
    return os2.str();
};



void ClsFESystemBuilder::importProcess(string _strFileName, bool bEmitSignals, bool bLink){
#ifdef DEBUG_CLSFESYSTEMBUILDER
    cout << "ClsFESystemBuilder::importProcess(string _strFileName)" << endl;
#endif
    string strProcessCont = readFileFromDisk(_strFileName);
    importFragment(strProcessCont, false, bEmitSignals, (bLink ? _strFileName : ""));
    
}

void ClsFESystemBuilder::parseSystemFile( ) {
#ifdef DEBUG_CLSFESYSTEMBUILDER
    cout << "ClsFESystemBuilder::parseSystemFile( )" << endl;
#endif

//	int iError = 0;

    clsSysFileParser.setInputBuffer( strSystemFileCont );

    try {
	clsSysFileParser.XMLPlatformUtilsInitialize();
    }
    catch (ClsSysFileParserException &e){
	throw;
//	    cout << e.getMessage() << endl;
//	    iError = -1;
//	    string strError = (string)"Error in system file parser\n" + e.getMessage() ;
//	    QMessageBox::warning( this, "SimControl", strError.c_str(), "Quit", 0 );
    }

    try {
	clsSysFileParser.parseBuffer(false); /* arg: validate */
    }
    catch (ClsSysFileParserException &e){
	throw;
//	    cout << e.getMessage() << endl;
//	    iError = -1;
//	    string strError = (string)"Error reading system file\n" + e.getMessage() ;
//	    QMessageBox::warning( this, "SimControl", strError.c_str(), "Quit", 0 );
    }
    mapDiagramLineInternal= clsSysFileParser.getMapDiagramLine();
    mapDiagramIconInternal = clsSysFileParser.getMapDiagramIcon();


};

void ClsFESystemBuilder::buildSystem(){
#ifdef DEBUG_CLSFESYSTEMBUILDER
    cout << "ClsFESystemBuilder::buildSystem( )" << endl;
#endif

    ClsInfoSystem clsInfoSystem;
    clsInfoSystem = clsSysFileParser.getSystemInfo();
    ClsFESystemManager::Instance()->setInfoSystem( clsInfoSystem);

    list<ClsInfoPrc> lstPrcs = clsSysFileParser.getListProcesses();
    list<ClsInfoPrc>::iterator itlstPrcs;
    for (itlstPrcs = lstPrcs.begin(); itlstPrcs != lstPrcs.end(); ++itlstPrcs){
	string strProcessPath = itlstPrcs->getPath();
	
	if(strProcessPath.length()>0){
	    lstExternalPathsProcesses.push_back(strProcessPath);
	} else {
	    string strPrcName = itlstPrcs->getProcessName();
	    string strPrcID = itlstPrcs->getPrcID();
	    
	    
#ifdef DEBUG_CLSFESYSTEMBUILDER
	    cout << "ClsFESystemBuilder::build process:" << strPrcID << endl;
#endif
	    ClsFESystemManager::Instance()->emitLoadProgress("Adding Process: " + strPrcName);
	    ClsFESystemManager::Instance()->addProcess(*itlstPrcs);
	    
	    try {
		clsSysFileParser.createPrcDOMTree( strPrcID);
	    }
	    catch (ClsSysFileParserException &e){
		throw;
	    }
	    
	    list<ClsInfoGroup> lstGroups = clsSysFileParser.getListLocalGroups( );
	    list<ClsInfoGroup>::iterator itlstGroups;
	    for (itlstGroups = lstGroups.begin(); itlstGroups != lstGroups.end(); ++itlstGroups){
		itlstGroups->setProcessID(strPrcID);
		string strGroupID = itlstGroups->getGroupID().c_str();
		string strGroupName =itlstGroups->getGroupName().c_str();
#ifdef DEBUG_CLSFESYSTEMBUILDER
		cout << "ClsFESystemBuilder::build group:" << strGroupID << endl;
#endif
		ClsFESystemManager::Instance()->emitLoadProgress("Adding Group: " + strGroupName);
		ClsFESystemManager::Instance()->addGroup(*itlstGroups);
	    }
	}
    }




/* FIRST WE HAVE TO GET THE INFO ABOUT THE LOCAL CONNECTIONS FROM THE PARSER; WE'LL ONLY USE THIS AFTER THE EXTERNAL
   STUFF HAS BEEN HANDLED...*/
    list<ClsInfoConnection> lstConnections = clsSysFileParser.getListConnection( );

/* NOW TO THE EXTERNAL STUFF */
    list<string>::iterator it;
    for(it=lstExternalPathsProcesses.begin(); it!=lstExternalPathsProcesses.end();it++){
 	string strPath = (*it);
 	/* CHECK PATH HERE */
	QFile qf(strPath.c_str());
	if(!qf.exists()){
	    string strError = "\n\"" + strPath + "\"";
	    ClsFESystemBuilderException clsFESystemBuilderException(ClsFESystemBuilderException::IMPORT_FRAGMENT_NOT_FOUND, strError);
	    throw clsFESystemBuilderException;
	} else {
	    importProcess(strPath, false, true);
	}
     }

/* AND NOW THE INTERNAL */
    list<ClsInfoConnection>::iterator itlstConnections;
    for (itlstConnections = lstConnections.begin(); itlstConnections != lstConnections.end(); ++itlstConnections){
	string strConnectionID = itlstConnections->getConnectionID( );
#ifdef DEBUG_CLSFESYSTEMBUILDER
	cout << "ClsFESystemBuilder::build connection:" << strConnectionID<< endl;
#endif
	ClsFESystemManager::Instance()->emitLoadProgress("Adding Connection: " + strConnectionID);

	bool bError = false;
	string strConnectionTargetID = itlstConnections->getConnectionTargetID( );
	string strConnectionSourceID = itlstConnections->getConnectionSourceID( );


	if(ClsFESystemManager::Instance()->getFEGroup(strConnectionSourceID) == NULL){
	    bError = true;
	    cerr << "Connection source does not exist" << endl;
	    string strError = string("Connection: ") + strConnectionID + string("\nsource group does not exist\nskipping connection");
//	    ClsFESystemBuilderException clsFESystemBuilderException(ClsFESystemBuilderException::MESSAGE, strError);
//	    throw clsFESystemBuilderException;
	    QMessageBox::warning( NULL, "iqr", strError.c_str(), "Ok", 0 );
	}

	if(ClsFESystemManager::Instance()->getFEGroup(strConnectionTargetID) == NULL){
	    bError = true;
	    cerr << "connection target does not exist" << endl;
	    string strError = string("Connection: ") + strConnectionID + string("\ntarget group does not exist\nskipping connection");
//	    ClsFESystemBuilderException clsFESystemBuilderException(ClsFESystemBuilderException::MESSAGE, strError);
//	    throw clsFESystemBuilderException;
	    QMessageBox::warning( NULL, "iqr", strError.c_str(), "Ok", 0 );
	}


	if(!bError){
	    ClsFESystemManager::Instance()->addConnection(*itlstConnections);
	}
    }

/* cleanup */
    lstExternalPathsProcesses.clear();
    


};

/* --------------------- */ 
void ClsFESystemBuilder::importFragment(string strCont, bool bValidate, bool bEmitSignals, string _strFileName) {
#ifdef DEBUG_CLSFESYSTEMBUILDER
    cout << "ClsFESystemBuilder::importFragment(string strCont, bool bValidate, bool bEmitSignals, string _strFileName)" << endl;
#endif

    try {
	clsSysFileParser.XMLPlatformUtilsInitialize();
    }
    catch (ClsSysFileParserException &e){
	throw;
    }

    try {
	clsSysFileParser.parseFragment(strCont, bValidate);
    }
    catch (ClsSysFileParserException &e){
	throw;
    }


    list<ClsInfoPrc> lstPrcs = clsSysFileParser.getListClipboardProcesses();

    /* FIRST THING WE HAVE TO CHECK IS THAT THERE IS NO PROCESS WITH THE SAME ID */
    list<ClsInfoPrc>::iterator itlstPrcs;
    for (itlstPrcs = lstPrcs.begin(); itlstPrcs != lstPrcs.end(); ++itlstPrcs){
	string strID = itlstPrcs->getPrcID();
 	if(ClsFESystemManager::Instance()->getFEProcess(strID)!=NULL){
 	    ClsFESystemBuilderException clsFESystemBuilderException(ClsFESystemBuilderException::IMPORT_DUPLICATE_IDS);
 	    throw clsFESystemBuilderException;
 	}
   }

    mapDiagramLineExternal= clsSysFileParser.getMapDiagramLine();
    mapDiagramIconExternal = clsSysFileParser.getMapDiagramIcon();


    string strPrcID;
    for (itlstPrcs = lstPrcs.begin(); itlstPrcs != lstPrcs.end(); ++itlstPrcs){
	string strPrcName = itlstPrcs->getProcessName();
//	cout << "PRCID :" << itlstPrcs->getPrcID() << endl;
	strPrcID = itlstPrcs->getPrcID();
	itlstPrcs->setPath(_strFileName);
	ClsFESystemManager::Instance()->addProcess( *itlstPrcs);
	if(bEmitSignals){
	    ClsFESystemManager::Instance()->emitItemDuplicated(ClsFESystemManager::ITEM_PROCESS, strPrcID);
	}
    }
    
    list<ClsInfoGroup> lstGroups = clsSysFileParser.getListClipboardGroups( );
    list<ClsInfoGroup>::iterator itlstGroups;
    for (itlstGroups = lstGroups.begin(); itlstGroups != lstGroups.end(); ++itlstGroups){
	itlstGroups->setProcessID(strPrcID);
	string strGroupID = itlstGroups->getGroupID().c_str();
	string strGroupName =itlstGroups->getGroupName().c_str();
//	cout << "GROUP ID: " << strGroupID << endl;
//	cout << "GRP PRC ID: " << itlstGroups->getProcessID() << endl;
	ClsFESystemManager::Instance()->addGroup( *itlstGroups);
	if(bEmitSignals){
	    ClsFESystemManager::Instance()->emitItemDuplicated(ClsFESystemManager::ITEM_GROUP, strGroupID);
	}
    }
    
    list<ClsInfoConnection> lstConnections = clsSysFileParser.getListClipboardConnections( );
    list<ClsInfoConnection>::iterator itlstConnections;
    for (itlstConnections = lstConnections.begin(); itlstConnections != lstConnections.end(); ++itlstConnections){
	string strConnectionID = itlstConnections->getConnectionID();
	ClsFESystemManager::Instance()->addConnection( *itlstConnections);
	if(bEmitSignals){
	    ClsFESystemManager::Instance()->emitItemDuplicated(ClsFESystemManager::ITEM_CONNECTION, strConnectionID);
	}
    }
}


void ClsFESystemBuilder::parseFragment(string strCont, bool bValidate, int iActiveItemType, string strActiveItemID) {
    try {
	clsSysFileParser.XMLPlatformUtilsInitialize();
    }
    catch (ClsSysFileParserException &e){
	throw;
    }
    try {
	clsSysFileParser.parseFragment(strCont, bValidate);
    }
    catch (ClsSysFileParserException &e){
	throw;
    }

    mapDiagramLineExternal= clsSysFileParser.getMapDiagramLine();
    mapDiagramIconExternal = clsSysFileParser.getMapDiagramIcon();


    /* if the list of processes is > 0, the subsequent groups will be children.
       if the list == 0, the the groups are independent and their parent process has to be determined... */

    if(iActiveItemType == ClsFESystemManager::ITEM_SYSTEM){
	string strPrcID = "";

	list<ClsInfoPrc> lstPrcs = clsSysFileParser.getListClipboardProcesses();
	list<ClsInfoPrc>::iterator itlstPrcs;
	for (itlstPrcs = lstPrcs.begin(); itlstPrcs != lstPrcs.end(); ++itlstPrcs){
//	    string strPrcName = itlstPrcs->getProcessName();
//	    cout << "strPrcName :" << strPrcName<< endl;
	    strPrcID = ClsFESystemManager::Instance()->duplicateProcess( *itlstPrcs);
	}

	list<ClsInfoGroup> lstGroups = clsSysFileParser.getListClipboardGroups( );
	list<ClsInfoGroup>::iterator itlstGroups;
	for (itlstGroups = lstGroups.begin(); itlstGroups != lstGroups.end(); ++itlstGroups){
	    itlstGroups->setProcessID(strPrcID);
	    ClsFESystemManager::Instance()->duplicateGroup( *itlstGroups, false);
	}

	list<ClsInfoConnection> lstConnections = clsSysFileParser.getListClipboardConnections( );
	list<ClsInfoConnection>::iterator itlstConnections;
	for (itlstConnections = lstConnections.begin(); itlstConnections != lstConnections.end(); ++itlstConnections){
	    ClsFESystemManager::Instance()->duplicateConnection( *itlstConnections, true, false);
	}
    }
    else if(iActiveItemType == ClsFESystemManager::ITEM_PROCESS){
	bool bUseNewGroups = false;
	list<ClsInfoGroup> lstGroups = clsSysFileParser.getListClipboardGroups( );
	list<ClsInfoConnection> lstConnections = clsSysFileParser.getListClipboardConnections( );

	if(lstGroups.size()>0 && lstConnections.size()>0){
	    /* we have a group/connection mix in the clipboard */
	    bUseNewGroups = true;
	} 


	list<ClsInfoGroup>::iterator itlstGroups;
	for (itlstGroups = lstGroups.begin(); itlstGroups != lstGroups.end(); ++itlstGroups){
	    itlstGroups->setProcessID(strActiveItemID);
	    ClsFESystemManager::Instance()->duplicateGroup( *itlstGroups);
	}
	list<ClsInfoConnection>::iterator itlstConnections;
	for (itlstConnections = lstConnections.begin(); itlstConnections != lstConnections.end(); ++itlstConnections){
	    string strConnectionID = itlstConnections->getConnectionID( );
		ClsFESystemManager::Instance()->duplicateConnection( *itlstConnections, bUseNewGroups);
	}
    }
}


ClsInfoDiagramLine ClsFESystemBuilder::getDiagramLine(string _strID){
    ClsInfoDiagramLine clsInfoDiagramLine;
    map<string,ClsInfoDiagramLine>::iterator it = mapDiagramLineInternal.find(_strID);
    if(it != mapDiagramLineInternal.end()){
	return it->second;
    }
    it = mapDiagramLineExternal.find(_strID);
    if(it != mapDiagramLineExternal.end()){
	return it->second;
    }
    return clsInfoDiagramLine;
}

ClsInfoDiagramIcon ClsFESystemBuilder::getDiagramIcon(string _strID){
//    cout << "ClsFESystemBuilder::getDiagramIcon(string _strID)" << endl;
    ClsInfoDiagramIcon clsInfoDiagramIcon;
    map<string,ClsInfoDiagramIcon>::iterator it = mapDiagramIconInternal.find(_strID);
    if(it != mapDiagramIconInternal.end()){
	return it->second;
    }
    it = mapDiagramIconExternal.find(_strID);
    if(it != mapDiagramIconExternal.end()){
	return it->second;
    }
    return clsInfoDiagramIcon;
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
