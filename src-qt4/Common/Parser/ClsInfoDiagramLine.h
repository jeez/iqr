/****************************************************************************
 ** $Filename: ClsInfoDiagramLine.h
 **
 ** $Author: Ulysses Bernardet
 **
 *****************************************************************************/


#ifndef CLSINFODIAGRAMLINE_H
#define CLSINFODIAGRAMLINE_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <vector>
#include "../Helper/iqrUtils.h"

#define DEFAULT_LOCATION 10

// 05/02/02 [jmb] Placed system file parser classes into namespace
// iqrcommon.
namespace iqrcommon {
    
    class ClsInfoDiagramLine
    {
    public:
	ClsInfoDiagramLine(){ 
	    iAPSource = -1;
	    iAPTarget = -1;	    
	};
	


	void addPoint(int iX, int iY) {
	    vector<int> vectorPoint;
	    vectorPoint.push_back(iX >= 0 ? iX : DEFAULT_LOCATION);
	    vectorPoint.push_back(iY >= 0 ? iY : DEFAULT_LOCATION);
	    vectorPoints.push_back(vectorPoint);
	};
	
	void addPoint(string strX, string strY) {
	    //cout << "ClsInfoDiagramLine::addPoint(string strX, string strY)" << endl;
	    int iX = iqrUtils::string2int(strX);
	    int iY = iqrUtils::string2int(strY);
	    vector<int> vectorPoint;
	    vectorPoint.push_back(iX >= 0 ? iX : DEFAULT_LOCATION);
	    vectorPoint.push_back(iY >= 0 ? iY : DEFAULT_LOCATION);
//--	    vectorPoint.push_back(iqrUtils::string2int(strX));
//--	    vectorPoint.push_back(iqrUtils::string2int(strY));
	    vectorPoints.push_back(vectorPoint);
	};
	
	void setPoints(vector<vector<int> > v) { vectorPoints = v; };
	vector<vector<int> > getPoints() { return vectorPoints; };

	void setAPSource(int i){iAPSource = i;};
	int getAPSource(){return iAPSource;};

	void setAPTarget(int i){iAPTarget = i;};
	int getAPTarget(){return iAPTarget;};
	
	

    private:
	vector<vector<int> > vectorPoints;
	int iAPSource, iAPTarget;
	

	friend ostream &operator<<(ostream &stream, 
				   ClsInfoDiagramLine clsInfoDiagramLine);
    };
}


#endif /* CLSINFODIAGRAMLINE_H */


/// Local Variables: 
/// mode: c++
/// End: 

