/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Sun Nov 23 16:56 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description: 
 **
 *****************************************************************************/

#ifndef XREFLIST_HPP
#define XREFLIST_HPP

#include <list>
using namespace std;

namespace iqrcommon {
    class ClsXRef;
    typedef list<ClsXRef*> XRefList;
};

#endif
