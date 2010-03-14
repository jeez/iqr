#include <string>
#include <list>

#include <QPushButton>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include <ClsFESystemManager.h>
#include <ClsFEGroup.h>
#include <ClsFEConnection.h>
#include <neuron.hpp>
#include <ClsDragDropDeEncoder.h>
#include <iqrUtils.h>

#include "ClsQHarborImpl.h"
#include "ClsQHarborDialogImpl.h"
#include "ClsQHarborValueDialog.h"

//#define DEBUG_CLSQHARBORIMPL

#define COL_TYPE  0
#define COL_NAME  1
#define COL_ID    2
#define COL_CHILD 3
#define COL_PARAM_LABEL 4
#define COL_MIN   5
#define COL_MAX   6
#define COL_VALUE 7
#define COL_PARAM_NAME 8


ClsQHarborImpl::ClsQHarborImpl(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl):
    QDialog(parent) {

    setupUi(this); 
    setAcceptDrops(true);

    tableWidget->setColumnCount ( 8);


//--    tableWidget->setHorizontalHeaderItem( COL_TYPE,        new QTableWidgetItem("Type" ));
//--    tableWidget->setHorizontalHeaderItem( COL_NAME,        new QTableWidgetItem("Name" ));
//--    tableWidget->setHorizontalHeaderItem( COL_ID,          new QTableWidgetItem("ID" ));
//--    tableWidget->setHorizontalHeaderItem( COL_CHILD,       new QTableWidgetItem("Child" ));
//--    tableWidget->setHorizontalHeaderItem( COL_PARAM_LABEL, new QTableWidgetItem("Parameter" ));
//--    tableWidget->setHorizontalHeaderItem( COL_MIN,         new QTableWidgetItem("Min"));
//--    tableWidget->setHorizontalHeaderItem( COL_MAX,         new QTableWidgetItem("Max"));
//--    tableWidget->setHorizontalHeaderItem( COL_VALUE,       new QTableWidgetItem("Value"));

    addHeaderItem( COL_TYPE,        "Type",      false);
    addHeaderItem( COL_NAME,        "Name",      false);
    addHeaderItem( COL_ID,          "ID",        false);
    addHeaderItem( COL_CHILD,       "Child",     false);
    addHeaderItem( COL_PARAM_LABEL, "Parameter", true);
    addHeaderItem( COL_MIN,         "Min",       false);
    addHeaderItem( COL_MAX,         "Max",       false);
    addHeaderItem( COL_VALUE,       "Value",     true);

    tableWidget->hideColumn(COL_PARAM_NAME);
    tableWidget->setAcceptDrops(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    bSimulationRunning = false;
    clsItem = NULL;
    mapParams.clear();


    connect(pbDeleteRow, SIGNAL(clicked()), SLOT(slotDeleteRow()));
    connect(pbSaveConfig, SIGNAL(clicked()), SLOT(slotSaveConfig()));
    connect(pbLoadConfig, SIGNAL(clicked()), SLOT(slotLoadConfig()));

    connect(pbSaveParamSet, SIGNAL(clicked()), SLOT(slotSaveParamSet()));
    connect(pbLoadParamSet, SIGNAL(clicked()), SLOT(slotLoadParamSet()));

    connect(pbRefresh, SIGNAL(clicked()), SLOT(slotRefresh()));
    connect(tableWidget, SIGNAL(cellDoubleClicked(int, int)), SLOT(slotCellDoubleClicked(int, int)));
}



void ClsQHarborImpl::dragEnterEvent(QDragEnterEvent* event) {
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::dragEnterEvent(QDragEnterEvent* event)" << endl;
#endif

    if(event->mimeData()->hasFormat ("text/iqr-plot" )){
	event->acceptProposedAction();
    }

//--    if(event->mimeData()->hasText()){
//--	event->accept( );
//--    }

}

void ClsQHarborImpl::dropEvent(QDropEvent* event) {
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::dropEvent(QDropEvent* event)" << endl;
#endif
    mapParams.clear();
    QString qstrDropString;


    if(event->mimeData()->hasFormat ("text/iqr-plot" )){
	qstrDropString = event->mimeData()->data ("text/iqr-plot");
    } else {
	return;
    }

//    cout << "drop text: " << qstrDropString.toStdString() << endl;
    dropInfo di = ClsDragDropDeEncoder::decode(qstrDropString.toStdString());
    int iItemType = di.ItemType;
    string strItemID = di.ItemID;
//    cout << "iItemType: " << iItemType << endl;
//    cout << "strItemID: " << strItemID << endl;

    doDialog(iItemType, strItemID, -1);
};


void ClsQHarborImpl::doDialog(int iItemType, string strItemID, int iIndex){

#ifndef _WINDOWS // cannot get this to work under minGW at the moment...
    string strItemType;
    string strItemName;
    string strSubItemName;

    ParameterList paramLst;
    list<string> lstParams;
    if(iItemType == ClsFESystemManager::ITEM_GROUP){
	strItemType = "Group";
	clsItem = ClsFESystemManager::Instance()->getFEGroup( strItemID );
	if(clsItem!=NULL){
	    strItemName = dynamic_cast<ClsFEGroup*>(clsItem)->getGroupName();
	    ClsNeuron* clsNeuron = dynamic_cast<ClsFEGroup*>(clsItem)->getNeuron( );
	    if(clsNeuron!=NULL){
		strSubItemName = dynamic_cast<ClsFEGroup*>(clsItem)->getGroupNeuronLabel();
		paramLst = clsNeuron->getListParameters();
	    }
	}
    } else if (iItemType == ClsFESystemManager::ITEM_CONNECTION){
	strItemType = "Connection";
	clsItem = ClsFESystemManager::Instance()->getFEConnection( strItemID );
	if(clsItem!=NULL){
	    strItemName = dynamic_cast<ClsFEConnection*>(clsItem)->getConnectionName();
	    ClsSynapse* clsSynapse = dynamic_cast<ClsFEConnection*>(clsItem)->getSynapse( );
	    if(clsSynapse!=NULL){
		strSubItemName = dynamic_cast<ClsFEConnection*>(clsItem)->getConnectionSynapseLabel();
		paramLst = clsSynapse->getListParameters();
	    }
	}
    }
	    
    for(ParameterList::iterator it=paramLst.begin(); it!=paramLst.end(); it++){
	if(dynamic_cast<ClsDoubleParameter*>(*it)){
	    pair<string, string> pairTemp((*it)->getLabel(), (*it)->getName());
	    mapParams.insert(pairTemp);
	    lstParams.push_back((*it)->getLabel());
	}
    }
    if(lstParams.size()>0){

	if(iIndex<0){
	    tableWidget->insertRow ( 0 );
	    addItem(0, COL_TYPE, strItemType.c_str());
	    addItem(0, COL_NAME, strItemName.c_str());
	    addItem(0, COL_ID, strItemID.c_str());
	    addItem(0, COL_CHILD, strSubItemName.c_str());
	    addItem(0, COL_PARAM_LABEL, "");
	    addItem(0, COL_MIN, "");
	    addItem(0, COL_MAX, "");
	    addItem(0, COL_VALUE, "");
	    addItem(0, COL_PARAM_NAME, "");
	} 
	
	ClsQHarborDialogImpl* clsQHarborDialogImpl = new ClsQHarborDialogImpl(strItemType, strItemID, 
									      strItemName, strSubItemName, 
									      lstParams, iIndex, this);
	connect(clsQHarborDialogImpl, SIGNAL(sigDoItem(string,int)), SLOT(slotDoItem(string,int)));
	connect(clsQHarborDialogImpl, SIGNAL(sigDoItemCanceled(int)), SLOT(slotDoItemCanceled(int)));
	clsQHarborDialogImpl->show();
    }
#endif
}

void ClsQHarborImpl::doValueDialog(int iItemType, string strItemID, int iIndex, string strParamName, string strParamLabel, double fMin, double fMax, double fValue){

    string strItemType;
    string strItemName;

    ClsQHarborValueDialog* clsQHarborValueDialog = new ClsQHarborValueDialog( this, 
									      iItemType, strItemID, 
									      iIndex,
									      strParamName, strParamLabel, 
									      fMin, fMax, fValue);
	


    connect( clsQHarborValueDialog, SIGNAL( sigChangeValue(int, string, int, string, double) ), this, SLOT( slotChangeValue(int, string, int, string, double) ) );
    
    
    
    clsQHarborValueDialog->show();

}

void ClsQHarborImpl::slotDoItem(string strParamLabel, int iIndex){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotDoItem(string)" << endl;
#endif
    if(iIndex<0){
	iIndex = 0; /* must be the first row, since -1 means new row was added */
    }
    if(mapParams.find(strParamLabel)!=mapParams.end()){
	string strParamName = mapParams.find(strParamLabel)->second;
//--	cout << "strParamName: " << strParamName << endl;

	changeItem(iIndex, COL_PARAM_LABEL, strParamLabel.c_str());
	changeItem(iIndex, COL_PARAM_NAME, strParamName.c_str());

	
	if(clsItem!=NULL){
	    fillMinMaxValueField(clsItem, strParamName, iIndex);
	}


	for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	    tableWidget->resizeColumnToContents(ii);
	}

    }
    clsItem=NULL;
}


