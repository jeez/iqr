#include "tagLibrary.hpp"

#include "ClsSubPopulations.h"
#include "intParameter.hpp"
#include "optionsParameter.hpp"

#include "ClsTopologyHex.h"

ClsTopologyHex::ClsTopologyHex() {//: ClsBaseTopology(ClsTagLibrary::HexTopologyTag()) {

    strType = ClsTagLibrary::HexTopologyTag();


    ClsOptionsParameter *op;

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



    pclsOrientation = addOptionsParameter(ClsTagLibrary::TopologyOrientationTag(),
						 "Orientation",
						 false,
						 "Description",
						 "Properties");

    op = static_cast<ClsOptionsParameter*>(getParameter(ClsTagLibrary::TopologyOrientationTag()));
    op->addOption(ClsTagLibrary::TopologyOrientationTagHorizontal());
    op->addOption(ClsTagLibrary::TopologyOrientationTagVertical());



    pclsOffset = addIntParameter(ClsTagLibrary::TopologyOffsetTag(),
		    "Offset",
		    0,
		    0, 1000,
		    "Offset",
		    "Properties");


/*
    addOptionsParameter(ClsTagLibrary::TopologyOffsetTag(),
						 "Offset",
						 false,
						 "Description",
						 "Properties");

    op = static_cast<ClsOptionsParameter*>(getParameter(ClsTagLibrary::TopologyOffsetTag()));
    op->addOption(ClsTagLibrary::ExcitatoryConnectionTag());
    op->addOption(ClsTagLibrary::InhibitoryConnectionTag());
*/

}

int ClsTopologyHex::Size() {
    int iWidth = pclsWidth->getValue();
    int iHeight = pclsHeight->getValue();

    return iWidth * iHeight;
}


list<int> ClsTopologyHex::getCellsForSubPopulation(ClsBaseSubPopulation *clsBaseSubPopulation){

    if(dynamic_cast<ClsAll*>(clsBaseSubPopulation)){
    }
    else if(dynamic_cast<ClsList*>(clsBaseSubPopulation)){
    }
    else if(dynamic_cast<ClsRegion*>(clsBaseSubPopulation)){
    }


    list<int> lst;
    return lst;
};


list<tIndexDist> ClsTopologyHex::getCellsForArborization(tiPoint /* tPointCenter */, ClsBaseArborization* /* clsBaseArborization */) {
    list<tIndexDist> lst;
    cerr << "NOT IMPLEMENTED" << endl;
    exit(1);

    return lst;

}
