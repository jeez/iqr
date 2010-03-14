#ifndef BASETOPOLOGY_HPP
#define BASETOPOLOGY_HPP

#include <string>
#include <map>
#include <list>

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

    virtual list<tiPoint> getPointsForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation) = 0;
    virtual list<int> getIndicesForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation) = 0;
    virtual list<tIndexDist> getCellsForArborization(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization) = 0;


    virtual list<tiPoint>getPoints4Rect(double fXCenter, double fYCenter, double fWidth, double fHeight) = 0;
    virtual list<tiPoint> getPoints4Ellipse(double fXCenter, double fYCenter, double fWidth, double fHeight) = 0;

    virtual int Size() = 0;
    virtual int nrCellsHorizontal() = 0;
    virtual int nrCellsVertical() = 0;
    virtual double DistMax() = 0;

    virtual tiPoint index2pos(int iIndex)=0;
    virtual int pos2index(int /* iX */, int /* iY */) = 0;
    virtual list<tiPoint> checkList(const list<tiPoint> &lstIn) = 0;


    static list<tiPoint> AndLists(const list<tiPoint> &lstOne, const list<tiPoint> &lstTwo);
    static list<tiPoint> XOrLists(const list<tiPoint> &lstOne, const list<tiPoint> &lstTwo);

protected:
    virtual list<tiPoint> getCellsForArborizationRect(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization);
};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
