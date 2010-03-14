#ifndef CLSCANVASNODEPROCESS_H
#define CLSCANVASNODEPROCESS_H

#include <string> 
#include <vector> 
#include <qcanvas.h> 
#include <qfontmetrics.h> 

#include "ClsCanvasNodeIcon.h"
#include "ClsCanvasNode.h"



using namespace std;

static const int nodeprocessRTTI = 984375;

class ClsCanvasNodeProcess :  public ClsCanvasNode {
public:
    ClsCanvasNodeProcess ( int _iType, int _x, int _y, int _width, int _height, QCanvas * _canvas, string _strName, string _strID ): 
	ClsCanvasNode(_iType, _x, _y, _width, _height, _canvas, _strName, _strID) {
	qCanvasRectangleIconHolderModule = NULL;
	qCanvasRectangleIconHolderLink = NULL;
	qbrushNotSelected.setColor( QColor(220,220,220) );
	qbrushNotSelected.setStyle ( SolidPattern ) ;
	qpenNotSelected.setColor( black );
	qpenNotSelected.setWidth(1);	
	qpenSelected.setColor(red);
	qpenSelected.setWidth(2);

 	setBrush( qbrushNotSelected );
 	setPen( qpenNotSelected );

	drawLabel();
	drawAPS();
    };


    void moveBy ( double dx, double dy ){
	ClsCanvasNode::moveBy( dx, dy );
	if(qCanvasRectangleIconHolderModule!=NULL){
	    qCanvasRectangleIconHolderModule->moveBy( dx, dy );
	}
	if(qCanvasRectangleIconHolderLink!=NULL){
	    qCanvasRectangleIconHolderLink->moveBy( dx, dy );
	}
    };

    int rtti () const { return nodeprocessRTTI; }

    void setModuleIcon(moduleIcon mi){
//	cout << "ClsCanvasNodeProcess::setModuleIcon(moduleIcon mi)" << endl;
	if(mi.data!=NULL) {
	    if(qCanvasRectangleIconHolderModule != NULL){
		removeModuleIcon(); 
	    }
	    qCanvasRectangleIconHolderModule = new ClsCanvasNodeIcon(canvas(), mi);
	    qCanvasRectangleIconHolderModule->move(this->rect().right() - mi.x0, this->rect().bottom() - mi.y0);
	    qCanvasRectangleIconHolderModule->setZ(1);
	    qCanvasRectangleIconHolderModule->show();
	}  
    };

    void removeModuleIcon(){
//	cout << "ClsCanvasNodeProcess::removeModuleIcon()" << endl;
	if(qCanvasRectangleIconHolderModule != NULL){
	    qCanvasRectangleIconHolderModule->hide();
	    delete qCanvasRectangleIconHolderModule;
	    qCanvasRectangleIconHolderModule = NULL;
	}
    }

    void setModuleEnabled(bool b){
	// cout << "ClsCanvasNodeProcess::setModuleEnabled(bool b):";
	qCanvasRectangleIconHolderModule->setEnabled(b);
    }

    void setIsExternal(bool b){
	if(b){
	    if(qCanvasRectangleIconHolderLink==NULL){
		int iX = this->rect().left();
		int iY = this->rect().top();
		qCanvasRectangleIconHolderLink = new ClsCanvasNodeIcon(canvas(), ClsCanvasNodeIcon::ICON_PROCESS_LINK);
		qCanvasRectangleIconHolderLink->move(iX - qCanvasRectangleIconHolderLink->width()*.5, 
						     iY - qCanvasRectangleIconHolderLink->height()*.5);
		qCanvasRectangleIconHolderLink->setZ(1);
		qCanvasRectangleIconHolderLink->show();
	    }
	} else {
	    if(qCanvasRectangleIconHolderLink!=NULL){
		qCanvasRectangleIconHolderLink->hide();
		delete qCanvasRectangleIconHolderLink;
		qCanvasRectangleIconHolderLink = NULL;
	    }
	}

    }

    void Hide(){
	if(qCanvasRectangleIconHolderModule != NULL){
	    qCanvasRectangleIconHolderModule->hide();
	}
	if(qCanvasRectangleIconHolderLink != NULL){
	    qCanvasRectangleIconHolderLink->hide();
	}
	ClsCanvasNode::Hide();
    }

private:
    ClsCanvasNodeIcon *qCanvasRectangleIconHolderModule;
    ClsCanvasNodeIcon *qCanvasRectangleIconHolderLink;
};

#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
