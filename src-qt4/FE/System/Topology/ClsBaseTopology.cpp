#include <algorithm> //MARK

#include "ClsBaseTopology.h"
#include "ClsSubPopulations.h"

#include "ClsRandom.h"
#include "ClsArbAll.h"
#include "ClsArbEllipse.h"
#include "ClsArbEllipseWindow.h"
#include "ClsArbRandom.h"
#include "ClsArbRect.h"
#include "ClsArbRectWindow.h"

//#define DEBUG_CLSBASETOPOLOGY




ClsBaseTopology::ClsBaseTopology(){
#ifdef DEBUG_CLSBASETOPOLOGY
    cout << "ClsBaseTopology::ClsBaseTopology()" << endl;
#endif
    bRuntimeEditable = false;
}

ClsBaseTopology::~ClsBaseTopology() {
#ifdef DEBUG_CLSBASETOPOLOGY
    cout << "ClsBaseTopology::~ClsBaseTopology()" << endl;
#endif
}


list<tiPoint> ClsBaseTopology::getCellsForArborizationRect(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization){
#ifdef DEBUG_CLSBASETOPOLOGY
    cout << "ClsBaseTopology::getCellsForArborization(double fXCenter, double fYCenter, ClsBaseArborization *clsBaseArborization)" << endl;
#endif

    list<tiPoint> lstPointsLocal;
    list<tiPoint> lstPointsLocalProbAdjusted;
    if(dynamic_cast<ClsArbAll*>(clsBaseArborization)){
	for(int iY=1;iY<=nrCellsVertical();iY++){
	    for(int iX=1;iX<=nrCellsHorizontal();iX++){
		tiPoint p(iX, iY);
		lstPointsLocal.push_back(p);
	    }
	}
    }
    else if(dynamic_cast<ClsArbRandom*>(clsBaseArborization)){
	int iMax = Size() -1;
//--	int iArbCount = iqrUtils::string2int(dynamic_cast<ClsArbRandom*>(clsBaseArborization)->getParameter(ClsTagLibrary::ArborizationCountTag())->getValueAsString());
	int iArbCount = dynamic_cast<ClsArbRandom*>(clsBaseArborization)->getCount();


	list<int> lst2 =  ClsRandom::getWeakRandom(0, iMax, iArbCount, true);
	list<int>::iterator itl;
	for(itl=lst2.begin(); itl!=lst2.end(); itl++){
	    tiPoint p = index2pos((*itl));
	    lstPointsLocal.push_back(p);
	}
    }
    else if(dynamic_cast<ClsArbEllipse*>(clsBaseArborization)){
//--	int iArbWidth =  iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationWidthTag())->getValueAsString());
//--	int iArbHeight = iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationHeightTag())->getValueAsString());
	int iArbWidth =  dynamic_cast<ClsArbEllipse*>(clsBaseArborization)->getWidth();
	int iArbHeight = dynamic_cast<ClsArbEllipse*>(clsBaseArborization)->getHeight();



	lstPointsLocal = getPoints4Ellipse(fXCenter, fYCenter, iArbWidth, iArbHeight);
    }
    else if(dynamic_cast<ClsArbEllipseWindow*>(clsBaseArborization)){
//--	int iArbOuterWidth =  iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationOuterWidthTag())->getValueAsString());
//--	int iArbOuterHeight = iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationOuterHeightTag())->getValueAsString());
//--	int iArbInnerWidth =  iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationInnerWidthTag())->getValueAsString());
//--	int iArbInnerHeight = iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationInnerHeightTag())->getValueAsString());

	int iArbInnerWidth =  dynamic_cast<ClsArbEllipseWindow*>(clsBaseArborization)->getInnerWidth();
	int iArbInnerHeight = dynamic_cast<ClsArbEllipseWindow*>(clsBaseArborization)->getInnerHeight();
	int iArbOuterWidth =  dynamic_cast<ClsArbEllipseWindow*>(clsBaseArborization)->getOuterWidth();
	int iArbOuterHeight = dynamic_cast<ClsArbEllipseWindow*>(clsBaseArborization)->getOuterHeight();


	list<pair<int, int> > lstPointsInner;
	lstPointsInner = getPoints4Ellipse(fXCenter, fYCenter, (double)iArbInnerWidth, (double)iArbInnerHeight);

	list<pair<int, int> > lstPointsOuter;
	lstPointsOuter = getPoints4Ellipse(fXCenter, fYCenter, (double)iArbOuterWidth, (double)iArbOuterHeight);

	lstPointsLocal = XOrLists(lstPointsOuter, lstPointsInner);
    }
    else if(dynamic_cast<ClsArbRect*>(clsBaseArborization)){
//--	int iArbWidth =  iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationWidthTag())->getValueAsString());
//--	int iArbHeight = iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationHeightTag())->getValueAsString());

	int iArbWidth =  dynamic_cast<ClsArbRect*>(clsBaseArborization)->getWidth();
	int iArbHeight = dynamic_cast<ClsArbRect*>(clsBaseArborization)->getHeight();


	lstPointsLocal = getPoints4Rect(fXCenter, fYCenter, (double)iArbWidth, (double)iArbHeight);
    }
    else if(dynamic_cast<ClsArbRectWindow*>(clsBaseArborization)){
//--	int iArbOuterWidth =  iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationOuterWidthTag())->getValueAsString());
//--	int iArbOuterHeight = iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationOuterHeightTag())->getValueAsString());
//--	int iArbInnerWidth =  iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationInnerWidthTag())->getValueAsString());
//--	int iArbInnerHeight = iqrUtils::string2int(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationInnerHeightTag())->getValueAsString());

	int iArbInnerWidth =  dynamic_cast<ClsArbRectWindow*>(clsBaseArborization)->getInnerWidth();
	int iArbInnerHeight = dynamic_cast<ClsArbRectWindow*>(clsBaseArborization)->getInnerHeight();
	int iArbOuterWidth =  dynamic_cast<ClsArbRectWindow*>(clsBaseArborization)->getOuterWidth();
	int iArbOuterHeight = dynamic_cast<ClsArbRectWindow*>(clsBaseArborization)->getOuterHeight();


	list<pair<int, int> > lstPointsInner;
	lstPointsInner = getPoints4Rect(fXCenter, fYCenter, (double)iArbInnerWidth, (double)iArbInnerHeight);

	list<pair<int, int> > lstPointsOuter;
	lstPointsOuter = getPoints4Rect(fXCenter, fYCenter, (double)iArbOuterWidth, (double)iArbOuterHeight);

	lstPointsLocal = XOrLists(lstPointsOuter, lstPointsInner);
    }

//--    double fProbability =  iqrUtils::string2double(clsBaseArborization->getParameter(ClsTagLibrary::ArborizationInitializationProbabilityTag())->getValueAsString());
    double fProbability =  clsBaseArborization->getProbability();


    list<int> lst2 =  ClsRandom::getWeakRandom(0, 100, lstPointsLocal.size(), false);
    list<tiPoint>::iterator it;
    list<int>::iterator itRand= lst2.begin();
    int aa=0;
    for(it=lstPointsLocal.begin();it!=lstPointsLocal.end();it++){
	aa++;
//	cout << aa << "\t" << *itRand << endl;
	if((*itRand)<=(fProbability*100)){
	    lstPointsLocalProbAdjusted.push_back((*it));
	}
	itRand++;
    }
//    cout << "size: " << lstPointsLocalProbAdjusted.size() << endl;

    return lstPointsLocalProbAdjusted;
};


