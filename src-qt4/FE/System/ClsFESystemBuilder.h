/****************************************************************************
 ** $Filename: ClsFESystemBuilder.h
 **
 ** $Author: Ulysses Bernardet
 **
 *****************************************************************************/


#ifndef CLSPRCSYSTEMBUILDER_H
#define CLSPRCSYSTEMBUILDER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <qmessagebox.h> 

//#include "ClsPrcMessageIn.h"
//#include "ClsPrcMessageOut.h"
#include "ClsSysFileParser.h"
//#include <ClsMessageParser.h>
#include "ClsInfoSystem.h"
#include "ClsInfoPrc.h"
#include "ClsInfoDiagramIcon.h"
#include "ClsInfoDiagramLine.h"
#include "ClsParameter.h"

#include "ClsFESystemBuilderException.h"

#define COUT cout << "[01;34m"
#define ENDL "[00m" << endl


class ClsFESystemManager;

using namespace std;
using namespace iqrcommon;

class ClsFESystemBuilder {
    
public:
    ClsFESystemBuilder();

    ~ClsFESystemBuilder (){
    } ;

    void readSystemFileFromDisk(string _strSystemFileName);
    string readFileFromDisk(string _strFileName);

    void importProcess(string, bool bEmitSignals, bool bLink);
    
    void parseSystemFile( );
    void parseFragment(string strCont, bool bValidate, int iActiveItemType, string strActiveItemID);


    void importFragment(string strCont, bool bValidate, bool bEmitSignals, string _strFileName);

    void buildSystem();

    ClsInfoDiagramLine getDiagramLine(string _strID);
    ClsInfoDiagramIcon getDiagramIcon(string _strID);
    
	    
private:

    string strSystemFileCont;
    ClsSysFileParser clsSysFileParser;
    friend class ClsFESystemManager;

    map<string, ClsInfoDiagramLine> mapDiagramLineInternal;
    map<string,ClsInfoDiagramIcon> mapDiagramIconInternal;
    map<string, ClsInfoDiagramLine> mapDiagramLineExternal;
    map<string,ClsInfoDiagramIcon> mapDiagramIconExternal;
    list<string> lstExternalPathsProcesses;

};

#endif

/// Local Variables: 
/// mode: c++
/// compile-command: "cd ../../ && make -k "
/// End: 
