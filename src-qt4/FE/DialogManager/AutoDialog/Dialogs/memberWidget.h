#ifndef MEMBERWIDGET_H
#define MEMBERWIDGET_H

#include <map>
#include <string>
#include <list>

#include <QtGui/qframe.h>
#include <QtGui/qwidget.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qpushbutton.h> 
#include <QtGui/qcombobox.h>
#include <QtGui/qgroupbox.h> 

#include "item.hpp"

using namespace std;

// namespace iqrcommon {
//     class ClsItem;
// }

using namespace iqrcommon;


namespace iqrfe {


    
//    class ClsMemberWidget : public QWidget {
    class ClsMemberWidget : public QGroupBox {
	Q_OBJECT
	
    public:
	ClsMemberWidget(QWidget *parent, string strName, bool bModal, string strValue, list<string> lstValues);
	string getName() { return strName; };
	string getValue() { return strValue; };
	bool changed() { return bValueChanged; };
	
	enum {
	    HIDE = 0,
	    SHOW
	};

 
public slots:
    void toggleEdit(bool);
    void setValue(string _strValue);
    void slotSelectorEnabled(bool);
    void slotEditEnabled(bool);
    
    

private slots:	
    void buttonClick(bool bChecked);
    void setValueChanged(const QString & qstrValue);
	

    signals:
    void toggleMemberEdit(string, string, unsigned int);
    void sigChanged();

	
    private:
	static const int SMALL_ATOM_HEIGHT = 14;
	static const int DEFAULT_BORDER = 0;
	static const int DEFAULT_FRAME_STYLE = QFrame::NoFrame;
	
	static const int SMALL_HSPACE = 10;
	static const int SMALL_VSPACE = 8;
	
	static const int WIDGET_V_STRETCH = 10;

	bool bValueChanged;
	
	string strName;
	bool bModal;
	string strValue;

	

	QLabel *qlblValue;
	
	QPushButton* qpb;
	QComboBox   *qcombo;	

    };
}



#endif
