/****************************************************************************
 ** $Filename: ClsFEMessageIn.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Sep  2 12:01:03 2001
 ** $Date: 2001/10/05 16:02:40 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEMESSAGEIN_H
#define CLSFEMESSAGEIN_H    /*+ To stop multiple inclusions. +*/

//#include <ClsFEBasePlot.h>
#include <ClsFEDataSink.h>

class ClsFEMessageIn {
public:
     ClsFEMessageIn(){};
     ClsFEMessageIn(string _strID): strID(_strID){};

     void setID(string _strID) {
	  strID = _strID;
     }
    
     void setData(string _strData) {
	  strData = _strData;
	  string::size_type stPosSemicolon;
	  iXPoints = 0; iYPoints = 0;
	  fValMin= 10000; fValMax= -10000;

	  vectorData.clear();
	  while( strData.size()>1 ){ 
	       stPosSemicolon = strData.find_first_of(";");
	       string strField = strData.substr(0, stPosSemicolon);
	       strData.erase(0,stPosSemicolon+1);
	    
	       if(!strField.find("#")){ strField.erase(0,1); }
	       if(strData.find_first_of("#") == 0){ iYPoints++; }
	    
	       double fVal = atof(strField.c_str());
	       vectorData.push_back(fVal);
	       fValMin= min(fValMin, fVal);
	       fValMax= max(fValMax, fVal);
	       iXPoints++;
	  }
	  iXPoints /= iYPoints;
     }
    

//       void setDataSinkPointer(ClsFEDataSink *_clsFEDataSink) {
//  	  clsFEDataSink = _clsFEDataSink;
	  
//       }


     void addDataSinkPointer(ClsFEDataSink *clsFEDataSink) {
	  lstDataSinks.push_back(clsFEDataSink);
     }
   
     void commit() {
	  list<ClsFEDataSink *>::iterator itlstDataSinks;
	  for (itlstDataSinks = lstDataSinks.begin(); itlstDataSinks != lstDataSinks.end(); ++itlstDataSinks){
//	       (vector<double>*)(itlstDataSinks)->swap(vectorData);
//	       vector<double>* t = (vector<double>*)itlstDataSinks;
//	       ClsFEDataSink *d = &(*itlstDataSinks);
//  	       ClsFEDataSink *d = (ClsFEDataSink *)itlstDataSinks;
//  	       ClsFEDataSink *dd = itlstDataSinks->();
//	       ClsFEDataSink *ddd = itlstDataSinks;
	       ClsFEDataSink *clsFEDataSink = *itlstDataSinks;
	       clsFEDataSink->swap(vectorData);

	       
	  }
	  
	  
     }
     
     string getID() { return strID; }
     int xPoints() { return iXPoints; }
     int yPoints() { return iYPoints; }
    
private:
     string strData, strID;
     int iXPoints, iYPoints;
     double fValMin, fValMax;
    
     vector<double> vectorData;
     ClsFEDataSink *clsFEDataSink;

     list<ClsFEDataSink *> lstDataSinks;
    
};

#endif /* CLSFEMESSAGEIN_H */



//// Local Variables: 
//// mode: c++
//// End: 

