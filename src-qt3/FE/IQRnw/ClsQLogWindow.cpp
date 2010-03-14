/****************************************************************************
 ** $Filename: ClsQLogWindow.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Sep 11 14:31:25 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include "ClsQLogWindow.h"

ClsQLogWindow* ClsQLogWindow::_instanceQLogWindow = NULL;

void ClsQLogWindow::initializeLogger(){
    _instanceQLogWindow = new ClsQLogWindow();
}


ClsQLogWindow* ClsQLogWindow::Instance(){
    return _instanceQLogWindow;
}


void ClsQLogWindow::report(int iTarget, int iType, string strMessage){
/*
    if(iTarget == TARGET_LOG ){
	appendText(strMessage);
    } else if(iTarget == TARGET_MESSAGE){
	if(iType == TYPE_INFO){
	    QMessageBox::information( this, "IQR", strMessage.c_str(), "close", 0 );
	} else if(iType == TYPE_WARNING){
	    QMessageBox::warning( this, "IQR", strMessage.c_str(), "close", 0 );
	} else if(iType == TYPE_ERROR){
	    QMessageBox::critical( this, "IQR", strMessage.c_str(), "close", 0 );
	} 
    } else if(iTarget == TARGET_BOTH){

    }
*/
}

ClsQLogWindow::ClsQLogWindow()  {
/*
    QBoxLayout *qvblt = new QVBoxLayout( this );

    qtxtLog = new QTextView(this, "log text");
    qvblt->addWidget( qtxtLog );

    QBoxLayout *qhblt = new QHBoxLayout( qvblt );
    qhblt->addStretch(1);
    qbtnClose = new QPushButton ( "close", this ) ;
    qhblt->addWidget( qbtnClose);
    connect(qbtnClose, SIGNAL(clicked()), this, SLOT(hide()));

    qhblt->addSpacing(1);
    qbtnClear = new QPushButton ( "clear", this ) ;
    qhblt->addWidget( qbtnClear);
    connect(qbtnClear, SIGNAL(clicked()), this, SLOT(clearText()));
    qhblt->addStretch(1);

    resize( 250, 400 );
*/
};
     


void ClsQLogWindow::appendText( string strText) {
    string str = strText + (string)"\n";

//    qtxtLog->append(str.c_str());




};     

void ClsQLogWindow::setText( string strText ) {
//    qtxtLog->setText(strText.c_str());
};     



