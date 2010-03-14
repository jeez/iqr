/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author$ 
 ** 
 ** Created: Tue Sep  2 11:01:15 2003
 ** Time-stamp: <Tue Sep 02 2003 11:03:14 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#ifndef MODULEMANAGER_HPP
#define MODULEMANAGER_HPP

#include <list>
using std::list;
#include <string>
using std::string;

#include <Common/Item/module.hpp> 
using iqrcommon::ClsModule;

#include "feTypeManager.hpp"

namespace iqrfe {

    void initializeModuleManager(list<string> lstPaths);
    typedef ClsTypeManager<ClsModule> ModuleManager;
}

#endif
