#include <iostream>
#include <list>
#include <algorithm> //MARK
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

// WARNING: THE UNIQUE CODE DOES NOT WORK: UNIQUE ONLY WORKS ON SORTED LISTS!!!!!


class ClsRandom {

public:
    static list<int> getStrongRandom(int iMin, int iMax, unsigned int iCount, bool bUnique){
	list<int> lst;

	int iRange = iMax - iMin;
	int iLength = (int)(ceil((double)iRange / (double)255)); /* always >1 ?? */
	
	FILE *fp;
	
	if ((fp = fopen("/dev/urandom", "r")) == NULL) {
	    cerr << "Error! Could not open /dev/urandom for read" << endl;
	}
	
	unsigned char c1;
	while(lst.size()<iCount) {
	    c1 = fgetc(fp); // value range 0-255 for 1 character //
	    int iv = 0;
	    iv +=  c1;
#ifndef WIN32
	    int iVal = (lrint(double(iv * iRange) / double(255* iLength)));
#else
	    int iVal = (int)(double(iv * iRange) / double(255* iLength));
#endif
	    if(bUnique){
		list<int>::iterator it = find(lst.begin(), lst.end(), iVal);
		if(it==lst.end()){
		    lst.push_back(iVal);
		}
	    } else {
		lst.push_back(iVal);
	    }
	};
	fclose(fp);
	return lst;
    }

    static list<int> getWeakRandom(int iMin, int iMax, unsigned int iCount, bool bUnique){
	list<int> lst;
	double fRange = iMax - iMin;
	while(lst.size()<iCount) {
#ifndef WIN32
	    int iVal = 	lrint(((double)rand() / (double)RAND_MAX) * fRange + iMin);
#else
	    int iVal = 	(int)(((double)rand() / (double)RAND_MAX) * fRange + iMin);
#endif
	    if(bUnique){
		list<int>::iterator it = find(lst.begin(), lst.end(), iVal);
		if(it==lst.end()){
		    lst.push_back(iVal);
		}
	    } else {
		lst.push_back(iVal);
	    }
	};

//	cout << "lst.size(): " << lst.size() << endl;
	return lst;
    }
};






//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k"
//// End:





