#include <iostream>
#include "ClsPatternBaseSelector.h"
#include "ClsBaseAutoGroup.h"

ClsPatternBaseSelector::ClsPatternBaseSelector( ClsBaseAutoGroup *_changeReceiver, const char *name ) : 
    QFrame( ), changeReceiver(_changeReceiver) {
};


string ClsPatternBaseSelector::Type() { return strType; };



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:

