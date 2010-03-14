#include <ClsQDivider.h>
#include <qpixmap.h> 
#include <qlayout.h> 
//#include <qlabel.h> 
#include <divide_right_6x40.xpm>
#include <divide_left_6x40.xpm>

ClsQDivider::ClsQDivider(QWidget * parent=0 ) : QFrame(parent){ 
       this->setFrameStyle(QFrame::WinPanel | QFrame::Raised );
       this->setLineWidth(2);
       this->setFixedWidth(8);

       QVBoxLayout * boxlayoutControls = new QVBoxLayout(this,0,0);
       bIsRight = false;
       qbtnMove  = new QPushButton(this);
       qbtnMove->setPixmap( QPixmap(divide_right ));
       boxlayoutControls->addWidget(qbtnMove,0, AlignVCenter | AlignHCenter );
       connect(qbtnMove, SIGNAL(clicked()), SLOT(slotMove ()));
};

void ClsQDivider::slotMove ( ) {
     if (bIsRight){
	  bIsRight = false;
	  qbtnMove->setPixmap( QPixmap(divide_right ));
     } else{
	  bIsRight = true;
	  qbtnMove->setPixmap( QPixmap(divide_left ));
     }
      emit clicked(bIsRight);
};

