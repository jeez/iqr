/****************************************************************************
 ** $Filename: ClsQDragHandle.cpp
 ** $Header$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Nov  6 15:54:51 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <QMouseEvent>

#include "ClsQDragHandle.h"


#include <drag_very_small_6x6.xpm>
#include <pick_14x14.xpm>

ClsQDragHandle::ClsQDragHandle(QWidget *parent ) :  QLabel(parent){ 
//     this->setPixmap( QPixmap(drag_very_small));	  
    QWidget::setCursor( Qt::PointingHandCursor );
     this->setPixmap( QPixmap(pick));	  
};

void ClsQDragHandle::mouseMoveEvent ( QMouseEvent * e ) { 
    if(e->buttons() == Qt::LeftButton ){
	  emit drag(false);
    }
    else if(e->buttons() == Qt::LeftButton || Qt::ControlModifier) {
	  emit drag(true);
    }
};

