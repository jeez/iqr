#include <string>
#include <list>

#include <qdragobject.h>
#include <qtable.h>
#include <qheader.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qdom.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include <ClsFESystemManager.h>
#include <ClsFEGroup.h>
#include <ClsFEConnection.h>
#include <neuron.hpp>
#include <ClsDragDropDeEncoder.h>
#include <iqrUtils.h>

#include "ClsQHarborImpl.h"
#include "ClsQHarborDialogImpl.h"
#include "ClsQHarborValueDialog.h"

//#include "moc_ClsQHarborImpl.cxx"

#define COL_TYPE  0
#define COL_NAME  1
#define COL_ID    2
#define COL_CHILD 3
#define COL_PARAM_LABEL 4
#define COL_MIN   5
#define COL_MAX   6
#define COL_VALUE 7
#define COL_PARAM_NAME 8


ClsQHarborImpl::ClsQHarborImpl(QWidget* parent, const char* name, bool modal, WFlags fl):
    ClsQHarbor(parent, name,modal, fl)  {
    setAcceptDrops(TRUE);
    QHeader *th = qtableEntries->horizontalHeader();
    th->setLabel( COL_TYPE, "Type" );
    th->setLabel( COL_NAME, "Name" );
    th->setLabel( COL_ID, "ID" );
    th->setLabel( COL_CHILD, "Child" );
    th->setLabel( COL_PARAM_LABEL, "Parameter" );
    th->setLabel( COL_MIN, "Min");
    th->setLabel( COL_MAX, "Max");
    th->setLabel( COL_VALUE, "Value");
    qtableEntries->hideColumn(COL_PARAM_NAME);

    bSimulationRunning = false;
    clsItem = NULL;
    mapParams.clear();

    void ();
    void ();
    void ();

    connect(pbDeleteRow, SIGNAL(clicked()), SLOT(slotDeleteRow()));
    connect(pbSaveConfig, SIGNAL(clicked()), SLOT(slotSaveConfig()));
    connect(pbLoadConfig, SIGNAL(clicked()), SLOT(slotLoadConfig()));

    connect(pbSaveParamSet, SIGNAL(clicked()), SLOT(slotSaveParamSet()));
    connect(pbLoadParamSet, SIGNAL(clicked()), SLOT(slotLoadParamSet()));


    connect(pbRefresh, SIGNAL(clicked()), SLOT(slotRefresh()));

    connect(qtableEntries, SIGNAL(doubleClicked(int, int, int, const QPoint &)), SLOT(slotCellDoubleClicked(int, int, int, const QPoint &)));



}



void ClsQHarborImpl::dragEnterEvent(QDragEnterEvent* event) {
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::dragEnterEvent(QDragEnterEvent* event)" << endl;
#endif
    event->accept( QTextDrag::canDecode(event) );
}

void ClsQHarborImpl::dropEvent(QDropEvent* event) {
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::dropEvent(QDropEvent* event)" << endl;
#endif
    mapParams.clear();
    QString text;
    if ( QTextDrag::decode(event, text) ) {
	if(event->provides("text/iqr-plot")){
//	    cout << "drop text: " << text << endl;
	    dropInfo di = ClsDragDropDeEncoder::decode(text.latin1());
	    int iItemType = di.ItemType;
	    string strItemID = di.ItemID;
	    doDialog(iItemType, strItemID, -1);
	}
    }
};


void ClsQHarborImpl::doDialog(int iItemType, string strItemID, int iIndex){

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
	    qtableEntries->insertRows(0,1);
	    qtableEntries->setText(0, COL_TYPE, strItemType);
	    qtableEntries->setText(0, COL_NAME, strItemName);
	    qtableEntries->setText(0, COL_ID, strItemID);
	    qtableEntries->setText(0, COL_CHILD, strSubItemName);
	} 
	
	ClsQHarborDialogImpl* clsQHarborDialogImpl = new ClsQHarborDialogImpl(strItemType, strItemID, 
									      strItemName, strSubItemName, 
									      lstParams, iIndex, this);
	connect(clsQHarborDialogImpl, SIGNAL(sigDoItem(string,int)), SLOT(slotDoItem(string,int)));
	connect(clsQHarborDialogImpl, SIGNAL(sigDoItemCanceled(int)), SLOT(slotDoItemCanceled(int)));
	clsQHarborDialogImpl->show();
    }
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
//	cout << "strParamName: " << strParamName << endl;
	qtableEntries->setText(iIndex, COL_PARAM_LABEL, strParamLabel);
	qtableEntries->setText(iIndex, COL_PARAM_NAME, strParamName);
	
	if(clsItem!=NULL){
	    fillMinMaxValueField(clsItem, strParamName, iIndex);
	}
	for(int ii = 0; ii<qtableEntries->numCols(); ii++){
	    qtableEntries->adjustColumn(ii);
	}
    }
    clsItem=NULL;
}


void ClsQHarborImpl::slotChangeValue(int iItemType,  string strItemID,  int iIndex, string strParamName,  double fValue){
//    cout << "ClsQHarborImpl::slotChangeValue()" << endl;

    QString qstr;
    qstr.setNum(fValue);
    string strParamValue = qstr.latin1();


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

    qtableEntries->setText(iIndex, COL_VALUE, strParamValue);



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
	ClsParameter* clsParam = clsSubItem->getParameter(strParamName);
	if(clsParam!=NULL){
	    qtableEntries->setText(iRow, COL_MIN, 
				   QString::number(dynamic_cast<ClsDoubleParameter*>(clsParam)->getMinimum()));
	    qtableEntries->setText(iRow, COL_MAX, 
				   QString::number(dynamic_cast<ClsDoubleParameter*>(clsParam)->getMaximum()));
	    qtableEntries->setText(iRow, COL_VALUE, clsParam->getValueAsString());
	}
    }
}


void ClsQHarborImpl::slotDoItemCanceled(int iIndex){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotDoItemCanceled(int iIndex)" << endl;
#endif
//    cout << "iIndex: " << iIndex << endl;
    if(iIndex<0){
	iIndex = 0; /* must be the first row, since -1 means new row was added */
	qtableEntries->removeRow(iIndex);
	clsItem = NULL;
	mapParams.clear();
    }
}


