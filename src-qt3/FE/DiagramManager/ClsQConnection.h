#ifndef CLSQCONNECTION_H
#define CLSQCONNECTION_H

#include "ClsQStateArrayViewRect.h"
#include <iostream>
#include <qobject.h> 

using namespace std;

class ClsQConnection : public QObject  {
    Q_OBJECT

public:
    ClsQConnection(ClsQStateArrayViewRect* _clsQGroupViewPre, ClsQStateArrayViewRect* _clsQGroupViewPost) :
	clsQGroupViewPre( _clsQGroupViewPre), clsQGroupViewPost(_clsQGroupViewPost){
	};
    
    
public slots:
    void setValue( int iX, int iY) {
    clsQGroupViewPost->setValue(0.7, iX, iY); 
    cout << "ClsQConnection:clickX: " << iX << endl;
    cout << "ClsQConnection:clickY: " << iY << endl;

    }

private:
    
    ClsQStateArrayViewRect* clsQGroupViewPre;
    ClsQStateArrayViewRect* clsQGroupViewPost;
};

#endif






