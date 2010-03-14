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

