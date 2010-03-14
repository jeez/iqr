#ifndef CLSDIAGITEMPROCESS_H
#define CLSDIAGITEMPROCESS_H

#include <string> 
#include <vector> 

#include <QGraphicsScene> 
#include <QFontMetrics> 

#include "ClsDiagItemIcon.h"
#include "ClsDiagItem.h"



using namespace std;

static const int nodeprocessRTTI = 984375;

class ClsDiagItemProcess :  public ClsDiagItem {
public:
    ClsDiagItemProcess ( int _iType, int _x, int _y, int _width, int _height, QGraphicsItem * _parent, string _strName, string _strID ): 
	ClsDiagItem(_iType, _x, _y, _width, _height, _parent, _strName, _strID) {

	prepareGeometryChange();

	clsDiagItemIconModule = NULL;
	clsDiagItemIconLink = NULL;

	qpenNotSelected.setColor( Qt::gray );
	qpenNotSelected.setWidth(3);	
	qpenNotSelected.setBrush(Qt::Dense2Pattern);

	qpenSelected.setColor(Qt::red);
	qpenSelected.setWidth(2);

//-- 	setBrush( qbrushNotSelected );
 	setPen( qpenNotSelected );

	drawLabel();
	drawShadow();

	drawAPS();
    };


    int type() const { return nodeprocessRTTI; }

    void setModuleIcon(moduleIcon mi){
//	cout << "ClsDiagItemProcess::setModuleIcon(moduleIcon mi)" << endl;
	if(mi.data!=NULL) {
	    if(clsDiagItemIconModule != NULL){
		removeModuleIcon(); 
	    }
	    clsDiagItemIconModule = new ClsDiagItemIcon(this, mi);
	    clsDiagItemIconModule->setPos(this->rect().right() - mi.x0, 
					  this->rect().bottom() - mi.y0);
	    clsDiagItemIconModule->setZValue(1);
	    clsDiagItemIconModule->show();
	}  
    };

    void removeModuleIcon(){
//	cout << "ClsDiagItemProcess::removeModuleIcon()" << endl;
	if(clsDiagItemIconModule != NULL){
	    clsDiagItemIconModule->hide();
	    delete clsDiagItemIconModule;
	    clsDiagItemIconModule = NULL;
	}
    }

    void setModuleEnabled(bool b){
	// cout << "ClsDiagItemProcess::setModuleEnabled(bool b):";
	clsDiagItemIconModule->setEnabled(b);
    }

    void setIsExternal(bool b){
	if(b){
	    if(clsDiagItemIconLink==NULL){
		int iX = this->rect().left();
		int iY = this->rect().top();
		clsDiagItemIconLink = new ClsDiagItemIcon(this, ClsDiagItemIcon::ICON_PROCESS_LINK);
		clsDiagItemIconLink->setPos(iX - clsDiagItemIconLink->rect().width()*.5, 
					    iY - clsDiagItemIconLink->rect().height()*.5);
		clsDiagItemIconLink->setZValue(1);
		clsDiagItemIconLink->show();
	    }
	} else {
	    if(clsDiagItemIconLink!=NULL){
		clsDiagItemIconLink->hide();
		delete clsDiagItemIconLink;
		clsDiagItemIconLink = NULL;
	    }
	}

    }

    void Hide(){
	if(clsDiagItemIconModule != NULL){
	    clsDiagItemIconModule->hide();
	}
	if(clsDiagItemIconLink != NULL){
	    clsDiagItemIconLink->hide();
	}
	ClsDiagItem::Hide();
    }


private:
    ClsDiagItemIcon *clsDiagItemIconModule;
    ClsDiagItemIcon *clsDiagItemIconLink;
};

#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