void ClsQHarborImpl::slotChangeValue(int iItemType,  string strItemID,  int iIndex, string strParamName,  double fValue){
//    cout << "ClsQHarborImpl::slotChangeValue()" << endl;

#ifndef _WINDOWS // cannot get this to work under minGW at the moment...
    QString qstr;
    qstr.setNum(fValue);
    string strParamValue = qstr.toStdString();


    if(iItemType == ClsFESystemManager::ITEM_GROUP){
	ClsFEGroup* clsFEGroup = ClsFESystemManager::Instance()->getFEGroup( strItemID );
	if(clsFEGroup!=NULL){
	    clsFEGroup->setNeuronParameter(strParamName, strParamValue);
	} else {
	    cerr << "ClsQHarborImpl::slotChangeValue: group not found" << endl;
	}
    } else if (iItemType == ClsFESystemManager::ITEM_CONNECTION){
 	ClsFEConnection* clsFEConnection = ClsFESystemManager::Instance()->getFEConnection( strItemID );
 	if(clsFEConnection!=NULL){
	    clsFEConnection->setSynapseParameter(strParamName, strParamValue);
 	} else {
	    cerr << "ClsQHarborImpl::slotChangeValue: connection not found" << endl;
	}
    }

    changeItem(iIndex, COL_VALUE, strParamValue.c_str());
#endif

}


