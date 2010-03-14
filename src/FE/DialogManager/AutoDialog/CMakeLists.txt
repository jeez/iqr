SET(CMAKE_VERBOSE_MAKEFILE TRUE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fpermissive")

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT -DLINUX)

SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt3 REQUIRED)


INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR} #/AutoDialog
  ${CMAKE_CURRENT_SOURCE_DIR}/Helper
  ${CMAKE_CURRENT_SOURCE_DIR}/Dialogs
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../Common
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/Helper 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/Item 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/Item/Neuron 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/Parser 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../Common/Types/Neurons 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../DiagramManager 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../IQR 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../System 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../System/Connection/Pattern 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../System/Topology 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../System
  ${QT_INCLUDE_DIR}
)






SET(libSrc
#  Widgets/doubleSpinBoxTest.cpp
#  Widgets/doubleSpinBox.cpp
  Widgets/intSpinBox.cpp
  Dialogs/autoGroupPatternForeach.cpp
  Dialogs/ClsPatternSelectorAll.cpp
  Dialogs/ClsBaseAutoGroup.cpp
  Dialogs/ClsPatternSelectorList.cpp
  Dialogs/xRefWidget.cpp
  Dialogs/stringWidget.cpp
  Dialogs/subtypedAutoDialog.cpp
  Dialogs/ClsPatternBaseSelector.cpp
  Dialogs/labelWidget.cpp
  Dialogs/intWidget.cpp
  Dialogs/longStringWidget.cpp
  Dialogs/ClsPatternSelectorRegion.cpp
  Dialogs/autoGroupPatternTuples.cpp
  Dialogs/xRefHolderWidget.cpp
  Dialogs/autoWidget.cpp
#  Dialogs/_unused/subtypeWidget.cpp
  Dialogs/dialogButtons.cpp
  Dialogs/boolWidget.cpp
  Dialogs/ClsSelectorSparse.cpp
  Dialogs/autoDialog.cpp
  Dialogs/doubleWidget.cpp
  Dialogs/optionsWidget.cpp
  Dialogs/widgetFactory.cpp
  Dialogs/parameterWidget.cpp
  Dialogs/ClsPatternSelectorTuples.cpp
  Dialogs/memberWidget.cpp
  Dialogs/autoGroup.cpp
)


SET(libMocHdr
  Dialogs/autoDialog.hpp
  Dialogs/autoGroup.hpp
  Dialogs/autoGroupPatternForeach.hpp
  Dialogs/autoGroupPatternTuples.hpp
  Dialogs/autoWidget.hpp
  Dialogs/boolWidget.hpp
  Dialogs/dialogButtons.hpp
  Dialogs/dialog.hpp
  Dialogs/doubleWidget.hpp
  Dialogs/intWidget.hpp
  Dialogs/labelWidget.hpp
  Dialogs/longStringWidget.hpp
  Dialogs/optionsWidget.hpp
  Dialogs/parameterWidget.hpp
  Dialogs/stringWidget.hpp
  Dialogs/subtypedAutoDialog.hpp
  Dialogs/widgetFactory.hpp
  Dialogs/xRefHolderWidget.hpp
  Dialogs/xRefWidget.hpp
  Dialogs/ClsBaseAutoGroup.h
  Dialogs/ClsListItems.h
  Dialogs/ClsPatternBaseSelector.h
  Dialogs/ClsPatternSelectorAll.h
  Dialogs/ClsPatternSelectorList.h
  Dialogs/ClsPatternSelectorRegion.h
  Dialogs/ClsPatternSelectorTuples.h
  Dialogs/ClsSelectorSparse.h
  Dialogs/ClsTuplesImporter.h
  Dialogs/memberWidget.h
  Widgets/doubleSpinBox.hpp
  Widgets/intSpinBox.hpp
)

QT_WRAP_CPP(AutoDialog libSrc ${libMocHdr} )

ADD_LIBRARY(AutoDialog STATIC ${libSrc})

TARGET_LINK_LIBRARIES(AutoDialog ${QT_LIBRARIES})

