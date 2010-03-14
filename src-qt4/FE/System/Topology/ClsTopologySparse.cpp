#include <algorithm> //MARK

#include "intParameter.hpp"
#include "ClsSubPopulations.h"
#include "tagLibrary.hpp"

#include "ClsTopologySparse.h"

#include "ClsArbAll.h"
#include "ClsArbEllipse.h"
#include "ClsArbEllipseWindow.h"
#include "ClsArbRandom.h"
#include "ClsArbRect.h"
#include "ClsArbRectWindow.h"

#include "item.hpp"
#include "iqrUtils.h"

//#define DEBUG_CLSTOPOLOGYSPARSE


pointCompareSparse::pointCompareSparse(int _iWidth) : iWidth(_iWidth)  {
}

bool pointCompareSparse::operator() (const tiPoint &p0, const tiPoint &p1) {
//    cout << "pointCompareSparse::operator()" << endl;
    return ClsTopologySparse::pos2indexFullGrid(iWidth,p0.first,p0.second) < ClsTopologySparse::pos2indexFullGrid(iWidth,p1.first,p1.second);
}

ClsTopologySparse::ClsTopologySparse() {//: ClsBaseTopology(ClsTagLibrary::SparseTopologyTag()) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse()" << endl;
#endif
    strType = ClsTagLibrary::SparseTopologyTag();
}

ClsTopologySparse& ClsTopologySparse::operator=(const ClsTopologySparse& r) {  /* copy operator */
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::operator=(const ClsTopologySparse& r)" << endl;
#endif
    lstPoints = r.lstPoints;

//XXXXX     iGroupWidth = 0;
//XXXXX
//XXXXX     list<tiPoint >::iterator it;
//XXXXX     for(it=lstPoints.begin(); it!=lstPoints.end();it++){
//XXXXX	int iX = (*it).first;
//XXXXX	iGroupWidth = (iX > iGroupWidth ? iX : iGroupWidth);
//XXXXX     }
//XXXXX

    iGroupWidth = nrCellsHorizontal();
    lstPoints.sort(pointCompareSparse(iGroupWidth));

    lstPoints.sort(pointCompareSparse(iGroupWidth));
    return *this;
}


void ClsTopologySparse::setList(list<tiPoint > _lst) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::setList(list<tiPoint > _lst)" << endl;
#endif
    lstPoints = _lst;
    iGroupWidth = 0;

/*
  list<tiPoint >::iterator it;
  for(it=lstPoints.begin(); it!=lstPoints.end();it++){
  int iX = (*it).first;
  iGroupWidth = (iX > iGroupWidth ? iX : iGroupWidth);
  }
*/

    iGroupWidth = nrCellsHorizontal();
    lstPoints.sort(pointCompareSparse(iGroupWidth));

/*
    list<tiPoint >::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	    iGroupWidth = (iX > iGroupWidth ? iX : iGroupWidth);
	    cout << "iX, iY: " << iX << ", " << iY << endl;
    }
*/
}

int ClsTopologySparse::nrCellsHorizontal() {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::nrCellsHorizontal()" << endl;
#endif
    int iHMax = 0;
    int iHMin = 99999;

    list<tiPoint >::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iX = (*it).first;
	iHMax = (iX > iHMax ? iX : iHMax);
	iHMin = (iX < iHMin ? iX : iHMin);
    }
//    cout << "iHMin, iHMax: " << iHMin << ", " << iHMax << endl;
    return iHMax - iHMin +1;
}

int ClsTopologySparse::nrCellsVertical() {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::nrCellsVertical()" << endl;
#endif
    int iVMax = 0;
    int iVMin = 99999;

    list<tiPoint >::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iY = (*it).second;
	iVMax = (iY > iVMax ? iY : iVMax);
	iVMin = (iY < iVMin ? iY : iVMin);
    }
//    cout << "iVMin, iVMax: " << iVMin << ", " << iVMax << endl;
    return iVMax - iVMin +1;
}

