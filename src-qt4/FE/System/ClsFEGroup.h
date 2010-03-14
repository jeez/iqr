#ifndef CLSFEGROUP_H
#define CLSFEGROUP_H

#include <string>
#include <list>

#include <QtCore/qmutex.h>

#include "ClsBaseGroup.h"

#include <stateArray.hpp>
#include "ClsParameter.h"

using namespace iqrcommon;
using namespace std;


class ClsGroupManipPattern;

class ClsFEGroup : public ClsBaseGroup {

public:
    ClsFEGroup ( );
    ClsFEGroup(string _strGroupID, string _strGroupName, string _strPrcID);

    ClsFEGroup *getFEGroup() { return this; }

    string getNotes(); // const;
    void setNotes(string _strNotes);

    void setColor( string _strColor);
    string getColor( );

    string validate();
    void update();

    void setMutex(QMutex* _qmutexThread);
    ClsGroupManipPattern* getGroupManipPattern();
    void removeGroupManipPattern();

private:
    ClsGroupManipPattern* clsGroupManipPattern;
    QMutex *qmutexThread;

    string strNotes;
};


#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:


