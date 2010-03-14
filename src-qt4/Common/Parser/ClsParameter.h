/****************************************************************************
 ** $Filename: ClsParameter.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Jan 17 12:21:56 2001
 **
 *****************************************************************************/

#ifndef CLSPPARAMETER_H
#define CLSPPARAMETER_H

#include <string>
#include <typeinfo>
#include <stdio.h>
#include <stdlib.h> //MARK


// 05/02/02 [jmb] Namespace changed from iqrprc to iqrcommon.

using namespace std;
namespace iqrcommon_old
{
  
    //template <class T> class CValue
    //{
    //  T tvalue;
    //
    // public:
    //  CValue(T _tvalue){ 
    //    tvalue = _tvalue;
    //  }
    //  
    //};
    //  ////////////

    //template <class T> class ClsPParameter
    //{
    //  string strName;
    //  T tvalue;
    //
    //public:
    //  ClsPParameter(string _strName, T _tvalue){ 
    //    strName = _strName;
    //    tvalue = _tvalue;
    //  };
    //  
    //  void setName(string _strName)  {
    //    strName = _strName;
    //  }
    //  
    //  void setValue(T _tvalue){
    //    tvalue = _tvalue;
    //  }
    //  
    //  void string getName()  {
    //    return strName;
    //  }
    //  
    //  T getValue()  {
    //    return tvalue;
    //  }
    //  
    //  string getType(){
    //    string strTemp = typeid(tvalue).name();
    //    cout << strTemp <<endl;
    //    string strReturn = "";
    //    string strPrefix = "";
    //    string strSuffix = "";
    //    string strType = "";
    //    
    //    int iOffSet = 0;
    //
    //    if( strTemp[0] == 'P' ){
    //	strSuffix = " *";
    //	cout << "pointer" << endl;
    //	iOffSet = 1;
    //    } 
    //
    //    if ( strTemp[0]=='U' ){
    //	strPrefix = "unsigned ";
    //	cout << "unsigned" << endl;
    //	iOffSet = 1;
    //    }
    //    else if ( strTemp[1]=='U' ){
    //	strPrefix = "unsigned ";
    //	iOffSet = 2;
    //	cout << "unsigned" << endl;
    //    }
    //    
    //    if(!strTemp.compare("i",iOffSet)) 
    //	strType = "int";
    //    else if(!strTemp.compare("l",iOffSet)) 
    //	strType = "long";
    //    else if(!strTemp.compare("b",iOffSet)) 
    //	strType = "boolean";
    //    else if(!strTemp.compare("c",iOffSet)) 
    //	strType = "char";
    //    else if(!strTemp.compare("s",iOffSet)) 
    //	strType = "short";
    //    else if(!strTemp.compare("l",iOffSet)) 
    //	strType = "long";
    //    else if(!strTemp.compare("d",iOffSet)) 
    //	strType = "double";
    //    else if(!strTemp.substr(iOffSet+3,12).compare("basic_string")) 
    //	strType = "string";
    //    else
    //	strType =  "unknown";
    //    
    //    strReturn = strPrefix + strType + strSuffix;
    //    return strReturn;
    //  }
    //};
    //
    //
    ////-------------------------------------------------------
    //
    //
    //class ClsParameter1
    //{
    //  string strName;
    //  int iValue;
    //  long lValue;
    //  bool bValue;
    //  short sValue;
    //  double dValue;
    //  
    // public:
    //  ClsParameter1(){ };
    //  
    //  void setName(string _strName)  {
    //    strName = _strName;
    //  }
    //  
    //  
    //  template<class T> setParameter(string _strName, T _value)
    //    {
    //	strName = _strName;
    //	T value = _value;
    //	//  Param summe =0;
    //	//  return summe;
    //    }
    //  
    //  template<class T> setValue(T _value)
    //    {
    //	//  Param summe =0;
    //	//  return summe;
    //    }
    //  
    //  string getName()  {
    //    return strName;
    //  }
    //
    //  template<class T> T getValue( T aa)
    //    {
    //	//  Param summe =0;
    //	//  return summe;
    //	T ii = 10;
    //	return ii;
    //    }
    //  
    //  //  T getValue()  {
    //  //    return tvalue;
    //  //  }
    //  
    //};
    //
    //
    //---------------------------
    class ClsParameter
    {

	//  long lValue;
	//  bool bValue;
	//  short sValue;
	//  double dValue;
	//  string strValue;
	//  char* pcValue;

    public:
	ClsParameter(){ 
#ifdef DEBUG_CLSPARAMETER
	    cout << "ClsParameter::ClsParameter()"<< endl;
#endif     
	};
  
