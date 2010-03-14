/****************************************************************************
 ** $Filename: ClsBaseDataSink.h
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Tue Sep 23 21:58:29 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSBASEDATASINK_H
#define CLSBASEDATASINK_H    /*+ To stop multiple inclusions. +*/

#include <numeric>
#include <iqrUtils.h>

using namespace std;

class ClsBaseDataSink {

public:
    ClsBaseDataSink (string _strItemID, string _strParamName, string _strRange = "") :
	strItemID (_strItemID), strParamName (_strParamName), strRange (_strRange), iMaths(MATH_NONE), iColor(0) {
    }

    virtual ~ClsBaseDataSink(){};
    string getItemID(){return strItemID;};
    string getParamName(){return strParamName;};
    string getRange(){return strRange;};
    void setMaths(int ii){iMaths = ii;};
    int getMaths() { return iMaths; };

    void setColor(int _iColor){
	iColor = _iColor;
//	  cout << "ClsFEDataSink::setColor(): " << iColor << endl;
    };
    int getColor(){
//	  cout << "ClsFEDataSink::getColor(): " << iColor << endl;
	return iColor;
    };

    virtual void changeSize()=0;
    virtual unsigned int size() =0;
    virtual double average()=0;
    virtual void update()=0;


    enum MATHS{
	MATH_NONE =0,
	MATH_AVERAGE = 1
    };

    virtual double operator[](int){return 0.;};

    static list<int> rangeString2rangeList(string strRange) {
	list<int> lstIndices;
	string::size_type pos;
	while((pos = strRange.find(";")) != string::npos){
	    if(strRange.substr(0, pos).length() > 0) {
		int iIndex = iqrUtils::string2int(strRange.substr(0, pos));
		lstIndices.push_back(iIndex);
	    }
	    strRange.erase(0, pos+1);
	}
	return lstIndices;
    }
    

protected:

    string strItemID;
    string strParamName;
    string strRange;
    int iMaths;
    int iColor;

//    friend ostream &operator<<(ostream &stream, ClsBaseDataSink &clsBaseDataSink);


};



//// Local Variables:
//// mode: c++
//// End:

#endif /* CLSBASEDATASINK_H */
 
