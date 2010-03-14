/****************************************************************************
 ** $Filename: widgetFactory.hpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Sat Dec 29 14:44:51 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef WIDGETFACTORY_HPP
#define WIDGETFACTORY_HPP

class QWidget;

// #include "parameter.hpp"
// #include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsParameter;
};

namespace iqrfe {

//     using namespace iqrcommon;
    class ClsParameterWidget;

    class ClsWidgetFactory
    {
    public:
	ClsParameterWidget* createWidget(iqrcommon::ClsParameter *_pParameter, 
					 QWidget *_pqwgtParent);
    };

};

#endif