double ClsTopologySparse::DistMax(){
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::DistMax()" << endl;
#endif
    int iGroupHeight = 0;
    list<tiPoint >::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iY = (*it).second;
	iGroupHeight = (iY > iGroupHeight ? iY : iGroupHeight);
    }
    return calculateDistance(1., 1., (double)iGroupWidth, (double)iGroupWidth);
}

int ClsTopologySparse::Size()  {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::Size()" << endl;
#endif
    return lstPoints.size();
}

list<tiPoint > ClsTopologySparse::getPointsForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation){
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::getPointsForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation)" << endl;
#endif

    list<tiPoint > lstPointsReturn;

    if(dynamic_cast<ClsAll*>(clsBaseSubPopulation)){
	lstPointsReturn = lstPoints;
    }

    else if(dynamic_cast<ClsList*>(clsBaseSubPopulation)){
	list<tiPoint > lstPopulationList = (dynamic_cast<ClsList*>(clsBaseSubPopulation))->getData();
	lstPointsReturn = AndLists(lstPoints, lstPopulationList);
    }
    else if(dynamic_cast<ClsRegion*>(clsBaseSubPopulation)){
	ClsRegion* clsRegion = dynamic_cast<ClsRegion*>(clsBaseSubPopulation);
	list<tiPoint >lstRegion = region2pointList(clsRegion);
	lstPointsReturn = AndLists(lstPoints, lstRegion);
    }

    return lstPointsReturn;

}

list<int> ClsTopologySparse::getIndicesForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation){
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::getIndicesForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation)" << endl;
#endif

    list<int> lst;
    lst =  posList2indexList(getPointsForSubPopulation(clsBaseSubPopulation));
    return lst;
};


list<tIndexDist> ClsTopologySparse::getCellsForArborization(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::getCellsForArborization(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization)" << endl;
#endif
    list<tIndexDist> lst;
    list<pair<int, int> > lstPointsLocal;

    lstPointsLocal = ClsBaseTopology::getCellsForArborizationRect(fXCenter, fYCenter, clsBaseArborization);

    /* calculate distance */
    int iGroupHeight = 0;
    list<tiPoint >::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iY = (*it).second;
	iGroupHeight = (iY > iGroupHeight ? iY : iGroupHeight);
    }

    for(it=lstPointsLocal.begin();it!=lstPointsLocal.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	/* only use points within the group boundaries */
	if(isValidCell(iX, iY) && iX>0 && iX<=iGroupWidth && iY>0 && iY<=iGroupHeight){
	    double fDist = calculateDistance(fXCenter, fYCenter, iX, iY);
	    int iIndex = pos2index(iX, iY);
	    tIndexDist tid(iIndex, fDist);
	    lst.push_back(tid);
	}
    }
    return lst;
}

list<tiPoint > ClsTopologySparse::checkList(const list<tiPoint >& lstIn) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::checkList(const list<tiPoint >& lstIn)" << endl;
#endif

    list<tiPoint > lst;
    list<tiPoint >::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	if(isValidCell(lstIn, iX, iY)){
	    tiPoint pairTemp(iX, iY);
	    lst.push_back(pairTemp);
	}
    }

    lst.sort(pointCompareSparse(iGroupWidth));
    return lst;
}



list<tiPoint > ClsTopologySparse::getPoints4Ellipse(double fXCenter, double fYCenter, double fWidth, double fHeight) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::getPoints4Ellipse(double fXCenter, double fYCenter, double fWidth, double fHeight)" << endl;
#endif

    list<tiPoint > lst;
    double fa = fWidth/2.;
    double fb = fHeight / 2.;
    for(double fx=ceil(-fWidth/2. + fXCenter); fx<=floor(fWidth/2. + fXCenter); fx++) {
	double fxx = fx - ceil(-fWidth/2. + fXCenter) - (floor(fWidth/2. + fXCenter) - ceil(-fWidth/2. + fXCenter)) / 2.;
	double fYPrime = sqrt(pow(fb,2) - (pow(fxx,2) * pow(fb,2)) / pow(fa,2));
	for(double fy=ceil(-fYPrime + fYCenter); fy<=floor(fYPrime + fYCenter); fy++) {
	    tiPoint  p((int)fx, (int)fy);
	    lst.push_back(p);
	}
    }
    list<tiPoint > lstReturn = AndLists(lst, lstPoints);
    return lstReturn;
}


