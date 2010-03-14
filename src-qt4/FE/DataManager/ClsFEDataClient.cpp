/****************************************************************************
 ** $Filename: ClsFEDataClient.cpp
 ** $Header$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Mon Nov 19 19:05:52 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <iostream>
#include "ClsFEDataClient.h"
#include <ClsFESystemManager.h>
#include "ClsFEGroup.h"


ClsFEDataClient::ClsFEDataClient(ClsFEDataManager *_clsFEDataManager, QMutex *_qmutexSysGUI, string _strDataClientID) :
    clsFEDataManager(_clsFEDataManager), qmutexSysGUI(_qmutexSysGUI), strDataClientID(_strDataClientID) {
}

//-- void ClsFEDataClient::addDataSink(string strID, string strGroupID, string strParamname, string strRange){ // id, paramtername, range, groupsize, (the two are mutaly redundant)
//--      cout << " ClsFEDataClient::addDataSink(string strID, string strGroupID, string strParamname, string strRange)" << endl;
//-- 
//-- //     clsFEDataManager->createDataSink( clsFEDataSinkTemp);//, strGroupID, strParamname, strRange );
//--      ClsBaseDataSink *clsDataSinkTemp = clsFEDataManager->createDataSink( strGroupID, strParamname, strRange );
//-- 
//-- 
//--      int iColor = clsColorPicker.getColor();
//--      clsDataSinkTemp->setColor(iColor);
//--      pair<string, ClsBaseDataSink* > pairTemp(strID, clsDataSinkTemp );
//--      mapDataSinks.insert(pairTemp);
//-- 
//--      DataSinkAdded(strID, iColor);
//-- };
//-- 

void ClsFEDataClient::addDataSink(string strID, ClsItem* clsItem, string strParamname, string strRange){
//    cout << "ClsFEDataClient::addDataSink(string strID, ClsItem* clsItem, string strParamname, string strRange)" << endl;
//    cout << "\tstrID: " << strID << endl;

//     ClsBaseDataSink *clsDataSinkTemp = clsFEDataManager->createDataSink( clsItem, strParamname, strRange );
    ClsDataSinkCopying *clsDataSinkTemp = clsFEDataManager->createDataSink( clsItem, strParamname, strRange );


    int iColor = clsColorPicker.getColor();
    clsDataSinkTemp->setColor(iColor);
//     pair<string, ClsBaseDataSink* > pairTemp(strID, clsDataSinkTemp );
    pair<string, ClsDataSinkCopying* > pairTemp(strID, clsDataSinkTemp );
    mapDataSinks.insert(pairTemp);
//     cout << "mapDataSinks.size(): " << mapDataSinks.size() << endl;
    DataSinkAdded(strID, iColor);
};




void ClsFEDataClient::removeDataSink(string strID){ 
//     cout << "ClsFEDataClient::removeDataSink(string strID)" << endl;
//     cout << "\tstrID: " << strID << endl;
    if(mapDataSinks.find(strID) != mapDataSinks.end()){
	clsColorPicker.putColor((mapDataSinks.find(strID)->second)->getColor());
	mapDataSinks.erase(strID);
    }
//  	     clsFEDataManager->cancelDataRequest( ClsFEDataSink *clsFEDataSink, string strGroupID, string strParamName, string strRange  ) { //TODO, TO IMPLEMENT


//     cout << "mapDataSinks.size(): " << mapDataSinks.size() << endl;

    DataSinkRemoved(strID);
};

int ClsFEDataClient::getDataSinkColor(string strID)  {
//    cout << "ClsFEDataClient::getDataSinkColor(string strID)" << endl;
    int iColor = -1;
    if(mapDataSinks.find(strID) != mapDataSinks.end()){
	iColor = (mapDataSinks.find(strID)->second)->getColor();
    }  
    return iColor;
};


void ClsFEDataClient::groupChanged(string _strGroupID){
//    cout << "ClsFEDataClient::groupChanged(string strGroupID)" << endl;

//    map<string, ClsBaseDataSink* >::iterator it;
    map<string, ClsDataSinkCopying* >::iterator it;
    for(it=mapDataSinks.begin(); it!=mapDataSinks.end(); it++){
	if(!_strGroupID.compare(it->second->getItemID())){
/*
  int iSize = ClsFESystemManager::Instance()->getFEGroup(_strGroupID)->getNumberOfNeurons();
  it->second->resize(iSize);
*/
	    it->second->changeSize();
	}
    }
}


void ClsFEDataClient::init(){
//    cout << "ClsFEDataClient::init()" << endl;
    map<string, ClsDataSinkCopying* >::iterator it;
    for(it=mapDataSinks.begin(); it!=mapDataSinks.end(); it++){
	it->second->changeSize();
    }
}



void ClsFEDataClient::setSinkMath (string strSinkID, int iFlag){
//    cout << "##ClsFEDataClient::setSinkMath (string strSinkID, int iFlag)" << endl;

    if(mapDataSinks.find(strSinkID)!=mapDataSinks.end()){
	mapDataSinks.find(strSinkID)->second->setMaths(iFlag);
    }


//    cout << "mapDataSinks.size(): " << mapDataSinks.size() << endl; 

/* DEBUG
   map<string, ClsDataSinkCopying* >::iterator it;
   for(it=mapDataSinks.begin(); it!=mapDataSinks.end(); it++){
   cout << "## sink id: " << it->first << endl;
   }
*/
    
    
    
};



void ClsFEDataClient::connectionChanged(string _strConnectionID){
//    cout << "ClsFEDataClient::connectionChanged(string strConnectionID)" << endl;

    map<string, ClsDataSinkCopying* >::iterator it;
    for(it=mapDataSinks.begin(); it!=mapDataSinks.end(); it++){
	if(!_strConnectionID.compare(it->second->getItemID())){
	    it->second->changeSize();
	}
    }
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