void ClsQHarborImpl::fillMinMaxValueField(ClsItem* _clsItem, string strParamName, int iRow){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::fillMinMaxValueField(ClsItem* _clsItem, string strParamName, int iRow)" << endl;
    cout << "strParamName: " << strParamName << endl;
#endif
    ClsItem* clsSubItem = NULL;
    if(dynamic_cast<ClsFEGroup*>(_clsItem)){
	clsSubItem = dynamic_cast<ClsFEGroup*>(_clsItem)->getNeuron( );
    } 
    else if(dynamic_cast<ClsFEConnection*>(_clsItem)){
	clsSubItem = dynamic_cast<ClsFEConnection*>(_clsItem)->getSynapse( );
    } 

    if(clsSubItem!=NULL){
#ifndef _WINDOWS // cannot get this to work under minGW at the moment...
	ClsParameter* clsParam = clsSubItem->getParameter(strParamName);
	if(clsParam!=NULL){
	    changeItem(iRow, COL_MIN, QString::number(dynamic_cast<ClsDoubleParameter*>(clsParam)->getMinimum()).toStdString());
	    changeItem(iRow, COL_MAX, QString::number(dynamic_cast<ClsDoubleParameter*>(clsParam)->getMaximum()).toStdString());
	    changeItem(iRow, COL_VALUE, clsParam->getValueAsString().c_str());
	}
#endif
    }
}


void ClsQHarborImpl::slotDoItemCanceled(int iIndex){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotDoItemCanceled(int iIndex)" << endl;
#endif
/* FIX */
//    cout << "iIndex: " << iIndex << endl;
    if(iIndex<0){
	iIndex = 0; /* must be the first row, since -1 means new row was added */
//--	qtableEntries->removeRow(iIndex);
	tableWidget->removeRow(iIndex);
	clsItem = NULL;
	mapParams.clear();
    }
}


