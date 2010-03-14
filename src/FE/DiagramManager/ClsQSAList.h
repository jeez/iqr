/****************************************************************************
 ** $Filename: ClsQSAList.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Thu Sep  4 14:41:10 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
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

#include <qframe.h>
#include <qlistbox.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qtooltip.h>

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


/* **********************************************************
   GQListBoxToolTip courtesy Jeff Prosa <jprosa@ptelecom.net> 
   ********************************************************** */

class GQListBoxToolTip : public QToolTip {
public:
    GQListBoxToolTip(QListBox* lb) : QToolTip(lb->viewport()), listbox_(lb) {
    };

    void maybeTip(const QPoint& pos) {
        // get pointed item
        QListBoxItem* item = listbox_->itemAt(pos);
        if (item == 0) {
            return;
        }
        // show tooltip if item is not fully visible
        QRect r = listbox_->itemRect(item);
        r.setWidth(item->width(listbox_));
        if (!listbox_->viewport()->rect().contains(r)) {
            tip(r, item->text());
        }
    };
protected:
    QListBox* listbox_;
};




class ClsListBoxSA : public QListBoxPixmap {

public:
    ClsListBoxSA(QListBox * listbox, const QPixmap & _pixmap) : QListBoxPixmap( _pixmap ) {
//	lbl = new QLabel(this);
    };

    vector <vector<double> > getMatrix() {
	return vCells;
    };

    void setMatrix(vector <vector<double> > _v) {
	vCells = _v;
	QString qstr = QString::number(vCells.size()-1) + QString("x") + QString::number(vCells[0].size()-1);
	setText(qstr);
//	lbl->setText(qstr);
    }

private:
    string strID;
    vector <vector<double> > vCells;
//    QLabel *lbl;
};






class ClsQSAList : public QFrame  {
    Q_OBJECT

public:
    ClsQSAList ( QWidget * parent = 0, ClsQGroupStateManip* _clsQGroupStateManip=0, const char * name = 0, WFlags f = 0 );
    void insertSA(QPixmap qpm, string, vector <vector<double> > v, bool bReplace = false);

    int getCount() { return qlbox->count(); };
    vector <vector<double> > getVectorAt(int i);

 public slots:


 private slots:

    void slotDeleteItem();
    void slotMoveUp();
    void slotMoveDown();
    void slotInvert();
    void slotDoubleClicked ( QListBoxItem * item );
    void slotSave();
    void slotSaveAs();
    void slotLoad();

    signals:
    void sigChangeMatrix(vector <vector<double> >);


private:
    ClsQGroupStateManip* clsQGroupStateManip;
    string strFilename;
    int iListViewVisibleWidth;

    QListBox* qlbox;
//    map<int, string> mapSA;

    GQListBoxToolTip *cQListBoxToolTip ;


};





/// Local Variables:
/// mode: c++
/// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
/// End:


#endif /* CLSQSALIST_H */
