/****************************************************************************
 ** $Filename: doubleSpinBoxTest.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Sat Dec 29 16:19:05 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <qapplication.h>

#include "doubleSpinBox.hpp"

int main(int argc, char **argv)
{
    QApplication qappDoubleSpinBoxTest(argc, argv);

    iqrfe::ClsDoubleSpinBox doubleSpinBox(0.0, 1.0, 3, 0.001, 0, "Main widget");
    doubleSpinBox.show();
    qappDoubleSpinBoxTest.setMainWidget(&doubleSpinBox);

    return qappDoubleSpinBoxTest.exec();
}
