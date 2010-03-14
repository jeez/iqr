/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2002 by Mark Blanchard
 ** $Author$
 ** 
 ** Created: Tue Nov 26 14:39:32 2002
 ** Time-stamp: <Thu Dec 12 2002 23:10:08 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

/**
 * Includes code for loading dynamic libraries.
 *
 * Client code should include this platform independent header.  This
 * file handles the inclusion of platform-specific implementations of
 * the type loader.  The required OS is selected via precompiler
 * flags.  Currently supported flags are:
 *
 *     OS     FLAG
 *     --     ----
 *     linux: -DLINUX
 *
 * The Makefile for each platform must also ensure that the correct
 * implementation files are used.
 *
 * If nothing is included by this file, the program will not compile
 * as no definition of the type loader class will be found.  No
 * default definition is provided.
 *
 * Each library loader class must implement the following functions:
 *
 *     
 */

#ifndef LIBRARYLOADER_HPP
#define LIBRARYLOADER_HPP

#ifdef LINUX
#include "libraryLoader_unix.hpp"
#endif

// Add other OS specific versions here.

#endif
