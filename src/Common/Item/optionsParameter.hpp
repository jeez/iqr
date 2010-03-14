/****************************************************************************
 ** $Filename: optionsParameter.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Aug 14 21:02:22 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef OPTIONSPARAMETER_HPP
#define OPTIONSPARAMETER_HPP

// #include <string>
#include <vector>
using namespace std;

#include "parameter.hpp"

namespace iqrcommon {

//     using std::string;
//     using std::vector;

    typedef vector<string> OptionsList;

    class ClsOptionsParameter : public ClsParameter
    {
    public:
	ClsOptionsParameter(string _strName, 
			    string _strLabel, 
			    string _strDescription = "",
			    string _strCategory = "");
	ClsOptionsParameter(string _strName, 
			    string _strLabel,
			    bool   _bReadOnly, 
			    string _strDescription = "", 
			    string _strCategory = "");

	ClsOptionsParameter(const ClsOptionsParameter &_source);

	~ClsOptionsParameter();

	ClsOptionsParameter& operator=(const ClsOptionsParameter &_source);

	string getValue() const;
	string getValueAsString() const;
	void setValue(string _strValue);
	void setValueFromString(string _strValue);

	ClsOptionsParameter* makeCopy() const;

	const OptionsList& getListOptions() const {return options;}
	void addOption(string _strOption);

	bool isReadOnly()  const {return bReadOnly;}
 	int  getSelected() const {return iSelected;}

    private:
	OptionsList options;

	int iSelected;

	bool bReadOnly;

	static const int NO_SELECTION = -1;

	static const bool READ_ONLY      = true;
    };
};

#endif

