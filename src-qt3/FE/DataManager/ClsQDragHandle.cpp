/****************************************************************************
 ** $Filename: ClsQDragHandle.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Nov  6 15:54:51 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include "ClsQDragHandle.h"


#include <drag_very_small_6x6.xpm>
#include <pick_14x14.xpm>

ClsQDragHandle::ClsQDragHandle(QWidget *parent ) :  QLabel(parent){ 
//     this->setPixmap( QPixmap(drag_very_small));	  
      QWidget::setCursor( PointingHandCursor );
     this->setPixmap( QPixmap(pick));	  
};

void ClsQDragHandle::mouseMoveEvent ( QMouseEvent * e ) { 
     if(e->state() == LeftButton )
	  emit drag(false);
     else if(e->state() == LeftButton || ControlButton)
	  emit drag(true);
};

