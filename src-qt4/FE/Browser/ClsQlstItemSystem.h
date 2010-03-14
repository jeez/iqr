/****************************************************************************
 ** $Filename: ClsQlstItemSystem.h
 ** $Header$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Sep 16 15:41:48 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQLSTITEMSYSTEM_H
#define CLSQLSTITEMSYSTEM_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>
#include <ClsQBaseItem.h>


class ClsQlstItemSystem: public ClsQBaseItem {

public:
     ClsQlstItemSystem( QTreeWidget *_parent, string _strID): ClsQBaseItem( _parent, _strID, ClsFESystemManager::ITEM_SYSTEM){
//	  iType = ITEM_SYSTEM;
     };
     ClsQlstItemSystem( QTreeWidgetItem *_parent, string _strID) : ClsQBaseItem( _parent, _strID, ClsFESystemManager::ITEM_SYSTEM) {
//	  iType = ITEM_SYSTEM;
     };

private:


};


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:


#endif /* CLSQLSTITEMSYSTEM_H */
