/****************************************************************************
 ** $Filename: ClsHyperLists.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Sep 12 15:37:56 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <limits.h> //MARK
#include "ClsHyperLists.h"


vector<pair<int, double> > ListIndexTriples::getByFirst(int i){
    vector<pair<int, double> > lst;
    vector<IndexTriple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	if((*it).first == i){
	    pair<int, double> pairTemp((*it).second, (*it).value);
	    lst.push_back(pairTemp);
	}
    }
    return lst;
};

vector<pair<int, double> > ListIndexTriples::getBySecond(int i){
    vector<pair<int, double> > lst;
    vector<IndexTriple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	if((*it).second == i){
	    pair<int, double> pairTemp((*it).first, (*it).value);
	    lst.push_back(pairTemp);
	}
    }
    return lst;
};



std::ostream& operator<<(std::ostream &_stream, ListIndexTriples  &_tl) {
    int iCount = 0;
    vector<IndexTriple>::iterator it;
    for(it=_tl.begin(); it!=_tl.end(); it++){
	iCount++;
	_stream << iCount << ": " << (*it).first << "<->"  << (*it).second << " d:" << (*it).value << endl;
    }
    return _stream;
};

/* --------------------------------------- */
vector<pair<int, double> > ListIndexQuadruples::getDistanceByFirst(int i){
    vector<pair<int, double> > lst;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	if((*it).first == i){
	    pair<int, double> pairTemp((*it).second, (*it).distance);
	    lst.push_back(pairTemp);
	}
    }
    return lst;
};

vector<pair<int, double> > ListIndexQuadruples::getDistanceBySecond(int i){
    vector<pair<int, double> > lst;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	if((*it).second == i){
	    pair<int, double> pairTemp((*it).first, (*it).distance);
	    lst.push_back(pairTemp);
	}
    }
    return lst;
};


vector<pair<int, double> > ListIndexQuadruples::getAttenuationByFirst(int i){
    vector<pair<int, double> > lst;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	if((*it).first == i){
	    pair<int, double> pairTemp((*it).second, (*it).attenuation);
	    lst.push_back(pairTemp);
	}
    }
    return lst;
};

vector<pair<int, double> > ListIndexQuadruples::getAttenuationBySecond(int i){
    vector<pair<int, double> > lst;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	if((*it).second == i){
	    pair<int, double> pairTemp((*it).first, (*it).attenuation);
	    lst.push_back(pairTemp);
	}
    }
    return lst;
};

vector<tiPoint> ListIndexQuadruples::getDelayByFirst(int i){
    vector<tiPoint> lst;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	if((*it).first == i){
	    tiPoint pairTemp((*it).second, (*it).delay);
	    lst.push_back(pairTemp);
	}
    }
    return lst;
};

vector<tiPoint> ListIndexQuadruples::getDelayBySecond(int i){
    vector<tiPoint> lst;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	if((*it).second == i){
	    tiPoint pairTemp((*it).first, (*it).delay);
	    lst.push_back(pairTemp);
	}
    }
    return lst;
};



vector<Indices> ListIndexQuadruples::getIndicesByFirst(int i){
    vector<Indices> lst;
    // Use a counter rather than an iterator in order to keep track of
    // the synapse number.  This assumes that synapses are ordered
    // according to the vector.  All vector users should adhere to this
    // convention.
    for(unsigned int synapse = 0; synapse < this->size(); ++synapse){
	if((*this)[synapse].first == i){
	    lst.push_back(Indices((*this)[synapse].first, 
				  (*this)[synapse].second, 
				  synapse,
				  (*this)[synapse].delay,
				  (*this)[synapse].attenuation));
	}
    }
    return lst;
};

vector<Indices> ListIndexQuadruples::getIndicesBySecond(int i){
    vector<Indices> lst;
    // Use a counter rather than an iterator in order to keep track of
    // the synapse number.  This assumes that synapses are ordered
    // according to the vector.  All vector users should adhere to this
    // convention.
    for(unsigned int synapse = 0; synapse < this->size(); ++synapse){
	if((*this)[synapse].second == i){
	    lst.push_back(Indices((*this)[synapse].first, 
				  (*this)[synapse].second, 
				  synapse,
				  (*this)[synapse].delay,
				  (*this)[synapse].attenuation));
	}
    }
    return lst;
};

