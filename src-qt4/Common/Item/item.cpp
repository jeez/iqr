/****************************************************************************
 ** $Filename: item.cpp
 **
 ** $Author: Mark Blanchard
 ** 
 ** $Description: Implementation of ClsItem.  See item.hpp for definition.
 **
 *****************************************************************************/

#include <algorithm>
#include <cassert>
#include <iostream>

#include <Common/Helper/deleteObject.hpp>

#include "item.hpp"

#include "boolParameter.hpp"
#include "doubleParameter.hpp"
#include "intParameter.hpp"
#include "optionsParameter.hpp"
#include "stringParameter.hpp"
#include "pathParameter.hpp"
#include "colorParameter.hpp"


#ifdef DEBUG_CLSITEM
static const bool bDebugItem = true;
#else
static const bool bDebugItem = false;
#endif

using namespace std;
using effective_stl::DeleteObject;

iqrcommon::ClsItem::ClsItem() {    
    if (bDebugItem) {
	cout << "ClsItem::ClsItem"
	     << endl;
    }    
    bRuntimeEditable = true;
}

iqrcommon::ClsItem::~ClsItem() {    
    if (bDebugItem) {
	cout << "ClsItem::~ClsItem"
	     << endl;
    }
        
    // Delete the parameter objects explicitly.
    for_each(lstParameters.begin(), lstParameters.end(), DeleteObject());
}

