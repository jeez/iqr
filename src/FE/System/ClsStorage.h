#ifndef CLSSTORAGE_H
#define CLSSTORAGE_H

#include <queue>


using namespace std;


template <class TParent, class TChild> class ClsStorage {

public:
    ClsStorage(){};
    
    ~ClsStorage() {
//	cout << "ClsStorage::~ClsStorage()" << endl;
	while(queueChildren.size()){
//	    delete queueChildren.front();  /* will crash */
	    queueChildren.pop();
	}
//	delete tParent; /* will crash */
    };
    
    

    void addParent(TParent _tParent) {
	tParent = _tParent;
    }
    
    void addChild(TChild tChild) {
	queueChildren.push(tChild);
    }
    
    
    

    TParent getParent() {
	return tParent;
    };
    
    TChild getChild() {
	TChild tChild = queueChildren.front();
	queueChildren.pop();
	return tChild;
    };
    
    
private:

    TParent tParent;
    queue<TChild> queueChildren;
    
	
};


#endif

//// Local Variables: 
//// mode: c++
//// End: 
