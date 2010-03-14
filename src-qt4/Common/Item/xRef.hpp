/****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
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
