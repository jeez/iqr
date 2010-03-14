/****************************************************************************
 ** $Filename: optionsParameter.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Aug 14 21:06:19 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <algorithm>
#include <iostream>

#include "optionsParameter.hpp"

#ifdef DEBUG_CLSOPTIONSPARAMETER
static const bool bDebugOptionsParameter = true;
#else
static const bool bDebugOptionsParameter = false;
#endif

using namespace std;

iqrcommon::ClsOptionsParameter::ClsOptionsParameter(string _strName, 
						    string _strLabel, 
						    string _strDescription, 
						    string _strCategory)
    : ClsParameter(_strName, 
		   _strLabel,
		   _strDescription, 
		   _strCategory),
      iSelected(NO_SELECTION),
      bReadOnly(READ_ONLY)
{
    if (bDebugOptionsParameter) {
	cout << "ClsOptionsParameter::ClsOptionsParameter: "
	     << getName()
	     << endl;
    }
}

iqrcommon::ClsOptionsParameter::ClsOptionsParameter(string _strName, 
						    string _strLabel,
						    bool _bReadOnly, 
						    string _strDescription,
						    string _strCategory)
    : ClsParameter(_strName, 
		   _strLabel,
		   _strDescription,
		   _strCategory),
      iSelected(NO_SELECTION),
      bReadOnly(_bReadOnly)
{
    if (bDebugOptionsParameter) {
	cout << "ClsOptionsParameter::ClsOptionsParameter: "
	     << getName()
	     << endl;
    }
}

iqrcommon::ClsOptionsParameter::ClsOptionsParameter(const ClsOptionsParameter &_source) 
    : ClsParameter(_source.getName(),
		   _source.getLabel(), 
		   _source.getDescription(), 
		   _source.getCategory()),
      options(_source.getListOptions()),
      iSelected(_source.getSelected()),
      bReadOnly(_source.isReadOnly())
{
    if (bDebugOptionsParameter) {
	cout << "ClsOptionsParameter::ClsOptionsParameter: "
	     << getName()
	     << " copied"
	     << endl;
    }
}

iqrcommon::ClsOptionsParameter::~ClsOptionsParameter()
{
    if (bDebugOptionsParameter) {
	cout << "ClsOptionsParameter::~ClsOptionsParameter: "
	     << getName()
	     << endl;
    }
}

/** 
 * Assignment operator copies the options list, current selection and
 * flags from the source.
 *
 * The name, description, category, and controller widget strings
 * remain unchanged.
 *
 * @return Reference to this parameter.
 * @param &_source Source parameter from which to copy the value.  */
iqrcommon::ClsOptionsParameter& 
iqrcommon::ClsOptionsParameter::operator= (const ClsOptionsParameter &_source)
{    
    if (this == &_source) {
	return *this;
    }
	
    // Copy the options list.
    options = _source.getListOptions();
    
    // Copy the selected index.
    iSelected = _source.getSelected();
    
    // Copy flags.
    bReadOnly = _source.isReadOnly();
    
    return *this;    
}

string
iqrcommon::ClsOptionsParameter::getValue() const
{
    if (bDebugOptionsParameter) {
	cout << "ClsOptionsParameter::getValue: selected option is "
	     << iSelected
	     << endl;
    }

    if (iSelected == NO_SELECTION) {
	return "No selection";
    }

    if (iSelected >= static_cast<int>(options.size())) {
	return "Invalid selection";
    }

    return options[iSelected];
}

string
iqrcommon::ClsOptionsParameter::getValueAsString() const
{
    return getValue();
}

/** 
 * Set the current selection to the specified string.
 *
 * If the string is not found in the list of options, this function
 * adds it to the list.
 *
 * @param _strValue New selection.
 */
void 
iqrcommon::ClsOptionsParameter::setValue(string _strValue)
{
    // Find string in options list.
    OptionsList::iterator itOption;
    itOption = find(options.begin(), options.end(), _strValue);
    if (itOption != options.end()) {
	// String found, store index.
 	iSelected = distance(options.begin(), itOption);
	if (bDebugOptionsParameter) {
		cout << "ClsOptionsParameter::setValue:"
		     << getName()
		     << ": found option "
		     << _strValue
		     << " at position "
		     << iSelected
		     << " in list"
		     << endl;
	}
    } else {
	// String not found, add it to the end of the list and select
	// it.
	options.push_back(_strValue);
	iSelected = options.size() - 1;
	if (bDebugOptionsParameter) {
	    cout << "ClsOptionsParameter::setValue:"
		 << getName()
		 << ": added option "
		 << _strValue
		 << ", number of options is "
		 << options.size()
		 << endl;
	}
    }
}

void 
iqrcommon::ClsOptionsParameter::setValueFromString(string _strValue)
{
    setValue(_strValue);
}

iqrcommon::ClsOptionsParameter* 
iqrcommon::ClsOptionsParameter::makeCopy() const
{
    // Use copy constructor.
    return new ClsOptionsParameter(*this);
}

/** 
 * Add new option.
 *
 * This function does not change the current selection.
 *
 * @param _strOption Name of new option.
 */
void iqrcommon::ClsOptionsParameter::addOption(string _strOption) 
{
    OptionsList::iterator itOption = options.begin();

    // Check whether option is in options list.
    itOption = find(options.begin(), options.end(), _strOption);
    if (itOption != options.end()) {
	// Option is already in list.
	if (bDebugOptionsParameter) {
	    cout << "ClsOptionsParameter::addOption:"
		 << getName()
		 << ": option "
		 << _strOption
		 << " already in list"
		 << endl;
	}
	return;
    }

    options.push_back(_strOption);

    if (bDebugOptionsParameter) {
	cout << "ClsOptionsParameter::addOption:"
	     << getName()
	     << ": added option "
	     << _strOption
	     << ", number of options is "
	     << options.size()
	     << endl;
    }
    // Don't change the selection, leave that to the user.  
}

