/****************************************************************************
 ** $Filename: ClsFunctions.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed May 21 14:02:47 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFUNCTIONS_H
#define CLSFUNCTIONS_H    /*+ To stop multiple inclusions. +*/

#include "tagLibrary.hpp"
#include "item.hpp"
#include "ClsBaseFunction.h"
#include "ClsRandom.h"

using namespace iqrcommon;
using namespace std;


class ClsFunGaussian : public ClsBaseFunction {
public:
    ClsFunGaussian(int _iTarget) : ClsBaseFunction(_iTarget) {
	strType = ClsTagLibrary::ConnectionFunctionGaussian();
	if(iTarget == ClsBaseFunction::delay){
	    addIntParameter(ClsTagLibrary::ConnectionFunctionArgMin(), 
			    "min",
			    0, 0, 1000,
			    "Description", 
			    "Properties");
	    
	    addIntParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			    "max",
			    1, 0, 1000,
			    "Description", 
			    "Properties");
	} else {
	    addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMin(), 
			       "min",
			       0, -1000, 1000,4,
			       "Description", 
			       "Properties");
	    
	    addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			       "max",
			       1, -1000, 1000,4,
			       "Description", 
			       "Properties");
	    
	}
	
	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgSigma(), 
			   "sigma",
			   0.5, .0001, 10, 4,
			   "Description", 
			   "Properties");

	fMaxValue = 0;
    };


    double max(){
/*
	double fMin = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMin())->getValueAsString());
	double fMax = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMax())->getValueAsString());

	return (fMax > fMin ? fMax : fMin);
*/

	return fMaxValue;

    }

    void Calculate(ListIndexQuadruples &listIndexQuadruples){
//	cout << "ClsFunGaussian::Calculate(ListIndexQuadruples &listIndexQuadruples)" << endl;
	double fMin = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMin())->getValueAsString());
	double fMax = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMax())->getValueAsString());
	double fSigma = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgSigma())->getValueAsString());
	if(fSigma == 0){
	    fSigma = 0.0001;
	}
	
	double fReturn = 0;
	double fDist;
	
	vector<IndexQuadruple>::iterator it;
	for(it=listIndexQuadruples.begin(); it!=listIndexQuadruples.end(); it++){
	    fDist = (*it).distance;
	    fReturn = fMax -(exp(-((fDist*fDist)/(2*fSigma*fSigma)))) * (fMax-fMin) + fMin;

	    fMaxValue = (fReturn > fMaxValue ? fReturn : fMaxValue);

	    if(iTarget == ClsBaseFunction::delay){
		int iVal = (int)round(fReturn);
		(*it).delay = (iVal>=0 ? iVal : 0);
	    } else {
		(*it).attenuation = (fReturn >= 0 ? fReturn : 0);
//		cout << fDist << " " << (*it).attenuation << endl;
	    }
	}


    }

private:
    double fMaxValue;

};

class ClsFunBlock : public ClsBaseFunction {
public:
    ClsFunBlock(int _iTarget) : ClsBaseFunction(_iTarget) {
	strType = ClsTagLibrary::ConnectionFunctionBlock();

	if(iTarget == ClsBaseFunction::delay){
	addIntParameter(ClsTagLibrary::ConnectionFunctionArgMin(), 
			"min",
			0, 0, 1000,
			"Description", 
			"Properties");

	addIntParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			"max",
			1, 0, 1000,
			"Description", 
			"Properties");
	} 
	else {
	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMin(), 
			"min",
			   0, 0, 1000,4,
			"Description", 
			"Properties");

	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			"max",
			   1, 0, 1000,4,
			"Description", 
			"Properties");
	
	}
	addIntParameter(ClsTagLibrary::ConnectionFunctionArgWidth(), 
			"width",
			1, 0, 1000,
			"Description", 
			"Properties");

/*
  addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgHeight(), 
  "height",
  "Description", 
  "Properties");
*/
    };

    double max(){
	double fMin = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMin())->getValueAsString());
	double fMax = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMax())->getValueAsString());
	return (fMax > fMin ? fMax : fMin);
    }

    void Calculate(ListIndexQuadruples &listIndexQuadruples){
//	cout << "ClsFunBlock::Calculate(ListIndexQuadruples &listIndexQuadruples)" << endl;
	double fMin = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMin())->getValueAsString());
	double fMax = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMax())->getValueAsString());
	int iWidth = iqrUtils::string2int(getParameter(ClsTagLibrary::ConnectionFunctionArgWidth())->getValueAsString());
/*	int iHeight = iqrUtils::string2int(getParameter(ClsTagLibrary::ConnectionFunctionArgHeight())->getValueAsString()); */


	vector<IndexQuadruple>::iterator it;
	for(it=listIndexQuadruples.begin(); it!=listIndexQuadruples.end(); it++){
	    if ((*it).distance <= (iWidth /2 )){
		if(iTarget == ClsBaseFunction::delay){
		    (*it).delay = (int)fMin;
		} else {
		    (*it).attenuation = fMin;
		}

	    } else {
		if(iTarget == ClsBaseFunction::delay){
		    (*it).delay = (int)fMax;
		} else {
		    (*it).attenuation = fMax;
		}
	    }
	}
    };

};
class ClsFunRandom : public ClsBaseFunction {
public:
    ClsFunRandom(int _iTarget) : ClsBaseFunction(_iTarget) {
	strType = ClsTagLibrary::ConnectionFunctionRandom();

	if(iTarget == ClsBaseFunction::delay){
	addIntParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			"max",
			0, 0, 1000, 
			"Description", 
			"Properties");
	} 
	else {
	    addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			"max",
			0, 0, iMaxAtt(), 4,
			"Description", 
			"Properties");
	}

