/****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Mon Nov 19 12:06:24 2001
 **
 ** $Description: ClsStateVariable provides a means for a neuron to
 **               indicate the names and types of the state variables
 **               it needs.  This class currently supports StateArray
 **               (== ClsStateArray<double>).
 **
 *****************************************************************************/

#ifndef STATEVARIABLE_HPP
#define STATEVARIABLE_HPP

#include <string>
using namespace std;

#include "stateArray.hpp"

namespace iqrcommon {

    class ClsStateVariable
    {
    public:
	ClsStateVariable(string _name, 
			 string _label, 
			 unsigned int _minLengthHistory = 1);
	~ClsStateVariable();

	string getName() const;
	string getLabel() const;
	
	StateArray& getStateArray();
	const StateArray& getStateArray() const;

	void setNumberElements(unsigned int _nElements);
	void setLengthHistory(unsigned int _lengthHistory);
	void setSize(unsigned int _nElements, unsigned int _lengthHistory = 1);
	void resetSize();


	void setNrCellsVertical(int ii){iYDim = ii;};
	int getNrCellsVertical(){return iYDim;};
	
	void setNrCellsHorizontal(int ii){iXDim = ii;};
	int getNrCellsHorizontal(){return iXDim;};


	// Advance the state array ring buffer pointer.
	void advance();

    private:
	// Hide the default and copy constructors.
	ClsStateVariable();
	ClsStateVariable(const ClsStateVariable &);

	string       name, label;
	StateArray   stateArray;
 	unsigned int minLengthHistory;
	int iXDim, iYDim;
    };
}

#endif
