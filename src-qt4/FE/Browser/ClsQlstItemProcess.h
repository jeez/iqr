/****************************************************************************
 ** $Filename: ClsQlstItemProcess.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Sep 16 15:41:48 2001
 ** $Date: 2001/10/16 09:59:35 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQLSTITEMPROCESS_H
#define CLSQLSTITEMPROCESS_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>
#include <QTreeWidget>
#include <ClsQBaseItem.h>

//--#include <browser/process.xpm>

class ClsQlstItemProcess: public ClsQBaseItem {

public:
     ClsQlstItemProcess( ClsQBaseItem *_parent, string _strPrcID) : ClsQBaseItem( _parent, _strPrcID, ClsFESystemManager::ITEM_PROCESS) {
//--	 QTreeWidgetItem::setIcon(0, QIcon(browser_process));
    };


    void setColor(string strColor){
	QPixmap pixmap = QPixmap(16, 16);
	pixmap.fill(Qt::transparent);
	QRadialGradient gradient(8, 8, 8, 8, 8);
	gradient.setColorAt(0, QColor(strColor.c_str()));
	gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0.3));
	QPainter painter(&pixmap);
	QPainterPath qpp;
	qpp.addEllipse ( 0, 0, 16, 16);
	painter.fillPath(qpp, gradient);
	painter.end();
	setIcon(0, QIcon(pixmap));
    }


     string getProcessID() { return strID; };     
private:
};


#endif /* CLSQLSTITEMPROCESS_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
