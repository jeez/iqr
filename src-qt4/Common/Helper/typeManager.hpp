#ifndef TYPEMANAGER_HPP
#define TYPEMANAGER_HPP

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <typeinfo>
using namespace std;

#include "libraryLoader.hpp"

//#define DEBUG_TYPEMANAGER

#ifdef DEBUG_TYPEMANAGER
static const bool bDebugTypeManager = true;
#else
static const bool bDebugTypeManager = false;
#endif

namespace iqrcommon {
    class LoadTypeError;
    class UnknownTypeError;

    string fileName2typeName(string _strFileName);

    /**
     * Type manager class template creates an object factory for types
     * with a common base class.
     *
     * Functions which create and destroy objects of each type are
     * stored together with the name used to identify the type.  The
     * type name is used whenever objects of the type must be created.
     * The actual naming scheme used is up to the caller.  The
     * identification of objects for destruction is made
     * automatically.
     *
     * For dynamically loaded types the following functions are
     * required:
     *   - label()   - a function which returns a human-readable name
     *                 for the type, which can be used in user 
     *                 interfaces.
     *   - create()  - a function which returns a new instance of the
     *                 class defined in the library.
     *   - destroy() - a function which deletes instances of the class 
     *                 defined in the library.
     *
     * The symbols used can be specified at runtime, the names shown
     * here are not fixed.
     *
     * All classes stored in a type manager must have a common base
     * class.  Pointers to new objects are returned via base class
     * pointers.  Existing objects are deleted via base class
     * pointers, so the base class must have a virtual destructor.
     */
    template <class T> class ClsTypeManager
    {
    private:
	/** Create function makes a new object of a type. */
	typedef T* CreateFn();
	/** Destroy function deletes an existing object. */
	typedef void DestroyFn(const T*);

    public:

    protected:
	ClsTypeManager() {}
	~ClsTypeManager() {}
	
 	string loadType(string _strTypeName, 
			string _strFileName,
			string _strLabelSymbol   = "label",
			string _strCreateSymbol  = "create",
			string _strDestroySymbol = "destroy");
	bool hasType(string _strTypeName);

	T* createObject(string _strTypeName);
	void destroyObject(T* _pObject);

    private:
	// Disable copy constructor and assignment operator.
	ClsTypeManager(const ClsTypeManager&);
	ClsTypeManager& operator=(const ClsTypeManager&);

	// TypeMap holds a type's create function pointer with the
	// type name supplied by the caller as the index key.
	typedef map<string,CreateFn*> TypeMap;
	TypeMap mapTypes;
	typedef typename TypeMap::iterator TypeMapIterator;

	// DestroyMap holds a type's destroy function pointer with the
	// typeid name as the index key.  This association allows
	// objects to be deleted without the caller specifying the
	// type explicitly.
	typedef map<string,DestroyFn*> DestroyMap;
	DestroyMap mapDestroy;

	// Type loader handles loading and closing of files.
	ClsLibraryLoader typeLoader;
    };
    
    /** 
     * Load a type from a file.
     *
     * The new type is stored under the supplied type name.  New
     * objects of the type are created using this name.  Type names
     * must be unique.
     *
     * The caller can specify alternative strings for the creator,
     * destroyer and label symbols used during loading.
     *
     * WARNING: loading a type derived from a class other than the
     * template class T will not be detected here.  This function must
     * use old-style C casting to cast the void pointers obtained from
     * the library into the create and destroy function pointers.  No
     * checking can be performed, so these recast pointers will fail
     * when they are first called.
     * 
     * NOTE: for IQR, it might be useful to add an extra symbol to
     * each library which describes the type.  The value of this
     * symbol could be compared here and any incorrect types rejected.
     * However this scheme is also not bullet proof, as the library
     * author might specify the incorrect type via the new symbol.
     *
     * @return The label to use for the type as specified in the file.
     *
     * @param _strTypeName Name under which to store the type.  This
     * name is used to request new objects of the type.
     * @param _strFileName Name of file to load.
     * @param _strLabelSymbol Symbol for label of dynamically loaded type.
     * @param _strCreateSymbol Symbol for create function, used for
     * building objects of the loaded type.
     * @param _strDestroySymbol Symbol for destroy function, used for
     * destroying objects of the loaded type.
     *
     * @exception LoadTypeError 
     *   - the type name is already in use.
     *   - the file cannot be loaded.
     *   - a symbol cannot be resolved.
     *   - an instance of the new type cannot be created.
     */
    template <class T> 
    string ClsTypeManager<T>::loadType(string _strTypeName,
				       string _strFileName,
				       string _strLabelSymbol,
				       string _strCreateSymbol,
				       string _strDestroySymbol)
    {
	if (hasType(_strTypeName)) {
	    // Don't overwrite the existing entry.
	    throw LoadTypeError(string("Type name ") 
				+ _strTypeName 
				+ " already in use");
	}

	try {
	    typeLoader.load(_strFileName);
	    
	    // The return type of typeLoader::getSymbol is platform
	    // dependent.  On Linux, the returned void* must be cast
	    // using C-style casting here, as void* cannot be cast
	    // using the new C++-style casts under ISO C++.
	    typedef const char* charFn();
	    charFn* pLabelFn = (charFn*)typeLoader.getSymbol(_strFileName, _strLabelSymbol);
	    CreateFn* pCreate = (CreateFn*)typeLoader.getSymbol(_strFileName, _strCreateSymbol);
	    DestroyFn* pDestroy = (DestroyFn*)typeLoader.getSymbol(_strFileName, _strDestroySymbol);

	    // Create an instance in order to get the type_info.
	    T* pInstance = pCreate();
	    if (!pInstance) {
		throw LoadTypeError(string("Couldn't create instance of type ")
				    + _strTypeName);
	    }
	    string strTypeId = typeid(*pInstance).name();
	    pDestroy(pInstance);

	    if (bDebugTypeManager) {
		cout << "Type "           << _strTypeName << endl
		     << "    in file "    << _strFileName << endl
		     << "    has typeid " << strTypeId    << endl;
	    }

	    mapTypes.insert(make_pair(_strTypeName, pCreate));
	    mapDestroy.insert(make_pair(strTypeId, pDestroy));

	    return pLabelFn();
	}
	catch (OpenLibraryError &e) {
	    throw LoadTypeError(string("Couldn't open file ") + e.what());
	}
	catch (UnresolvedSymbolError &e) {
	    typeLoader.close(_strFileName);
	    throw LoadTypeError(string("Couldn't resolve symbol ") + e.what());
	}
    }
    
