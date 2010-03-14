/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2002 by Mark Blanchard
 ** $Author$ 
 ** 
 ** Created: Tue Dec 10 12:03:03 2002
 ** Time-stamp: <Wed Mar 26 2003 23:52:28 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

/**
 * Declaration of helper functions for obtaining path names, file
 * lists and file extensions for loading libraries.
 *
 * The implementations of these functions are platform dependent, and
 * may also vary between FE and Prc.  All implementations and all
 * client code should include this header file - the correct
 * implementation must be selected in the makefile.
 */

#ifndef PATHFUNCTIONS_HPP
#define PATHFUNCTIONS_HPP

#include <list>
using std::list;
#include <string>
using std::string;

namespace iqrcommon {

    char getDirectorySeparator();
    char getExtensionSeparator();
    string getLibraryExtension();

    bool fileNameHasExtension(string _strFileName, string _strExtension);
    string getFileBaseName(string _strFileName);
    list<string> getListFiles(string _strDirectory, string _strExtension);

    string addDirectorySeparator(string _strPath);
    list<string> getListLibraries(string _strDirectory);
    string getPathToFile(string _strDirectory, string _strFileName);
}

#endif
