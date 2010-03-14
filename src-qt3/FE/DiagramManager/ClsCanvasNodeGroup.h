#ifndef CLSCANVASNODEGROUP_H
#define CLSCANVASNODEGROUP_H

#include <string> 
#include <vector> 
#include <qcanvas.h> 
#include <qfontmetrics.h> 

#include "ClsCanvasNodeIcon.h"
#include "ClsCanvasNode.h"

using namespace std;

static const int nodegroupRTTI = 984376;

class ClsCanvasNodeGroup :  public ClsCanvasNode {
public:
    ClsCanvasNodeGroup ( int _iType, int _x, int _y, int _width, int _height, QCanvas * _canvas, string _strName, string _strID ): 
	ClsCanvasNode(_iType, _x, _y, _width, _height, _canvas, _strName, _strID) {
//	cout << "ClsCanvasNodeGroup::ClsCanvasNodeGroup ( int _iType, int _x, int _y, int _width, int _height, QCanvas * _canvas, string _strName, string _strID )" << endl;

	qbrushNotSelected.setColor( white );
	qbrushNotSelected.setStyle ( SolidPattern ) ;
	qpenNotSelected.setColor( black );
	qpenNotSelected.setWidth(1);	
	qpenSelected.setColor(red);
	qpenSelected.setWidth(2);

 	setBrush( qbrushNotSelected );
 	setPen( qpenNotSelected );

	drawLabel();
	drawAPS();

	
	qCanvasRectangleIconHolderInputFromModule = NULL;
	qCanvasRectangleIconHolderOutputToModule = NULL;
	qCanvasRectangleIconHolderSynapse = NULL;


    };

    void setHasInputFromModule(bool b) {
//	cout << "ClsCanvasNodeGroup::setHasInputFromModule"<< endl;
//	cout << "strName: " << strName << endl;
//	cout << "b :" << (b ? "true" : "false") << endl;
	if(b){
	    if(qCanvasRectangleIconHolderInputFromModule==NULL){
		int iX = this->rect().right();
		int iY = this->rect().top();
		qCanvasRectangleIconHolderInputFromModule = new ClsCanvasNodeIcon(canvas(), ClsCanvasNodeIcon::ICON_GROUP_MODULE_IN);
//--		qCanvasRectangleIconHolderInputFromModule = new QCanvasRectangle(canvas());
		qCanvasRectangleIconHolderInputFromModule->move(iX, iY - qCanvasRectangleIconHolderInputFromModule->height()  );
		qCanvasRectangleIconHolderInputFromModule->setZ(1);
		qCanvasRectangleIconHolderInputFromModule->show();
	    }
	} else {
	    if(qCanvasRectangleIconHolderInputFromModule!=NULL){
		qCanvasRectangleIconHolderInputFromModule->hide(); 
		delete qCanvasRectangleIconHolderInputFromModule;
		qCanvasRectangleIconHolderInputFromModule = NULL;
	    }

	}
    };

    void setHasOutputToModule(bool b) {
//	cout << "ClsCanvasNodeGroup::setHasOutputToModule"<< endl;
//	cout << "strName: " << strName << endl;
//	cout << "b :" << (b ? "true" : "false") << endl;
	if(b){
	    if(qCanvasRectangleIconHolderOutputToModule==NULL){
		int iX = this->rect().left();
		int iY = this->rect().bottom();
		qCanvasRectangleIconHolderOutputToModule = new ClsCanvasNodeIcon(canvas(), ClsCanvasNodeIcon::ICON_GROUP_MODULE_OUT);
//--		qCanvasRectangleIconHolderOutputToModule = new QCanvasRectangle(canvas());
		qCanvasRectangleIconHolderOutputToModule->move(iX + this->rect().width(), iY /*- qCanvasRectangleIconHolderOutputToModule->height()*/  );
		qCanvasRectangleIconHolderOutputToModule->setZ(1);
		qCanvasRectangleIconHolderOutputToModule->show();
	    }
	} else {
	    if(qCanvasRectangleIconHolderOutputToModule!=NULL){
		qCanvasRectangleIconHolderOutputToModule->hide();
		delete qCanvasRectangleIconHolderOutputToModule;
		qCanvasRectangleIconHolderOutputToModule = NULL;
	    }
	}
    };

    void setHasInputFromSynapse(bool b) {
	cout << "ClsCanvasNodeGroup::setHasInputFromSynapse"<< endl;
	if(b){
	    if(qCanvasRectangleIconHolderSynapse==NULL){
		int iX = this->rect().left();
		int iY = this->rect().top();
		qCanvasRectangleIconHolderSynapse = new ClsCanvasNodeIcon(canvas(), ClsCanvasNodeIcon::ICON_GROUP_SYNAPSE_IN);
//--		qCanvasRectangleIconHolderSynapse = new QCanvasRectangle(canvas());
		qCanvasRectangleIconHolderSynapse->move(iX, iY - qCanvasRectangleIconHolderSynapse->height()  );
		qCanvasRectangleIconHolderSynapse->setZ(1);
		qCanvasRectangleIconHolderSynapse->show();
	    }
	} else {
	    if(qCanvasRectangleIconHolderSynapse!=NULL){
		qCanvasRectangleIconHolderSynapse->hide();
		delete qCanvasRectangleIconHolderSynapse;
		qCanvasRectangleIconHolderSynapse = NULL;
	    }
	}
    };



    void moveBy ( double dx, double dy ){
	ClsCanvasNode::moveBy( dx, dy );
	if(qCanvasRectangleIconHolderInputFromModule!=NULL){
	    qCanvasRectangleIconHolderInputFromModule->moveBy( dx, dy );
	}

	if(qCanvasRectangleIconHolderOutputToModule!=NULL){
	    qCanvasRectangleIconHolderOutputToModule->moveBy( dx, dy );
	}

	if(qCanvasRectangleIconHolderSynapse!=NULL){
	    qCanvasRectangleIconHolderSynapse->moveBy( dx, dy );
	}

    };

    void Hide(){
	if(qCanvasRectangleIconHolderInputFromModule!=NULL){
	    qCanvasRectangleIconHolderInputFromModule->hide();
	}
	
	
	if(qCanvasRectangleIconHolderOutputToModule!=NULL){
	    qCanvasRectangleIconHolderOutputToModule->hide();
	}
	
	
	if(qCanvasRectangleIconHolderSynapse!=NULL){
	    qCanvasRectangleIconHolderSynapse->hide();
	}
	
	ClsCanvasNode::Hide();
    }

    int rtti () const { return nodegroupRTTI; }

private:

    ClsCanvasNodeIcon *qCanvasRectangleIconHolderInputFromModule;
    ClsCanvasNodeIcon *qCanvasRectangleIconHolderOutputToModule;
    ClsCanvasNodeIcon *qCanvasRectangleIconHolderSynapse;

};

#endif


//// Local Variables: 
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End: 