	//  setParameter(string _strName, int _iValue) 
	//    { 
	//	strName = _strName; 
	//	iValue = _iValue; 
	//	CValue<int> *bb; 
	//	bb = new CValue<int>(_iValue); 
	//	//  CValue<int> bb(5); 
	//	
	//	vv = bb; 
	//    } 
  
	template<class T> void setParameter(string _strName, T _value) {
	    strName = _strName;
	    vv = new T(_value);
	    strType = getType(_value);
	}

	template<class T> void setParameter(string _strName, string _strType, T _value) {
	    strName = _strName;
	    vv = new T(_value);
	    strType = _strType;
	}

	void setParameter(string _strName, string _strType, string _strValue) {
	    strName = _strName;
	    strValue = _strValue;
	    strType = _strType;
	}
  
	string getName()  {
	    return strName;
	}

	string getValueAsString()  {
	    return strValue;
	};

	int getValueAsInt()  {
	    int iValue;
	    iValue = to_integer(strValue);
	    return iValue;
	};

	double getValueAsDouble()  {
	    double fValue;
	    fValue = to_double(strValue);
	    return fValue;
	};
  
	template<class T> T getValue(T test) {
	    T *ii = (T*)vv;
	    return *ii;
	}

	string getType() {
	    return strType;
	}  

	string getType( char cValue) {  
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( char cValue)" << endl; 
#endif
	    strType = "char";
	    strValue = cValue;
	    return strType;
	};

	string getType( char * pcValue) {  
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( char * pcValue)" << endl; 
#endif
	    strType = "char *";
	    strValue = pcValue;
	    return strType;
	};
  
	string getType( char *caValue[]) {
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( char *caValue[] )" << endl; 
#endif
	    strType = "char *[]";
	    strValue = **caValue;
	    return strType;
	};

	string getType( string _strValue) {  
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( string strValue)" << endl; 
#endif
	    strType = "string";
	    strValue = _strValue;
	    return strType;
	};

	string getType( unsigned int iValue) {  
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( unsigned int iValue)" << endl; 
#endif    
	    strType = "unsigned int";
	    char p[100];
	    sprintf(p, "%d", iValue);
	    strValue = p;
	    return strType;
	};

	string getType( bool bValue ) {  
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( bool bValue )" << endl; 
#endif
	    strType = "boolean";
	    // this is a bit very complicated, but the compiler does a type check on _value, and the below seems to work for all types...
	    char p[3];
	    sprintf(p, "%d", bValue);
	    int ii = atoi(p);
	    if(ii==0)
		strValue = "false";
	    else
		strValue = "true";
	    return strType;
	};

	string getType( int iValue) { 
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( int iValue)" << endl; 
#endif
	    strType = "int";
	    char p[100];
	    sprintf(p, "%d", iValue);
	    strValue = p;
	    return strType;
	};

	// 06/02/02 [jmb] Changed fValue to iValue for short int
	// argument in function below.
	string getType( short iValue ) {  
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( short iValue )" << endl; 
#endif
	    strType = "short";
	    char p[100];
	    // 06/02/02 [jmb] Changed modifier to short int below.
	    sprintf(p, "%hd", iValue);
	    strValue = p;
	    return strType;
	};

	string getType( long iValue ) {  
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( long iValue )" << endl; 
#endif
	    strType = "long";
	    char p[100];
	    // 06/02/02 [jmb] Changed modifier to long int below.
	    sprintf(p, "%ld", iValue);
	    strValue = p;
	    return strType;
	};

	string getType( float fValue ) {  
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( float fValue )" << endl; 
#endif
	    strType = "float";
	    char p[100];
	    sprintf(p, "%f", fValue);
	    strValue = p;
	    return strType;
	};

	string getType( double fValue) { 
#ifdef DEBUG_CLSPARAMETER
	    cout << "getType( double fValue)" << endl;  
#endif      
	    strType = "double";
	    char p[100];
	    sprintf(p, "%f", fValue);
	    strValue = p;
	    return strType;
	};

	//  string getType( ) {  cout << "" << endl; };
    
