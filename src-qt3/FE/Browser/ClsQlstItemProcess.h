/****************************************************************************
 ** $Filename: ClsQlstItemProcess.h
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ClsQlstItemProcess.h,v 1.1 2001/10/16 09:59:35 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Sep 16 15:41:48 2001
 ** $Date: 2001/10/16 09:59:35 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQLSTITEMPROCESS_H
#define CLSQLSTITEMPROCESS_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>
#include <qlistview.h>
#include <ClsQBaseItem.h>


class ClsQlstItemProcess: public ClsQBaseItem {

public:
     ClsQlstItemProcess( ClsQBaseItem *_parent, string _strPrcID) : ClsQBaseItem( _parent, _strPrcID, ClsFESystemManager::ITEM_PROCESS) {
    };

     string getProcessID() { return strID; };     
private:
};


#endif /* CLSQLSTITEMPROCESS_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
