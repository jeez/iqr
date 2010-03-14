/****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#ifndef XREFHOLDER_HPP
#define XREFHOLDER_HPP

#include <list>
#include <string>
using namespace std;

#include "xRef.hpp"
#include "xRefList.hpp"

namespace iqrcommon {
    class ClsXRefHolder
    {
    public:
	ClsXRefHolder();
	~ClsXRefHolder();
	
	void setName(string _strName);
	const string getName() const;


	void setCategory(string _strCategory);
	const string getCategory() const;

	ClsXRef* addXRef(string _name, string _label);
	const XRefList& getListXRefs() const;
	const int getXRefsCount();

	// Access to state variables needed to set maximum state
	// array lengths during simulation initialization.
	ClsXRef* getXRefByName(string _name);
	ClsXRef* getXRefByLabel(string _label);

	void setXRefTargetByName(string _name, string _target);
	void setXRefTargetByLabel(string _label, string _target);

    private:
	// Disable copy constructor.
	ClsXRefHolder(const ClsXRefHolder &);
	
	XRefList lstXRefs;
	string strName;
	string strCategory;
    };
}

#endif
