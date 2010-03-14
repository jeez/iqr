/**
 * Includes code for loading dynamic libraries.
 *
 * Client code should include this platform independent header.  This
 * file handles the inclusion of platform-specific implementations of
 * the type loader.  The required OS is selected via precompiler
 * flags.  Currently supported flags are:
 *
 *     OS       FLAG
 *     --       ----
 *     linux:   -DLINUX
 *     windows: -DWIN32
 *
 * The Makefile for each platform must also ensure that the correct
 * implementation files are used.
 *
 * If nothing is included by this file, the program will not compile
 * as no definition of the type loader class will be found.  No
 * default definition is provided.
 *
 */

#ifndef LIBRARYLOADER_HPP
#define LIBRARYLOADER_HPP

#ifdef LINUX
#include "libraryLoader_unix.hpp"
#endif

#ifdef _WINDOWS
#include "libraryLoader_windows.hpp"
#endif


// Add other OS specific versions here.

#endif
