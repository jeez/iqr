/****************************************************************************
 ** $Filename: autoWidget.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Feb  6 00:25:26 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSAUTOWIDGET_HPP
#define CLSAUTOWIDGET_HPP

//#include <qlist.h> debian sarge didn't like this...
#include <qwidget.h>

#include "parameterWidget.hpp"
#include "xRefHolderWidget.hpp"

namespace iqrcommon {
    class ClsItem;
}

namespace iqrfe {

    using iqrcommon::ClsItem;

    class ClsAutoWidget : public QWidget
    {    
	Q_OBJECT
	
    public:    
	ClsAutoWidget(ClsItem &_item,
		      QWidget *_pqwgtParent = NULL, 
		      const char *_pcName = 0);
	~ClsAutoWidget();
	
	const ParameterWidgetMap& getParameterWidgets() const {
	    return mapParameterWidgets;
	}

	const list<ClsXRefHolderWidget*>& getXRefHolderWidgets() const {
	    return lstXRefHolderWidget;
	}


//     public slots:
//         void apply();
// 	void undo();

    signals:
//         void endOfApply();
	void changed();
    
    protected:
// 	// The parameter widgets should be synchronized with their
// 	// objects when the widget is shown.
// 	virtual void showEvent(QShowEvent *);
	
//     private slots:
//         void widgetChange();

    private:
// 	const ClsItem &item;

	// A list of the child parameter widgets is needed when
	// applying changes.
// 	TParameterWidgetList tlstParameterWidgets;
	ParameterWidgetMap mapParameterWidgets;
	list<ClsXRefHolderWidget*> lstXRefHolderWidget;
	
// 	bool bWidgetChanged;
    };

};

#endif
