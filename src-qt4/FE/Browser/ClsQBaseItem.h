/****************************************************************************
 ** $Filename: ClsBaseItem.h
 ** $Header$
 **
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

#include <string>

#include <QTreeWidgetItem>
#include <QIcon>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QPainter>

#include <ClsFESystemManager.h>
using namespace std;


class ClsQBaseItem  : public QTreeWidgetItem{
     
public:     
    ClsQBaseItem( QTreeWidget *parent, string _strID, int _iType): QTreeWidgetItem( parent ), 
								   strID(_strID), iType(_iType) {
	setText(1, strID.c_str());
    };
    
    
    ClsQBaseItem( QTreeWidgetItem *parent, string _strID, int _iType) : QTreeWidgetItem( parent ), 
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

    virtual void setColor(string strColor){};
     
protected:
    string strID;
    string strName;
    
    int iType;
     
};

#endif /* CLSQBASEITEM_H */

    
     
//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
