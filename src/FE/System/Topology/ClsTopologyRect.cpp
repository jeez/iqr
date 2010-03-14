#include "ClsTopologyRect.h"

#include "ClsRandom.h"
#include "tagLibrary.hpp"
#include "intParameter.hpp"
#include "item.hpp"

#include "iqrUtils.h"

#include "ClsSubPopulations.h"


#include "ClsArbAll.h"
#include "ClsArbEllipse.h"
#include "ClsArbEllipseWindow.h"
#include "ClsArbRandom.h"
#include "ClsArbRect.h"
#include "ClsArbRectWindow.h"


pointCompareRect::pointCompareRect(int _iWidth) : iWidth(_iWidth)  {
}

bool pointCompareRect::operator() (const tiPoint &p0, const tiPoint &p1) {
//    cout << "pointCompareRect::operator()" << endl;

    return ClsTopologyRect::pos2index(iWidth,p0.first,p0.second) < ClsTopologyRect::pos2index(iWidth,p1.first,p1.second);
}

ClsTopologyRect::ClsTopologyRect() {//: ClsBaseTopology(ClsTagLibrary::RectTopologyTag()) {

    strType = ClsTagLibrary::RectTopologyTag();


    pclsWidth = addIntParameter(ClsTagLibrary::TopologyWidthTag(), "Width",
				1,
				1, 1000,
				"Width of group",
				"Properties");

    pclsHeight = addIntParameter(ClsTagLibrary::TopologyHeightTag(), "Height",
				 1,
				 1, 1000,
				 "Height of group",
				 "Properties");


    clsBoolParameterVerticalCylinder = addBoolParameter(ClsTagLibrary::TopologyCylinderVertical(), /* _strName */ 
							"Treat as vertical cylinder", /* _strLabel */
							false, /* _bValue */
							"Treat group as vertical cylinder", /* _strDescription */
							"Properties" /* _strCategory */);

    clsBoolParameterHorizontalCylinder = addBoolParameter(ClsTagLibrary::TopologyCylinderHorizontal(), /* _strName */ 
							  "Treat as horizontal cylinder", /* _strLabel */
							  false, /* _bValue */
							  "Treat group as horizontal cylinder", /* _strDescription */
							  "Properties" /* _strCategory */);

}

int ClsTopologyRect::nrCellsHorizontal(){
    return pclsWidth->getValue();
};

int ClsTopologyRect::nrCellsVertical(){
    return pclsHeight->getValue();
};

double ClsTopologyRect::DistMax(){
    double fWidth = pclsWidth->getValue();
    double fHeight = pclsHeight->getValue();
    return calculateDistance(1., 1., fWidth, fHeight);
}


int  ClsTopologyRect::Size()  {
    int iWidth = pclsWidth->getValue();
    int iHeight = pclsHeight->getValue();
    return iWidth * iHeight;
}


vector<tiPoint> ClsTopologyRect::getPointsForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation){
    vector<tiPoint> lstPointsReturn;

    int iWidth = pclsWidth->getValue();
    int iHeight = pclsHeight->getValue();

    if(dynamic_cast<ClsAll*>(clsBaseSubPopulation)){
	lstPointsReturn = getAllPoints(iWidth, iHeight);
    }
    else if(dynamic_cast<ClsList*>(clsBaseSubPopulation)){
	vector<tiPoint> lstPairsSubPopulation = (dynamic_cast<ClsList*>(clsBaseSubPopulation))->getData();
	vector<tiPoint> lstPoints;
	lstPoints = getAllPoints(iWidth, iHeight);
	lstPointsReturn = AndLists(lstPoints, lstPairsSubPopulation);
    }
    else if(dynamic_cast<ClsRegion*>(clsBaseSubPopulation)){
	ClsRegion* clsRegion = dynamic_cast<ClsRegion*>(clsBaseSubPopulation);
	int iRegionXStart = clsRegion->getXStart();
	int iRegionYStart = clsRegion->getYStart();
	int iRegionWidth = clsRegion->getWidth();
	int iRegionHeight = clsRegion->getHeight();
	lstPointsReturn = getPoints4Rect( iRegionXStart, iRegionYStart, iRegionWidth, iRegionHeight);
    }

    return lstPointsReturn;

}