list<ClsParamTrade> ClsQHarborImpl::getParamTrades(){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::getParamTrades()" << endl;
#endif
    list<ClsParamTrade> lstParamTrades;

    for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	if(getItemValue(ii, COL_PARAM_NAME).length()>0){ 
	    ClsParamTrade paramTrade;
	    string strType = getItemValue(ii, COL_TYPE);                          paramTrade.Type = strType;
	    string strName = getItemValue(ii, COL_NAME);                          paramTrade.Name = strName ;
	    string strID  = getItemValue(ii, COL_ID);                             paramTrade.ID  = strID  ;
	    string strChild = getItemValue(ii, COL_CHILD);                        paramTrade.Child = strChild;
	    string strParam = getItemValue(ii, COL_PARAM_NAME);                   paramTrade.Param = strParam;
	    double fMin   = iqrUtils::string2double(getItemValue(ii, COL_MIN));   paramTrade.Min = fMin ;
	    double fMax   = iqrUtils::string2double(getItemValue(ii, COL_MAX));   paramTrade.Max = fMax ;
	    double fValue = iqrUtils::string2double(getItemValue(ii, COL_VALUE)); paramTrade.Value = fValue;
	    lstParamTrades.push_back(paramTrade);
	}
    }
    
//    cout << "ClsQHarborImpl::lstParamTrades.size(): " << lstParamTrades.size() << endl;
    return lstParamTrades;
};



void ClsQHarborImpl::slotDeleteRow(){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotDeleteRow" << endl;
#endif

    while(!tableWidget->selectedItems().isEmpty()){
	QList<QTableWidgetItem *> lst = tableWidget->selectedItems ();
	QTableWidgetItem* item = lst.front();
	lst.pop_front ();
	tableWidget->removeRow(item->row());
    }

    for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	tableWidget->resizeColumnToContents(ii);
    }



};

void ClsQHarborImpl::slotSaveConfig(){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotSaveConfig()" << endl;
#endif

    QString qstrFileName = "";
    bool bValidFile = false;
    while(!bValidFile){
	qstrFileName = QFileDialog::getSaveFileName(
	    this /*parent*/,
	    "Save File"
	    "Choose a filename to save under", /*name*/
	    "" /*dirName*/, 
	    "Harbor Config (*.hconf)" /*filter*/);

	QFileInfo qFileInfo(qstrFileName );
	QString qstrPath = qFileInfo.dirPath(true);
	qFileInfo.setFile (qstrPath);

	if ( !qFileInfo.isWritable() ) {
	    int iReturn = QMessageBox::critical( this, "iqr",
						 "No permission to write in this directory\n",
						 "Retry", "Cancel", 0, 0, 1 );
	    if(iReturn == 1){
		return;
	    }
	} else {
	    bValidFile = true;
	}
    }
    
    if(qstrFileName.length()<=0){
	return;
    }

    if ( QFile::exists(qstrFileName) ) {
/*      warning on overwrite */
	int iReturn = QMessageBox::warning( this, "iqr",
					    "A file with the seleteced name exists\n",
					    "Overwrite", "Cancel", 0, 0, 1 );
	if(iReturn == 1){
	    return;
	}
    }
    
    QDomDocument doc( "HarborConfig" );
    QDomElement root = doc.createElement( "HarborConfig" );
    doc.appendChild( root );
    
    for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	QDomElement paramTrade = doc.createElement( "ParamTrade" );
	paramTrade.setAttribute ( "itemType", getItemValue(ii, COL_TYPE).c_str());
	paramTrade.setAttribute ( "itemID", getItemValue(ii, COL_ID).c_str());
	paramTrade.setAttribute ( "itemChild", getItemValue(ii, COL_CHILD).c_str());
	paramTrade.setAttribute ( "ParamLabel", getItemValue(ii, COL_PARAM_LABEL).c_str());
	paramTrade.setAttribute ( "ParamName", getItemValue(ii, COL_PARAM_NAME).c_str());
	root.appendChild( paramTrade );
    }	    




    QString xml = doc.toString();
//    cout << xml << endl;


    QFile file(qstrFileName);
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
	stream << doc.toString() << "\n";
        file.close();
    }
    

    
};


