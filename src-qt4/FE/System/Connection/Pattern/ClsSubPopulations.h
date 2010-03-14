/****************************************************************************
 ** $Filename: ClsSubPopulations.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Jun  3 23:11:16 2003
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
    void setData( list<pair<int, int> > _lst);
    list<pair<int, int> > getData();
    void setParameter(ClsSysFileNode &_clsSysFileNode);
    void print();
    
private:
    list<pair<int, int> > lst; 

    
};

class ClsAll : public ClsBaseSubPopulation {
public:
    ClsAll();
    void print();

};

typedef list<pair<int, int> > tListOfPairs;

class ClsTuples : public ClsBaseSubPopulation {
public:
    ClsTuples();
//    ClsTuples& operator=(const ClsTuples& r);
    void addData(tListOfPairs lstSource, tListOfPairs lstTarget);
    list<pair<tListOfPairs, tListOfPairs> > getData();
    void setData(list<pair<tListOfPairs, tListOfPairs> > lst);

    void setParameter(ClsSysFileNode &_clsSysFileNode);
    void print();

private:
    list<pair<tListOfPairs, tListOfPairs> > lstTuples;
  
    
};


#endif /* CLSSUBPOPULATIONS_H */

/// Local Variables: 
/// mode: c++
/// End: 

