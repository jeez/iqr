/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author$ 
 ** 
 ** Created: Thu Jan  9 18:21:01 2003
 ** Time-stamp: <Mon Sep 01 2003 18:27:49 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#ifndef NEURONMANAGER_HPP
#define NEURONMANAGER_HPP

#include <list>
using std::list;
#include <string>
using std::string;

#include <Common/Item/neuron.hpp>
using iqrcommon::ClsNeuron;

#include "feTypeManager.hpp"

namespace iqrfe {

    void initializeNeuronManager(list<string> lstPaths);

    // @@@ 09/01/03 [jmb] Add typedef for neuron manager here.
    typedef ClsTypeManager<ClsNeuron> NeuronManager;
}

#endif
