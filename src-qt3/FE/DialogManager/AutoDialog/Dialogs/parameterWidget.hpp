/****************************************************************************
 ** $Filename: parameterWidget.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Mon Feb  5 22:54:35 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef PARAMETERWIDGET_HPP
#define PARAMETERWIDGET_HPP

#include <map>
#include <string>

#include <qframe.h>
#include <qwidget.h>

namespace iqrcommon {
    class ClsParameter;
}

namespace iqrfe {

    using std::map;
    using std::string;
    using iqrcommon::ClsParameter;

    class ClsParameterWidget;

    // For convenience, define a parameter widget container.
    typedef map<string,ClsParameterWidget*> ParameterWidgetMap;

    class ClsParameterWidget : public QWidget
    {
	Q_OBJECT
    
    public:
	ClsParameterWidget(ClsParameter &_parameter,
			   bool _bStretchable, 
			   QWidget *_pqwgtParent, 
			   const char *_pcName);
	virtual ~ClsParameterWidget();

	bool canStretch() const {return bStretchable;}

	string getName()    const {return strName;}
	string getEnabler() const {return strEnabler;}

	// Get the value as a string.  This provides a common
	// inferface for all parameter types, but will incur a
	// performance penalty for non-string parameter types due to
	// conversions to and from a string.
	virtual string getValueAsString() const = 0;
	virtual void   setValueFromString(string _strValue) = 0;

	virtual void connectWidget(const ParameterWidgetMap &_mapWidgets);

	bool valueHasChanged() const {return bValueChanged;}
	virtual void apply(ClsParameter *pParameter);

    signals:
 	void changed();

    protected:
	void showChanged(bool _bChanged);
	void connectToEnabler(const ParameterWidgetMap &_mapWidgets);
	ClsParameterWidget* findWidget(const ParameterWidgetMap &_mapWidgets,
				       string _strWidgetName);

	bool bValueChanged;

	// Constants which define the widgets' look & feel.
	static const int SMALL_ATOM_HEIGHT = 14;
	static const int DEFAULT_BORDER = 0;
	static const int DEFAULT_FRAME_STYLE = QFrame::NoFrame;

	static const int SMALL_HSPACE = 10;
	static const int SMALL_VSPACE = 8;

    private:
	string strName, strEnabler;

	QPalette *pqpalChanged;

	bool bStretchable;
    };
};

#endif
