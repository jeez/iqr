/****************************************************************************
 ** $Filename: ClsFEDataSink.h
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Sep 14 17:02:32 2001
 ** $Date: 2003/08/07 16:16:12 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEDATASINK_H
#define CLSFEDATASINK_H    /*+ To stop multiple inclusions. +*/

#include <list>
#include <string>
#include <vector>
#include <valarray>

#include "ClsBaseDataSink.h"
#include "stateArray.hpp"

using namespace std;
using namespace iqrcommon;


class ClsFEDataSink : public ClsBaseDataSink {

public:
    ClsFEDataSink (string _strGroupID, string _strParamName, string _strRange) : 
	ClsBaseDataSink ( _strGroupID,  _strParamName,  _strRange){
    }

    unsigned int size(){return vData.size();};
    double average() { 
	return accumulate(vData.begin(), vData.end(), 0.) / (double)vData.size(); 
    };
    double operator[](int ii){
	return vData[ii];
    };

private:
    vector<double> vData;
    friend ostream &operator<<(ostream &stream, ClsFEDataSink &clsDataSink);
};

class ClsDataSinkCopying : public ClsBaseDataSink {
public:
//--    ClsDataSinkCopying (string _strItemID, string _strParamName, StateArray* _pStateArray, int iSize) : 
//-- 	ClsBaseDataSink ( _strItemID,  _strParamName), pStateArray(_pStateArray) {
//--        cout << "ClsDataSinkCopying (string _strItemID, string _strParamName, string _strRange, StateArray* _pStateArray, list<int> lstIndices)" << endl;
//--        bUseIndices = false;
//--        vaData.resize(iSize);
//-- 
//--     }

   ClsDataSinkCopying (string _strItemID, string _strParamName, StateArray* _pStateArray) : 
	ClsBaseDataSink ( _strItemID,  _strParamName), pStateArray(_pStateArray) {
       /*
       cout << "ClsDataSinkCopying (string _strItemID, string _strParamName, string _strRange, StateArray* _pStateArray)" << endl;
	cout << "+++pStateArray->getWidth(): " << pStateArray->getWidth() << endl;
	cout << "+++(*pStateArray[0]).size(): " << (*pStateArray)[0].size() << endl;
       */

       bUseIndices = false;
       vaData.resize(pStateArray->getWidth());
    }

   ClsDataSinkCopying (string _strItemID, string _strParamName, StateArray* _pStateArray, string _strRange) : 
	ClsBaseDataSink ( _strItemID,  _strParamName,  _strRange), pStateArray(_pStateArray) {
       /*
       cout << "ClsDataSinkCopying (string _strItemID, string _strParamName, string _strRange, StateArray* _pStateArray, list<int> lstIndices)" << endl;
	cout << "+++pStateArray->getWidth(): " << pStateArray->getWidth() << endl;
	cout << "+++(*pStateArray[0]).size(): " << (*pStateArray)[0].size() << endl;
       */

       if(strRange.size()>0){
	   bUseIndices = true;
	   list<int> lstIndices = rangeString2rangeList(strRange);
	   vaIndices.resize(lstIndices.size());
	   list<int>::iterator it;
	   int ii = 0;
	   for(it=lstIndices.begin(); it!=lstIndices.end(); it++){
	       vaIndices[ii] = *it;
//	   cout << "*it: " << *it << endl;
	       ii++;
	   }
	   vaData.resize(vaIndices.size());
       } else {
	   bUseIndices = false;
	   vaData.resize(pStateArray->getWidth());
       }
   }



    void update(){
/*
	cout << "ClsDataSinkCopying::update()::bUseIndices: " << bUseIndices << endl;
	cout << "+++pStateArray->getWidth(): " << pStateArray->getWidth() << endl;
	cout << "+++(*pStateArray[0]).size(): " << (*pStateArray)[0].size() << endl;
	cout << "+++vaData.size(): " << vaData.size() << endl;
*/
	if(bUseIndices){
	    vaData = (*pStateArray)[0][vaIndices];
//	    cout << "(*pStateArray)[0][vaIndices]" << endl;
	} else {
	    vaData = (*pStateArray)[0];
//	    cout << "(*pStateArray)[0]" << endl;
	}

//	cout << "vaData: " << vaData << endl;

    }

    double average() { 
	return (double)vaData.sum() / (double)vaData.size();
    };


    unsigned int size() {
	return vaData.size();
    };

    double operator[](int ii){
	return vaData[ii];
    };


    StateArray* getStateArray(){
	return pStateArray;
    }
    

//--     void resize(int iSize){
//-- 	cout << "ClsDataSinkCopying::resize()" << endl;
//-- 	if(!bUseIndices){
//-- 	    vaData.resize(iSize);
//-- 	}
//--     };
    void changeSize(){
//	cout << "ClsDataSinkCopying::changeSize()" << endl;
	if(!bUseIndices){
	    vaData.resize(pStateArray->getWidth());
	}
    };



private:

    bool bUseIndices;
    StateArray* pStateArray;
    valarray<double> vaData;
    valarray<size_t> vaIndices;

    friend ostream &operator<<(ostream &stream, ClsDataSinkCopying &clsDataSink);

};

class ClsDataSinkStateArrayPointer : public ClsBaseDataSink {
public:
    ClsDataSinkStateArrayPointer (string _strGroupID, string _strParamName, string _strRange) : 
	ClsBaseDataSink ( _strGroupID,  _strParamName,  _strRange){
    }

    void setData(StateArray* _pStateArray) { pStateArray = _pStateArray;};
    unsigned int size() {
	return (*pStateArray)[0].size();
    };

    double average() { 
	return (*pStateArray)[0].sum() / (double)(*pStateArray)[0].size();
//	return 0;
    };

    double operator[](int ii){
	return (*pStateArray)[0][ii];
//	return 0;
    };

private:

    StateArray* pStateArray;
    
    friend ostream &operator<<(ostream &stream,  ClsDataSinkStateArrayPointer &clsDataSink);

};

class ClsDataSinkStateArrayRange : public ClsBaseDataSink {
public:
    ClsDataSinkStateArrayRange (string _strGroupID, string _strParamName, string _strRange) : 
	ClsBaseDataSink ( _strGroupID,  _strParamName,  _strRange){
    }

    unsigned int size(){return vData.size();};
    double average() { 
	double fSum = 0;
	for(unsigned int ii=0; ii<vData.size(); ii++){
	    fSum += *(vData[ii]);
	}
	return fSum / (double)vData.size();
    };

/*
    void resize(int i){
	vData.resize(i);
    };
*/

    void push_back(const double *pE){ vData.push_back(pE); };

    double operator[](int ii){
	return *(vData[ii]);
    };

private:
    vector<const double*> vData;
    friend ostream &operator<<(ostream &stream, ClsDataSinkStateArrayRange &clsDataSink);

};

#endif /* CLSFEDATASINK_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:

