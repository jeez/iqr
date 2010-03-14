/****************************************************************************
 ** $Filename: ClsQlstItemGroup.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Sep  9 22:21:05 2001
 ** $Date: 2001/10/16 09:58:28 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/
 
#ifndef CLSQLSTITEMGROUP_H
#define CLSQLSTITEMGROUP_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>

#include <QTreeWidget>
#include <qwidget.h>


#include <ClsQBaseItem.h>
#include <ClsSysFileNode.h>

//--#include <browser/group.xpm>

namespace iqrprc {
     class ClsParameter;
}

class ClsQlstItemGroup: public QWidget, public ClsQBaseItem { 

public:
     ClsQlstItemGroup(ClsQBaseItem *_parent, string _strGroupID) : ClsQBaseItem( _parent, _strGroupID, ClsFESystemManager::ITEM_GROUP) {
	  strPrcID = _parent->getID();
//--	  QTreeWidgetItem::setIcon(0, QIcon(browser_group));
     };
    
    void setColor(string strColor){
	QPixmap pixmap = QPixmap(16, 16);
	pixmap.fill(Qt::white);
	QLinearGradient gradient(4, 4, 12, 12);
	gradient.setColorAt(0, QColor(strColor.c_str()));
	gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0.3));
	QPainter painter(&pixmap);
	painter.fillRect(0, 0, 16, 16, gradient);
	painter.setPen(QPen(QBrush(Qt::gray), 2)); 
	painter.drawRect( 0, 0, 16, 16 );
	painter.end();
	QTreeWidgetItem::setIcon(0, QIcon(pixmap));
    }

 
    void setGroupName(string _strName) { 
	setItemName(_strName);
    };

    string getGroupName() { return strName; };
    string getProcessID() { return strPrcID; };
    string getGroupID() { return strID; };
    
private slots:

private:

string strPrcID;
};

#endif /* CLSQLSTITEMGROUP_H */




//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
