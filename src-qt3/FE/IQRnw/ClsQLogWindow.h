/****************************************************************************
 ** $Filename: ClsQLogWindow.h
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Sep 11 14:31:38 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQLOGWINDOW_H
#define CLSQLOGWINDOW_H    /*+ To stop multiple inclusions. +*/

#include <string>


using namespace std;

class ClsQLogWindow  {

public:

    static ClsQLogWindow* Instance();
    static void initializeLogger();

  
    enum {
	TARGET_LOG,
	TARGET_MESSAGE,
	TARGET_BOTH
    };

    enum {
	TYPE_INFO,
	TYPE_WARNING,
	TYPE_ERROR
    };

    void report(int iTarget, int iType, string strMessage);


private:

    ClsQLogWindow();
    static ClsQLogWindow* _instanceQLogWindow;

     void appendText( string strText);     
     void setText( string strText );     

};



#endif /* CLSQLOGWINDOW_H */





//// Local Variables: 
//// mode: c++
//// End: 
