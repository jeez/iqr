/****************************************************************************
 ** $Filename: ClsSubPopulations.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Jun  3 23:11:16 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSSUBPOPULATIONS_H
#define CLSSUBPOPULATIONS_H    /*+ To stop multiple inclusions. +*/

#include <iostream>
#include <string>
#include <list>
#include <map>

//#include "item.hpp"

#include "ClsSysFileNode.h"
#include "tagLibrary.hpp"
#include "ClsHyperLists.h"

using namespace iqrcommon;
using namespace std;


class ClsBaseSubPopulation {//: public ClsItem  {

public:
    ClsBaseSubPopulation();
    virtual ~ClsBaseSubPopulation();
//    virtual ClsBaseSubPopulation& operator=(const ClsBaseSubPopulation& r);

    virtual string Type();
    virtual void print();
    virtual void setParameter(ClsSysFileNode &_clsSysFileNode);

protected:
    string strType;
};
 

class ClsRegion : public ClsBaseSubPopulation  { //, public ClsItem {
public:
    ClsRegion();
    


//    ClsRegion(ClsRegion& r); 


    ClsRegion& operator=(const ClsRegion& r);
    void setParameter(ClsSysFileNode &_clsSysFileNode);

    void print();
    
    int getXStart();
    int getYStart();
    int getWidth();
    int getHeight();
    
    void setXStart(int i);
    void setYStart(int i);
    void setWidth(int i);
    void setHeight(int i);
    
    
private:
    int xStart, yStart;
    int width, height;

};

class ClsList : public ClsBaseSubPopulation {
public:
    ClsList();
    void setData( vector<tiPoint> _lst);
    vector<tiPoint> getData();
    void setParameter(ClsSysFileNode &_clsSysFileNode);
    void print();
    
private:
    vector<tiPoint> lst; 

    
};

class ClsAll : public ClsBaseSubPopulation {
public:
    ClsAll();
    void print();

};

//--typedef vector<tiPoint> tListOfPairs;
typedef vector<tiPoint> tListOfPoints;

class ClsTuples : public ClsBaseSubPopulation {
public:
    ClsTuples();
//    ClsTuples& operator=(const ClsTuples& r);
    void addData(tListOfPoints lstSource, tListOfPoints lstTarget);
    list<pair<tListOfPoints, tListOfPoints> > getData();
    void setData(list<pair<tListOfPoints, tListOfPoints> > lst);

    void setParameter(ClsSysFileNode &_clsSysFileNode);
    void print();

private:
    list<pair<tListOfPoints, tListOfPoints> > lstTuples;
  
    
};


#endif /* CLSSUBPOPULATIONS_H */

/// Local Variables: 
/// mode: c++
/// End: 

