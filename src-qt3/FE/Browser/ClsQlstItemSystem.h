/****************************************************************************
 ** $Filename: ClsQlstItemSystem.h
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
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
#include <qlistview.h>
//#include <ClsBaseItem.h>
#include <ClsQBaseItem.h>


//class ClsQlstItemSystem: public ClsBaseItem, public QListViewItem {
class ClsQlstItemSystem: public ClsQBaseItem {

public:
//     ClsQlstItemSystem( QListView *parent): QListViewItem( parent ){
     ClsQlstItemSystem( QListView *_parent, string _strID): ClsQBaseItem( _parent, _strID, ClsFESystemManager::ITEM_SYSTEM){
//	  iType = ITEM_SYSTEM;
     };
//     ClsQlstItemSystem( QListViewItem *_parent) : QListViewItem( _parent ) {
     ClsQlstItemSystem( QListViewItem *_parent, string _strID) : ClsQBaseItem( _parent, _strID, ClsFESystemManager::ITEM_SYSTEM) {
//	  iType = ITEM_SYSTEM;
     };

private:


};



//// Local Variables: 
//// mode: c++
//// End: 

#endif /* CLSQLSTITEMSYSTEM_H */
