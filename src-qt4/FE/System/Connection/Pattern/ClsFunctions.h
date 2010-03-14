/****************************************************************************
 ** $Filename: ClsFunctions.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed May 21 14:02:47 2003
 **
 *****************************************************************************/


#ifndef CLSFUNCTIONS_H
#define CLSFUNCTIONS_H    /*+ To stop multiple inclusions. +*/

#include "tagLibrary.hpp"
#include "item.hpp"
#include "ClsBaseFunction.h"

using namespace iqrcommon;
using namespace std;


class ClsFunGaussian : public ClsBaseFunction {
public:
    ClsFunGaussian(){
	strType = ClsTagLibrary::ConnectionFunctionGaussian();
	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMin(), 
			   "min",
			   "Description", 
			   "Properties");

	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			   "max",
			   "Description", 
			   "Properties");

	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgSigma(), 
			   "sigma",
			   "Description", 
			   "Properties");
    };
};

class ClsFunBlock : public ClsBaseFunction {
public:
    ClsFunBlock(){
	strType = ClsTagLibrary::ConnectionFunctionBlock();

	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMin(), 
			   "min",
			   "Description", 
			   "Properties");

	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			   "max",
			   "Description", 
			   "Properties");

	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgWidth(), 
			   "width",
			   "Description", 
			   "Properties");

	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgHeight(), 
			   "height",
			   "Description", 
			   "Properties");
    };
};
class ClsFunRandom : public ClsBaseFunction {
public:
    ClsFunRandom(){
	strType = ClsTagLibrary::ConnectionFunctionRandom();
	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgProbability(), 
			   "probability",
			   "Description", 
			   "Properties");
    };
};

class ClsFunLinear : public ClsBaseFunction {
public:
    ClsFunLinear(){
	strType = ClsTagLibrary::ConnectionFunctionLinear();
	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMin(), 
			   "min",
			   "Description", 
			   "Properties");

	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgMax(), 
			   "max",
			   "Description", 
			   "Properties");
    };
};

class  ClsFunUniform : public ClsBaseFunction {
public:
    ClsFunUniform(){
	strType = ClsTagLibrary::ConnectionFunctionUniform();
	addDoubleParameter(ClsTagLibrary::ConnectionFunctionArgValue(), 
			   "value",
			   "Description", 
			   "Properties");
    };
};

/*
  class  ClsFunKernel : public ClsBaseFunction {
  public:
  ClsFunKernel(){
  strType = ClsTagLibrary::ConnectionFunctionKernel();
  };
  };
*/

/// Local Variables: 
/// mode: c++
/// End: 

#endif /* CLSFUNCTIONS_H */









