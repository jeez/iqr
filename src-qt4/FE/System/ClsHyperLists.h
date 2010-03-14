/****************************************************************************
 ** $Filename: ClsHyperLists.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Sep 12 15:37:48 2003
 **
 *****************************************************************************/


#ifndef CLSHYPERLISTS_H
#define CLSHYPERLISTS_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <iomanip>
#include <iostream>
#include <list>
#include <vector>


using namespace std;

typedef pair<int, double> tIndexDist;

class IndexTriple {
public:
    IndexTriple(int _first, int _second, double _value) : first(_first), second(_second), value(_value) {
    }

    bool operator<(const IndexTriple& /*tt*/){
	return false;
    }

    int first, second;
    double value;
};


class IndexQuadruple {
public:
    IndexQuadruple(int _first, int _second, double _distance, int _delay) : first(_first), second(_second), distance(_distance), delay(_delay) {
    }

    bool operator<(const IndexQuadruple& /*tt*/){
	return false;
    }

    int first, second;
    double distance;
    int delay;
    double attenuation;
};




typedef pair<int, int> tiPoint;
typedef pair<double, double> tfPoint;

class PointTriple {
public:
    PointTriple(tfPoint _first, tfPoint _second, double _value) : first(_first), second(_second), value(_value) {
    }

    bool operator<(const PointTriple& /*pp*/){
	return false;
    }

    tfPoint first, second;
    double value;
};


typedef list<PointTriple> ListPointTriples;

class ListIndexTriples : public list<IndexTriple> {

public:
    list<pair<int, double> > getByFirst(int i);
    list<pair<int, double> > getBySecond(int i);

private:
    friend std::ostream& operator<<(std::ostream &_stream, ListIndexTriples  &_tl);
};

// struct allows easy return of multiple values.
typedef struct Indices {
    Indices(int _neuronPre, int _neuronPost, int _synapse, int _delay, double _attenuation)
	: neuronPre(_neuronPre),
	  neuronPost(_neuronPost),
	  synapse(_synapse),
	  delay(_delay),
	  attenuation(_attenuation)
	  
    {}
    int    neuronPre;
    int    neuronPost;
    int    synapse;
    int    delay;
    double attenuation;
};

class ListIndexQuadruples : public vector<IndexQuadruple> {

public:
    vector<pair<int, double> > getDistanceByFirst(int i);
    vector<pair<int, double> > getDistanceBySecond(int i);

    vector<pair<int, double> > getAttenuationByFirst(int i);
    vector<pair<int, double> > getAttenuationBySecond(int i);

    vector<pair<int, int> > getDelayByFirst(int i);
    vector<pair<int, int> > getDelayBySecond(int i);

    // 22.10.2003 [jmb] New access functions to extract data for
    // connection building.
    vector<Indices> getIndicesByFirst(int i);
    vector<Indices> getIndicesBySecond(int i);
    vector<Indices> getIndicesByDelay(int i);
    Indices getIndicesBySynapse(int i);

    double getMinDistance();
    double getMaxDistance();

    int getMinDelay();
    int getMaxDelay();

    double getMinAttenuation();
    double getMaxAttenuation();


    int getDelayCount(); /* use unique */ 
    list<int> getDelays();

    int getSecondCount();
    list<int> getSeconds();

private:
    friend std::ostream& operator<<(std::ostream &_stream, ListIndexQuadruples  &_tl);
};





#endif /* CLSHYPERLISTS_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