list<ClsParamTrade> ClsQHarborImpl::getParamTrades(){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::getParamTrades()" << endl;
#endif
    list<ClsParamTrade> lstParamTrades;
    for(int ii = 0; ii<qtableEntries->numRows(); ii++){
	if(qtableEntries->text(ii, COL_PARAM_NAME).length()>0){ /* cos we deleted this entry for
								   invalid param names */
	    ClsParamTrade paramTrade;
	    string strType = qtableEntries->text(ii, COL_TYPE); paramTrade.Type = strType;
	    string strName = qtableEntries->text(ii, COL_NAME); paramTrade.Name = strName ;
	    string strID  = qtableEntries->text(ii, COL_ID); paramTrade.ID  = strID  ;
	    string strChild = qtableEntries->text(ii, COL_CHILD); paramTrade.Child = strChild;
	    string strParam = qtableEntries->text(ii, COL_PARAM_NAME); paramTrade.Param = strParam;
	    double fMin   = iqrUtils::string2double(qtableEntries->text(ii, COL_MIN)); paramTrade.Min = fMin ;
	    double fMax   = iqrUtils::string2double(qtableEntries->text(ii, COL_MAX)); paramTrade.Max = fMax ;
	    double fValue = iqrUtils::string2double(qtableEntries->text(ii, COL_VALUE)); paramTrade.Value = fValue;
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
    list<int> lstSelections;
    for(int ii = qtableEntries->numRows()-1; ii>=0; ii--){ /* we have to start at the end of the table
							      because deleting shifts the rows... */
	if(qtableEntries->isRowSelected (ii, true )){
//	    cout << "selected: " << ii << endl;
	    lstSelections.push_back(ii);
	}
    }

    for(list<int>::iterator it = lstSelections.begin(); it!=lstSelections.end(); it++){
	    qtableEntries->removeRow(*it);
//	    cout << "to delete: " << *it << endl;
    }
    for(int ii = 0; ii<qtableEntries->numCols(); ii++){
	qtableEntries->adjustColumn(ii);
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
	    /*QDir::homeDirPath ()*/ "", 
	    "Harbor Config (*.hconf)",
	    this,
	    "Save File"
	    "Choose a filename to save under" );
	
	/* append default extension here */
	QString qstrExtension( ".hconf" );
	QString qstrCurrentTail = qstrFileName.right(qstrExtension.length());
	if(qstrCurrentTail.compare(qstrExtension)){
	    qstrFileName.append(qstrExtension);
	} 
	/* ------------------------------ */


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
    
    for(int ii = 0; ii<qtableEntries->numRows(); ii++){
	QDomElement paramTrade = doc.createElement( "ParamTrade" );
	paramTrade.setAttribute ( "itemType", qtableEntries->text(ii, COL_TYPE) );
	paramTrade.setAttribute ( "itemID", qtableEntries->text(ii, COL_ID) );
	paramTrade.setAttribute ( "itemChild", qtableEntries->text(ii, COL_CHILD) );
	paramTrade.setAttribute ( "ParamLabel", qtableEntries->text(ii, COL_PARAM_LABEL) );
	paramTrade.setAttribute ( "ParamName", qtableEntries->text(ii, COL_PARAM_NAME) );
	root.appendChild( paramTrade );
    }	    
    QString xml = doc.toString();
//    cout << xml << endl;


    QFile file(qstrFileName);
    if ( file.open( IO_WriteOnly ) ) {
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
	"",
	"Harbor Config (*.hconf)",
	this,
	"Open File"
	"Choose a file" );
    
    loadConfig(filename);
};

void ClsQHarborImpl::loadConfig(string strConfigName){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::loadConfig(string strConfigName)" << endl;
#endif

    if(strConfigName.length()>0){
	QFile qfile( strConfigName );
	if ( !qfile.open( IO_ReadOnly ) ) {
	    QMessageBox::critical( 0, tr( "Critical Error" ),
				   tr( "Cannot open file %1" ).arg( strConfigName ) );
	    return;
	}
	
	QDomDocument domTree;
	if ( !domTree.setContent( &qfile ) ) {
	    QMessageBox::critical( 0, tr( "Critical Error" ),
				   tr( "Parsing error for file %1" ).arg( strConfigName ) );
	    qfile.close();
	    return;
	}
	qfile.close();

//	clearTable();
	
	// get the header information from the DOM
	QDomElement root = domTree.documentElement();
	QDomNode node;
	node = root.firstChild(); /* ParamTrade */
	
	while ( !node.isNull() ) {
	    string strItemType = node.toElement().attribute("itemType");
	    string strItemID = node.toElement().attribute("itemID");
	    string strSubItemName = node.toElement().attribute("itemChild");
	    string strParamName = node.toElement().attribute("ParamName");
	    string strParamLabel = node.toElement().attribute("ParamLabel");
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
	    
	    qtableEntries->insertRows(0,1);
	    qtableEntries->setText(0, COL_ID, strItemID);
	    qtableEntries->setText(0, COL_TYPE, strItemType);
	    qtableEntries->setText(0, COL_CHILD, strSubItemName);

	    if(clsItemTemp!=NULL){
		qtableEntries->setText(0, COL_NAME, strItemName);
		qtableEntries->setText(0, COL_PARAM_LABEL, strParamLabel);
		qtableEntries->setText(0, COL_PARAM_NAME, strParamName);
		fillMinMaxValueField(clsItemTemp, strParamName, 0);	
	    }
	    node = node.nextSibling();
	}
	for(int ii = 0; ii<qtableEntries->numCols(); ii++){
	    qtableEntries->adjustColumn(ii);
	}
    }
}


void ClsQHarborImpl::clearTable(){
    for(int ii = qtableEntries->numRows()-1; ii>=0; ii--){ /* we have to start at the end of the table
							      because deleting shifts the rows... */
	qtableEntries->removeRow(ii);
    }
}

void ClsQHarborImpl::slotItemDeleted(int /*_iType*/, string _strID ){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotItemDeleted(int iType, string strID )" << endl;
#endif
    /* remove item from the list */
    for(int ii = qtableEntries->numRows()-1; ii>=0; ii--){ /* we have to start at the end of the table
							      because deleting shifts the rows... */
	string strID = qtableEntries->text(ii, COL_ID);
	if(!strID.compare(_strID)){
	    qtableEntries->removeRow(ii);
	}
    }
    for(int ii = 0; ii<qtableEntries->numCols(); ii++){
	qtableEntries->adjustColumn(ii);
    }
};

void ClsQHarborImpl::slotItemChanged(int iItemType, string _strID ){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::slotItemChanged(int iType, string strID )" << endl;
#endif

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
	    for(int ii = 0; ii<qtableEntries->numRows(); ii++){
		string strID = qtableEntries->text(ii, COL_ID);
		if(!strID.compare(_strID)){
		    bool bSubItemTypeChanged = false;
		    /* check (omitted) + change name */
		    qtableEntries->setText(ii, COL_NAME, _strItemName);
		
		    string str = qtableEntries->text(ii, COL_CHILD);
//		cout << "str: " << str << endl;
//		cout << "_strSubItemName: " << _strSubItemName << endl;
		    if(_strSubItemName.compare(str)){
//		    cout << "subitem type changed" << endl;
			bSubItemTypeChanged = true;
			qtableEntries->setText(ii, COL_CHILD, _strSubItemName);
		    } else {
//		    cout << "subitem type NOT changed" << endl;
			bSubItemTypeChanged = false;
		    }

		    bool bParameterSupported = true;
		    if(bSubItemTypeChanged){
			/* check if subitem supports parameter */
			string str = qtableEntries->text(ii, COL_PARAM_NAME);
			if(std::find(lstParams.begin(), lstParams.end(), str)!=lstParams.end()){
//			cout << "PARAMETER SUPPORTED" << endl;
			} else {
//			cout << "PARAMETER _NOT_ SUPPORTED" << endl;
			    bParameterSupported = false;
			}
		    }
		
		    if(bParameterSupported){
			string strParamName = qtableEntries->text(ii, COL_PARAM_NAME);
			fillMinMaxValueField(clsItemTemp, strParamName, ii);
		    
		    }
		    else {
			qtableEntries->setText(ii, COL_PARAM_LABEL, "");
			qtableEntries->setText(ii, COL_PARAM_NAME, "");
			qtableEntries->setText(ii, COL_MIN, "");
			qtableEntries->setText(ii, COL_MAX, "");
			qtableEntries->setText(ii, COL_VALUE, "");
		    }
		}
	    }
	}
	for(int ii = 0; ii<qtableEntries->numCols(); ii++){
	    qtableEntries->adjustColumn(ii);
	}
    }
};



