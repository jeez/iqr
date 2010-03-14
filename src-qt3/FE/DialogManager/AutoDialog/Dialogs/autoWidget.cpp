/****************************************************************************
 ** $Filename: autoWidget.cpp
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/DialogManager/AutoDialog/Dialogs/RCS/autoWidget.cpp,v 1.1 2003/12/04 23:54:47 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 **
 ** $CreateDate: Mon May 21 23:25:42 2001
 ** $Date: 2003/12/04 23:54:47 $
 **
 ** $Keywords:
 ** $Description: auto widget creates a QWidget-derived widget
 **               comprising parameter widgets.  It can be used
 **               in conjunction with an auto dialog frame or on
 **               its own.
 **
 *****************************************************************************/

#include <iostream>

#include <qlayout.h>
#include <qtabwidget.h>
#include <qstringlist.h>

#include "autoWidget.hpp"
#include "item.hpp"
#include "parameter.hpp"
#include "parameterList.hpp"
#include "qstringConversions.hpp"
#include "widgetFactory.hpp"
#include "xRefHolder.hpp"

//#include "xRefWidget.hpp"
//#include "xRefHolderWidget.hpp"

#define NO_STRETCH 0
#define WIDGET_STRETCH 10
#define TAB_V_STRETCH 10

#define TAB_BORDER 5

//#define DEBUG_CLSAUTOWIDGET
#ifdef DEBUG_CLSAUTOWIDGET
static const bool bDebugAutoWidget = true;
#else
static const bool bDebugAutoWidget = false;
#endif

using namespace iqrcommon;

