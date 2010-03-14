/****************************************************************************
 ** $Filename: ClsCanvasNodeIcon.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Nov 19 10:18:09 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSCANVASNODEICON_H
#define CLSCANVASNODEICON_H    /*+ To stop multiple inclusions. +*/

#include <iostream>
#include <qcanvas.h> 
//#include <qpixmap.h>
#include <qimage.h>
#include <qpainter.h>
#include <qsize.h>


#include "moduleIcon.hpp"

namespace {
#include <group_module_in_14x14.xpm>
#include <group_module_out_14x14.xpm>
#include <group_synapse_in_14x14.xpm>
#include <process_link_23x29.xpm>
}



static const int nodegroupIconRTTI = 984385;


class ClsCanvasNodeIcon :  public QCanvasRectangle {

public:

    enum ICON_TYPE {
	ICON_GROUP_MODULE_IN,
	ICON_GROUP_MODULE_OUT,
	ICON_GROUP_SYNAPSE_IN,
	ICON_PROCESS_LINK
    };
	
    ClsCanvasNodeIcon( QCanvas * _canvas) : QCanvasRectangle(_canvas){
	image = NULL;
	imageDisabled = NULL;
	bEnabled = true;
    }

    ClsCanvasNodeIcon( QCanvas * _canvas, int iIconType) : QCanvasRectangle(_canvas){
	/* pcData = NULL; */
	bEnabled = true;
	image = NULL;
	imageDisabled = NULL;
	if(iIconType == ICON_GROUP_MODULE_IN){
	    image = new QImage(group_module_in);
	} else if (iIconType == ICON_GROUP_MODULE_OUT){
	    image = new QImage(group_module_out);
	} else if (iIconType == ICON_GROUP_SYNAPSE_IN){
	    image = new QImage(group_synapse_in);
	} else if (iIconType == ICON_PROCESS_LINK){
	    image = new QImage(process_link);
	}
	if(image!=NULL){
	    setSize( image->width(), image->height());
	    qsizeIcon.setWidth(image->width());
	    qsizeIcon.setHeight(image->height());	
	}
    }

    ClsCanvasNodeIcon( QCanvas * _canvas, moduleIcon mi) : QCanvasRectangle(_canvas){
	/// cout << "ClsCanvasNodeIcon( QCanvas * _canvas, moduleIcon mi)" << endl;
	/* pcData = NULL; */
	image = NULL;
	imageDisabled = NULL;
	if(mi.data != NULL){
	    /*
	      pcData = new unsigned char[mi.size];
	      memcpy(pcData, mi.data, mi.size);
	      image->loadFromData ( pcData, mi.size, 0);
	    */

	    image = new QImage();
	    image->loadFromData ( mi.data, mi.size, 0);
	    setSize( image->width(), image->height());
	    qsizeIcon.setWidth(image->width());
	    qsizeIcon.setHeight(image->height());	
	    imageDisabled = new QImage(*image);
	    image->detach();
	    imageDisabled->invertPixels(false);
	}
    }

    ~ClsCanvasNodeIcon(){
//	cout << "ClsCanvasNodeIcon::~ClsCanvasNodeIcon()" << endl;
	if(image!=NULL){
	    delete image;
	    image = NULL;
	}

	if(imageDisabled!=NULL){
	    delete imageDisabled;
	    imageDisabled = NULL;
	}

/*
  if(pcData != NULL){
  free(pcData);
  }
*/
    }


    void selectIcon(int iIconType){
	if(image!=NULL){
	    delete image;
	    image = NULL;
	}
	if(iIconType == ICON_GROUP_MODULE_IN){
	    image = new QImage(group_module_in);
	} else if (iIconType == ICON_GROUP_MODULE_OUT){
	    image = new QImage(group_module_out);
	} else if (iIconType == ICON_GROUP_SYNAPSE_IN){
	    image = new QImage(group_synapse_in);
	} else if (iIconType == ICON_PROCESS_LINK){
	    image = new QImage(process_link);
	}
	if(image!=NULL){
	    setSize( image->width(), image->height());
	    qsizeIcon.setWidth(image->width());
	    qsizeIcon.setHeight(image->height());	

	}	
    }


    int rtti () const { return nodegroupIconRTTI; }

    void setEnabled(bool b){
	bEnabled = b;
	update();
    }



private:
    
    void drawShape( QPainter &p ){
//	cout << "ClsCanvasNodeIcon:drawShape(QPainter &p)" << endl;

	if(bEnabled) {
//	    cout << "ENABLED" << endl;
	    if(image!=NULL){
		p.drawImage( int(x()), int(y()), *image );
	    }
	} else {
//	    cout << "DISABLED" << endl;
	    if(imageDisabled!=NULL){
		p.drawImage( int(x()), int(y()), *imageDisabled );
	    }
	}
    }
    
    QImage *image;
    QImage *imageDisabled;
    bool bEnabled;
    /* unsigned char *pcData; */
    QSize qsizeIcon;
};

#endif /* CLSCANVASNODEICON_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
