/****************************************************************************
 ** $Filename: xRefWidget.hpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Dec  5 18:58:29 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSXREFWIDGET_HPP
#define CLSXREFWIDGET_HPP

#include <qcombobox.h>
#include <qframe.h>
#include <qwidget.h>

#include "xRefHolder.hpp"

using namespace std;

namespace iqrfe {
    
    class ClsXRefWidget : public  QWidget
    {
	Q_OBJECT
	
    public:	
	ClsXRefWidget(iqrcommon::ClsXRef* _xRef, 
		      list<string> lstXRefValues, 
		      string strTargetName,
		      QWidget *_pqwgtParent, 
		      const char *_pcName = 0);
 	~ClsXRefWidget();

/*
	string getValue() const;
	string getValueAsString() const;
	void setValue(int _iSelected);
	void setValueFromString(string _strValue);
*/
	
//	void connectWidget(const ParameterWidgetMap &_mapWidgets);
	bool valueHasChanged() const {return bValueChanged;}
//	void apply(ClsParameter *pParameter);

	const string getValue() const { return  pqcmbValueWidget->currentText().latin1(); };

    private slots:
        void setValueChanged();

    signals:
 	void changed();

    private:
	QComboBox   *pqcmbValueWidget;
	iqrcommon::ClsXRef* xRef;
	

	bool bValueChanged;
	bool bStretchable;

	// Constants which define the widgets' look & feel.
	static const int SMALL_ATOM_HEIGHT = 14;
	static const int DEFAULT_BORDER = 0;
	static const int DEFAULT_FRAME_STYLE = QFrame::NoFrame;

	static const int SMALL_HSPACE = 10;
	static const int SMALL_VSPACE = 8;

	friend class ClsXRefHolderWidget;

    };
};

#endif
//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