vector<Indices> ListIndexQuadruples::getIndicesByDelay(int i){
    vector<Indices> lst;
    // Use a counter rather than an iterator in order to keep track of
    // the synapse number.  This assumes that synapses are ordered
    // according to the vector.  All vector users should adhere to this
    // convention.
    for(unsigned int synapse = 0; synapse < this->size(); ++synapse){
	if((*this)[synapse].delay == i){
	    lst.push_back(Indices((*this)[synapse].first, 
				  (*this)[synapse].second, 
				  synapse,
				  (*this)[synapse].delay,
				  (*this)[synapse].attenuation));
	}
    }
    return lst;
};

Indices ListIndexQuadruples::getIndicesBySynapse(int i)
{
    // WARNING: parameter value not checked against size of this.
    return Indices((*this)[i].first, (*this)[i].second, i, (*this)[i].delay, (*this)[i].attenuation);
};


double ListIndexQuadruples::getMinDistance(){
    double fDistanceMin = INT_MAX;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	fDistanceMin = ((*it).distance < fDistanceMin ? (*it).distance : fDistanceMin);
    }
    return fDistanceMin;
}

double ListIndexQuadruples::getMaxDistance(){
    double fDistanceMax = 0.;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	fDistanceMax = ((*it).distance > fDistanceMax ? (*it).distance : fDistanceMax);
    }
    return fDistanceMax;
}

int ListIndexQuadruples::getMinDelay(){
    int iDelayMin = INT_MAX;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	iDelayMin = ((*it).delay < iDelayMin ? (*it).delay : iDelayMin);
    }
    return iDelayMin;
}

int ListIndexQuadruples::getMaxDelay(){
    int iDelayMax = 0;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	iDelayMax = ((*it).delay > iDelayMax ? (*it).delay : iDelayMax);
    }
    return iDelayMax;
}

double ListIndexQuadruples::getMinAttenuation(){
    double fAttenuationMin = INT_MAX;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	fAttenuationMin = ((*it).attenuation < fAttenuationMin ? (*it).attenuation : fAttenuationMin);
    }
    return fAttenuationMin;
}

double ListIndexQuadruples::getMaxAttenuation(){
    double fAttenuationMax = 0.;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	fAttenuationMax = ((*it).attenuation > fAttenuationMax ? (*it).attenuation : fAttenuationMax);
    }
    return fAttenuationMax;
}

int ListIndexQuadruples::getDelayCount(){
    list<int> lst = getDelays();
    return lst.size();
}

list<int> ListIndexQuadruples::getDelays(){
    list<int> lst;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	lst.push_back((*it).delay);
    }
    // 22.10.2003 [jmb] Vector needs to be sorted before calling unique.
    lst.sort();
    lst.unique();
    return lst;
}


int ListIndexQuadruples::getSecondCount(){
    list<int> lst = getDelays();
    return lst.size();
}

list<int> ListIndexQuadruples::getSeconds(){
    list<int> lst;
    vector<IndexQuadruple>::iterator it;
    for(it=this->begin(); it!=this->end(); it++){
	lst.push_back((*it).second);
    }
    // 22.10.2003 [jmb] Vector needs to be sorted before calling unique.
    lst.sort();
    lst.unique();
    return lst;
}




std::ostream& operator<<(std::ostream &_stream, ListIndexQuadruples  &_tl) {
    int iCount = 0;
    vector<IndexQuadruple>::iterator it;
    for(it=_tl.begin(); it!=_tl.end(); it++){
	_stream << iCount << ":\t" << (*it).first << "<->"  << (*it).second << "\tdist:" << (*it).distance <<  "\t\tdelay:" << (*it).delay << endl;
	// 22.10.03 [jmb] Counted incremented after output to synapse
	// index
	iCount++;
    }
    return _stream;
};



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