list<pair<int, int> > ClsBaseTopology::AndLists(const list<pair<int, int> >& lst0, const list<pair<int, int> > &lst1) {
#ifdef DEBUG_CLSBASETOPOLOGY
    cout << "ClsBaseTopology::AndLists(const list<pair<int, int> >& lst0, const list<pair<int, int> > &lst1)" << endl;
#endif
    list<pair<int, int> > lstReturn;

    list<pair<int, int> >::const_iterator it0;
    for(it0 = lst0.begin(); it0!=lst0.end(); it0++){
	if(find(lst1.begin(), lst1.end(), *it0)  != lst1.end() ) {
	    lstReturn.push_back((*it0));
	}
    }
    return lstReturn;
}

list<pair<int, int> > ClsBaseTopology::XOrLists(const list<pair<int, int> > &lst0, const list<pair<int, int> > &lst1) {
#ifdef DEBUG_CLSBASETOPOLOGY
    cout << "ClsBaseTopology::XOrLists(const list<pair<int, int> > &lst0, const list<pair<int, int> > &lst1)" << endl;
#endif
    list<pair<int, int> > lstReturn;

    list<pair<int, int> >::const_iterator it0;
    for(it0 = lst0.begin(); it0!=lst0.end(); it0++){
	if(find(lst1.begin(), lst1.end(), *it0)  == lst1.end() ) {
	    lstReturn.push_back((*it0));
	}
    }
    return lstReturn;
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