    /** 
     * Test whether the manager contains the specified type.
     * 
     * @return True if the type name is found, otherwise false.
     * @param _strTypeName Name of type.
     */
    template <class T> 
    bool ClsTypeManager<T>::hasType(string _strTypeName)
    {
	TypeMapIterator it;
	it = mapTypes.find(_strTypeName);
	
	if (it == mapTypes.end()) {
	    return false;
	}
	
	return true;
    }
    
    /** 
     * Create a new object of the desired type.
     *
     * The caller must delete the returned object using deleteObject
     * for the appropriate type.
     *
     * @return New object of the desired type.
     * @param _strTypeName Name of the type to create.
     * @exception UnknownTypeError type name not found.
     */
    template <class T> 
    T* ClsTypeManager<T>::createObject(string _strTypeName)
    {
	TypeMapIterator it = mapTypes.find(_strTypeName);
	
	if (it == mapTypes.end()) {
	    throw UnknownTypeError(_strTypeName);
	}
	
	// Execute the create function to make a new object of this
	// type.
	T* pNewObject = (it->second)();
	return pNewObject;
    }
    
    /** 
     * Destroy an existing object.
     *
     * The type of the object is identified automatically.  If a
     * corresponding destroy function cannot be found, the instance is
     * deleted.
     *
     * @param _pObject Base class pointer to the object to be destroyed.
     * The base class must have a virtual destructor in order to
     * ensure correct deletion of the object.
     */
    template <class T> 
    void ClsTypeManager<T>::destroyObject(T* _pObject)
    {
	if (!_pObject) {
	    return;
	}

	string strTypeId = typeid(*_pObject).name();
	typename DestroyMap::iterator it = mapDestroy.find(strTypeId);
	
	if (it != mapDestroy.end()) {
	    // Execute the destroy function for this type.
	    (it->second)(_pObject);
	    if (bDebugTypeManager) {
		cout << "Destroyed object with typeid " << strTypeId << endl;
	    }
	} else {
	    // Fallback onto delete.  This shouldn't cause problems,
	    // but if the shared objects have been compiled with a
	    // different compiler from this function, there may be
	    // incompatibilities and runtime errors.
	    delete _pObject;
	    cerr << "ERROR: no destroy function found for object with typeid " 
		 << strTypeId << "; deleted" << endl;
	}
    }

    /** 
     * Exception thrown by ClsTypeManager when loading a type from a
     * file fails.
     */
    class LoadTypeError : public runtime_error
    {
    public:
	/** 
	 * @param _strReason Reason for failure.
	 */
	LoadTypeError(const string& _strReason)
	    : runtime_error(_strReason)	{}
    };

    /** 
     * Exception thrown by ClsTypeManager when an unknown type is
     * requested.
     */
    class UnknownTypeError : public runtime_error
    {
    public:
	/** 
	 * @param _strTypeName Name of unknown type.
	 */
	UnknownTypeError(const string& _strTypeName)
	    : runtime_error(_strTypeName) {}
    };
}

#endif
