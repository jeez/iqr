#include <stdio.h>
#include <sys/times.h>

#include <qaccel.h>
#include <qapp.h>
#include <qimage.h>
#include <qnamespace.h>
#include <qpainter.h>
#include <qwidget.h>


#include "ccvt.h"

class CImageWidget: public QWidget
{
private:
   QSize m_Size;
   QImage m_Image;

protected:
   void paintEvent(QPaintEvent *e)
   {
      QPainter p;
   
      p.begin(this);
      p.drawImage(0, 0, m_Image);
      p.end();
   }
   
public:
   CImageWidget(int width, int height, QWidget *parent = 0, const char *name = 0)
     : QWidget(parent, name)
   {
      m_Size.setWidth(width);
      m_Size.setHeight(height);
      m_Image.create(width, height, 32);
   }
   
   void SetData(const void *src)
   {
      memcpy(m_Image.bits(), src, m_Image.numBytes());
      update();
   }

   QSize sizeHint() const { return m_Size; };
};




int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
   CImageWidget *iw;
   QAccel *acc;

   unsigned char Input[256 * 256 + 2 * 128 * 128];
   unsigned char Output[4 * 256 * 256];
   unsigned char *p;
   int x, y, tx, ty;

   iw = new CImageWidget(256, 256);
   app.setMainWidget(iw);
   acc = new QAccel(iw);
   acc->connectItem(acc->insertItem(Qt::Key_Q + Qt::CTRL), &app, SLOT(quit()));
   
   /* Fill input buffer with pattern for testing. Use a square centered 
      gradient for the gray values, and both horizontal and vertical 
      color gradients. 
    */
   p = &Input[0];
   for (x = 0; x < 256; x++) {
      for (y = 0; y < 256; y++) {
         if (x < 128)
           tx = x;
         else 
           tx = 255 - x;
         if (y < 128)
           ty = y;
         else
           ty = 255 - y;
         if (tx < ty)
           *p++ = tx;
         else
           *p++ = ty;
      }
   }
   for (x = 0; x < 128; x++)
      for (y = 0; y < 128; y++)
         *p++ = 64 + x;
   for (x = 0; x < 128; x++)
      for (y = 0; y < 128; y++)
         *p++ = 64 + y;

   ccvt_420p_bgr32(256, 256, Input, Output);
//   ccvt_420p_rgb32(256, 256, Input, Output);
//   ccvt_420p_bgr24(256, 256, Input, Output);
//   ccvt_420p_rgb24(256, 256, Input, Output);

   iw->show();
   iw->SetData(Output);
   return app.exec();
}
