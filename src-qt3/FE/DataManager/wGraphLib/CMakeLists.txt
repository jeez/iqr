SET(CMAKE_VERBOSE_MAKEFILE TRUE)


ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)


SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt3 REQUIRED)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/Item/ 
  ${QT_INCLUDE_DIR}

)


SET(libSrc
  wBarPlot.cpp
  wColorBar.cpp
  wGraphAxis.cpp
  wGraphFrame.cpp
  wGraphLegend.cpp
  wGraphPlot.cpp
  wGraphTrace.cpp
  wGraphWidget.cpp
  wLinePlot.cpp
  wMath.cpp
  wRangeEdit.cpp
  wSpacePlot.cpp
)

SET(libMocHdr
  wBarPlot.h 
  wColorBar.h 
  wGraphAxis.h
  wGraphFrame.h 
  wGraphLegend.h
  wGraphPlot.h 
  wGraphTrace.h 
  wGraphWidget.h 
  wLinePlot.h 
  wRangeEdit.h
  wSpacePlot.h
)

QT_WRAP_CPP(wGraph libSrc ${libMocHdr} )

ADD_LIBRARY(wGraph STATIC ${libSrc})

TARGET_LINK_LIBRARIES(wGraph ${QT_LIBRARIES})




