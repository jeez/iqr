/****************************************************************************
 ** $Filename: systemFileWriter.hpp
 ** $Author: Mark Blanchard
 **
 ** $CreateDate: Mon Sep 17 17:20:40 2001
 ** $Date: 2003/04/13 12:32:44 $
 **
 *****************************************************************************/

#ifndef CLSFESERIALIZER_HPP
#define CLSFESERIALIZER_HPP

#include <string>
#include <list>

#include "parameterList.hpp"

#include <QtXml>

class ClsFESystem;
class ClsFEGroup;
class ClsFEProcess;
class ClsFEConnection;

namespace iqrcommon {
    class ClsInfoDiagramIcon;
    class ClsInfoDiagramLine;
    class ClsItem;
}

using namespace iqrcommon;
using namespace std;

class ClsKey {
public:

    ClsKey( string _ID, string _origID, string _source, string _target, string _type) :
    ID(_ID), origID(_origID), source(_source), target(_target), type (_type) {};
    string ID;
    string origID;
    string source;
    string target;
    string type;


};


class ClsFESerializer {

public:

    ClsFESerializer();


    ~ClsFESerializer();

    bool SaveSystem(string _strFileName, bool bVerifySystem = false, bool bVerifyFile = false);
    string getSystemAsString( bool bVerifySystem = false);

    string getProcessesAsXML(list<string> lstIDs);
    string getGroupsAsXML(list<string> lstIDs);
    string getConnectionsAsXML(list<string> lstIDs);
    string getGroupsWidthConnectionsAsXML(list<string> lstIDGroups, list<string> lstIDConnections);

    int serializeToAER(string strExportFilename);

private:

    void CreateDOMTree(bool _bIncludeXMLDeclNode = true);
    int SerializeDOMTree(string &strSystemCont);



    bool IsDOMTreeValid();
    bool IsFileValid(string _strFileName);

    void writeExternalProcesses();

    QDomElement *addFESystem(QDomDocument *ddocRoot, ClsFESystem*);
    QDomElement *addFEProcess(QDomDocument *ddocRoot, ClsFEProcess*);
    QDomElement *addFEGroup(QDomDocument *ddocRoot, ClsFEGroup*);
    QDomElement *addFEConnection(QDomDocument *ddocRoot, ClsFEConnection*);

    QDomElement* addParameterizedNode(QDomDocument *ddocRoot,
                     string strNodeTag,
                     string strNodeType,
                     ParameterList parameterList);

    list<QDomElement*> getAERConnections(QDomDocument *ddocRoot);

    QDomDocument ddocSystem; //necessary for windows

    void addXRefHolders(QDomDocument *ddocRoot, QDomElement *delemParent, ClsItem* itemParent);


    static const bool INCLUDE_XML_DECL_NODE = true;
    static const bool EXCLUDE_XML_DECL_NODE = false;

    bool bXMLPlatformInitialized;

    list<string> lstExternalProcesses;


};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
