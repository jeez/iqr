/****************************************************************************
 ** $Filename: stateVariableList.hpp
 **
 ** $Author: Mark Blanchard
 ** 
 ** $Description: For convenience, this header defines a typedef for a
 **               state variable object(pointer)s.
 **  
 **               Owner's of instances of this type are responsible
 **               for ensuring that the objects pointed to by the
 **               entries in the list are deleted correctly.  Deleting
 **               the list will not delete these objects
 **               automatically.
 **
 *****************************************************************************/

#ifndef STATEVARIABLELIST_HPP
#define STATEVARIABLELIST_HPP

#include <list>
// using std::list;
using namespace std;

namespace iqrcommon {

    class ClsStateVariable;
    typedef list<ClsStateVariable*> StateVariableList;

};

#endif