void ClsQHarborImpl::slotLoadConfig(){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotLoadConfig()" << endl;
#endif

    QString filename = QFileDialog::getOpenFileName(
	this,
	"Open File",
	"",
	"Harbor Config (*.hconf)");
    loadConfig(filename.toStdString());
};

void ClsQHarborImpl::loadConfig(string strConfigName){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::loadConfig(string strConfigName)" << endl;
#endif

#ifndef _WINDOWS // cannot get this to work under minGW at the moment...

    if(strConfigName.length()>0){
	QFile qfile( strConfigName.c_str() );
	if ( !qfile.open( QIODevice::ReadOnly ) ) {
	    QMessageBox::critical( 0, tr( "Critical Error" ),
				   tr( "Cannot open file %1" ).arg( strConfigName.c_str() ) );
	    return;
	}
	
	QDomDocument domTree;
	if ( !domTree.setContent( &qfile ) ) {
	    QMessageBox::critical( 0, tr( "Critical Error" ),
				   tr( "Parsing error for file %1" ).arg( strConfigName.c_str() ) );
	    qfile.close();
	    return;
	}
	qfile.close();

	// get the header information from the DOM
	QDomElement root = domTree.documentElement();
	QDomNode node;
	node = root.firstChild(); /* ParamTrade */
	
	while ( !node.isNull() ) {
	    string strItemType = node.toElement().attribute("itemType").toStdString();
	    string strItemID = node.toElement().attribute("itemID").toStdString();
	    string strSubItemName = node.toElement().attribute("itemChild").toStdString();
	    string strParamName = node.toElement().attribute("ParamName").toStdString();
	    string strParamLabel = node.toElement().attribute("ParamLabel").toStdString();
	    string strItemName = "";
	    
	    ClsItem* clsItemTemp = NULL;
	    if(!strItemType.compare("Group")){
		clsItemTemp = ClsFESystemManager::Instance()->getFEGroup( strItemID );
		if(clsItemTemp!=NULL){
		    strItemName = dynamic_cast<ClsFEGroup*>(clsItemTemp)->getGroupName();
		}
	    } 
	    else if(!strItemType.compare("Connection")){
		clsItemTemp = ClsFESystemManager::Instance()->getFEConnection( strItemID );
		if(clsItemTemp!=NULL){
		    strItemName = dynamic_cast<ClsFEConnection*>(clsItemTemp)->getConnectionName();
		}
	    }
	    
	    tableWidget->insertRow ( 0 );
	    addItem(0, COL_ID, strItemID.c_str());
	    addItem(0, COL_TYPE, strItemType.c_str());
	    addItem(0, COL_CHILD, strSubItemName.c_str());


	    if(clsItemTemp!=NULL){
		addItem(0, COL_NAME, strItemName.c_str());
		addItem(0, COL_PARAM_LABEL, strParamLabel.c_str());
		addItem(0, COL_PARAM_NAME, strParamName.c_str());
		addItem(0, COL_MIN, "");
		addItem(0, COL_MAX, "");
		addItem(0, COL_VALUE, "");
		fillMinMaxValueField(clsItemTemp, strParamName, 0);	
	    }
	    node = node.nextSibling();
	}
	for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	    tableWidget->resizeColumnToContents(ii);
	}
    }
#endif
}


void ClsQHarborImpl::clearTable(){

    tableWidget->clear();
    tableWidget->setRowCount(0);


}

void ClsQHarborImpl::slotItemDeleted(int /*_iType*/, string _strID ){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotItemDeleted(int iType, string strID )" << endl;
#endif
    /* remove item from the list */
    for(int ii = tableWidget->rowCount()-1; ii>=0; ii--){ 
	string strID = getItemValue(ii, COL_ID);
	if(!strID.compare(_strID)){
	    tableWidget->removeRow(ii);
	}
    }
    for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	tableWidget->resizeColumnToContents(ii);
    }

};

