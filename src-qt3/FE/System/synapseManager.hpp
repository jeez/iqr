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

#ifndef SYNAPSEMANAGER_HPP
#define SYNAPSEMANAGER_HPP

#include <list>
using std::list;
#include <string>
using std::string;

#include <Common/Item/synapse.hpp>
using iqrcommon::ClsSynapse;

#include "feTypeManager.hpp"

namespace iqrfe {

    void initializeSynapseManager(list<string> lstPaths);
    typedef ClsTypeManager<ClsSynapse> SynapseManager;
}

#endif