list<int> ClsTopologyRect::getIndicesForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation){

    list<int> lst;
    lst =  posList2indexList(getPointsForSubPopulation(clsBaseSubPopulation));
    return lst;
};


list<tIndexDist> ClsTopologyRect::getCellsForArborization(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization) {
//    cout << "ClsTopologyRect::getCellsForArborization(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization)" << endl;
    list<tIndexDist> lst;
    vector<tiPoint> lstPointsLocal;

    lstPointsLocal = ClsBaseTopology::getCellsForArborizationRect(fXCenter, fYCenter, clsBaseArborization);

    /* calculate distance */
    int iGroupWidth = pclsWidth->getValue();
    int iGroupHeight = pclsHeight->getValue();
    bool bVerticalCylinder = clsBoolParameterVerticalCylinder->getValue();
    bool bHorizontalCylinder = clsBoolParameterHorizontalCylinder->getValue();
    vector<tiPoint>::iterator it;
    for(it=lstPointsLocal.begin();it!=lstPointsLocal.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;

	/* 1. only use points within the group boundaries */
	/* 20050319 */
	double fDist = calculateDistance(fXCenter, fYCenter, iX, iY);

	/* vertical cylinder */
	if(bVerticalCylinder){
	    iX = (iX <= iGroupWidth ? iX : iX-iGroupWidth);
	    iX = (iX > 0 ? iX : iGroupWidth+iX);
	}

	/* horizontal cylinder */
	if(bHorizontalCylinder){
	    iY = (iY <= iGroupHeight ? iY : iY-iGroupHeight);
	    iY = (iY > 0 ? iY : iGroupHeight+iY);
	}


	if(iX>0 && iX<=iGroupWidth && iY>0 && iY<=iGroupHeight){
	    int iIndex = pos2index(iX, iY);
	    tIndexDist tid(iIndex, fDist);
	    lst.push_back(tid);
	}
    }
    return lst;
}


vector<tiPoint> ClsTopologyRect::checkList(const vector<tiPoint> &lstIn) {
    vector<tiPoint> lst;

    int iWidth = pclsWidth->getValue();
    int iHeight = pclsHeight->getValue();

    vector<tiPoint>::const_iterator it;
    for(it=lstIn.begin(); it!=lstIn.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	if(iX <= iWidth && iY <= iHeight){
	    
	    tiPoint pointTemp;
	    pointTemp.first = iX;
	    pointTemp.second = iY;

	    lst.push_back(pointTemp);
	}
    }
//--    lst.sort(pointCompareRect(iWidth));
    sort(lst.begin(), lst.end(), pointCompareRect(iWidth));

    return lst;

}

tiPoint ClsTopologyRect::index2pos(int iIndex){
    int iGroupWidth = pclsWidth->getValue();
    int iY = (int)(iIndex/iGroupWidth) + 1;
    int iX = iIndex - (iY-1)*iGroupWidth +1;

    tiPoint pointTemp;
    pointTemp.first = iX;
    pointTemp.second = iY;

    return pointTemp;
};

int ClsTopologyRect::pos2index(int _iWidth, int iX, int iY) {
    int iIndex = (iY - 1) * _iWidth + iX -1;
    return iIndex;
}

int ClsTopologyRect::pos2index(int iX, int iY) {
    int iGroupWidth = pclsWidth->getValue();
    int iIndex = (iY - 1) * iGroupWidth + iX -1;
    return iIndex;
}

list<int> ClsTopologyRect::posList2indexList(vector<tiPoint> lstIn) {
    list<int> lstReturn;
    int iGroupWidth = pclsWidth->getValue();

    vector<tiPoint>::iterator it;
    for(it=lstIn.begin(); it!=lstIn.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	int iIndex = pos2index(iGroupWidth, iX, iY);
	lstReturn.push_back(iIndex);
    }
    return lstReturn;
}



bool ClsTopologyRect::isValidIndex(vector<tiPoint> _lst, int iGroupWidth, int iIndex) {
//    cout << "ClsTopologyRect::isValidIndex(vector<tiPoint> _lst, int iGroupWidth, int iIndex)" << endl;

    int _iX;
    int _iY;

    _iY = iIndex / iGroupWidth + 1;
    _iX = iIndex + 1 - iGroupWidth * _iY + iGroupWidth;

//    cout << "iIndex, _iX, _iY: " << iIndex << ", " << _iX << ", " << _iY << endl;


    vector<tiPoint>::iterator it;
    for(it=_lst.begin(); it!=_lst.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	if(_iX == iX && _iY == iY){
	    return true;
	}
    }
    return false;
}