void ClsQHarborImpl::slotItemChanged(int iItemType, string _strID ){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotItemChanged(int iType, string strID )" << endl;
#endif
#ifndef _WINDOWS // cannot get this to work under minGW at the moment...

    if(!bSimulationRunning){
	ClsItem* clsItemTemp = NULL;
	ParameterList paramLst;
	string _strSubItemName;
	string _strItemName;
	if(iItemType == ClsFESystemManager::ITEM_GROUP){
	    clsItemTemp = ClsFESystemManager::Instance()->getFEGroup( _strID );
	    if(clsItemTemp!=NULL){
		_strItemName = dynamic_cast<ClsFEGroup*>(clsItemTemp)->getGroupName();
		ClsNeuron* clsNeuron = dynamic_cast<ClsFEGroup*>(clsItemTemp)->getNeuron( );
		if(clsNeuron!=NULL){
		    _strSubItemName = dynamic_cast<ClsFEGroup*>(clsItemTemp)->getGroupNeuronLabel();
		    paramLst = clsNeuron->getListParameters();
		}
	    }
	} else if (iItemType == ClsFESystemManager::ITEM_CONNECTION){
	    clsItemTemp = ClsFESystemManager::Instance()->getFEConnection( _strID );
	    if(clsItemTemp!=NULL){
		_strItemName = dynamic_cast<ClsFEConnection*>(clsItemTemp)->getConnectionName();
		ClsSynapse* clsSynapse = dynamic_cast<ClsFEConnection*>(clsItemTemp)->getSynapse( );
		if(clsSynapse!=NULL){
		    _strSubItemName = dynamic_cast<ClsFEConnection*>(clsItemTemp)->getConnectionSynapseLabel();
		    paramLst = clsSynapse->getListParameters();
		}
	    }
	}

	list<string> lstParams;
	for(ParameterList::iterator it=paramLst.begin(); it!=paramLst.end(); it++){
	    if(dynamic_cast<ClsDoubleParameter*>(*it)){
		lstParams.push_back((*it)->getName());
	    }
	}

	
/* checks:
   - item name
   - item subtype
   - subtype parameter
   - subtype min, max, value
*/

	if(clsItemTemp!=NULL){
//	cout << "/* check the min, max, value settings */" << endl;
	    for(int ii = 0; ii<tableWidget->rowCount(); ii++){
		string strID = getItemValue(ii, COL_ID);
		if(!strID.compare(_strID)){
		    bool bSubItemTypeChanged = false;
		    /* check (omitted) + change name */
		    changeItem(ii, COL_NAME, _strItemName.c_str());
		
		    string str = getItemValue(ii, COL_CHILD);
//		cout << "str: " << str << endl;
//		cout << "_strSubItemName: " << _strSubItemName << endl;
		    if(_strSubItemName.compare(str)){
//		    cout << "subitem type changed" << endl;
			bSubItemTypeChanged = true;
			changeItem(ii, COL_CHILD, _strSubItemName.c_str());
		    } else {
//		    cout << "subitem type NOT changed" << endl;
			bSubItemTypeChanged = false;
		    }

		    bool bParameterSupported = true;
		    if(bSubItemTypeChanged){
			/* check if subitem supports parameter */
			string str = getItemValue(ii, COL_PARAM_NAME);
			if(std::find(lstParams.begin(), lstParams.end(), str)!=lstParams.end()){
//			cout << "PARAMETER SUPPORTED" << endl;
			} else {
//			cout << "PARAMETER _NOT_ SUPPORTED" << endl;
			    bParameterSupported = false;
			}
		    }
		
		    if(bParameterSupported){
			string strParamName = getItemValue(ii, COL_PARAM_NAME);
			fillMinMaxValueField(clsItemTemp, strParamName, ii);
		    
		    }
		    else {
			changeItem(ii, COL_PARAM_LABEL, "");
			changeItem(ii, COL_PARAM_NAME, "");
			changeItem(ii, COL_MIN, "");
			changeItem(ii, COL_MAX, "");
			changeItem(ii, COL_VALUE, "");

		    }
		}
	    }
	}
	for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	    tableWidget->resizeColumnToContents(ii);
	}
    }
#endif
};