list<tiPoint > ClsTopologySparse::getPoints4Rect(double fXCenter, double fYCenter, double fWidth, double fHeight) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::getPoints4Rect(double fXCenter, double fYCenter, double fWidth, double fHeight)" << endl;
#endif

    list<tiPoint > lst;
    for(double fx=ceil(-fWidth/2. + fXCenter); fx<=floor(fWidth/2. + fXCenter); fx++) {
//	    cout << "fx:" << fx << endl;
	for(double fy=ceil(-fHeight/2. + fYCenter); fy<=floor(fHeight/2. + fYCenter); fy++) {
	    tiPoint  p((int)fx, (int)fy);
	    lst.push_back(p);
	}
    }
    list<tiPoint > lstReturn = AndLists(lst, lstPoints);
    return lstReturn;
}

list<tiPoint > ClsTopologySparse::region2pointList(ClsRegion *clsRegion) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::region2pointList(ClsRegion *clsRegion)" << endl;
#endif

    list<tiPoint > lst;
    int iRegionXStart = clsRegion->getXStart();
    int iRegionYStart = clsRegion->getYStart();
    int iRegionWidth = clsRegion->getWidth();
    int iRegionHeight = clsRegion->getHeight();

    for(int ii=iRegionYStart; ii<iRegionYStart+iRegionHeight; ii++){
	for(int jj=iRegionXStart; jj<iRegionXStart+iRegionWidth; jj++){
	    tiPoint pairTemp(jj, ii);
	    lst.push_back(pairTemp);
	}
    }
    return lst;
}


bool ClsTopologySparse::isValidCell(int _iX, int _iY) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::isValidCell(int _iX, int _iY)" << endl;
#endif

/* old way
   list<tiPoint >::iterator it;
   for(it=lstPoints.begin(); it!=lstPoints.end();it++){
   int iX = (*it).first;
   int iY = (*it).second;
   if(_iX == iX && _iY == iY){
   return true;
   }
   }
   return false;
*/

    return isValidCell(lstPoints, _iX, _iY);

}

bool ClsTopologySparse::isValidCell(const list<tiPoint >& _lst, int _iX, int _iY) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::isValidCell(const list<tiPoint >& _lst, int _iX, int _iY)" << endl;
#endif

/* old way
   list<tiPoint >::iterator it;
   for(it=_lst.begin(); it!=_lst.end();it++){
   int iX = (*it).first;
   int iY = (*it).second;
   if(_iX == iX && _iY == iY){
   return true;
   }
   }
*/

    tiPoint p(_iX, _iY);
    if(find(_lst.begin(), _lst.end(), p)!=_lst.end()){
	return true;
    }

    return false;
}


bool ClsTopologySparse::isValidIndex(int /* _iWidth */, int iIndex) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::isValidIndex(int /* _iWidth */, int iIndex)" << endl;
#endif

    tiPoint p = index2pos(iIndex);
    return isValidCell(lstPoints, p.first, p.second);

/* old way
   int _iX;
   int _iY;

   _iX = iIndex + 1 - _iWidth*_iY + _iWidth;
   _iY = iIndex / _iWidth + 1;

   list<tiPoint >::iterator it;
   for(it=lstPoints.begin(); it!=lstPoints.end();it++){
   int iX = (*it).first;
   int iY = (*it).second;
   if(_iX == iX && _iY == iY){
   return true;
   }
   }
   return false;
*/
}

bool ClsTopologySparse::isValidIndex(const list<tiPoint >& _lst, int /* _iWidth */, int iIndex) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::isValidIndex(const list<tiPoint >& _lst, int /* _iWidth */, int iIndex)" << endl;
#endif

    tiPoint p = index2pos(iIndex);
    return isValidCell(_lst, p.first, p.second);

