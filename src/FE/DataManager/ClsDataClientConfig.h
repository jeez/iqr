/****************************************************************************
 ** $Filename: ClsDataClientConfig.h
 ** $Id: ClsDataClientConfig.h,v 1.1 2003/10/04 10:08:42 ulysses Exp ulysses $
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Wed Oct  1 00:34:20 2003
 ** $Date: 2003/10/04 10:08:42 $
 **
 ** $Log: ClsDataClientConfig.h,v $
 ** Revision 1.1  2003/10/04 10:08:42  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSDATACLIENTCONFIG_H
#define CLSDATACLIENTCONFIG_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>
#include <map>
#include <iostream>

#include "ConfigTagLibrary.h"

using namespace std;


class ClsStateVariableDisplayConfig {
public:
    ClsStateVariableDisplayConfig(/*string _strItemType,*/ string _strID, string _strItemID) : 
	/* strItemType(_strItemType),*/ strID(_strID), strItemID(_strItemID) {};
    ClsStateVariableDisplayConfig(/*string _strItemType,*/ string _strID, string _strItemID, string _strSelectedIndices) : 
	/*strItemType(_strItemType),*/  strID(_strID), strItemID(_strItemID), strSelectedIndices(_strSelectedIndices) {};

    string getID(){return strID;};
    string getItemID(){return strItemID;};
    string getSelectedIndices(){return strSelectedIndices;};

    void addParameter(pair<string, string> p){ lstParameters.push_back(p); };
    list<pair<string, string> > getListParameters() { return lstParameters; };

/*
    string getItemType(){
	return strItemType; 
    }
*/

//     void setItemType(string str){
// 	strItemType = str;
//     }

private:
//--    string strItemType;
    string strID;
    string strItemID;
    string strSelectedIndices;
    list<pair<string, string> > lstParameters;
};


class ClsDataClientConfig {

public:
    ClsDataClientConfig(string _strID) : strID(_strID) { };
    ClsDataClientConfig(string _strID, string _strType) : strID(_strID), strType(_strType) { };

    void setType(string _strType){ strType = _strType;};
    string getType(){ return strType;};

    void setPosition(int iX, int iY){
	pPosition.first = iX;
	pPosition.second = iY;
    };

    void setPosition(pair<int, int> _pPosition){
	pPosition = _pPosition;
    };

    void setGeometry(int iW, int iH){
	pGeometry.first = iW;
	pGeometry.second = iH;
    };

    void setGeometry(pair<int, int> _pGeometry){
	pGeometry = _pGeometry;
    };

    void addStateVariableDisplayConfig(ClsStateVariableDisplayConfig _clsStateVariableDisplayConfig){
//	cout << "ClsDataClientConfig::addStateVariableDisplayConfig(ClsStateVariableDisplayConfig _clsStateVariableDisplayConfig)" << endl;
	lstCVDConfigs.push_back(_clsStateVariableDisplayConfig);
    };

    list<ClsStateVariableDisplayConfig> getListStateVariableDisplayConfig(){
	return lstCVDConfigs;
    };


    void addParameter(pair<string, string> p){ lstParameters.push_back(p); };
    list<pair<string, string> > getListParameters() { return lstParameters; };

//    string getCaption(){return strCaption;};
    pair<int,int> getPosition(){return pPosition;};
    pair<int,int> getGeometry(){return pGeometry;};

    string getID(){return strID;};


private:
//    string strCaption;
    string strID;
    string strType;
    pair<int, int> pPosition;
    pair<int, int> pGeometry;
    list<ClsStateVariableDisplayConfig> lstCVDConfigs;
    list<pair<string, string> > lstParameters;

};

#endif /* CLSDATACLIENTCONFIG_H */



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