void ClsQHarborImpl::slotCellDoubleClicked(int row, int col, int /*button*/, const QPoint & /*mousePos*/){
//    cout << "ClsQHarborImpl::slotCellDoubleClicked(int row, int col, int button, const QPoint & mousePos)" << endl;
    string _strID = qtableEntries->text(row, COL_ID);
//	cout << _strID << endl;
    int iItemType = -1;
    string strItemType = qtableEntries->text(row, COL_TYPE);
    if(!strItemType.compare("Group")){
	iItemType = ClsFESystemManager::ITEM_GROUP;
    } 
    else if(!strItemType.compare("Connection")){
	iItemType = ClsFESystemManager::ITEM_CONNECTION;
    }
    
    if(col == COL_PARAM_LABEL){
	doDialog(iItemType, _strID, row);
    } else if(col == COL_VALUE){
	string strParamLabel = qtableEntries->text(row, COL_PARAM_LABEL);
	string strParamName = qtableEntries->text(row, COL_PARAM_NAME);
	double fMin = qtableEntries->text(row, COL_MIN).toDouble();
	double fMax = qtableEntries->text(row, COL_MAX).toDouble();
	double fValue = qtableEntries->text(row, COL_VALUE).toDouble();

	doValueDialog(iItemType, _strID, row, strParamName, strParamLabel, fMin, fMax, fValue);
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
	for(int ii = 0; ii<qtableEntries->numRows(); ii++){
	    if(qtableEntries->text(ii, COL_PARAM_NAME).length()>0){ /* cos we deleted this entry for
								       invalid param names */
		string strItemType = qtableEntries->text(ii, COL_TYPE);
		string strID  = qtableEntries->text(ii, COL_ID);
		string strParamName = qtableEntries->text(ii, COL_PARAM_NAME); 
		
		ClsItem* clsItemTemp = NULL;
		if(!strItemType.compare("Group")){
		    clsItemTemp = ClsFESystemManager::Instance()->getFEGroup( strID );
		} else if (!strItemType.compare("Connection")){
		    clsItemTemp = ClsFESystemManager::Instance()->getFEConnection( strID );
		    
		}
		
		if(clsItemTemp!=NULL){
		    fillMinMaxValueField(clsItemTemp, strParamName, ii);
		} 
	    }
	}
	qtableEntries->adjustColumn(COL_VALUE);    
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
	    /*QDir::homeDirPath ()*/ "", 
	    "Harbor ParamSet (*.pconf)",
	    this,
	    "Save File"
	    "Choose a filename to save under" );
	
	/* append default extension here */
	QString qstrExtension( ".pconf" );
	QString qstrCurrentTail = qstrFileName.right(qstrExtension.length());
	if(qstrCurrentTail.compare(qstrExtension)){
	    qstrFileName.append(qstrExtension);
	} 
	/* ------------------------------ */


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

    
    for(int ii = 0; ii<qtableEntries->numRows(); ii++){
	QDomElement parameter = doc.createElement( "Parameter" );
	parameter.setAttribute ( "itemType", qtableEntries->text(ii, COL_TYPE) );
	parameter.setAttribute ( "itemID", qtableEntries->text(ii, COL_ID) );
	parameter.setAttribute ( "name", qtableEntries->text(ii, COL_PARAM_NAME) );
	parameter.setAttribute ( "value", qtableEntries->text(ii, COL_VALUE) );
	
	root.appendChild( parameter );

    }	    
    QString xml = doc.toString();
//    cout << xml << endl;


    QFile file(qstrFileName);
    if ( file.open( IO_WriteOnly ) ) {
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
	"",
	"Harbor ParamSet (*.pconf)",
	this,
	"Open File"
	"Choose a file" );
    
    loadParamSet(filename);
};

