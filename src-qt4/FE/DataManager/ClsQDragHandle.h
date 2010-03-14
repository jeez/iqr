#ifndef CLSQDRAGHANDLE_H
#define CLSQDRAGHANDLE_H

#include <qpushbutton.h>
#include <qlabel.h>
#include <qcursor.h>

class ClsQDragHandle : public QLabel {
  Q_OBJECT
public:
     ClsQDragHandle(QWidget *parent );

signals:
     void drag(bool);

private slots:

void mouseMoveEvent ( QMouseEvent * e );

};



#endif



//// Local Variables: 
//// mode: c++
//// End: 