iqrfe::ClsAutoWidget::ClsAutoWidget(ClsItem &_item,
				    QWidget *_pqwgtParent,
				    const char *_pcName)
    : QWidget(_pqwgtParent,
	      _pcName)
{
    if (bDebugAutoWidget) {
	cout << "ClsAutoWidget::ClsAutoWidget: item has "
	     << _item.getListParameters().size()
	     << " parameters"
	     << endl;
    }

    // Category is used to group parameters into different categories.
    QStringList qstrlstCategories;
    bool        bCategoryFound;

    // 1st pass. Scan the parameter list to...
    ParameterList::const_iterator itParameter;
    for (itParameter = _item.getListParameters().begin();
	 itParameter != _item.getListParameters().end();
	 ++itParameter) {

	ClsParameter &parameter = *(*itParameter);

	// ...identify the category to which each parameter belongs.
	// The category is not the same as the type, but describes the
	// (user-defined) logical groupings among the parameters.

	bCategoryFound = false;

	QStringList::Iterator qstrlstitCategory = qstrlstCategories.begin();
	while (qstrlstitCategory != qstrlstCategories.end()
	       && !bCategoryFound) {

	    if (string2QString(parameter.getCategory()) == *qstrlstitCategory) {
		// An entry for this category has already been created.
		bCategoryFound = true;
	    } else {
		qstrlstitCategory++;
	    }
	}

	if (!bCategoryFound) {
	    // Category was not found, append its name to the
	    // categories string list.
	    qstrlstCategories.append(string2QString(parameter.getCategory()));

	    if (bDebugAutoWidget) {
		cout << "ClsAutoWidget::ClsAutoWidget: category "
		     << parameter.getCategory()
		     << " added"
		     << endl;
	    }
	}
    }


    unsigned int iMinNrTabs = 0;

    // 1'nd pass. Do we have XRefHolders?
    list<string> lstXRefHolders = _item.getListOfXRefHolders();

    if(lstXRefHolders.size()>0){ /* don't show the tab if the list is empty */
	iMinNrTabs = 0;
    } else {
	iMinNrTabs = 1;
    }

    // 2nd pass, create the dialog.
    // Create factory for constructing parameter widgets.
    ClsWidgetFactory widgetFactory;

    // Create top-level layout manager for widget.
    QVBoxLayout *pqlayTop = new QVBoxLayout(this);
    QTabWidget *pqtabwgtContainer = NULL;

    // Was more than one category found?
    if (qstrlstCategories.count() >= iMinNrTabs) {

	// Yes, create the dialog using tabs.  Construct the widgets here.
	if (bDebugAutoWidget) {
	    cout << "ClsAutoWidget::ClsAutoWidget: building tab dialog"
		 << endl;
	}

	// "The QTabWidget class provides a stack of tabbed widgets."
	pqtabwgtContainer = new QTabWidget(this);

	QWidget            *pqwgtTab;
	QVBoxLayout        *pqlayTabLayout;
	ClsParameterWidget *pParameterWidget;

	// For each category...
	for (QStringList::Iterator qstrlstitCategory =
		 qstrlstCategories.begin();
	     qstrlstitCategory != qstrlstCategories.end();
	     qstrlstitCategory++) {

	    // ...create a new widget and layout.  Name of category is
	    // passed as widget name.
	    
	    string strTabCaption = 
		((*qstrlstitCategory).isEmpty() ? "Properties" : (*qstrlstitCategory).latin1());

//	    pqwgtTab = new QWidget(this, (*qstrlstitCategory).latin1());
	    pqwgtTab = new QWidget(this, strTabCaption.c_str());

	    // Layout for this tab widget.
	    pqlayTabLayout = new QVBoxLayout(pqwgtTab, TAB_BORDER);

	    bool bStretchAssigned = false;

	    ParameterList::const_iterator itParameter;
	    for (itParameter = _item.getListParameters().begin();
		 itParameter != _item.getListParameters().end();
		 ++itParameter) {

		ClsParameter *pParameter = *itParameter;

		if (pParameter->getCategory() == *qstrlstitCategory) {
		    // Use parameter widget factory to create
		    // appropriate type of parameter widget.
		    pParameterWidget = widgetFactory.createWidget(pParameter,
								  pqwgtTab);

		    if (pParameterWidget != 0) {
			// Add pointer to parameter widget to
			// parameter widget map.

			mapParameterWidgets.insert(make_pair(pParameter->getName(),
							     pParameterWidget));

			if (pParameterWidget->canStretch()) {
			    pqlayTabLayout->addWidget(pParameterWidget,
						      WIDGET_STRETCH);
			    bStretchAssigned = true;
			} else {
			    // Add parameter to current tab widget and tab layout.
			    pqlayTabLayout->addWidget(pParameterWidget,
						      NO_STRETCH);
			}

			// Make connections between dialog and parameter
			// widget here.
			connect(pParameterWidget, SIGNAL(changed()),
				this, SIGNAL(changed()));
		    } else {
			// @@@ 04/01/02 How should failed construction
			// be handled?  Currently it is ignored.
			if (bDebugAutoWidget) {
			    cout << "ClsAutoWidget::ClsAutoWidget: "
				 << "failed to create widget for parameter "
				 << pParameter->getName()
				 << endl;
			}
		    }
		}
	    }

	    if (!bStretchAssigned) {
		// No stretchable widgets were added to this tab, add
		// stretch to bottom of tab layout.  Tabs are unlikely
		// to be of equal length, and this stretch ensures
		// that all empty space is kept together at the bottom
		// of the tab panel.
		pqlayTabLayout->addStretch(TAB_V_STRETCH);
	    }

	    // Add new tab widget to tab widget container.
	    pqtabwgtContainer->addTab(pqwgtTab, pqwgtTab->name());

	    if (bDebugAutoWidget) {
		cout << "ClsAutoWidget::ClsAutoWidget: added tab "
		     << QString2string(pqwgtTab->name())//.latin1()
		     << endl;
	    }
	}

	// Add tab container to dialog's main layout.  Tab container
	// owned by dialog so loss of pointer is not important.
	/* had to move this further down
	   pqlayTop->addWidget(pqtabwgtContainer);
	*/
    } else {
	// No, use a single widget with no tab.
	if (bDebugAutoWidget) {
	    cout << "ClsAutoWidget::ClsAutoWidget: building normal dialog"
		 << endl;
	}

	// @@@ 09/08/01 TAB_BORDER should be renamed.
	QVBoxLayout *pqlayParameterLayout = new QVBoxLayout(pqlayTop,
							    TAB_BORDER);
	ClsParameterWidget *pParameterWidget;

	bool bStretchAssigned = false;

	ParameterList::const_iterator itParameter;
	for (itParameter = _item.getListParameters().begin();
	     itParameter != _item.getListParameters().end();
	     ++itParameter) {

// 	    ClsParameter &parameter = *(*itParameter);
	    ClsParameter *pParameter = *itParameter;

	    // Get appropriate parameter widget from
	    // parameter widget factory.
//  	    pParameterWidget = widgetFactory.createWidget(parameter);
 	    pParameterWidget = widgetFactory.createWidget(pParameter, this);

	    if (pParameterWidget != 0) {
		// Add pointer to parameter widget to parameter widget map.
// 		mapParameterWidgets.insert(make_pair(parameter.getName(),
		mapParameterWidgets.insert(make_pair(pParameter->getName(),
						     pParameterWidget));

		if (pParameterWidget->canStretch()) {
		    pqlayParameterLayout->addWidget(pParameterWidget,
						    WIDGET_STRETCH);
		    bStretchAssigned = true;
		} else {
		    // Add parameter to current tab widget and tab layout.
		    pqlayParameterLayout->addWidget(pParameterWidget,
						    NO_STRETCH);
		}

		// Make connections between dialog and parameter
		// widget.  One of these should trigger an update
		// from the parameter object->parameter widget,
		// which should be called when the dialog is
		// opened (ShowEvent()).
		connect(pParameterWidget, SIGNAL(changed()),
			this, SIGNAL(changed()));
	    } else {
		// @@@ 04/01/02 How should failed construction be
		// handled?  Currently it is ignored.
		if (bDebugAutoWidget) {
		    cout << "ClsAutoWidget::ClsAutoWidget: "
			 << "failed to create widget for parameter "
// 			 << parameter.getName()
			 << pParameter->getName()
			 << endl;
		}
	    }
	}

	if (!bStretchAssigned) {

	    // No stretchable widgets were added to this tab, add
	    // stretch to bottom of tab layout.  This stretch ensures
	    // that all empty space is kept together at the bottom
	    // of the tab panel.
	    pqlayParameterLayout->addStretch(TAB_V_STRETCH);

	}

    }



    // 2'nd pass, create the tabs and dialogs for XRefs.
    if(lstXRefHolders.size()>0 && pqtabwgtContainer !=NULL){
//  	QWidget            *pqwgtTab;
//	QVBoxLayout        *pqlayTabLayout;

	list<string>::iterator itXRefHolders;
	for(itXRefHolders=lstXRefHolders.begin(); itXRefHolders!=lstXRefHolders.end(); itXRefHolders++){
	    if (bDebugAutoWidget) {
		cout << "XRefHolder name: " << (*itXRefHolders) << endl;
	    }
	    const ClsXRefHolder* xRefHolder = _item.getXRefHolder(*itXRefHolders);
	    if(xRefHolder!=NULL){
		ClsXRefHolderWidget* clsXRefHolderWidget 
		    = new ClsXRefHolderWidget(_item, xRefHolder, this, xRefHolder->getCategory().c_str());
		connect(clsXRefHolderWidget, SIGNAL(changed()), this, SIGNAL(changed()));
		lstXRefHolderWidget.push_back(clsXRefHolderWidget);
		pqtabwgtContainer->addTab(clsXRefHolderWidget, xRefHolder->getCategory().c_str());
		pqlayTop->addWidget(clsXRefHolderWidget);
	    }
	}
    }

    /* DOES THIS HAVE TO BE HERE??? */
    if(pqtabwgtContainer !=NULL){
	pqlayTop->addWidget(pqtabwgtContainer);
    }

// 3rd pass, connect the widgets.
    ParameterWidgetMap::const_iterator itWidget;
    for (itWidget = mapParameterWidgets.begin();
	 itWidget != mapParameterWidgets.end();
	 ++itWidget) {

	ClsParameterWidget *pWidget = itWidget->second;

	pWidget->connectWidget(mapParameterWidgets);
    }
//     for (ClsParameterWidget *pclsWidget = tlstParameterWidgets.first();
// 	 pclsWidget != 0; pclsWidget = tlstParameterWidgets.next()) {

// 	pclsWidget->Connect(tlstParameterWidgets);

//     }
}

