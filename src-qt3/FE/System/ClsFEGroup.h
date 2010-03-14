#ifndef CLSFEGROUP_H
#define CLSFEGROUP_H

#include <string>
#include <list>

#include <qmutex.h>

#include "ClsBaseGroup.h"
//--#include "ClsCompGroup.h" //@@@@

#include <stateArray.hpp>
#include "ClsParameter.h"

using namespace iqrcommon;
using namespace std;

// namespace iqrcommon {
//     class ClsNeuron;
// //    class NeuronManager;
// }

class ClsGroupManipPattern;

class ClsFEGroup : public ClsBaseGroup {
//---class ClsFEGroup : public ClsCompGroup {

public:
    ClsFEGroup ( );
    ClsFEGroup(string _strGroupID, string _strGroupName, string _strPrcID);

/*
  ClsFEGroup (string _strGroupID, string _strGroupName, string _strPrcID,
  ClsSysFileNode clsSysFileNodeGroupTopologyParameter,
  ClsSysFileNode clsSysFileNodeGroupNeuronParameter);
*/

    ClsFEGroup *getFEGroup() { return this; }

    string getNotes(); // const;
    void setNotes(string _strNotes);
    string validate();
    void update();

    void setMutex(QMutex* _qmutexThread);
    ClsGroupManipPattern* getGroupManipPattern();
    void removeGroupManipPattern();

private:
    ClsGroupManipPattern* clsGroupManipPattern;
    QMutex *qmutexThread;

    string strNotes;
//    ClsStringParameter  *pclsNotes;

};


#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
