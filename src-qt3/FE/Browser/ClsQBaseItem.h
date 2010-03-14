/****************************************************************************
 ** $Filename: ClsBaseItem.h
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Sep 16 15:34:31 2001
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQBASEITEM_H
#define CLSQBASEITEM_H    /*+ To stop multiple inclusions. +*/

#include <qlistview.h>
#include <string>

#include <ClsFESystemManager.h>
using namespace std;


class ClsQBaseItem  : public QListViewItem{
     
public:     

     ClsQBaseItem( QListView *parent, string _strID, int _iType):  QListViewItem( parent ), 
	  strID(_strID), iType(_iType) {
	 setText(1, strID.c_str());
     };
    
    
    ClsQBaseItem( QListViewItem *parent, string _strID, int _iType) : QListViewItem( parent ), 
								      strID(_strID), iType(_iType)  {
	setText(1, strID.c_str());
    };

     
     void setID(string _strID) { 
	 strID = _strID; 
	 setText(1, strID.c_str());
     };

     string getID() { return strID; } ;

     void setItemName(string _strName) { 
	 strName = _strName; 
	 setText(0, strName.c_str());
     };
     string getItemName() { return strName; } ;

    

     void setType (int _iType) { iType = _iType; };

     int getType () { return iType; };

/*
     enum ITEM_TYPE{
	  ITEM_SYSTEM = 0,
	  ITEM_PRC,
	  ITEM_GROUP
     };
*/
     
     
protected:
     string strID;
    string strName;
    
     int iType;
     
     };

#endif /* CLSQBASEITEM_H */

//// Local Variables: 
//// mode: c++
//// End: 

     
     
