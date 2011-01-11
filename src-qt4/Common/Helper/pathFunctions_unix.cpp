/**
 * UNIX implementation of path functions needed to load libraries.
 *
 * The declarations of the functions are platform-independent.
 */

// @@@ 10/12/2002 [jmb] It's possible that this code cannot be shared
// between FE and Prc.  Be prepared to change the implementations to
// fit the differing requirements of these programs.

#include <iostream>
using std::cout;
using std::endl;

// Headers needed for UNIX directory handling.
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "pathFunctions.hpp"

#ifdef DEBUG_PATHFUNCTIONS
static const bool bDebugPathFunctions = true;
#else
static const bool bDebugPathFunctions = false;
#endif

//////////////////////////////////
// Constants for UNIX filesystems.
char
iqrcommon::getDirectorySeparator()
{
    return '/';
}

char
iqrcommon::getExtensionSeparator()
{
    return '.';
}

string
iqrcommon::getLibraryExtension()
{
#if defined(_APPLE)
    return string(1, getExtensionSeparator()) + "dylib";
#elif defined(LINUX)
    return string(1, getExtensionSeparator()) + "so";
#endif
}
//
//////////////////////////////////

/** 
 * Tests the extension of a file name against a supplied extension.
 *
 * The extension can be supplied either with or without the separator
 * character.
 *
 * @return True if the filename extension and the supplied extension
 * match.  Also returns true is an empty extension is requested (all
 * files match).  Returns false if the file has an extension which
 * doesn't match the specified extension pattern.
 * @param _strFileName File name to test.
 * @param _strExtension File name extension to test for.
 */
bool 
iqrcommon::fileNameHasExtension(string _strFileName, string _strExtension)
{
    if (_strExtension.empty()) {
	return true;
    }

    // Search for the last extension in the filename.
    string::size_type 
	position = _strFileName.find_last_of(getExtensionSeparator());

    if (position == string::npos) {
	// File name has no extension
	return false;
    }

    // Get the extension from the file name
    string fileExtension;
    // If the supplied extension includes the separator character...
    if (_strExtension.find_first_of(getExtensionSeparator()) == 0) {
	// ...include the separator character
	fileExtension = _strFileName.substr(position);
    } else {
	// ...else strip the separator
	fileExtension = _strFileName.substr(position + 1);	
    }
    
    // Compare the extensions
    return fileExtension == _strExtension;
}

/** 
 * Get the base name of a file.
 *
 * The string returned is the file name without any extensions.
 * 
 * @return Base name of the file.
 * @param _strFileName File name.
 */
string
iqrcommon::getFileBaseName(string _strFileName)
{
    string::size_type position;
    string fileName;
    
    position = _strFileName.find_last_of(getDirectorySeparator());
    if (position != string::npos) {
	// Strip away all directory names.
	fileName = _strFileName.substr(position + 1);
    } else {
	fileName = _strFileName;
    }
    
    position = fileName.find_first_of(getExtensionSeparator());

    if (position == string::npos) {
	// File name has no extension
	return fileName;
    }

    // Strip the extension
    return fileName.substr(0, position);    
}

/** 
 * Get the list of files with the specified extension from the given
 * directory.
 *
 * This function tests the the file status of each file to ensure it
 * is a regular file or a symlink to a regular file, and also checks
 * the extension on the filename if an extension was specified.  No
 * checking of the file contents is performed.
 *
 * @return List of files or symlinks to files with the specified
 * extension.  If an error occurs, the list may be empty - the caller
 * must check for this case.
 * @param _strDirectory Path of the directory to be searched.
 * @param _strExtension File name extension (e.g. for text files,
 * specify ".txt").
 */
list<string>
iqrcommon::getListFiles(string _strDirectory, string _strExtension)
{
    list<string> fileList;

    // Ensure the directory name ends with a separator.
    string directoryName(addDirectorySeparator(_strDirectory));

    // Check the path name really describes a directory
    struct stat directoryStatus;
    if (stat(directoryName.c_str(), &directoryStatus) < 0) {
	// Error getting status of directory
	return fileList;
    }

    if (S_ISDIR(directoryStatus.st_mode) == 0) {
	// Specified path is not a directory
	return fileList;
    }

    struct dirent* pDirEntry;
    DIR* pDir;
    
    pDir = opendir(directoryName.c_str());
    if (pDir == NULL) {
	// Couldn't open directory
	return fileList;
    }

    struct stat entryStatus;
    string entryName;
    while ((pDirEntry = readdir(pDir)) != NULL) {
	entryName = directoryName + pDirEntry->d_name;

	if (stat(entryName.c_str(), &entryStatus) == 0) {
	    if (S_ISREG(entryStatus.st_mode)) {
		// Entry is a regular file.
		if (fileNameHasExtension(pDirEntry->d_name, _strExtension)) {
		    // Entry has matching extension
		    fileList.push_back(pDirEntry->d_name);
		}
	    }
	}
    }

    // No error checking here, there is nothing to be done if the call
    // failed.
    closedir(pDir);

    if (bDebugPathFunctions) {
	// Print out resulting file list.
	cout << endl
	     << "The following files match the extension: "
	     << _strExtension
	     << endl;
	for (list<string>::iterator it = fileList.begin(); 
	     it != fileList.end(); ++it) {
	    cout << "    "
		 << *it
		 << endl;
	}
    }

    return fileList;
}

/** 
 * Ensures that the path finishes with a directory separator character.
 * 
 * @return Directory name with terminating directory separator.
 * @param _strPath Unformatted path.
 */
string
iqrcommon::addDirectorySeparator(string _strPath)
{
    // Check for empty strings to avoid accessing invalid memory addresses.
    if (_strPath.empty()) {
	// @@@ 09/01/03 [jmb] What should be returned here?  On
	// UNIX/Linux, the current implementation returns the path to
	// the root directory.
	return _strPath;
    }
    
    if (_strPath[_strPath.length()-1] == getDirectorySeparator()) {
	// The last character is a directory separator.
	return _strPath;
    }

    // Append an extra directory separator at the end of the pathname.
    return _strPath + getDirectorySeparator();
}

/** 
 * Get the list of libraries within a directory.
 *
 * @return List of libraries found in the specified directory.
 * @param _strDirectory Name of the directory to check.
 */
list<string>
iqrcommon::getListLibraries(string _strDirectory)
{
    return getListFiles(_strDirectory, getLibraryExtension());
}

/** 
 * Form a path name by merging a directory name and file name.
 * 
 * This function guarantees one directory separator character is
 * included after the directory name.  The directory name can be
 * absolute or relative.
 *
 * @return Path name.
 * @param _strDirectory Directory name.
 * @param _strFileName File name.
 */
string
iqrcommon::getPathToFile(string _strDirectory, 
			 string _strFileName)
{
    return addDirectorySeparator(_strDirectory) + _strFileName;
}