/** 
 * Add a new boolean parameter.
 *
 * This function creates a parameter with the default value.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsBoolParameter*
iqrcommon::ClsItem::addBoolParameter(string _strName, 
				     string _strLabel,
				     string _strDescription,
				     string _strCategory)
{
    ClsBoolParameter *pParam = new ClsBoolParameter(_strName, 
						    _strLabel,
						    _strDescription,
						    _strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added bool parameter "
	     << _strName
	     << "...";
    }
    
    return pParam;    
}

/** 
 * Add a new boolean parameter.
 *
 * This function creates a parameter with a specified value.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _bValue Initial value of the parameter.
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsBoolParameter*
iqrcommon::ClsItem::addBoolParameter(string _strName, 
				     string _strLabel,
				     bool   _bValue,
				     string _strDescription,
				     string _strCategory)
{
    ClsBoolParameter *pParam = new ClsBoolParameter(_strName, 
						    _strLabel,
						    _bValue,
						    _strDescription,
						    _strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added bool parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}

/** 
 * Add a new double parameter.
 *
 * This function creates a parameter with the default value and range.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsDoubleParameter*
iqrcommon::ClsItem::addDoubleParameter(string _strName, 
				       string _strLabel,
				       string _strDescription,
				       string _strCategory)
{
    ClsDoubleParameter *pParam = new ClsDoubleParameter(_strName, 
							_strLabel,
							_strDescription,
							_strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added double parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}

/** 
 * Add a new double parameter.
 *
 * This function creates a parameter with a specified value and range.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _dValue Initial value of the parameter.
 * @param _dMinimum Minimum value of the parameter.
 * @param _dMaximum Maximum value of the parameter.
 * @param _iPrecision Precision (number of decimal places) of the
 * parameter.
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsDoubleParameter*
iqrcommon::ClsItem::addDoubleParameter(string _strName, 
				       string _strLabel,
				       double  _dValue, 
				       double  _dMinimum, 
				       double  _dMaximum, 
				       int     _iPrecision,
				       string _strDescription,
				       string _strCategory)
{    
    ClsDoubleParameter *pParam = new ClsDoubleParameter(_strName, 
							_strLabel,
							_dValue, 
							_dMinimum, 
							_dMaximum, 
							_iPrecision,
							_strDescription,
							_strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added double parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}

/** 
 * Add a new int parameter.
 *
 * This function creates a parameter with the default value and range.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsIntParameter*
iqrcommon::ClsItem::addIntParameter(string _strName, 
				    string _strLabel,
				    string _strDescription,
				    string _strCategory)
{
    ClsIntParameter *pParam = new ClsIntParameter(_strName, 
						  _strLabel,
						  _strDescription,
						  _strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added int parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}

/** 
 * Add a new int parameter.
 *
 * This function creates a parameter with a specified value and range.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _iValue Initial value of the parameter.
 * @param _iMinimum Minimum value of the parameter.
 * @param _iMaximum Maximum value of the parameter.
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsIntParameter*
iqrcommon::ClsItem::addIntParameter(string _strName, 
				    string _strLabel,
				    int    _iValue, 
				    int    _iMinimum, 
				    int    _iMaximum, 
				    string _strDescription,
				    string _strCategory)
{
    ClsIntParameter *pParam = new ClsIntParameter(_strName, 
						  _strLabel,
						  _iValue, 
						  _iMinimum, 
						  _iMaximum, 
						  _strDescription,
						  _strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added int parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}

/** 
 * Add a new options parameter.
 *
 * This function creates a parameter with an empty read-only options
 * list.  The options must be added explicitly using
 * ClsOptionsParameter::addOption.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsOptionsParameter*
iqrcommon::ClsItem::addOptionsParameter(string _strName, 
					string _strLabel,
					string _strDescription,
					string _strCategory)
{
    ClsOptionsParameter *pParam = new ClsOptionsParameter(_strName, 
							  _strLabel,
							  _strDescription,
							  _strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added options parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}

/** 
 * Add a new options parameter.
 *
 * This function creates a parameter with an empty read-only options
 * list.  The options must be added explicitly using
 * ClsOptionsParameter::addOption.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _bReadOnly Sets the options list to read-only when true.
 * For non-readonly lists, the corresponding parameter widget will
 * allow the user to add new options to the list.
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsOptionsParameter*
iqrcommon::ClsItem::addOptionsParameter(string _strName, 
					string _strLabel,
					bool   _bReadOnly,
					string _strDescription,
					string _strCategory)
{
    ClsOptionsParameter *pParam = new ClsOptionsParameter(_strName, 
							  _strLabel,
							  _bReadOnly,
							  _strDescription,
							  _strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added options parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}

/** 
 * Add a new string parameter.
 *
 * This function creates a short editable empty parameter string.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsStringParameter*
iqrcommon::ClsItem::addStringParameter(string _strName, 
				       string _strLabel,
				       string _strDescription,
				       string _strCategory)
{
    ClsStringParameter *pParam = new ClsStringParameter(_strName, 
							_strLabel,
							_strDescription,
							_strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added string parameter "
	     << _strName
	     << "...";
    }
    
    return pParam;    
}

/** 
 * Add a new string parameter.
 *
 * This function creates a parameter with the specified value and
 * properties.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strValue Initial value of the parameter.
 * @param _bEditable Sets whether the string is editable (true) or
 * readonly (false).
 * @param _bLong Sets whether the string is multiline (true) or single
 * line (false).
 * @param _strDescription Description of the parameter, used for
 * creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange
 * parameter widgets into categories in a dialog.
 */
