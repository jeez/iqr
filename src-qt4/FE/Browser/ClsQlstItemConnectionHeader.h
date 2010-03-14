/****************************************************************************
 ** $Filename: ClsQlstItemConnectionHeader.h
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

#ifndef CLSQLSTITEMCONNECTIONHEADER_H
#define CLSQLSTITEMCONNECTIONHEADER_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>

#include <QTreeWidget>
#include <qwidget.h>
#include <qfont.h>
#include <qpainter.h>


#include <ClsQBaseItem.h>

//#include <ClsParameter.h>
#include <ClsSysFileNode.h>
#include <ClsBaseConnection.h>

namespace iqrprc {
     class ClsParameter;
}


class ClsQlstItemConnectionHeader: public QWidget, public ClsQBaseItem { 

public:
     ClsQlstItemConnectionHeader(ClsQBaseItem *_parent) : 
	 ClsQBaseItem( _parent, "", -1) {
	 setItemName("Connections");
     };
     
   

private:

void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment ){
    QFont f( p->font() );
//    f.setItalic( true );
    f.setUnderline( true );
//    f.setBold( true );
    p->setFont( f );
//FIX    QTreeWidgetItem::paintCell( p, cg, column, width, alignment );
}

};

#endif




//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
