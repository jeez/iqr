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


#include <qobject.h>
#include <qpushbutton.h> 
#include <qwidget.h>
#include <qstring.h>
#include <qtextview.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qmessagebox.h>

using namespace std;

class ClsQLogWindow: public QDialog  {
     Q_OBJECT

public:

    static ClsQLogWindow* Instance();
    static void initializeLogger(QWidget *parent);

     
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

public slots:
     void clearText();     

private:

    ClsQLogWindow(QWidget *parent);
    static ClsQLogWindow* _instanceQLogWindow;

     void appendText( string strText);     
     void setText( string strText );     

     QTextView *qtxtLog;     
     QPushButton *qbtnClose;
     QPushButton *qbtnClear;
};



#endif /* CLSQLOGWINDOW_H */





//// Local Variables: 
//// mode: c++
//// End: 
