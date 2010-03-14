/****************************************************************************
 ** $Filename: ClsQSAList.h
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Thu Sep  4 14:41:10 2003
 **
 *****************************************************************************/


#ifndef CLSQSALIST_H
#define CLSQSALIST_H    /*+ To stop multiple inclusions. +*/

#include <string.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <valarray>

#include <QListWidget>
#include <QPixmap>
#include <QWidget>
#include <QToolTip>

using namespace std;


template <class T> std::ostream& operator<<(std::ostream &_stream, std::vector<std::vector<T> >  &_v) {
    int iNrCellsX = _v.size();
    int iNrCellsY = _v[0].size();

    for ( int i = 0; i < iNrCellsX; i++ ) {
	for ( int j = 0; j < iNrCellsY; j++ ) {
	    double fValue = _v[i][j];
	    _stream << fValue << (j < iNrCellsY-1 ? "\t" : "");
	}
	_stream << endl;
    }
    return _stream;
};


template <class T> std::ifstream& operator>>(std::ifstream &_stream, std::vector<std::vector<T> >  &_v) {
    char str[1024];
    while(_stream) {
	_stream.getline(str,1024);
	if(strlen(str)==0){
	    break;
	}
	vector<double> v2;
	istringstream is(str);
	while(!is.eof()) {
	    double f;
	    is >> f;
	    v2.push_back(f);
	}
	_v.push_back(v2);
    }
    return _stream;
};



class ClsQGroupStateManip;


class ClsPixmapListWidgetItem : public QListWidgetItem {

public:
    ClsPixmapListWidgetItem(QListWidget * listbox, const QPixmap & _pixmap) : QListWidgetItem(listbox, QListWidgetItem::UserType+1)  {
	QListWidgetItem::setIcon ( QIcon(_pixmap) );
    };

    vector <vector<double> > getMatrix() {
	return vCells;
    };

    void setMatrix(vector <vector<double> > _v) {
	vCells = _v;
	QString qstr = QString::number(vCells.size()-1) + QString("x") + QString::number(vCells[0].size()-1);
	QListWidgetItem::setText(qstr);
    }

private:
    string strID;
    vector <vector<double> > vCells;
};






class ClsQSAList : public QFrame  {
    Q_OBJECT

public:
    ClsQSAList ( QWidget * parent = 0, ClsQGroupStateManip* _clsQGroupStateManip=0, const char * name = 0, Qt::WindowFlags f = 0 );
    void insertSA(QPixmap qpm, string, vector <vector<double> > v, bool bReplace = false);

    int getCount() { return qlbox->count(); };
    vector <vector<double> > getVectorAt(int i);

 public slots:


 private slots:

    void slotDeleteItem();
    void slotMoveUp();
    void slotMoveDown();
    void slotInvert();
    void slotDoubleClicked ( const QModelIndex & index );
    void slotSave();
    void slotSaveAs();
    void slotLoad();

    signals:
    void sigChangeMatrix(vector <vector<double> >);


private:
    ClsQGroupStateManip* clsQGroupStateManip;
    string strFilename;
    int iListViewVisibleWidth;

    QListWidget* qlbox;


};




#endif /* CLSQSALIST_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
