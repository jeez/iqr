/****************************************************************************
 ** $Filename: ClsBaseItem.h
 ** $Header$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Sep 16 15:34:31 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSBASEITEM_H
#define CLSBASEITEM_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>
using namespace std;


class ClsBaseItem{
     
public:     

     ClsBaseItem( string _strID): strID(_strID) {
     };
     
     void setID(string _strID) { strID = _strID; };
     string getID() { return strID; } ;

    
     
protected:
     string strID;
     
};

#endif /* CLSBASEITEM_H */

//// Local Variables: 
//// mode: c++
//// End: 

     
     
