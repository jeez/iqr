#ifndef CLSQINFOTIP_H
#define CLSQINFOTIP_H

#include <string>
#include <iostream>

#include <qlabel.h>
#include <qtooltip.h>

using namespace std;

#define TIPOFFSET 20

class ClsQInfoTip : public QLabel {

public:
    ClsQInfoTip(QPoint qpLocation, string strInfo) : QLabel( NULL, "", WStyle_StaysOnTop + WStyle_Customize + WStyle_NoBorder + WStyle_Tool) {
	setPalette(QToolTip::palette ());
	setMargin(1);
	setIndent(0);
	setAutoMask( FALSE );
	setFrameStyle( QFrame::Plain | QFrame::Box );
	setLineWidth( 1 );
	setAlignment( AlignLeft | AlignTop );
	polish();
	setText( strInfo.c_str() );
	adjustSize();
	move(qpLocation.x() - TIPOFFSET, qpLocation.y() - TIPOFFSET);
	show();
    }

private:
    void leaveEvent ( QEvent * ){
//	cout << "leaveEvent ( QEvent * )" << endl;
	hide();
	close();
    }
};

#endif



//// Local Variables: 
//// mode: c++
//// End: 



    
