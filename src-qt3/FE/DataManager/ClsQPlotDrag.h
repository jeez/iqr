/****************************************************************************
 ** $Filename: ClsQPlotDrag.h
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Oct 26 18:38:41 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQPLOTDRAG_H
#define CLSQPLOTDRAG_H    /*+ To stop multiple inclusions. +*/

#include <qdragobject.h> 
#include <qcstring.h> 
#include <string>
#include <iqrUtils.h>


//  class ClsQPlotDrag :  public QDragObject{
//  public:
//       ClsQPlotDrag(QWidget * dragSource, string _strGroupID, string _strParamName, int _iRange) :
//  	  QDragObject(dragSource, ""), strGroupID(_strGroupID), strParamName(_strParamName), iRange(_iRange){
//       };

//       string getGroupID() { return strGroupID; };
//       string getParamName() { return strParamName; };
//       int getRange() { return iRange; };

//       static bool canDecode ( const QMimeSource * e ){};
//       static bool decode ( const QMimeSource * e, QString & s ){};
     

//  private:
//       string strGroupID, strParamName;
//       int iRange;

//       const char* format ( int n = 0 ) const { return "application/iqr-plot";};
//       bool provides ( const char *cpMsg ) { 
//  	  string str = cpMsg;
//  	  if(!str.compare("application/iqr-plot"))
//  	       return true;
//  	  else
//  	       return false;
//       };
    
//       QByteArray encodedData ( const char * ) const  { 
//  	  string str = strGroupID + ":" + strParamName + ":" + iqrUtils::int2string(iRange);
//  	  static const char *bindata = strGroupID.c_str();//, strParamName.c_str(), iRange };
//  	  QByteArray  a;
//  	  a.setRawData( bindata, sizeof(bindata) );
//  	  return a;
//       }; 
//  };


class ClsQPlotDrag :  public QTextDrag{
public:
     ClsQPlotDrag(QWidget * dragSource, string _strGroupID, string _strParamName, int _iRange) :
	  strGroupID(_strGroupID), strParamName(_strParamName), iRange(_iRange){
  	  string str = strGroupID + ":" + strParamName + ":" + iqrUtils::int2string(iRange);
	  
	  QTextDrag ( QWidget * dragSource = 0, const char * name = 0 ) 
	       void setText ( const QString & ) 
	       void setSubtype ( const QCString & ) 
	  
	  
     };

     

private:
     string strGroupID, strParamName;
     int iRange;
  
};








#endif /* CLSQPLOTDRAG_H */

//// Local Variables: 
//// mode: c++
//// End: 
