/****************************************************************************
 ** $Filename: ClsQlstItemGroup.h
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ClsQlstItemGroup.h,v 1.1 2001/10/16 09:58:28 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Sep  9 22:21:05 2001
 ** $Date: 2001/10/16 09:58:28 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSQLSTITEMGROUP_H
#define CLSQLSTITEMGROUP_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>

#include <qlistview.h>
#include <qwidget.h>


#include <ClsQBaseItem.h>
#include <ClsSysFileNode.h>

namespace iqrprc {
     class ClsParameter;
}

class ClsQlstItemGroup: public QWidget, public ClsQBaseItem { 

public:
     ClsQlstItemGroup(ClsQBaseItem *_parent, string _strGroupID) : ClsQBaseItem( _parent, _strGroupID, ClsFESystemManager::ITEM_GROUP) {
	  strPrcID = _parent->getID();
     };
     
    void setGroupName(string _strName) { 
	setItemName(_strName);
    };

    string getGroupName() { return strName; };
    string getProcessID() { return strPrcID; };
    string getGroupID() { return strID; };
    
private slots:

private:

string strPrcID;
};

#endif /* CLSQLSTITEMGROUP_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:



