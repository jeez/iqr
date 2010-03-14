#ifndef WSPACEPLOT_HPP
#define WSPACEPLOT_HPP

#include "wColorBar.h"

class QTimer;

/*! The WSpacePlot inherits WGraphWidget and adds functionalities to
  display two-dimensional data. The data is visualized in an array of
  cells, which can be selected at the GUI level.

  The values are displayed by a color code, which can be visualized by
  a WColorBar associated to the WSpacePlot.*/

class WSpacePlot : public WGraphWidget {
  Q_OBJECT
public:
  enum {
    empty=0x00, 
    normal=0x01,
    staticSelected=0x02, 
    tmpSelected=0x04, 
    selected=0x06,
    marked=0x08
  } CellAttrType;
  
  /*! This data structure carries the data and attributes of a cell
    within the 2D array of a WSpacePlot.*/
  class SpaceCell {
    friend class WSpacePlot;
  public:
    /*! Sets the value of this cell to \em v. No effect for empty
      cells.*/
    void setValue(float v) { data=v; }
    /*! Adds \em v to the current value of this cell. No effect for
      empty cells.*/
    void addValue(float v) { data+=v; }
    /*! Sets the attribute of this cell to empty if \em e = true, to
      normal otherwise. Empty cells do not display any data, and
      cannot be selected.*/
    void setEmpty(bool e) { if (e) attr&=~normal; else attr|=normal; }
    bool isSelected() const { return (attr & selected) && (attr & normal); }
  private:
    float data;
    char  attr;
    bool  visible;
    QRect rect;
  };

  /*! Constructs a WSpacePlot and passes \em parent, \em xAxis, \em
    yAxis and \ border to WGraphWidget. The size of the array cells
    is given by \em size. If \em drawGrid = true, no data will be
    displayed, but a grid instead.*/
  WSpacePlot(QSize size, bool drawGrid,
	     QWidget* parent, QWidget *colorBarParent=0,
	     WGraphXAxis::Position xAxisPos=WGraphXAxis::bottom,
	     WGraphYAxis::Position yAxisPos=WGraphYAxis::left,
	     int border=0);
  ~WSpacePlot();

  void writeXML(QDomElement &);
  void readXML(QDomElement &);

  /*! Returns true, if auto scaling is enabled, false otherwise. \sa
    setAutoScale()*/
  bool isAutoScale() { return colorBar?colorBar->isAutoScale():autoScale; }; 
  /*! Returns the range over which the current data extends.*/
  WRange getDataRange() { return dataRange; };
  

  /*! Returns a reference to a QMemArray<SpaceCell> which contains the
    cell data and attributes.*/
  QMemArray<SpaceCell> &getCellData() { return cellData; }

  /*! Clears all the cells by setting their values to zero. All cells
    become normal cells, i.e. non-empty.

    NOTE: by default, selections and markings are also cleared*/
  void clearCells(char =0);
  /*! Returns a pointer to the WColorBar generated in the
    constructor. If no color bar was created by passing a 0 pointer
    to WSpacePlot(), a 0 pointer is returned.*/
  WColorBar *getColorBar() const { return colorBar; };

  /*! Returns a reference to a list of indices of the currently
    selected cells.*/
  QMemArray<int> &getSelecIndexList();

  /*! Selects the cells with indices given in \em list. */
  void setSelecIndexList(QMemArray<int > &list);
  
  /*! Returns the cell array size. */
  QSize getSize() const { return viewSize; }
 
public slots:

  /*! Sets the cell array size to \em size and calls clearCells(). */
  void setSize(QSize size);
  /*! Enables auto scaling if \em autoScale = true, disables it
    otherwise. Auto scaling means, that the cell with the smallest
    value is balck, while the cell with the maximal value has the
    color given by setColor().*/
  void setAutoScale(bool a) { 
    autoScale=a; 
    if (colorBar) colorBar->setAutoScale(a);
    updateGraph(); 
  };
  /*! If \em multi = true, mutliple cells can be selected at the GUI
    level, only one at a time otherwise.*/
  void setMultiSelect(bool multi);
  /*! Sets the data range manually to \em r such that cells with
    values below or above this range will be black or have the color
    given by setColor() respectively. This will have no effect if auto
    scaling is enabled.*/
  void setDataRange(WRange r) { dataRange=r; emit rangeChanged(r); update(); };
  /*! Sets the current drawing color to \em c*/
  void setColor(QColor c) { paintColor=c; emit colorChanged(c); };
  /*! Selects all the non-empty cells with values in \em r, unselects
    all the others.*/
  void selectRange(WRange r);
  
  void markSelected();
  void exchgMarkedSelected();
  void clearMarked();

  /*! Clears the current selection.*/
  void clearSelection();
  /*! Selects all the cells. */
  void selectAll() { selectRect(QRect(QPoint(0,0),viewSize)); }
  /*! Inverts the current selection.*/
  void invertSelection();
  /*! Selects the cells within the rectangle given by rect if \em
    select is true, unselects them otherwise. */
  void selectRect(const QRect &rect) {
    selectRectTmp(rect,true);
    acceptTmpSelected();
  }
  
signals:

  /*! This signal is emitted, when the range of the cell values changes.*/
  void rangeChanged(WRange);
  /*! This signal is emitted, when the color changes.*/
  void colorChanged(QColor);
  /*! This signal is emitted, when the selection of cells changes.*/
  void selectionChanged();
  /*! This signal is emitted, when the user moves the mouse while
    keeping the left mouse button pressed. Thus, hook up a slot to
    this signal to handle draging of cell selections.*/
  void wouldDrag();

protected slots:

  void checkDataRange();

  void clearCellLabel() { cellLabelRect=QRect(); updateGraph(); };
  void calcGeometry();

  void autoPan();
  
protected:
  
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void focusInEvent(QFocusEvent *) { updateGraph(); }
  void focusOutEvent(QFocusEvent *) { updateGraph(); }
  void keyPressEvent(QKeyEvent *);
  
  virtual void drawContent(QPainter & , bool);
  
  void selectRectTmp(const QRect &, bool);  
  void clearTmpSelected();
  void acceptTmpSelected();
  int countSelectedCells();
  
  void setCellLabel(QPoint, QPoint);
  
  int lastButton;

  QTimer *autoPanTimer, *labelClearTimer;

  QSize viewSize;

  QColor paintColor;
  WRange dataRange;

  QRect *recRects;

  QRect cellLabelRect;
  QString cellLabelString;

  WColorBar  *colorBar;

  QPoint currCursorPos, startCell, currCell;

  QMemArray<int> selecIndexList;
  QMemArray<SpaceCell> cellData;

  bool selecting, dragEnabled;
  bool drawGrid, autoScale, multiSelect;

};

#endif // WSPACEPLOT_HPP
