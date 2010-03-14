#ifndef CLSDIAGITEMGROUP_H
#define CLSDIAGITEMGROUP_H

#include <string> 
#include <vector> 

#include <QGraphicsScene> 
#include <QFontMetrics> 

#include "ClsDiagItemIcon.h"
#include "ClsDiagItem.h"

using namespace std;

static const int nodegroupRTTI = 984376;

class ClsDiagItemGroup :  public ClsDiagItem {
public:
    ClsDiagItemGroup ( int _iType, int _x, int _y, int _width, int _height, QGraphicsItem * _parent, string _strName, string _strID ): 
	ClsDiagItem(_iType, _x, _y, _width, _height, _parent, _strName, _strID) {

	prepareGeometryChange();

	qpenNotSelected.setColor( Qt::black );
	qpenNotSelected.setWidth(1);	

	qpenSelected.setColor(Qt::red);
	qpenSelected.setWidth(2);

 	setPen( qpenNotSelected );

	drawLabel();
	drawShadow();
	drawAPS();

 	

	clsDiagItemIconInputFromModule = NULL;
	clsDiagItemIconOutputToModule = NULL;
	clsDiagItemIconSynapse = NULL;
    };

    void setHasInputFromModule(bool b) {
//	cout << "ClsDiagItemGroup::setHasInputFromModule"<< endl;
//	cout << "strName: " << strName << endl;
//	cout << "b :" << (b ? "true" : "false") << endl;
	if(b){
	    if(clsDiagItemIconInputFromModule==NULL){
		int iX = this->rect().right();
		int iY = this->rect().top();
		clsDiagItemIconInputFromModule = new ClsDiagItemIcon(this, ClsDiagItemIcon::ICON_GROUP_MODULE_IN);
		clsDiagItemIconInputFromModule->setPos(iX, iY - clsDiagItemIconInputFromModule->rect().height()  );
		clsDiagItemIconInputFromModule->setZValue(1);
		clsDiagItemIconInputFromModule->show();
	    }
	} else {
	    if(clsDiagItemIconInputFromModule!=NULL){
		clsDiagItemIconInputFromModule->hide(); 
		delete clsDiagItemIconInputFromModule;
		clsDiagItemIconInputFromModule = NULL;
	    }

	}
    };

    void setHasOutputToModule(bool b) {
//	cout << "ClsDiagItemGroup::setHasOutputToModule"<< endl;
//	cout << "strName: " << strName << endl;
//	cout << "b :" << (b ? "true" : "false") << endl;
	if(b){
	    if(clsDiagItemIconOutputToModule==NULL){
		int iX = this->rect().left();
		int iY = this->rect().bottom();
		clsDiagItemIconOutputToModule = new ClsDiagItemIcon(this, ClsDiagItemIcon::ICON_GROUP_MODULE_OUT);
		clsDiagItemIconOutputToModule->setPos(iX + this->rect().width(), iY /*- clsDiagItemIconOutputToModule->height()*/  );
		clsDiagItemIconOutputToModule->setZValue(1);
		clsDiagItemIconOutputToModule->show();
	    }
	} else {
	    if(clsDiagItemIconOutputToModule!=NULL){
		clsDiagItemIconOutputToModule->hide();
		delete clsDiagItemIconOutputToModule;
		clsDiagItemIconOutputToModule = NULL;
	    }
	}
    };

    void setHasInputFromSynapse(bool b) {
//	cout << "ClsDiagItemGroup::setHasInputFromSynapse"<< endl;
	if(b){
	    if(clsDiagItemIconSynapse==NULL){
		int iX = this->rect().left();
		int iY = this->rect().top();
		clsDiagItemIconSynapse = new ClsDiagItemIcon(this, ClsDiagItemIcon::ICON_GROUP_SYNAPSE_IN);
		clsDiagItemIconSynapse->setPos(iX, iY - clsDiagItemIconSynapse->rect().height()  );
		clsDiagItemIconSynapse->setZValue(1);
		clsDiagItemIconSynapse->show();
	    }
	} else {
	    if(clsDiagItemIconSynapse!=NULL){
		clsDiagItemIconSynapse->hide();
		delete clsDiagItemIconSynapse;
		clsDiagItemIconSynapse = NULL;
	    }
	}
    };

    void Hide(){
	if(clsDiagItemIconInputFromModule!=NULL){
	    clsDiagItemIconInputFromModule->hide();
	}
	
	
	if(clsDiagItemIconOutputToModule!=NULL){
	    clsDiagItemIconOutputToModule->hide();
	}
	
	
	if(clsDiagItemIconSynapse!=NULL){
	    clsDiagItemIconSynapse->hide();
	}
	
	ClsDiagItem::Hide();
    }

    int type() const { return nodegroupRTTI; }


private:

    ClsDiagItemIcon *clsDiagItemIconInputFromModule;
    ClsDiagItemIcon *clsDiagItemIconOutputToModule;
    ClsDiagItemIcon *clsDiagItemIconSynapse;

};

#endif


//// Local Variables: 
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End: 

