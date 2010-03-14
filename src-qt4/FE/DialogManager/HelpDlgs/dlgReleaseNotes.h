#ifndef DLGRELEASENOTES_H
#define DLGRELEASENOTES_H

#include "ui_iqrReleaseNotes.h"
using namespace std;

class dlgReleaseNotes : public QDialog, public Ui::dlgReleaseNotes {
    Q_OBJECT

public:
    dlgReleaseNotes(QWidget* parent = 0);


};

#endif

