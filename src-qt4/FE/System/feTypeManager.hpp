/****************************************************************************
 ** 
 ** Created: Mon Jan 13 12:52:01 2003
 **
 *****************************************************************************/

#ifndef IQRFE_TYPEMANAGER_HPP
#define IQRFE_TYPEMANAGER_HPP

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <stdexcept>
#include <sstream>
#include <string>
using namespace std;

#include <Common/Helper/compareMapValue.hpp>
// using iqrcommon::CompareMapValue;
#include <Common/Helper/pathFunctions.hpp>
// using iqrcommon::getListLibraries;
// using iqrcommon::getPathToFile;
#include <Common/Helper/typeManager.hpp>
// using iqrcommon::fileName2typeName;
using namespace iqrcommon;


namespace iqrfe {

#ifdef DEBUG_IQRFE_CLSTYPEMANAGER
  static const bool bDebugTypeManager = true;
#else
  static const bool bDebugTypeManager = false;
#endif

  //     class UnknownTypeError;

  /**
   * Class template for creation and deletion of instances of
   * dynamic types for the IQR front end.
   *
   * Types can be specified either using their type name or their
   * human-readable label.  The type name is obtained automatically
   * from the file name of the library, the label is obtained from
   * the library.  Type names are stored in the system file, labels
   * are used only in the user interface.
   *
   * The user of this class should use destroy to delete objects
   * constructed using create.
   */
  template <class T> class ClsTypeManager
    : public iqrcommon::ClsTypeManager<T>
  {
  public:
    static ClsTypeManager<T>& instance();

    void setName(string _strName);
    const string& getName() const;

    void loadTypes(string _strDirectory);

    const string& label2type(string _strLabel) const;
    const string& type2label(string _strType) const;
	
    T* createByLabel(string _strLabel);
    T* createByType(string _strType);
    void destroy(T* _pObject);

    const list<string>& getListLabels() const;

  private:
    ClsTypeManager() {}
    ~ClsTypeManager() {}

    string strName;

    typedef map<string,string> TypeLabelMap;
    TypeLabelMap mapLabelLookup;
    list<string> listLabels;
  };

  /** 
   * Get reference to type manager.
   * 
   * @return Reference to unique (singleton) type manager for class
   * T.
   */
  template <class T>
  ClsTypeManager<T>& ClsTypeManager<T>::instance()
  {
    // Each template class T will create a version of this
    // function during compilation, so each type T has its own
    // type manager.
    static ClsTypeManager<T> manager;
    return manager;
  }

  /** 
   * @param _strName Name for type manager instance.
   */
  template <class T>
  void ClsTypeManager<T>::setName(string _strName)
  {
    strName = _strName;
  }

  /**
   * @return Name of type manager instance.
   */
  template <class T>
  const string& ClsTypeManager<T>::getName() const
  {
    return strName;
  }

  /** 
   * Load all types from specified directory.
   *
   * All files within a directory should contain an object of the
   * correct type.  Incorrect types, which will not be detected
   * during loading, will cause a runtime error.
   *
   * Types with duplicate names (based on the file name) are
   * rejected.  Duplicate labels are fixed by appending a number
   * after the duplicate.  (Duplicate labels should be avoided by
   * authors of user-defined types.)
   *
   * @param _strDirectory Name of directory from which to load
   * types.
   */
  template <class T>
  void ClsTypeManager<T>::loadTypes(string _strDirectory) {
    if (bDebugTypeManager) {
      cout << "ClsTypeManager<T>::loadTypes(string _strDirectory)" << endl;
    }
    list<string> listTypes = getListLibraries(_strDirectory);
	
    typename list<string>::iterator it;
    for (it = listTypes.begin(); it != listTypes.end(); ++it) {
      string strFileName = it->data();
      string strType = fileName2typeName(strFileName);
      string strPath = getPathToFile(_strDirectory, strFileName);
      if (bDebugTypeManager) {
	cout << "strPath: " << strPath << endl;
      }



      try {
	string strLabel = loadType(strType, strPath);

	if (bDebugTypeManager) {
	  cout << "strLabel: " << strLabel << endl;
	}


	// Guard against duplicate label.
	typename TypeLabelMap::iterator entry;
	entry = mapLabelLookup.find(strLabel);
	if (entry != mapLabelLookup.end()) {
	  cerr << "WARNING: duplicate label "
	       << strLabel
	       << ", scanning for new label:"
	       << endl;
		    
	  int i = 1;
	  ostringstream tmp;
	  do {
	    tmp.str("");
	    ++i;
	    tmp << strLabel << " #" << i;
	  } while (mapLabelLookup.find(tmp.str()) 
		   != mapLabelLookup.end());
	  strLabel = tmp.str();
	}

	mapLabelLookup.insert(make_pair(strLabel, strType));
	listLabels.push_back(strLabel);
		
	if (bDebugTypeManager) {
	  cout << "Loaded " 
	       << strLabel
	       << ", stored as "
	       << strType
	       << endl;
	}
      }
      catch (iqrcommon::LoadTypeError &e) {
	cerr << getName()
	     << ": couldn't load "
	     << strFileName
	     << endl
	     << "    "
	     << e.what()
	     << endl;
      }
    }

    // Sort the list of names alphabetically.
    listLabels.sort();
  }

