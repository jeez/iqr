#ifndef BASETOPOLOGY_HPP
#define BASETOPOLOGY_HPP

#include <string>
#include <map>

#include "item.hpp"
#include "iqrUtils.h"
#include "ClsHyperLists.h"

using namespace std;
using namespace iqrcommon;

class ClsBaseSubPopulation;
class ClsBaseArborization;


class ClsBaseTopology : public ClsItem {

public:
    ClsBaseTopology();
    virtual ~ClsBaseTopology();

    virtual vector<tiPoint> getPointsForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation) = 0;
    virtual list<int> getIndicesForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation) = 0;
    virtual list<tIndexDist> getCellsForArborization(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization) = 0;


    virtual vector<tiPoint>getPoints4Rect(double fXCenter, double fYCenter, double fWidth, double fHeight) = 0;
    virtual vector<tiPoint> getPoints4Ellipse(double fXCenter, double fYCenter, double fWidth, double fHeight) = 0;

    virtual int Size() = 0;
    virtual int nrCellsHorizontal() = 0;
    virtual int nrCellsVertical() = 0;
    virtual double DistMax() = 0;

    virtual tiPoint index2pos(int iIndex)=0;
    virtual int pos2index(int /* iX */, int /* iY */) = 0;
    virtual vector<tiPoint> checkList(const vector<tiPoint> &lstIn) = 0;

    static vector<tiPoint> AndLists(const vector<tiPoint> &lst0, const vector<tiPoint> &lst1);
    static vector<tiPoint> XOrLists(const vector<tiPoint> &lst0, const vector<tiPoint> &lst1);

protected:
    virtual vector<tiPoint> getCellsForArborizationRect(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization);
};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