void ClsQHarborImpl::slotCellDoubleClicked(int row, int col){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotCellDoubleClicked(int row, int col)" << endl;
#endif
    string _strID = getItemValue(row, COL_ID);
//    cout << _strID << endl;
    int iItemType = -1;
    string strItemType = getItemValue(row, COL_TYPE);
    if(!strItemType.compare("Group")){
	iItemType = ClsFESystemManager::ITEM_GROUP;
    } 
    else if(!strItemType.compare("Connection")){
	iItemType = ClsFESystemManager::ITEM_CONNECTION;
    }
    
    if(col == COL_PARAM_LABEL){
//	cout << "col == COL_PARAM_LABEL" << endl;
	doDialog(iItemType, _strID, row);
    } else if(col == COL_VALUE){
//	cout << "col == COL_VALUE" << endl;
	string strParamLabel = getItemValue(row, COL_PARAM_LABEL);
	string strParamName = getItemValue(row, COL_PARAM_NAME);
	double fMin = QString(getItemValue(row, COL_MIN).c_str()).toDouble();
	double fMax = QString(getItemValue(row, COL_MAX).c_str()).toDouble();
	double fValue = QString(getItemValue(row, COL_VALUE).c_str()).toDouble();

	doValueDialog(iItemType, _strID, row, strParamName, strParamLabel, fMin, fMax, fValue);
    } else {
//	cout << "other col" << endl;
    }
}


void ClsQHarborImpl::setSimulationRunning(bool b){
    bSimulationRunning = b;
}


void ClsQHarborImpl::slotRefresh(){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotRefresh" << endl;
#endif
    if(bSimulationRunning){
	for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	    if(getItemValue(ii, COL_PARAM_NAME).length()>0){ /* cos we deleted this entry for
								       invalid param names */
		string strItemType = getItemValue(ii, COL_TYPE);
		string strID  = getItemValue(ii, COL_ID);
		string strParamName = getItemValue(ii, COL_PARAM_NAME); 
		
		ClsItem* clsItemTemp = NULL;
		if(!strItemType.compare("Group")){
#ifndef _WINDOWS // cannot get this to work under minGW at the moment...
		    clsItemTemp = ClsFESystemManager::Instance()->getFEGroup( strID );
#endif
		} else if (!strItemType.compare("Connection")){
#ifndef _WINDOWS // cannot get this to work under minGW at the moment...
		    clsItemTemp = ClsFESystemManager::Instance()->getFEConnection( strID );
#endif		    
		}
		
		if(clsItemTemp!=NULL){
		    fillMinMaxValueField(clsItemTemp, strParamName, ii);
		} 
	    }
	}
	tableWidget->resizeColumnToContents(COL_VALUE);    
    }
}



void ClsQHarborImpl::slotSaveParamSet(){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotSaveParamSet()" << endl;
#endif


    QString qstrFileName = "";
    bool bValidFile = false;
    while(!bValidFile){
	qstrFileName = QFileDialog::getSaveFileName(
	    this, 
	    "Save File"
	    "Choose a filename to save under",
	    "",
 	    "Harbor ParamSet (*.pconf)" );

	QFileInfo qFileInfo(qstrFileName );
	QString qstrPath = qFileInfo.dirPath(true);
	qFileInfo.setFile (qstrPath);
	if ( !qFileInfo.isWritable() ) {
	    int iReturn = QMessageBox::critical( this, "iqr",
						 "No permission to write in this directory\n",
						 "Retry", "Cancel", 0, 0, 1 );
	    if(iReturn == 1){
		return;
	    }
	} else {
	    bValidFile = true;
	}
    }
    
    if(qstrFileName.length()<=0){
	return;
    }


    if ( QFile::exists(qstrFileName) ) {
/*      warning on overwrite */
	int iReturn = QMessageBox::warning( this, "iqr",
					    "A file with the seleteced name exists\n",
					    "Overwrite", "Cancel", 0, 0, 1 );
	if(iReturn == 1){
	    return;
	}
    }


    QDomDocument doc( "ParamSet" );
    QDomElement root = doc.createElement( "ParamSet" );
    doc.appendChild( root );

    
    for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	QDomElement parameter = doc.createElement( "Parameter" );
	parameter.setAttribute ( "itemType", getItemValue(ii, COL_TYPE).c_str() );
	parameter.setAttribute ( "itemID", getItemValue(ii, COL_ID).c_str() );
	parameter.setAttribute ( "name", getItemValue(ii, COL_PARAM_NAME).c_str() );
	parameter.setAttribute ( "value", getItemValue(ii, COL_VALUE).c_str() );
	
	root.appendChild( parameter );

    }	    
    QString xml = doc.toString();
