/****************************************************************************
 ** $Filename: ClsBaseArborization.h
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri May 16 13:02:36 2003
 **
 *****************************************************************************/


#ifndef CLSBASEARBORIZATION_H
#define CLSBASEARBORIZATION_H

#include <math.h>

#include "tagLibrary.hpp"
#include "item.hpp"
#include "iqrUtils.h"
#include "intParameter.hpp"

using namespace iqrcommon;
using namespace std;


class ClsBaseArborization : public ClsItem {

public:
    ClsBaseArborization();
    virtual void getListOfCells( int iX, int iY );

    string Type() const {return strType;}
    virtual double DistMax() = 0;
    double getProbability(){
	return pclProbability->getValue();
    }

/*
    void setAreaWidth(int ii){ iAreaWidth = ii; };
    void setAreaHeight(int ii){ iAreaHeight = ii; };
    int getAreaWidth(){ return iAreaWidth; };
    int getAreaHeight(){ return iAreaHeight; };
*/

protected:
    string strType;

    double calculateDistance(double fAx, double fAy, double fBx, double fBy){
//	cout << "calculateDistance(double fAx, double fAy, double fBx, double fBy)" << endl;
	return sqrt(pow((fAx - fBx),2.) + pow((fAy - fBy),2.));
    }

/*    int iAreaWidth, iAreaHeight; */
    ClsDoubleParameter *pclProbability;
    ClsOptionsParameter *pclDirection;


};

#endif // CLSBASEARBORIZATION_H


/// Local Variables:
/// mode: c++
/// End:
