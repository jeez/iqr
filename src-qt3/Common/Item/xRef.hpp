/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Sun Nov 2 14:15:00 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description: 
 **
 *****************************************************************************/

#ifndef XREF_HPP
#define XREF_HPP

#include <string>
#include <list>

using namespace std;

namespace iqrcommon {
    class ClsXRef
    {
    public:
	ClsXRef(string _name, string _label);
	~ClsXRef();

	string getName() const;
	string getLabel() const;
	
	string getTarget() const;
	void setTarget(string _target);

/*
	void setListXRefCandidates(list<string>);
	const list<string>& getListXRefCandidates() const;
*/


    private:
	// Hide the default and copy constructors.
	ClsXRef();
	ClsXRef(const ClsXRef &);

	list<string> lstXRefCandidates;
	string name;
	string label;
	string target;
    };
}

#endif
