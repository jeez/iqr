#ifndef DLGABOUTIQR_H
#define DLGABOUTIQR_H

#include "ui_iqrAbout.h"
using namespace std;

class dlgAboutIqr : public QDialog, private Ui::dlgAboutIqr {
    Q_OBJECT

public:
    dlgAboutIqr(QWidget* parent = 0);


};

#endif