vector<tiPoint> ClsTopologyRect::getPoints4Ellipse(double fXCenter, double fYCenter, double fWidth, double fHeight) {
    vector<tiPoint> lst;

    double fa = fWidth/2.;
    double fb = fHeight / 2.;

    for(double fx=ceil(-fWidth/2. + fXCenter); fx<=floor(fWidth/2. + fXCenter); fx++) {
	double fxx = fx - ceil(-fWidth/2. + fXCenter) - (floor(fWidth/2. + fXCenter) - ceil(-fWidth/2. + fXCenter)) / 2.;
	double fYPrime = sqrt(pow(fb,2) - (pow(fxx,2) * pow(fb,2)) / pow(fa,2));
	for(double fy=ceil(-fYPrime + fYCenter); fy<=floor(fYPrime + fYCenter); fy++) {
	    tiPoint pointTemp;
	    pointTemp.first = (int)fx;
	    pointTemp.second = (int)fy;
	    lst.push_back(pointTemp);
	}
    }
    return lst;
}


vector<tiPoint> ClsTopologyRect::getPoints4Rect(double fXCenter, double fYCenter, double fWidth, double fHeight) {
//    cout << " ClsTopologyRect::getPoints4Rect(double fXCenter, double fYCenter, double fWidth, double fHeight)" << endl;
    vector<tiPoint> lst;
    for(double fx=ceil(-fWidth/2. + fXCenter); fx<=floor(fWidth/2. + fXCenter); fx++) {
//	    cout << "fx:" << fx << endl;
	for(double fy=ceil(-fHeight/2. + fYCenter); fy<=floor(fHeight/2. + fYCenter); fy++) {
	    tiPoint pointTemp;
	    pointTemp.first = (int)fx;
	    pointTemp.second = (int)fy;
	    lst.push_back(pointTemp);
	}
    }
    return lst;
}



vector<tiPoint> ClsTopologyRect::getPoints4Rect(int iXStart, int iYStart, int iWidth, int iHeight) {
//    cout << "ClsTopologyRect::getPoints4Rect(int iXStart, int iYStart, int iWidth, int iHeight)" << endl;
    vector<tiPoint> lstPointsReturn;

    for(int ii=iYStart; ii<iYStart+iHeight; ii++){
	for(int jj=iXStart; jj<iXStart+iWidth; jj++){
	    tiPoint pointTemp;
	    pointTemp.first = jj;
	    pointTemp.second = ii;

	    lstPointsReturn.push_back(pointTemp);
	}
    }
    return lstPointsReturn;
}


list<int> ClsTopologyRect::getPoints4Rect(int iGroupWidth, int iRectXStart, int iRectYStart, int iRectWidth, int iRectHeight) {
    list<int> lstIndexReturn;

    int iPosStart =  pos2index(iGroupWidth, iRectXStart, iRectYStart);
    for(int ii=0; ii<iRectHeight; ii++){
	for(int jj=iPosStart; jj<iPosStart+iRectWidth; jj++){
	    lstIndexReturn.push_back(jj);
	}
	iPosStart += iGroupWidth;
    }
    return lstIndexReturn;
}


vector<tiPoint> ClsTopologyRect::getAllPoints(int iWidth, int iHeight) {
    vector<tiPoint> lstPointsReturn;

    for(int iy=1; iy<=iHeight; iy++){
	for(int ix=1; ix<=iWidth; ix++){
	    tiPoint pointTemp;
	    pointTemp.first = ix;
	    pointTemp.second = iy;
	    lstPointsReturn.push_back(pointTemp);
	}
    }

    return lstPointsReturn;

}


double ClsTopologyRect::calculateDistance(tiPoint p0, tiPoint p1){
    return sqrt(pow((double)(p0.first - p1.first),2.) + pow((double)(p0.second - p1.second),2.));
}

double ClsTopologyRect::calculateDistance(double fAx, double fAy, double fBx, double fBy){
    return sqrt(pow((fAx - fBx),2.) + pow((fAy - fBy),2.));
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
