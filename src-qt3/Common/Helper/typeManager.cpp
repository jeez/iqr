/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author$ 
 ** 
 ** Created: Thu Mar 27 00:15:56 2003
 ** Time-stamp: <Thu Mar 27 2003 00:16:54 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#include <string>
using std::string;

#include "pathFunctions.hpp"
#include "typeManager.hpp"

/**
 * Convenience function to convert a file name into a type name.
 *
 * This common definition aids compatibility between FE and Prc
 * type manager instances.
 */
string
iqrcommon::fileName2typeName(string _strFileName)
{
    return getFileBaseName(_strFileName);
}