void ClsQHarborImpl::loadParamSet(string strParamSetName){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "ClsQHarborImpl::loadParamSet(string strParamSetName)" << endl;
#endif
    
    if(strParamSetName.length()>0){
	QFile qfile( strParamSetName );
	if ( !qfile.open( IO_ReadOnly ) ) {
	    QMessageBox::critical( 0, tr( "Critical Error" ),
				   tr( "Cannot open file %1" ).arg( strParamSetName ) );
	    return;
	}
	
	QDomDocument domTree;
	if ( !domTree.setContent( &qfile ) ) {
	    QMessageBox::critical( 0, tr( "Critical Error" ),
				   tr( "Parsing error for file %1" ).arg( strParamSetName ) );
	    qfile.close();
	    return;
	}
	qfile.close();
	
	
	// get the header information from the DOM
	QDomElement root = domTree.documentElement();
	QDomNode node;
	node = root.firstChild(); /* Parameter */
	
	while ( !node.isNull() ) {
	    string strItemType = node.toElement().attribute("itemType");
	    string strItemID = node.toElement().attribute("itemID");
	    string strParamName = node.toElement().attribute("name");
	    string strParamValue = node.toElement().attribute("value");
	    string strItemName = "";
	    
	    if(!setTableItemValue(strItemID, strParamName,strParamValue)){
		cerr << "parameter not found" << endl;
		string strError = string("Cannot find parameter \"") + strParamName + string("\"\nfor item \"") + strItemID + string("\"\n");
		QMessageBox::warning( this, "iqr Harbor",
				      strError,
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
	for(int ii = 0; ii<qtableEntries->numCols(); ii++){
	    qtableEntries->adjustColumn(ii);
	}
    }
    
}









bool ClsQHarborImpl::checkForItem(string _strItemID, string _strParamName){
#ifdef DEBUG_CLSQHARBORIMPL
    cout << "checkForItem(string strItemID, string strParamName)" << endl;
#endif

    for(int ii = 0; ii<qtableEntries->numCols(); ii++){
	string strItemID = qtableEntries->text(ii, COL_ID);
	string strParamName = qtableEntries->text(ii, COL_PARAM_NAME);
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

    for(int ii = 0; ii<qtableEntries->numCols(); ii++){
	string strItemID = qtableEntries->text(ii, COL_ID);
	string strParamName = qtableEntries->text(ii, COL_PARAM_NAME);
	if(!strItemID.compare(_strItemID) && !strParamName.compare(_strParamName)){
	    qtableEntries->setText(ii, COL_VALUE, strValue);
	    return true;
	}
    }
    return false;


}