iqrfe::ClsAutoWidget::~ClsAutoWidget()
{

//     // Clear the pointers from the list.  Autodelete is false, so
//     // there should be no runtime problems.
//     tlstParameterWidgets.clear();

}

/**
 * Updates values in parameter widgets when the dialog is opened.
 *
 * @param *e Describes type of show event.  Spontaneous event are
 *           produced by the window manager, non-spontaneous
 *           event by the application.
 */
// void
// iqrfe::ClsAutoWidget::showEvent(QShowEvent *e)
// {

//     // Is this slot being called by the application or the window
//     // manager?
//     if (!e->spontaneous()) {

// 	// Dialog is being opened, synchronize values in parameter
// 	// widgets.
// 	ClsParameterWidget *pParameterWidget;

// 	for (pParameterWidget = tlstParameterWidgets.first ();
// 	     pParameterWidget != 0;
// 	     pParameterWidget = tlstParameterWidgets.next()) {

// 	    pParameterWidget->Update();

// 	}

// 	bWidgetChanged = false;

//     }

// }


/**
 * Apply changes.
 *
 * This function does not update the parent object directly.  Instead,
 * the Apply() member function of each parameter widget is called in
 * turn.
 */
// void
// iqrfe::ClsAutoWidget::Apply()
// {

//     bool bChangesApplied = false;

//     for (ClsParameterWidget *pclsWidget = tlstParameterWidgets.first();
// 	 pclsWidget != 0; pclsWidget = tlstParameterWidgets.next()) {

// 	// Widget apply functions return true if a new value was
// 	// assigned.
// 	bChangesApplied = pclsWidget->Apply();

//     }

//     bWidgetChanged = false;

//     // This signal is useful when the parent has undo functions.
//     if (bChangesApplied) {

// 	emit EndOfApply();

//     }

// }

// void
// iqrfe::ClsAutoWidget::Undo()
// {

//     if (bWidgetChanged) {

// 	// The value in at least one widget has changed since the last
// 	// apply.  Undo to last applied values locally.
// 	for (ClsParameterWidget *pclsWidget = tlstParameterWidgets.first ();
// 	     pclsWidget != 0; pclsWidget = tlstParameterWidgets.next()) {

// 	    pclsWidget->Update();

// 	}

// 	bWidgetChanged = false;

//     } else {

// 	// Trigger undo in parent item.

// 	// Check whether Undo information is available, disable button is not.

//     }

// }

// void
// iqrfe::ClsAutoWidget::WidgetChange()
// {

//     // Flag a change in a widget.
//     bWidgetChanged = true;

//     emit Changed();

// }

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
