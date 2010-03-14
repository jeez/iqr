#include <iostream>
#include <qfile.h>
#include <qmessagebox.h>

#include "ClsSystemDB.h"

using namespace std;

ClsSystemDB::ClsSystemDB( ) {
}

int ClsSystemDB::openFile( const QString fileName) {
    // read the XML file and create DOM tree
    QFile qfileSystem( fileName );
    if ( !qfileSystem.open( QIODevice::ReadOnly ) ) {
	QMessageBox::critical( 0,
			       QObject::tr( "Critical Error" ),
			       QObject::tr( "Cannot open file %1" ).arg( fileName ) );
	return -1;
    }
    if ( !domTree.setContent( &qfileSystem ) ) {
	QMessageBox::critical( 0,
			       QObject::tr( "Critical Error" ),
			       QObject::tr( "Parsing error for file %1" ).arg( fileName ) );
	qfileSystem.close();
	return -1;
    }
    qfileSystem.close();

    // get the header information from the DOM
    QDomElement root = domTree.documentElement();
    
    qDNodeListProcesses = domTree.elementsByTagName("Process");
    qDNodeListGroups = domTree.elementsByTagName("Group");
    qDNodeListConnections = domTree.elementsByTagName("Connection");
    qDNodeListNeurons = domTree.elementsByTagName("Neuron");
    qDNodeListSynapses = domTree.elementsByTagName("Synapse");
    return 0;
}

QDomNodeList ClsSystemDB::getDomNodeList(int iItemType){
    QDomNodeList qDomNodeList;
    if(iItemType == ITEM_PROCESS){
	qDomNodeList = qDNodeListProcesses;
    }
    else if(iItemType == ITEM_GROUP || iItemType == ITEM_NEURON){
	qDomNodeList = qDNodeListGroups;
    }
    else if(iItemType == ITEM_CONNECTION || iItemType == ITEM_SYNAPSE){
	qDomNodeList = qDNodeListConnections;
    }
    return qDomNodeList;
}

QStringList ClsSystemDB::getListItemNames(int iItemType){
    return getListItemByAttribute(iItemType, "name");
};


QStringList ClsSystemDB::getListItemIDs(int iItemType){
    return getListItemByAttribute(iItemType, "id");
};



QStringList ClsSystemDB::getListItemByAttribute(int iItemType, QString qstrAttribute){
    QStringList qstrLst; 
    QDomNodeList qDomNodeList = getDomNodeList(iItemType);
    for(unsigned int ii=0; ii<qDomNodeList.count(); ii++){
	if( qDomNodeList.item(ii).isElement()){
	    QDomAttr qDomAttr = qDomNodeList.item(ii).toElement().attributeNode(qstrAttribute);
//	    cout << qDomAttr.value() << endl;
	    qstrLst.append(qDomAttr.value());
	}
    }
    return qstrLst;
}



QDomNode ClsSystemDB::getItem(int iItemType, QString qstrIdentifierType, QString qstrIdentifier){
    QDomNode qDomNode;
    QDomNodeList qDomNodeList = getDomNodeList(iItemType);  
    for(unsigned int ii=0; ii<qDomNodeList.count(); ii++){
	qDomNode = qDomNodeList.item(ii);
	QDomNamedNodeMap qDomNamedNodeMap = qDomNode.attributes();
	if(!qstrIdentifier.compare(qDomNamedNodeMap.namedItem(qstrIdentifierType).nodeValue())){
	    return qDomNode;
	}
    }
    return qDomNode;
}


QStringList ClsSystemDB::getItemParameters(int iItemType, QString qstrIdentifierType, QString qstrIdentifier){
    QStringList qstrLst; 
    QDomNode qDomNode = getItem(iItemType, qstrIdentifierType, qstrIdentifier);
    QDomNamedNodeMap qDomNamedNodeMap = qDomNode.attributes();
    for(unsigned int i2=0; i2<qDomNamedNodeMap.count(); i2++){
//	cout<< qDomNamedNodeMap.item(i2).nodeName() << ": " << qDomNamedNodeMap.item(i2).nodeValue() << endl;
	qstrLst.append(	qDomNamedNodeMap.item(i2).nodeName());
    }
    return qstrLst;
};


QStringList ClsSystemDB::getSubItemParameters(int iItemType, QString qstrIdentifierType, QString qstrIdentifier){
    QStringList qstrLst; 
    QDomNode qDomNode = getItem(iItemType, qstrIdentifierType, qstrIdentifier);
    QDomNodeList qDomNodeListChildren = qDomNode.childNodes();
    for(unsigned int ii=0; ii<qDomNodeListChildren.count(); ii++){
//	cout << "qDomNodeListChildren: " << qDomNodeListChildren.item(ii).nodeName() << endl;
	QString qstrNodeNameChild = qDomNodeListChildren.item(ii).nodeName();
	if(!qstrNodeNameChild.compare("Neuron") || !qstrNodeNameChild.compare("Synapse")){
	    QDomNodeList qDomNodeListChildChildren = qDomNodeListChildren.item(ii).childNodes();
	    for(unsigned int i2=0; i2<qDomNodeListChildChildren.count(); i2++){
//		cout << qDomNodeListChildChildren.item(i2).nodeName() << endl;
		QString strNodeNameChildChild = qDomNodeListChildChildren.item(i2).nodeName();
		if(!strNodeNameChildChild.compare("Parameter")){
		    QDomAttr qDomAttr = qDomNodeListChildChildren.item(i2).toElement().attributeNode("name");
		    qstrLst.append(qDomAttr.value());
		}
	    }	    
	    continue;
	}
    }
    return qstrLst;
};