/*
  addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgProbability(), 
  "probability",
  "Description", 
  "Properties");
*/
    };

    double max(){
	return iMaxAtt();
    }


    void Calculate(ListIndexQuadruples &listIndexQuadruples){
//	cout << "ClsFunRandom::Calculate(ListIndexQuadruples &listIndexQuadruples)" << endl;
//	double fProb = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgProbability())->getValueAsString());

	double fReturn;
	const double fFactor = 10000.;
	double fMax = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMax())->getValueAsString());


	list<int> lstRandomValues =  ClsRandom::getWeakRandom(0, (int)(fMax * fFactor), listIndexQuadruples.size(), false);
	vector<IndexQuadruple>::iterator it;
	for(it=listIndexQuadruples.begin(); it!=listIndexQuadruples.end(); it++){
	    fReturn = (double)lstRandomValues.front() / fFactor;
	    if(iTarget == ClsBaseFunction::delay){
		(*it).delay = (int)round(fReturn);
	    } else {
		(*it).attenuation = fReturn;
	    }
	    lstRandomValues.pop_front();
	}
    };

private:
    const unsigned int iMaxAtt(){ return 1;};

};

class ClsFunLinear : public ClsBaseFunction {
public:
    ClsFunLinear(int _iTarget) : ClsBaseFunction(_iTarget) {
	strType = ClsTagLibrary::ConnectionFunctionLinear();
	if(iTarget == ClsBaseFunction::delay){
	    addIntParameter(ClsTagLibrary::ConnectionFunctionArgMin(), 
			    "min",
			    0, 0, 1000, 
			    "Description", 
			    "Properties");
	    
	    addIntParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			    "max",
			    1, 0, 1000, 
			    "Description", 
			    "Properties");
	} else {
	    addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMin(), 
			    "min",
			       0, 0, 1000, 4,
			    "Description", 
			    "Properties");
	    
	    addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			    "max",
			       1, 0, 1000, 4,
			    "Description", 
			    "Properties");
	}

    };

    double max(){
	double fMin = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMin())->getValueAsString());
	double fMax = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMax())->getValueAsString());
	return (fMax > fMin ? fMax : fMin);
    }

    void Calculate(ListIndexQuadruples &listIndexQuadruples){
//	cout << "ClsFunLinear::Calculate(ListIndexQuadruples &listIndexQuadruples)" << endl;
	double fMin = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMin())->getValueAsString());
	double fMax = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgMax())->getValueAsString());
	double fDist = 0;
	double fSlope, fReturn;
	
	double fDistMax = listIndexQuadruples.getMaxDistance();

	vector<IndexQuadruple>::iterator it;
	for(it=listIndexQuadruples.begin(); it!=listIndexQuadruples.end(); it++){
	    fDist = (*it).distance;
	    fSlope = (fMax - fMin) / fDistMax;
	    fReturn= fSlope * fDist + fMin;
	    if(iTarget == ClsBaseFunction::delay){
	    (*it).delay = (int)round(fReturn);
	    } else {
	    (*it).attenuation = fReturn;
	    }
	}
    }
};

class  ClsFunUniform : public ClsBaseFunction {
public:
    ClsFunUniform(int _iTarget) : ClsBaseFunction(_iTarget) {
	strType = ClsTagLibrary::ConnectionFunctionUniform();
	if(iTarget == ClsBaseFunction::delay){
	    addIntParameter(ClsTagLibrary::ConnectionFunctionArgValue(), 
			    "value",
			    0, 0, 1000,
			    "Description", 
			    "Properties");
	} 
	else {
	    addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgValue(), 
			       "value",
			       0, 0, iMaxAtt(), 4,
			       "Description", 
			       "Properties");
	}


    };

    double max(){
	return iMaxAtt();
    }


    void Calculate(ListIndexQuadruples &listIndexQuadruples){
//	cout << "ClsFunUniform::Calculate(ListIndexQuadruples &listIndexQuadruples)" << endl;
	double fValue = iqrUtils::string2double(getParameter(ClsTagLibrary::ConnectionFunctionArgValue())->getValueAsString());
	vector<IndexQuadruple>::iterator it;
	for(it=listIndexQuadruples.begin(); it!=listIndexQuadruples.end(); it++){
	    if(iTarget == ClsBaseFunction::delay){
		(*it).delay = (fValue >= 0 ? (int)fValue : 0);
	    } else {
		(*it).attenuation = (fValue >= 0 ? fValue : 0);
	    }
	}
    }

private:
    const unsigned int iMaxAtt(){ return 1;};

};

/*
  class  ClsFunKernel : public ClsBaseFunction {
  public:
  ClsFunKernel(){
  strType = ClsTagLibrary::ConnectionFunctionKernel();
  };
  };
*/

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:

#endif /* CLSFUNCTIONS_H */