  /**
   * @return List of type labels.
   */
  template <class T>
  const list<string>& ClsTypeManager<T>::getListLabels() const
  {
    return listLabels;
  }

  /** 
   * Finds the type name corresponding to the specified label.
   * 
   * @return Type name.
   * @param _strLabel Label for type.
   * @exception UnknownTypeError
   *   - label not recognized.
   */
  template <class T>
  const string& ClsTypeManager<T>::label2type(string _strLabel) const
  {
    typename TypeLabelMap::const_iterator entry;
    entry = mapLabelLookup.find(_strLabel);
	
    if (entry == mapLabelLookup.end()) {
      throw UnknownTypeError(_strLabel);
    }

    return entry->second;
  }

  /** 
   * Finds the label corresponding to the specified type.
   *
   * @return Label.
   * @param _strType Type name.
   * @exception UnknownTypeError
   *   - type not recognized.
   */
  template <class T>
  const string& ClsTypeManager<T>::type2label(string _strType) const
  {
    typename TypeLabelMap::const_iterator entry;
    entry = find_if(mapLabelLookup.begin(), mapLabelLookup.end(),
		    CompareMapValue<string,string>(_strType));
	
    if (entry == mapLabelLookup.end()) {
      throw UnknownTypeError(_strType);
    }

    return entry->first;
  }

  /** 
   * Create a new object.
   *
   * The type is specified using the label.
   *
   * @return New object of the specified type.
   * @param _strLabel Label for the type of object to be created.
   * @exception UnknownTypeError
   *   - label was not recognized
   *   - type could not be found.
   */
  template <class T>
  T* ClsTypeManager<T>::createByLabel(string _strLabel)
  {
    // getTypeName exceptions passed onto caller.
    string strType = label2type(_strLabel);

    T* pNewObject;
    try {
      pNewObject = createObject(strType);
    }
    catch (iqrcommon::UnknownTypeError &e) {
      throw UnknownTypeError(_strLabel);
    }

    return pNewObject;
  }

  /** 
   * Create a new object by type name.
   *
   * @return New object of the specified type.
   * @param _strType Name the type of object to be created.
   * @exception UnknownTypeError
   *   - type name not recognized.
   */
  template <class T>
  T* ClsTypeManager<T>::createByType(string _strType)
  {
    T* pNewObject;
    try {
      pNewObject = createObject(_strType);
    }
    catch (iqrcommon::UnknownTypeError &e) {
      throw UnknownTypeError(_strType);
    }

    return pNewObject;
  }

  /** 
   * Destroys the specified object.
   *
   * @param _pObject Pointer to object to be destroyed.
   */
  template <class T>
  void ClsTypeManager<T>::destroy(T* _pObject)
  {
    destroyObject(_pObject);
  }

  /////////////
  // Exceptions
  //     class UnknownTypeError : public runtime_error
  //     {
  //     public:
  // 	/**
  // 	 * @param _strType Unknown type or label.
  // 	 */
  // 	UnknownTypeError(const string& _strType)
  // 	    : runtime_error(string("Unknown type ") + _strType)
  // 	{}
  //     };
}

#endif
