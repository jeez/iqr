#include <QtGui/qwidget.h>
#include <QScrollArea>

#include "item.hpp"
#include "xRefHolder.hpp"

//#include "xRefWidget.hpp"

 

using namespace std;
using namespace iqrcommon;

namespace iqrfe {
    class ClsXRefWidget;
    
    class ClsXRefHolderWidget : public  QScrollArea /*QWidget*/
    {
	Q_OBJECT
	
    public:	
	ClsXRefHolderWidget(ClsItem& _item, const ClsXRefHolder* xRefHolder, 
			    QWidget *_pqwgtParent, const char *_pcName = 0);

	void apply();
    signals:
 	void changed();

    private:
	list<ClsXRefWidget*> lstXRefWidgets;

	int iOwnerType;
	string strOwnerParentID;
	QWidget *qwgtContainer;
 

    };
}
    
//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