iqrcommon::ClsStringParameter*
iqrcommon::ClsItem::addStringParameter(string _strName, 
				       string _strLabel,
				       string _strValue,
				       bool   _bEditable,
				       bool   _bLong,
				       string _strDescription,
				       string _strCategory)
{
    ClsStringParameter *pParam = new ClsStringParameter(_strName, 
							_strLabel,
							_strValue,
							_bEditable,
							_bLong,
							_strDescription,
							_strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added string parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}

/** 
 * Add a new path parameter.
 *
 * This function creates a short editable empty parameter path.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strDescription Description of the parameter, used for creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange parameter widgets into categories in a dialog.
 */
iqrcommon::ClsPathParameter*
iqrcommon::ClsItem::addPathParameter(string _strName, 
				       string _strLabel,
				       string _strDescription,
				       string _strCategory)
{
    ClsPathParameter *pParam = new ClsPathParameter(_strName, 
							_strLabel,
							_strDescription,
							_strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added path parameter "
	     << _strName
	     << "...";
    }
    
    return pParam;    
}

/** 
 * Add a new path parameter.
 *
 * This function creates a parameter with the specified value and
 * properties.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strValue Initial value of the parameter.
 * @param _bEditable Sets whether the path is editable (true) or readonly (false).
 * @param _strDescription Description of the parameter, used for creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange parameter widgets into categories in a dialog.
 */
iqrcommon::ClsPathParameter*
iqrcommon::ClsItem::addPathParameter(string _strName, 
				       string _strLabel,
				       string _strValue,
				       bool   _bEditable,
				       string _strDescription,
				       string _strCategory)
{
    ClsPathParameter *pParam = new ClsPathParameter(_strName, 
							_strLabel,
							_strValue,
							_bEditable,
							_strDescription,
							_strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added path parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}

/** 
 * Add a new color parameter.
 *
 * This function creates a short editable empty parameter color.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strDescription Description of the parameter, used for creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange parameter widgets into categories in a dialog.
 */
iqrcommon::ClsColorParameter*
iqrcommon::ClsItem::addColorParameter(string _strName, 
				       string _strLabel,
				       string _strDescription,
				       string _strCategory)
{
    ClsColorParameter *pParam = new ClsColorParameter(_strName, 
							_strLabel,
							_strDescription,
							_strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added color parameter "
	     << _strName
	     << "...";
    }
    
    return pParam;    
}

/** 
 * Add a new color parameter.
 *
 * This function creates a parameter with the specified value and
 * properties.
 *
 * @return Pointer to the new parameter object.
 * @param _strName Name of the new parameter (used in the system
 * file).
 * @param _strLabel Label to use for this parameter in a dialog.
 * @param _strValue Initial value of the parameter.
 * @param _bEditable Sets whether the color is editable (true) or readonly (false).
 * @param _strDescription Description of the parameter, used for creating tooltips/help.
 * @param _strCategory Category of the parameter, used to arrange parameter widgets into categories in a dialog.
 */
iqrcommon::ClsColorParameter*
iqrcommon::ClsItem::addColorParameter(string _strName, 
				       string _strLabel,
				       string _strValue,
				       bool   _bEditable,
				       string _strDescription,
				       string _strCategory)
{
    ClsColorParameter *pParam = new ClsColorParameter(_strName, 
							_strLabel,
							_strValue,
							_bEditable,
							_strDescription,
							_strCategory);
    addParameterToList(pParam);
    addParameterToMap(pParam);

    if (bDebugItem) {
	cout << "ClsItem: added color parameter "
	     << _strName
	     << endl;
    }
    
    return pParam;    
}





/** 
 * Export the parameter list.
 *
 * @return Reference to list of parameter objects.
 */
const iqrcommon::ParameterList&
iqrcommon::ClsItem::getListParameters() const
{
    return lstParameters;
}

/** 
 * Find a named parameter.
 *
 * This function searches the parameter map for the specified name
 * and, if the name is found, returns the corresponding parameter
 * pointer.  WARNING: if the specified name is not found, this
 * function returns 0.  It is the responsibility of the caller to
 * check the return value before use.
 *
 * @return Pointer to the requested parameter object, or 0 if the
 * specified name was not found in the parameter map.
 * @param _strName Name of the desired parameter.
 */
iqrcommon::ClsParameter*
iqrcommon::ClsItem::getParameter(string _strName)
{
    // Search the parameter map for the specified name.
    ParameterMap::iterator itParameter;
    itParameter = mapParameters.find(_strName);
    
    if (itParameter == mapParameters.end()) {
	// The specified name was not found in the map.
	return 0;
    }
    
    // Return the parameter object pointer.
    return itParameter->second;
}

/** 
 * Set the value of a named parameter.
 *
 * If the parameter cannot be found, this function does nothing.
 *
 * @param _strName Name of the parameter to set.
 * @param _strValue Value of the parameter as a std::string.  The
 * parameter object handles translation of the string into the
 * relevant type.
 */
void
iqrcommon::ClsItem::setParameter(string _strName, string _strValue)
{
    ClsParameter *pParameter = getParameter(_strName);
    if (pParameter != 0) {
	pParameter->setValueFromString(_strValue);
	if (bDebugItem) {
	    cout << "ClsItem::setParameter:"
		 << " set value of parameter "
		 << _strName
		 << " to "
		 << _strValue
		 << endl;
	}
    } else {
	// The parameter was not found, do nothing.
	if (bDebugItem) {
	    cout << "ClsItem::setParameter:"
		 << " parameter "
		 << _strName
		 << " not found"
		 << endl;
	}
    }
}

/** 
 * Set the values of the parameters.
 *
 * If the parameters cannot be found, this function does nothing.
 *
 * @param _lstParameters List of parameter objects.
 */
void
iqrcommon::ClsItem::setParameters(const ParameterList &_lstParameters)
{
    string strName, strValue;

    if (bDebugItem) {
	cout << "ClsItem::setParameters:"
	     << " parameter list has "
	     << _lstParameters.size()
	     << " elements"
	     << endl;
    }
    // Loop over the supplied parameter list.
    ParameterList::const_iterator itParameter;
    for (itParameter = _lstParameters.begin();
	 itParameter != _lstParameters.end();
	 ++itParameter) {

	strName  = (*itParameter)->getName();
	strValue = (*itParameter)->getValueAsString();
	
	if (bDebugItem) {
	    cout << "ClsItem::setParameters:"
		 << " looking for parameter "
		 << strName
		 << endl;
	}
	
	// Find the parameter which matches the name of the list
	// parameter.
	ClsParameter *pParameter = getParameter(strName);
	if (pParameter != 0) {
	    pParameter->setValueFromString(strValue);
	    if (bDebugItem) {
		cout << "ClsItem::setParameters:"
		     << " set value of parameter "
		     << strName
		     << " to "
		     << strValue
		     << endl;
	    }
	} else {
	    // The parameter was not found, do nothing.
	    if (bDebugItem) {
		cout << "ClsItem::setParameters:"
		     << " parameter "
		     << strName
		     << " not found"
		     << endl;
	    }
	}
    }
}

/** 
 * Add a parameter object pointer to the parameter list.
 *
 * A parameter list is maintained in order to preserve the creation
 * order of the parameters.  This might be important for the user
 * interface, where automatically-generated dialogs are used.  In
 * addition, the parameter list is used to delete the parameter
 * objects in the destructor of this class.
 *
 * This function does not check whether the pointer is already in the
 * list.
 *
 * If the supplied pointer is 0, this function does nothing.
 *
 * @param *_pParam Pointer to a parameter object.
 */
void 
iqrcommon::ClsItem::addParameterToList(ClsParameter *_pParam)
{
    if (_pParam != 0) {
	lstParameters.push_back(_pParam);
    } else {
	if (bDebugItem) {
	    cout << "WARNING: invalid pointer to parameter not added to list"
		 << endl;
	}
    }
}

/** 
 * Add a parameter object pointer to the parameter map.
 *
 * A parameter map is maintained in order to speed-up access to
 * parameter objects when setting values.
 *
 * This function does not check whether the pointer is already in the
 * map.
 *
 * If the supplied pointer is 0, this function does nothing.
 *
 * @param *_pParam Pointer to a parameter object.
 */
void 
iqrcommon::ClsItem::addParameterToMap(ClsParameter *_pParam)
{
    if (_pParam != 0) {
	mapParameters.insert(make_pair(_pParam->getName(), _pParam));
    } else {
	if (bDebugItem) {
	    cout << "WARNING: invalid pointer to parameter not added to map"
		 << endl;
	}
    }
}
