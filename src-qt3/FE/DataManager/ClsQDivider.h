#ifndef CLSQDIVIDER_H
#define CLSQDIVIDER_H

#include <qpushbutton.h> 
#include <qframe.h> 

class ClsQDivider :  public QFrame  {
     Q_OBJECT

public: 
     ClsQDivider(QWidget * parent );

private slots:
     void slotMove( );
     
signals:
     void clicked(bool bIsRight);

private:
     bool bIsRight;
     QPushButton *qbtnMove;
};


#endif

//// Local Variables: 
//// mode: c++
//// End: 
