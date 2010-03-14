#ifndef TOPOLOGYRECT_H
#define TOPOLOGYRECT_H


#include "ClsBaseTopology.h"

using namespace iqrcommon;

class pointCompareRect  {
public:
    pointCompareRect(int _iWidth);
    bool operator() (const tiPoint &p0, const tiPoint &p1);

private:
    int iWidth;

};

class ClsTopologyRect : public ClsBaseTopology {

public:

    ClsTopologyRect();
    ~ClsTopologyRect() {};

    vector<tiPoint> getPointsForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation);
    list<int> getIndicesForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation);
    list<tIndexDist> getCellsForArborization(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization);

    vector<tiPoint>getPoints4Rect(double fXCenter, double fYCenter, double fWidth, double fHeight);
    vector<tiPoint>getPoints4Rect(int iXStart, int iYStart, int iWidth, int iHeight);
    list<int> getPoints4Rect(int iGroupWidth, int iRectXStart, int iRectYStart, int iRectWidth, int iRectHeight);
    vector<tiPoint> getPoints4Ellipse(double fXCenter, double fYCenter, double fWidth, double fHeight);

    int Size() ;
    int nrCellsHorizontal();
    int nrCellsVertical();
    double DistMax();

    tiPoint index2pos(int iIndex);
    int pos2index(int iX, int iY);
    list<int> posList2indexList(vector<tiPoint> lstIn);

    vector<tiPoint> checkList(const vector<tiPoint> &lstIn);
    static int pos2index(int _iWidth, int iX, int iY);

    static const char *pcName;

private:
    vector<tiPoint>getAllPoints(int iWidth, int iHeight);

    double calculateDistance(tiPoint p0, tiPoint p1);
    double calculateDistance(double fAx, double fAy, double fBx, double fBy);
    bool isValidIndex(vector<tiPoint> _lst, int iGroupWidth, int iIndex);
    ClsIntParameter *pclsWidth, *pclsHeight;
    ClsBoolParameter *clsBoolParameterVerticalCylinder;
    ClsBoolParameter *clsBoolParameterHorizontalCylinder;

    friend class pointCompareRect;

};

#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:




