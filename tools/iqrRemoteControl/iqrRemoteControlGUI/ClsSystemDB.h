#ifndef CLSSYSTEMDB_H
#define CLSSYSTEMDB_H

#include <qdom.h>
#include <qstringlist.h>

class ClsSystemDB {
public:
    ClsSystemDB( );
    int openFile(const QString fileName);

    QStringList getListItemIDs(int iItemType);
    QStringList getListItemNames(int iItemType);

    QStringList getItemParameters(int iItemType, QString qstrIdentifierType, QString qstrIdentifier);
    QStringList getSubItemParameters(int iItemType, QString qstrIdentifierType, QString qstrIdentifier);

    enum ITEM_TYPES{
	ITEM_GROUP,
	ITEM_PROCESS,
	ITEM_CONNECTION,
	ITEM_NEURON,
	ITEM_SYNAPSE
    };


private:
    QStringList getListItemByAttribute(int iItemType, QString qstrAttribute);

    QDomNodeList getDomNodeList(int iItemType);
    QDomNode getItem(int iItemType, QString qstrIdentifierType, QString qstrIdentifier);

    QDomDocument domTree;

    QDomNodeList qDNodeListProcesses;
    QDomNodeList qDNodeListGroups;
    QDomNodeList qDNodeListConnections;
    QDomNodeList qDNodeListNeurons;
    QDomNodeList qDNodeListSynapses;

};

#endif 