    private:
//  	template<class T> string getType( T _value){
//  #ifdef DEBUG_CLSPARAMETER
//  	    cout << "template<class T> string getType( T _value)" << endl; 
//  #endif
//  	    string strTemp = typeid(_value).name();
//  	    string strReturn = "";
//  	    string strPrefix = "";
//  	    string strSuffix = "";
//  	    string strType = "";
//  #ifdef DEBUG_CLSPARAMETER     
//  	    cout << strTemp << endl;
//  #endif
    
//  	    int iOffSet = 0;

//  	    if( strTemp[0] == 'P' ){
//  		strSuffix = " *";
//  		//      cout << "pointer" << endl;
//  		iOffSet = 1;
//  	    } 

//  	    if ( strTemp[0]=='U' ){
//  		strPrefix = "unsigned ";
//  		//      cout << "unsigned" << endl;
//  		iOffSet = 1;
//  	    } else if ( strTemp[1]=='U' ){
//  		strPrefix = "unsigned ";
//  		iOffSet = 2;
//  		//      cout << "unsigned" << endl;
//  	    } 

//  	    if ( strTemp[0]=='C' ){
//  		strPrefix = "array ";
//  		//      cout << "array" << endl;
//  		iOffSet = 1;
//  	    } else if ( strTemp[1]=='C' ){
//  		strPrefix = "array ";
//  		iOffSet = 2;
//  		//      cout << "array" << endl;
//  	    } 
    

//  	    //    if(!strTemp.compare("i",iOffSet)) { //ok
//  	    //	strType = "int";
//  	    //	char p[100];
//  	    //	sprintf(p, "%d", _value);
//  	    //	strValue = p;
//  	    //    } 
//  	    //    else if(!strTemp.compare("l",iOffSet)){ //ok
//  	    //	strType = "long";
//  	    //	char p[100];
//  	    //	sprintf(p, "%d", _value);
//  	    //	strValue = p;
//  	    //    } 
//  	    //    else if(!strTemp.compare("b",iOffSet)) { //----
//  	    //	strType = "boolean";
//  	    //	// this is a bit very complicated, but the compiler does a type check on _value, and the below seems to work for all types...
//  	    //	char p[3];
//  	    //	sprintf(p, "%d", _value);
//  	    //	int ii = atoi(p);
//  	    //	if(ii==0)
//  	    //	  strValue = "false";
//  	    //	else
//  	    //	  strValue = "true";
//  	    //    } 
//  	    //    else 

//  	    // @@@ 06/02/02 [jmb] The line below fails to compile with
//  	    // gcc3.  The syntax of the comparison is not found in
//  	    // std::string.
//  	    if(!strTemp.compare("c",iOffSet)){  //ok
//  		//      cout << "SDFSDFSDFSDFSDF" << endl;
//  		strType = "char";
//  		//	       strValue = _value;
//  	    } 

//  	    //    else if(!strTemp.compare("s",iOffSet)){ 
//  	    //	strType = "short";
//  	    //	char p[100];
//  	    //	sprintf(p, "%f", _value);
//  	    //	strValue = p;
//  	    //    } 
//  	    //    else if(!strTemp.compare("d",iOffSet)){ 
//  	    //	strType = "double";
//  	    //	char p[100];
//  	    //	sprintf(p, "%f", _value);
//  	    //	strValue = p;
//  	    //    } 
//  	    //
//  	    //    else 
//  	    //    if (strTemp.length()>10){  //just in case thinks f*up before
//  	    //	if(!strTemp.substr(iOffSet+3,12).compare("basic_string")){  //ok
//  	    //	  strType = "string";
//  	    //	  strValue = _value;
//  	    //	} 
//  	    //    }
//  	    else {
//  		strType =  "unknown";
//  		strValue = "unknown";
//  	    }
    
//  	    strReturn = strPrefix + strType + strSuffix;
//  	    return strReturn;
//  	}

	template<class T> string getType( T _value){
#ifdef DEBUG_CLSPARAMETER
	    cout << "template<class T> string getType( T _value)" << endl; 
#endif
	    string strReturn = "";
    
	    if(typeid(_value) == typeid(char)){
		 strType = "char";
	    } 
	    else if(typeid(_value) == typeid(char*)){
		 strType = "pointerchar";
	    }
	    else {
		 strReturn =  "unknown";
	    }
	    
	    // 7/11/2002 [jmb] Removed "statement with no effect" to
	    // prevent compiler warning.
	    //strReturn;
	    return strReturn;
	};



	double to_double(string str) {
	    double fReturn;
	    fReturn = atof(str.c_str());
	    return fReturn;
	}

	int to_integer(string str) {
	    int iReturn;
	    int iLength;
	    iLength = str.length();
	    iReturn = 0;
	    for (int j = 0; j < iLength; ++j)
		iReturn = 10 * iReturn + str[j] - '0';
	    return iReturn;
	}

	const void * vv;
	string strName;
	string strType;
	string strValue;


    };
    //////////////// 
}

#endif


/// Local Variables: 
/// mode: c++
/// compile-command: "make -f Makefile.ClsParameter"
/// End: 



