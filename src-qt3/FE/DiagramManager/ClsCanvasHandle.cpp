#include <string> 
#include <qcanvas.h> 
#include <qmap.h>

using namespace std;

#include "ClsInfoConnection.h"
#include "ClsCanvasHandle.h"
#include "ClsCanvasConnection.h"

#include <square8blue_8x8.xpm>
#include <square8green_8x8.xpm>
#include <square8red_8x8.xpm>

#include <square8blueSelected_8x8.xpm>
#include <square8greenSelected_8x8.xpm>
#include <square8redSelected_8x8.xpm>

ClsCanvasHandle::ClsCanvasHandle ( ClsCanvasConnection *_parent, int x, int y, QCanvas * _canvas): 
    QCanvasRectangle(_canvas), parent(_parent){

    qpmExc = QPixmap(square8red);
    qpmMod = QPixmap(square8green); 
    qpmInh = QPixmap(square8blue);

    qpmExcSelected = QPixmap(square8redSelected);
    qpmModSelected = QPixmap(square8greenSelected); 
    qpmInhSelected = QPixmap(square8blueSelected);


    qpmCurrentNS = qpmExc;
    qpmCurrentSelected = qpmExcSelected;
    qpmCurrent = qpmCurrentNS;

    move(x-HANDLESIZE/2,y-HANDLESIZE/2);
    setSize(HANDLESIZE,HANDLESIZE);

//     int z = rand()%256;
// //	qbrushSelected = new QBrush(QColor(z,z,z));
//     qbrushSelected.setColor( red );
//     qpenSelected.setColor(red);
//     qpenSelected.setWidth(30);

//     z = rand()%256;
// //	qbrushNotSelected = new QBrush(QColor(z,z,z));
//     this->qbrushNotSelected.setColor(QColor(z,z,z));
//     qpenNotSelected.setColor(black);
//     qpenNotSelected.setWidth(3);	

//     setBrush( qbrushNotSelected );
//     setPen( qpenNotSelected );

};

int  ClsCanvasHandle::rtti () const { return handleRTTI; }
    
void ClsCanvasHandle::setConnectionType(int iConnectionType){
    
    if(iConnectionType == ClsInfoConnection::CONN_EXCITATORY){
	qpmCurrent = qpmExc;
	qpmCurrentNS = qpmExc;
	qpmCurrentSelected = qpmExcSelected;
    }
    else if(iConnectionType == ClsInfoConnection::CONN_INHIBITORY){
	qpmCurrent = qpmInh;
	qpmCurrentNS = qpmInh;
	qpmCurrentSelected = qpmInhSelected;
    }
    else if(iConnectionType == ClsInfoConnection::CONN_MODULATORY){
	qpmCurrent = qpmMod;
	qpmCurrentNS = qpmMod;
	qpmCurrentSelected = qpmModSelected;
    }
};


void  ClsCanvasHandle::setSelected(bool b ) {
//    cout << "ClsCanvasHandle::setSelected(bool b )" << endl;
     if(b){
	 qpmCurrent = qpmCurrentSelected;
// 	setBrush(qbrushSelected  );
// 	setPen(qpenSelected );
     } else {
	 qpmCurrent = qpmCurrentNS;
// 	setBrush(qbrushNotSelected  );
// 	setPen(qpenNotSelected );
     }
     QCanvasRectangle::setSelected(b);
     update();
};

    
void ClsCanvasHandle::moveBy ( double dx, double dy ){
//    cout << "ClsCanvasHandle::moveBy ( double dx, double dy )" << endl;
    
    QCanvasRectangle::moveBy( dx, dy );

  QCanvasLine *qlinePre = parent->getPrevSegment(this);
  if(qlinePre!=NULL){
      qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), (int)(this->x()+HANDLESIZE/2), (int)(this->y()+HANDLESIZE/2));
  }
  
  QCanvasLine *qlinePost = parent->getNextSegment(this);
  if(qlinePost!=NULL){
      qlinePost->setPoints((int)(this->x()+HANDLESIZE/2), (int)(this->y()+HANDLESIZE/2), qlinePost->endPoint().x(), qlinePost->endPoint().y());
  }
};
    
void ClsCanvasHandle::refresh ( ){
  QCanvasLine *qlinePre = parent->getPrevSegment(this);
  if(qlinePre!=NULL){
      qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), (int)(this->x()+HANDLESIZE/2), (int)(this->y()+HANDLESIZE/2));
  }
  
  QCanvasLine *qlinePost = parent->getNextSegment(this);
  if(qlinePost!=NULL){
      qlinePost->setPoints((int)(this->x()+HANDLESIZE/2), (int)(this->y()+HANDLESIZE/2), qlinePost->endPoint().x(), qlinePost->endPoint().y());
  }
};


void ClsCanvasHandle::drawShape( QPainter &p ) {
    p.drawPixmap( int(x()), int(y()), qpmCurrent );
//    p.drawPixmap( int(x()), int(y()), QPixmap(square8green) );
//    p.drawPixmap( int(x()), int(y()), QPixmap(square8red) );
};


void ClsCanvasHandle::remove(  ) {
    parent->removeHandle(this);
};


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
