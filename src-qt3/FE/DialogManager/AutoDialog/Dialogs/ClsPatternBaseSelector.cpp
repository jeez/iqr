#include <iostream>
#include "ClsPatternBaseSelector.h"
#include "ClsBaseAutoGroup.h"

ClsPatternBaseSelector::ClsPatternBaseSelector( QWidget *parent, ClsBaseAutoGroup *_changeReceiver, const char *name ) : 
    QFrame( parent, name ), changeReceiver(_changeReceiver) {};


string ClsPatternBaseSelector::Type() { return strType; };



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:

