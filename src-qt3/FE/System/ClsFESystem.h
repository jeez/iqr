#ifndef CLSFESYSTEM_H
#define CLSFESYSTEM_H

#include <string>
#include "item.hpp"
#include "ClsSysFileNode.h"

using namespace iqrcommon;
using namespace std;


class ClsFESystem : public ClsItem {

public:

    ClsFESystem();

    ClsFESystem(string _strSystemID, string _strSystemName);
    
    void setSystemID( string _strSystemID );
    string getSystemID( );
    
    
    void setNotes(string _str);
    string getNotes();
    
    string getSystemName();

    string getSystemHostName();
    int getSystemPort();

    int getCyclesPerSecond();
    bool getSyncPlots();


private:
    
    string strSystemID;

};


#endif




/// Local Variables: 
/// mode: c++
/// End: 