//    cout << xml << endl;


    QFile file(qstrFileName);
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
	stream << doc.toString() << "\n";
        file.close();
    }


}

void ClsQHarborImpl::slotLoadParamSet(){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotLoadParamSet()" << endl;
#endif

    QString filename = QFileDialog::getOpenFileName(
	this,
	"Open File"
	"Choose a file",
	"",
	"Harbor ParamSet (*.pconf)");
    
    loadParamSet(filename.toStdString());
};

void ClsQHarborImpl::loadParamSet(string strParamSetName){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::loadParamSet(string strParamSetName)" << endl;
#endif

#ifndef _WINDOWS // cannot get this to work under minGW at the moment...
    
    if(strParamSetName.length()>0){
	QFile qfile( strParamSetName.c_str() );
	if ( !qfile.open( QIODevice::ReadOnly ) ) {
	    QMessageBox::critical( 0, tr( "Critical Error" ),
				   tr( "Cannot open file %1" ).arg( strParamSetName.c_str() ) );
	    return;
	}
	
	QDomDocument domTree;
	if ( !domTree.setContent( &qfile ) ) {
	    QMessageBox::critical( 0, tr( "Critical Error" ),
				   tr( "Parsing error for file %1" ).arg( strParamSetName.c_str() ) );
	    qfile.close();
	    return;
	}
	qfile.close();
	
	
	// get the header information from the DOM
	QDomElement root = domTree.documentElement();
	QDomNode node;
	node = root.firstChild(); /* Parameter */
	
	while ( !node.isNull() ) {
	    string strItemType = node.toElement().attribute("itemType").toStdString();
	    string strItemID = node.toElement().attribute("itemID").toStdString();
	    string strParamName = node.toElement().attribute("name").toStdString();
	    string strParamValue = node.toElement().attribute("value").toStdString();
	    string strItemName = "";
	    
	    if(!setTableItemValue(strItemID, strParamName,strParamValue)){
		cerr << "parameter not found" << endl;
		string strError = string("Cannot find parameter \"") + strParamName + string("\"\nfor item \"") + strItemID + string("\"\n");
		QMessageBox::warning( this, "iqr Harbor",
				      strError.c_str(),
				      "Ok");
	    } else {
		
		if(!strItemType.compare("Group")){
		    ClsFEGroup* clsFEGroup = ClsFESystemManager::Instance()->getFEGroup( strItemID );
		    if(clsFEGroup!=NULL){
			clsFEGroup->setNeuronParameter(strParamName, strParamValue);
		    } else {
			cerr << "ClsQHarborImpl::slotChangeValue: group not found" << endl;
		    }
		} 
		else if(!strItemType.compare("Connection")){
		    ClsFEConnection* clsFEConnection = ClsFESystemManager::Instance()->getFEConnection( strItemID );
		    if(clsFEConnection!=NULL){
			clsFEConnection->setSynapseParameter(strParamName, strParamValue);
		    } else {
			cerr << "ClsQHarborImpl::slotChangeValue: connection not found" << endl;
		    }
		}
	    }
	    node = node.nextSibling();
 	}
	for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	    tableWidget->resizeColumnToContents(ii);
	}
    }
#endif    
}









bool ClsQHarborImpl::checkForItem(string _strItemID, string _strParamName){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "checkForItem(string strItemID, string strParamName)" << endl;
#endif
/*FIX*/
    for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	string strItemID = getItemValue(ii, COL_ID);
	string strParamName = getItemValue(ii, COL_PARAM_NAME);
	if(!strItemID.compare(_strItemID) && !strParamName.compare(_strParamName)){
	    return true;
	}
    }
    return false;
}



bool ClsQHarborImpl::setTableItemValue(string _strItemID, string _strParamName, string strValue){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "setTableItemValue(string _strItemID, string _strParamName, string strValue)" << endl;
#endif
/*FIX*/
    for(int ii = 0; ii<tableWidget->rowCount(); ii++){
	string strItemID = getItemValue(ii, COL_ID);
	string strParamName = getItemValue(ii, COL_PARAM_NAME);
	if(!strItemID.compare(_strItemID) && !strParamName.compare(_strParamName)){
	    changeItem(ii, COL_VALUE, strValue.c_str());
	    return true;
	}
    }
    return false;


}
