/****************************************************************************
 ** $Filename: ClsQlstItemConnection.h
 ** $Header$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate$
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSQLSTITEMCONNECTION_H
#define CLSQLSTITEMCONNECTION_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>

#include <QTreeWidget>

#include <qwidget.h>


#include <ClsQBaseItem.h>

#include <ClsSysFileNode.h>
#include <ClsBaseConnection.h>

#include <browser/connection.xpm>

namespace iqrprc {
     class ClsParameter;
}


class ClsQlstItemConnection: public QWidget, public ClsQBaseItem { 

public:
     ClsQlstItemConnection(ClsQBaseItem *_parent, string _strConnectionID, int _iType) : 
	 ClsQBaseItem( _parent, _strConnectionID, ClsFESystemManager::ITEM_CONNECTION), iType(_iType) {

	if(iType == ClsBaseConnection::CONN_EXCITATORY){
	    col.setNamedColor("red");
	}
	else if(iType == ClsBaseConnection::CONN_INHIBITORY){
	    col.setNamedColor("blue");
	}
	else {
	    col.setNamedColor("green");
	}

	QTreeWidgetItem::setIcon(0, QIcon(browser_connection));


     };
     

    void setConnectionName(string _strName) { 
	setItemName(_strName);
    };

    string getConnectionName() { return strName; };
    string getConnectionID() { return strID; };
    
    void setConnectionType(int iType){
	if(iType == ClsBaseConnection::CONN_EXCITATORY){
	    col.setNamedColor("red");
	}
	else if(iType == ClsBaseConnection::CONN_INHIBITORY){
	    col.setNamedColor("blue");
	}
	else {
	    col.setNamedColor("green");
	}
    }

    
private slots:

private:

void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment ){
    QColorGroup _cg( cg );
    QColor c = _cg.text();
    
    _cg.setColor( QColorGroup::Text, col );
//FIX    QTreeWidgetItem::paintCell( p, _cg, column, width, alignment );
    _cg.setColor( QColorGroup::Text, c );



}


int iType;
//    QColorGroup qcg;
    QColor col;
};

#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
