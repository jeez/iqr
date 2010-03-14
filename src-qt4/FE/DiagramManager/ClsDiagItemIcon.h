/****************************************************************************
 ** $Filename: ClsDiagItemIcon.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Nov 19 10:18:09 2003
 **
 *****************************************************************************/


#ifndef CLSCANVASNODEICON_H
#define CLSCANVASNODEICON_H    /*+ To stop multiple inclusions. +*/

#include <iostream>

#include <QGraphicsScene> 
#include <QGraphicsRectItem>
#include <QImage>
#include <QPainter>
#include <QSize>


#include "moduleIcon.hpp"

namespace {
#include <group_module_in_14x14.xpm>
#include <group_module_out_14x14.xpm>
#include <group_synapse_in_14x14.xpm>
#include <process_link_23x29.xpm>
}



static const int nodegroupIconRTTI = 984385;


class ClsDiagItemIcon :  public QGraphicsRectItem {

public:

    enum ICON_TYPE {
	ICON_GROUP_MODULE_IN,
	ICON_GROUP_MODULE_OUT,
	ICON_GROUP_SYNAPSE_IN,
	ICON_PROCESS_LINK
    };
	
    ClsDiagItemIcon( QGraphicsItem * _parent) : QGraphicsRectItem(_parent){
	image = NULL;
	imageDisabled = NULL;
	bEnabled = true;
    }

    ClsDiagItemIcon( QGraphicsItem * _parent, int iIconType) : QGraphicsRectItem(_parent){
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
//zzz CHECK	    setSize( image->width(), image->height()); 
	    setRect(0,0, image->width(), image->height());

	    qsizeIcon.setWidth(image->width());
	    qsizeIcon.setHeight(image->height());	
	}
    }

    ClsDiagItemIcon( QGraphicsItem * _parent, moduleIcon mi) : QGraphicsRectItem(_parent){
	/// cout << "ClsDiagItemIcon( QCanvas * _parent, moduleIcon mi)" << endl;
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
//zzz CHECK	    setSize( image->width(), image->height());
	    setRect(0,0,  image->width(), image->height());

	    qsizeIcon.setWidth(image->width());
	    qsizeIcon.setHeight(image->height());	
	    imageDisabled = new QImage(*image);
	    image->detach();
	    imageDisabled->invertPixels(false);
	}
    }

    ~ClsDiagItemIcon(){
//	cout << "ClsDiagItemIcon::~ClsDiagItemIcon()" << endl;
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
//zzz check	    setSize( image->width(), image->height());
	    setRect(0,0, image->width(), image->height());

	    qsizeIcon.setWidth(image->width());
	    qsizeIcon.setHeight(image->height());	

	}	
    }


    int type() const { return nodegroupIconRTTI; }

    void setEnabled(bool b){
	bEnabled = b;
	update();
    }



private:
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/) {
	if(bEnabled) {
//	    cout << "ENABLED" << endl;
	    if(image!=NULL){
		painter->drawImage(boundingRect(), *image );
	    }
	} else {
//	    cout << "DISABLED" << endl;
	    if(imageDisabled!=NULL){
		painter->drawImage(boundingRect(), *imageDisabled );
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
