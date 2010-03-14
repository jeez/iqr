/****************************************************************************
 ** $Filename: ClsQLogWindow.h
 **
 ** $Author: Ulysses Bernardet
 **
 *****************************************************************************/


#ifndef CLSQLOGWINDOW_H
#define CLSQLOGWINDOW_H    /*+ To stop multiple inclusions. +*/

#include <string>


#include <QObject>
#include <QPushButton> 
#include <QWidget>
#include <QString>
#include <QLayout>
#include <QDialog>
#include <QMessageBox>
#include <QPlainTextEdit>

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
    void reportSysChange(int iItemType, string strID, string strMessage);

public slots:
     void clearText();     
     void saveLog();     


private:

    ClsQLogWindow(QWidget *parent);
    static ClsQLogWindow* _instanceQLogWindow;

     void appendText( string strText);     
     void setText( string strText );     

     QPlainTextEdit *qtxtLog; 
    
     QPushButton *qbtnSave;
     QPushButton *qbtnClose;
     QPushButton *qbtnClear;
};



#endif /* CLSQLOGWINDOW_H */





//// Local Variables: 
//// mode: c++
//// End: 
