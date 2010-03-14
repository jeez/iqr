#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <qtimer.h>
#include <qwidget.h>
#include <qmainwindow.h>

#include "wSpacePlot.hpp"
#include "wLinePlot.hpp"
#include "wBarPlot.hpp"

class MainWin : public QMainWindow {
  Q_OBJECT
public:
  MainWin();

protected:
  QTimer *timer;
};


class SpaceWidget : public QWidget {
  Q_OBJECT
public:
  SpaceWidget();
  
public slots:

  void update();

protected:

  float xphase, yphase;

  WSpacePlot *plot;
};


class LineWidget : public QWidget {
  Q_OBJECT
public:
  LineWidget();
  
public slots:

  void update();

protected:

  float sinPhase, cosPhase;

  WLinePlot *plot;
};



class BarWidget : public QWidget {
  Q_OBJECT
public:
  BarWidget();
  
public slots:

  void update();

protected:
  WBarPlot *plot;
  float sinPhase;
};

#endif // EXAMPLE_HPP