/* old way
   int _iX;
   int _iY;

   _iY = iIndex / _iWidth;
   _iX = iIndex + 1 - _iWidth*_iY + _iWidth;

   list<tiPoint >::iterator it;
   for(it=_lst.begin(); it!=_lst.end();it++){
   int iX = (*it).first;
   int iY = (*it).second;
   if(_iX == iX && _iY == iY){
   return true;
   }
   }
   return false;
*/
}


// tiPoint ClsTopologySparse::index2pos_old(int iIndex){
//     int iY = (int)(iIndex/iGroupWidth) + 1;
//     int iX = iIndex - (iY-1)*iGroupWidth +1;
//     tiPoint pairTemp(iX, iY);
//     return pairTemp;
//};

tiPoint ClsTopologySparse::index2pos(int iIndex){
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::index2pos(int iIndex)" << endl;
#endif
    lstPoints.sort(pointCompareSparse(iGroupWidth));
    list<tiPoint >::iterator it;
    it = lstPoints.begin();
    advance(it,iIndex);
    int iX = (*it).first;
    int iY = (*it).second;

    tiPoint pairTemp(iX, iY);
//    cout << "\tIindex: iX, iY: " << iIndex << ": " << iX << << iY << ", "  << endl;
    return pairTemp;
};


int ClsTopologySparse::pos2indexFullGrid(int _iWidth, int iX, int iY) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::pos2indexFullGrid(int _iWidth, int iX, int iY)" << endl;
#endif

    int iIndex = (iY - 1) * _iWidth + iX -1;
    return iIndex;
}

int ClsTopologySparse::pos2index(int _iWidth, int iX, int iY) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::pos2index(int _iWidth, int iX, int iY)" << endl;
#endif
    lstPoints.sort(pointCompareSparse(_iWidth));
    tiPoint p(iX, iY);
    list<tiPoint >::iterator pos;
    pos = find(lstPoints.begin(), lstPoints.end(), p);
    int iIndex = distance(lstPoints.begin(), pos);
    return iIndex;
}

// int ClsTopologySparse::pos2index_old(int iX, int iY) {
// //    cout << "iGroupWidth: " << iGroupWidth << endl;
//     int iIndex = (iY - 1) * iGroupWidth + iX -1;
//     return iIndex;
// }

int ClsTopologySparse::pos2index(int iX, int iY) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::pos2index(int iX, int iY)" << endl;
#endif
    lstPoints.sort(pointCompareSparse(iGroupWidth));
    tiPoint p(iX, iY);
    list<tiPoint >::iterator pos;
    pos = find(lstPoints.begin(), lstPoints.end(), p);
    int iIndex = distance(lstPoints.begin(), pos);
//    cout << "\tiX, iY, Iindex: " << iX << ", " << iY << ", " << iIndex << endl;
    return iIndex;
}


list<int> ClsTopologySparse::posList2indexList(const list<tiPoint >& lstIn) {
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::posList2indexList(const list<tiPoint >& lstIn)" << endl;
#endif
    list<int> lstReturn;
    list<tiPoint >::const_iterator it;
    for(it=lstIn.begin(); it!=lstIn.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	int iIndex = pos2index(iGroupWidth, iX, iY);
	lstReturn.push_back(iIndex);
    }
    return lstReturn;
}


double ClsTopologySparse::calculateDistance(tiPoint p0, tiPoint p1){
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::calculateDistance(tiPoint p0, tiPoint p1)" << endl;
#endif
    return sqrt(pow((double)(p0.first - p1.first),2.) + pow((double)(p0.second - p1.second),2.));
}

double ClsTopologySparse::calculateDistance(double fAx, double fAy, double fBx, double fBy){
#ifdef DEBUG_CLSTOPOLOGYSPARSE
    cout << "ClsTopologySparse::calculateDistance(double fAx, double fAy, double fBx, double fBy)" << endl;
#endif
    return sqrt(pow((fAx - fBx),2.) + pow((fAy - fBy),2.));
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
