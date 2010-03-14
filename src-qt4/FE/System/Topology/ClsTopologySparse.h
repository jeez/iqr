#ifndef TOPOLOGYSPARSE_H
#define TOPOLOGYSPARSE_H

#include <list>
#include "ClsBaseTopology.h"

class ClsRegion;

using namespace std;
using namespace iqrcommon;

class pointCompareSparse  {
public:
    pointCompareSparse(int _iWidth);
    bool operator() (const tiPoint &p0, const tiPoint &p1);

private:
    int iWidth;

};


class ClsTopologySparse : public ClsBaseTopology {

public:
    ClsTopologySparse();
    ~ClsTopologySparse() {};

    ClsTopologySparse& operator=(const ClsTopologySparse& r);

    list<tiPoint > getList() { return lstPoints; }
    void setList(list<tiPoint > _lst);

    int nrCellsHorizontal();
    int nrCellsVertical();
    double DistMax();
    int Size() ;

    list<tiPoint > getPointsForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation);
    list<int> getIndicesForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation);
    list<tiPoint >getPoints4Rect(double fXCenter, double fYCenter, double fWidth, double fHeight);
    list<tiPoint >getPoints4Rect(int iXStart, int iYStart, int iWidth, int iHeight);
    list<tiPoint > getPoints4Ellipse(double fXCenter, double fYCenter, double fWidth, double fHeight);

    list<tIndexDist> getCellsForArborization(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization);

//    tiPoint index2pos_old(int iIndex);
//    int pos2index_old(int iX, int iY);

    tiPoint index2pos(int iIndex);
    int pos2index(int iX, int iY);

    list<tiPoint > checkList(const list<tiPoint >& lstIn);
    list<int> posList2indexList(const list<tiPoint >& lstIn);
    bool isValidCell(int _iX, int _iY);

private:

    list<tiPoint > lstPoints;

    list<tiPoint > region2pointList(ClsRegion *clsRegion);

    bool isValidIndex(int iGroupWidth, int iIndex);
    bool isValidIndex(const list<tiPoint >& _lst, int iGroupWidth, int iIndex);

    bool isValidCell(const list<tiPoint >& _lst, int _iX, int _iY);

    static int pos2indexFullGrid(int _iWidth, int iX, int iY);
    int pos2index(int _iWidth, int iX, int iY);

    double calculateDistance(tiPoint p0, tiPoint p1);
    double calculateDistance(double fAx, double fAy, double fBx, double fBy);

    int iGroupWidth;

    friend class pointCompareSparse;
};

#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
