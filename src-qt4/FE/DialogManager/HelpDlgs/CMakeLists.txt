SET(CMAKE_VERBOSE_MAKEFILE TRUE)

cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE(Qt4 REQUIRED)

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

SET(QT_MT_REQUIRED TRUE)
SET(QT_USE_QTUITOOLS TRUE)
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../
  ${QT_INCLUDE_DIR}
)

SET(SOURCES
 dlgAboutIqr.cpp
 dlgReleaseNotes.cpp
)

SET(UIS
  iqrAbout.ui
  iqrReleaseNotes.ui
)


SET(MOC_HEADERS
  dlgAboutIqr.h
  dlgReleaseNotes.h
  ui_iqrReleaseNotes.h
  ui_iqrAbout.h
)


QT4_WRAP_UI(UI_HEADERS  ${UIS})
QT4_WRAP_CPP(SOURCES ${MOC_HEADERS} )


ADD_LIBRARY(HelpDlgs STATIC ${SOURCES})

TARGET_LINK_LIBRARIES(HelpDlgs ${QT_LIBRARIES})




