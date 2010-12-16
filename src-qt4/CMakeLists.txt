PROJECT(iqr)

# with this variable we can create all inclusive binary distributions that include all the
# required libs...
SET(FULL_BIN false)


cmake_minimum_required(VERSION 2.6)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeScripts)
INCLUDE(getVersion)

IF(NOT WIN32)
  INCLUDE(getLinuxDistro)
ENDIF(NOT WIN32)

STRING(REGEX MATCHALL "[0-9]*\\.[0-9]*" IQR_MAJOR "${iqrVersion}" )
MESSAGE( STATUS "MAJOR: " ${IQR_MAJOR} )

STRING(REGEX REPLACE "([0-9]*\\.[0-9]-)([0-9]*)(.*)" "\\2" IQR_REL "${iqrVersion}" )
MESSAGE( STATUS "REL: " ${IQR_REL} )

#IF(UNIX)
 SET(CMAKE_CXX_FLAGS 
 "${CMAKE_CXX_FLAGS} -pipe -W -Wpointer-arith -Wall -Wuninitialized -fexceptions -fpermissive -fno-use-cxa-atexit -g3 -O2"
)                           
#ENDIF(UNIX)

#IF(WIN32)
# SET(CMAKE_CXX_FLAGS 
# "${CMAKE_CXX_FLAGS}  -W0 -O2"
#)                           
#ENDIF(WIN32)


#IF(WIN32)
#set(XERCESC_INCLUDE_DIR "C:/xerces-c-2.8.0/include")
#set(XERCESC_LIBRARIES "C:/xerces-c-2.8.0/lib/libxerces-c.a")
#set(XERCESC_ROOT_DIR "C:/xerces-c-2.8.0")
#ENDIF(WIN32)

IF(UNIX)
#IF(NOT CMAKE_OSX_ARCHITECTURES)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -pthread")
ENDIF(UNIX)
#ENDIF(NOT CMAKE_OSX_ARCHITECTURES)


ADD_DEFINITIONS(
  -DQT_THREAD_SUPPORT 
  -DQT_NO_DEBUG 
  -DQT3_SUPPORT
  -D_REENTRANT 
  -DUSE_USER_TYPES
)
#  -DDEBUG_IQRFE_CLSTYPEMANAGER 
#  -DDEBUG_MODULEMANAGER 
#  -DDEBUG_NEURONMANAGER 
#  -DDEBUG_SYNAPSEMANAGER 

IF(UNIX)
  ADD_DEFINITIONS(
    -D__KERNEL_STRICT_NAMES
    -DLINUX
  )
ENDIF(UNIX)

IF(WIN32)
  ADD_DEFINITIONS(
    -D_WINDOWS
  )
ENDIF(WIN32)


SET(CMAKE_VERBOSE_MAKEFILE TRUE)



SET(QT_MT_REQUIRED TRUE)
SET(QT_USE_QT3SUPPORT TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTNETWORK TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})



FIND_PACKAGE(XercesC REQUIRED)

SET(appSRC
  FE/System/ClsBaseConnection.cpp 
  FE/System/ClsBaseGroup.cpp 
  FE/System/ClsFEComputeEngine.cpp 
  FE/System/ClsFEConnection.cpp 
  FE/System/ClsFEGroup.cpp 
  FE/System/ClsFEProcess.cpp 
  FE/System/ClsFESystem.cpp 
  FE/System/ClsFESystemBuilder.cpp 
  FE/System/ClsFESystemManager.cpp 
  FE/System/ClsGroupManipPattern.cpp 
  FE/System/ClsHyperLists.cpp 
  FE/System/Connection/Arborization/ClsArbAll.cpp 
  FE/System/Connection/Arborization/ClsArbEllipse.cpp 
  FE/System/Connection/Arborization/ClsArbEllipseWindow.cpp 
  FE/System/Connection/Arborization/ClsArbRandom.cpp 
  FE/System/Connection/Arborization/ClsArbRect.cpp 
  FE/System/Connection/Arborization/ClsArbRectWindow.cpp 
  FE/System/Connection/Arborization/ClsBaseArborization.cpp 
  FE/System/Connection/Pattern/ClsBasePattern.cpp 
  FE/System/Connection/Pattern/ClsPatterns.cpp 
  FE/System/Connection/Pattern/ClsSubPopulations.cpp 
  FE/System/Topology/ClsBaseTopology.cpp 
  FE/System/Topology/ClsTopologyHex.cpp 
  FE/System/Topology/ClsTopologyRect.cpp 
  FE/System/Topology/ClsTopologySparse.cpp 
  FE/System/moduleManager.cpp 
  FE/System/neuronManager.cpp 
  FE/System/synapseManager.cpp 
  FE/IQR/idGenerator.cpp 
  FE/SettingsHandler/ClsFESettings.cpp 
  FE/SettingsHandler/ClsSettingsReader.cpp 
  FE/SettingsHandler/ClsSettingsWriter.cpp 
  FE/Browser/ClsQSystemBrowser.cpp 
  FE/Browser/ClsQSysBrwsrFrame.cpp
  FE/CommHandler/ClsFECommHandlerLocal.cpp 
  FE/DataManager/ClsDataClientConfigReader.cpp 
  FE/DataManager/ClsDataClientConfigWriter.cpp 
  FE/DataManager/ClsFEBasePlot.cpp 
  FE/DataManager/ClsFEConnectionDiagram.cpp 
  FE/DataManager/ClsFEDataClient.cpp 
  FE/DataManager/ClsFEDataManager.cpp 
  FE/DataManager/ClsFEDataSampler.cpp 
  FE/DataManager/ClsFEGroupPlot.cpp 
  FE/DataManager/ClsFEPlotFramework.cpp 
  FE/DataManager/ClsFETimePlot.cpp 
  FE/DataManager/ClsFEDataBroadcaster.cpp 
  FE/DataManager/ClsQBaseStateVariableDisplay.cpp 
  FE/DataManager/ClsQDivider.cpp 
  FE/DataManager/ClsQDragHandle.cpp 
  FE/DataManager/ClsQNeuronStateVariableDisplay.cpp 
  FE/DataManager/ClsQSynapseStateVariableDisplay.cpp 
  FE/DiagramManager/ClsBaseQStateArrayView.cpp 
  FE/DiagramManager/ClsBlockDiagram.cpp 
  FE/DiagramManager/ClsDiagBaseConnection.cpp 
  FE/DiagramManager/ClsDiagConnection.cpp 
  FE/DiagramManager/ClsDiagPhantomConnection.cpp 
  FE/DiagramManager/ClsDiagHyperConnection.cpp
  FE/DiagramManager/ClsDiagConnectionHandle.cpp 
  FE/DiagramManager/ClsDiagConnectionHandleEnd.cpp 
  FE/DiagramManager/ClsDiagConnectionHandleStart.cpp 
  FE/DiagramManager/ClsDiagItemAP.cpp 
  FE/DiagramManager/ClsDiagItem.cpp 
  FE/DiagramManager/ClsFEDiagramManager.cpp 
  FE/DiagramManager/ClsScene.cpp 
  FE/DiagramManager/ClsPanner.cpp 
  FE/DiagramManager/ClsPannerSceneView.cpp 
  FE/DiagramManager/ClsSceneView.cpp 
  FE/DiagramManager/ClsSubDiagram.cpp 
  FE/DiagramManager/ClsQGroupStateManip.cpp 
  FE/DiagramManager/ClsQSAList.cpp 
  FE/DiagramManager/ClsQStateArrayViewHex.cpp 
  FE/DiagramManager/ClsQStateArrayViewRect.cpp 
  FE/DiagramManager/ClsQStateArrayViewSparse.cpp 
  FE/DialogManager/ClsFEDialogManager.cpp 
  FE/IQR/clo.cxx 
  FE/IQR/iqrMain.cpp 
  FE/IQR/iqrMainWindow.cpp 
  FE/IQR/ipcserver.cpp 
  FE/RTInfo/ClsQLogWindow.cpp 
  FE/SystemFile/ClsFESerializer.cpp
)

#  FE/DiagramManager/ClsPannerItem.cpp 



SET(appMOC_HDR
  FE/System/ClsFESystemManager.h 
  FE/Browser/ClsQSystemBrowser.h 
  FE/Browser/ClsQSysBrwsrFrame.h
  FE/DataManager/ClsFEConnectionDiagram.h 
  FE/DataManager/ClsFEDataManager.h 
  FE/DataManager/ClsFEDataSampler.h 
  FE/DataManager/ClsFEGroupPlot.h 
  FE/DataManager/ClsFEPlotFramework.h 
  FE/DataManager/ClsFETimePlot.h 
  FE/DataManager/ClsFEDataBroadcaster.h
  FE/DataManager/ClsQDivider.h 
  FE/DataManager/ClsQDragHandle.h 
  FE/DataManager/ClsQNeuronStateVariableDisplay.h 
  FE/DataManager/ClsQSynapseStateVariableDisplay.h 
  FE/DiagramManager/ClsBaseQStateArrayView.h 
  FE/DiagramManager/ClsBlockDiagram.h 
  FE/DiagramManager/ClsFEDiagramManager.h 
  FE/DiagramManager/ClsScene.h 
  FE/DiagramManager/ClsPanner.h 
  FE/DiagramManager/ClsPannerSceneView.h 
  FE/DiagramManager/ClsSceneView.h 
  FE/DiagramManager/ClsSubDiagram.h 
  FE/DiagramManager/ClsQGroupStateManip.h 
  FE/DiagramManager/ClsQSAList.h 
  FE/DiagramManager/ClsQStateArrayViewHex.h 
  FE/DiagramManager/ClsQStateArrayViewRect.h 
  FE/DiagramManager/ClsQStateArrayViewSparse.h 
  FE/DialogManager/ClsFEDialogManager.h 
  FE/IQR/iqrMainWindow.h 
  FE/IQR/ipcserver.h 
  FE/RTInfo/ClsQLogWindow.h 
  FE/Harbor/ClsFEParamRelais.h
)

#  FE/DiagramManager/ClsQConnection.h 
#  FE/DiagramManager/ClsPannerItem.h 


INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}
  Common 
  Common/Communication 
  Common/Helper 
  Common/Item 
  Common/Parser 
  Common/SystemFile 
  Common/Types/Arborizations 
  Common/Types/Modules 
  Common/Types/Neurons 
  Common/Types/Synapses 
  Common/Types/Topologies 
  FE/CommHandler 
  FE/DataManager 
  FE/IQR 
  FE/SettingsHandler 
  FE/System 
  FE/System/Connection/Arborization 
  FE/System/Connection/Function 
  FE/System/Connection/Pattern 
  FE/System/Topology 
  FE/SystemFile 
  FE/Browser 
  FE/DataManager/wGraphLib 
  FE/RTInfo 
  FE/IQR 
  FE/DialogManager 
  FE/DialogManager/AutoDialog/Dialogs 
  FE/DialogManager/AutoDialog/Widgets 
  FE/icons 
  FE/DiagramManager 
  FE/Harbor 
  ${QT_INCLUDE_DIR}
  ${QT_INCLUDE_DIR}/Qt3Support
  ${QT_INCLUDE_DIR}/QtGui
  ${QT_INCLUDE_DIR}/QtCore
  ${QT_INCLUDE_DIR}/QtXml
  ${QT_INCLUDE_DIR}/QtSvg
  ${XERCESC_INCLUDE_DIR}
  C:/pthreads.2/include
)

QT_WRAP_CPP(iqr appSRC ${appMOC_HDR} )


ADD_SUBDIRECTORY(Common/Helper               )#Common/Helper               )
ADD_SUBDIRECTORY(Common/Item                 )#Common/Item                 )
ADD_SUBDIRECTORY(Common/Parser               )#Common/Parser               )
ADD_SUBDIRECTORY(FE/Harbor                   )#FE/Harbor                   )
ADD_SUBDIRECTORY(FE/DataManager/wGraphLib    )#FE/DataManager/wGraphLib    )
ADD_SUBDIRECTORY(FE/DialogManager/AutoDialog )#FE/DialogManager/AutoDialog )
ADD_SUBDIRECTORY(FE/DialogManager/HelpDlgs   )#FE/DialogManager/HelpDlgs   )
ADD_SUBDIRECTORY(StandardTypes)


ADD_EXECUTABLE(iqr ${appSRC})

IF(UNIX)
TARGET_LINK_LIBRARIES(iqr
  IqrItem
  IqrHelper
  AutoDialog 
  SysFileParser 
  wGraph 
  IqrParamRelais 
  HelpDlgs
  ${QT_LIBRARIES}
  ${XERCESC_LIBRARIES}
)
ENDIF(UNIX)

IF(WIN32)
TARGET_LINK_LIBRARIES(iqr
  IqrItem
  IqrHelper
  AutoDialog 
  SysFileParser 
  wGraph 
  IqrParamRelais 
  HelpDlgs
  ${QT_LIBRARIES}
  ${XERCESC_LIBRARIES}
  pthreadGCE2
)
ENDIF(WIN32)





IF(UNIX)
  INSTALL(TARGETS iqr RUNTIME DESTINATION bin )

  IF(FULL_BIN)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/iqr.version                     DESTINATION lib/..)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr-logo.gif         DESTINATION lib/..)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr-logo-lg.gif      DESTINATION lib/..)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/doc/iqrUserManual.pdf           DESTINATION doc)

    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/INSTALL                     DESTINATION lib/..  COMPONENT applications)

    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libQt3Support.so.4 DESTINATION lib  COMPONENT applications)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libQtSvg.so.4      DESTINATION lib  COMPONENT applications)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libQtGui.so.4      DESTINATION lib  COMPONENT applications)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libQtXml.so.4      DESTINATION lib  COMPONENT applications)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libQtSql.so.4      DESTINATION lib  COMPONENT applications)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libQtNetwork.so.4  DESTINATION lib  COMPONENT applications)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libQtCore.so.4     DESTINATION lib  COMPONENT applications)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libxerces-c.so.28  DESTINATION lib  COMPONENT applications)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libv4l2.so.0       DESTINATION lib  COMPONENT applications)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libv4lconvert.so.0 DESTINATION lib  COMPONENT applications)
  ELSE(FULL_BIN)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/iqr.version                     DESTINATION lib/iqr)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr-logo.gif         DESTINATION lib/iqr)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr-logo-lg.gif      DESTINATION lib/iqr)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr.desktop          DESTINATION share/applications/)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr.xpm              DESTINATION share/icons/)
    INSTALL(FILES  ${PROJECT_SOURCE_DIR}/doc/iqrUserManual.pdf           DESTINATION share/doc/iqr)
  ENDIF(FULL_BIN)
ENDIF(UNIX)

IF(WIN32)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/iqr.version                         DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr-logo.gif             DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr-logo-lg.gif          DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr.ico                  DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/doc/iqrUserManual.pdf               DESTINATION doc     COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libxerces-c2_8_0.dll     DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/Qt3Support4.dll          DESTINATION lib/..  COMPONENT applications) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/QtCore4.dll		   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/QtGui4.dll		   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/QtNetwork4.dll	   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/QtSql4.dll		   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/QtSvg4.dll		   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/QtXml4.dll		   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libcv200.dll		   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libcvaux200.dll	   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libcxcore200.dll	   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libcxts200.dll	   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libhighgui200.dll	   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libml200.dll		   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/libopencv_ffmpeg200.dll  DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/mingwm10.dll		   DESTINATION lib/..  COMPONENT applications)
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/pthreadVC2.dll           DESTINATION lib/..  COMPONENT applications)

  INSTALL(TARGETS iqr                                                      RUNTIME DESTINATION lib/.. COMPONENT applications)

  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Helper/iqrUtils.h            DESTINATION include COMPONENT headers)     
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/boolParameter.hpp       DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/colorParameter.hpp      DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/doubleParameter.hpp     DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/indirectPtr.hpp         DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/intParameter.hpp        DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/item.hpp                DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/module.hpp              DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/moduleIcon.hpp          DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/namedPtr.hpp            DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/neuron.hpp              DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/optionsParameter.hpp    DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/parameter.hpp           DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/parameterList.hpp       DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/pathParameter.hpp       DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/pattern.hpp             DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/ringBuffer.hpp          DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/stateArray.hpp          DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/stateVariable.hpp       DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/stateVariableHolder.hpp DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/stateVariableList.hpp   DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/stringParameter.hpp     DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/synapse.hpp             DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/threadModule.hpp        DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/xRef.hpp                DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/xRefHolder.hpp          DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/Common/Item/xRefList.hpp            DESTINATION include COMPONENT headers) 
  INSTALL(FILES  ${PROJECT_SOURCE_DIR}/doc/iqrUserdefinedTypes.pdf         DESTINATION doc     COMPONENT headers)
ENDIF(WIN32)



SET_TARGET_PROPERTIES(iqr PROPERTIES VERSION ${IQR_MAJOR}-${IQR_REL})

IF(UNIX)
SET(CPACK_GENERATOR TGZ;RPM;DEB)

IF(FULL_BIN)
SET(CPACK_GENERATOR TGZ)
ELSE(FULL_BIN)
SET(CPACK_GENERATOR RPM;DEB)
ENDIF(FULL_BIN)


ENDIF(UNIX)

IF(WIN32)
SET(CPACK_GENERATOR NSIS)
ENDIF(WIN32)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "large-scale neuronal systems simulator")
SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_VERSION ${IQR_MAJOR}.${IQR_REL})

IF(NOT WIN32) 
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.${LINUX_DISTRO}.${CMAKE_SYSTEM_PROCESSOR}")
ENDIF(NOT WIN32) 

IF(WIN32) 
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_PROCESSOR}_setup")
ENDIF(WIN32) 

IF(FULL_BIN)
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_PROCESSOR}_fullbin")
ENDIF(FULL_BIN)


IF(UNIX)
  SET(CPACK_RESOURCE_FILE_README "README")
ENDIF(UNIX)

SET(CPACK_SOURCE_IGNORE_FILES "CMakeFiles;\\\\.tar\\\\.gz$;/_CPack_Packages/;CMakeCache;\\\\.o$;\\\\.so$;/CVS/;/\\\\.svn/;\\\\.swp$;\\\\.#;/#;.*~;cscope.*")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-src")

SET(CPACK_STRIP_FILES "bin/iqr")

SET(CPACK_BINARY_RPM "ON")
SET(CPACK_BINARY_STGZ "OFF")
SET(CPACK_BINARY_TBZ2 "OFF")
SET(CPACK_BINARY_TGZ "OFF")
SET(CPACK_BINARY_TZ "OFF")
SET(CPACK_SOURCE_TBZ2 "OFF")
SET(CPACK_SOURCE_TGZ "ON")
SET(CPACK_SOURCE_TZ "OFF")

SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/gpl-3.0.txt")



SET(CPACK_RPM_PACKAGE_DESCRIPTION_SUMMARY "large-scale neuronal systems simulator")
SET(CPACK_RPM_PACKAGE_GROUP "X11/KDE/Applications")
SET(CPACK_RPM_PACKAGE_LICENSE "GPL")	
SET(CPACK_RPM_PACKAGE_VENDOR "specs@upf")

SET(CPACK_RPM_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_PROCESSOR}")



SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ulysses Bernardet")
SET(CPACK_PACKAGE_CONTACT "bernuly@gmail.com")


#NEW STUFF FOR NSIS INSTALLER
set(CPACK_PACKAGE_INSTALL_DIRECTORY "iqr")

set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/inst_files/iqr.ico")

IF(WIN32)
#ist of pairs of executables and labels. Used by the NSIS generator to create Start Menu shortcuts.
set(CPACK_PACKAGE_EXECUTABLES "..\\\\iqr" "iqr" )
ENDIF(WIN32)

# Tell CPack all of the components to install. The "ALL"
# refers to the fact that this is the set of components that
# will be included when CPack is instructed to put everything
# into the binary installer (the default behavior).
set(CPACK_COMPONENTS_ALL applications types headers)

# Set the displayed names for each of the components to install.
# These will be displayed in the list of components inside the installer.
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "iqr Application")
set(CPACK_COMPONENT_TYPES_DISPLAY_NAME "Types")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "Headers")

# Provide descriptions for each of the components to install.
# When the user hovers the mouse over the name of a component, 
# the description will be shown in the "Description" box in the
# installer. If no descriptions are provided, the "Description" 
# box will be removed.
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "large-scale neuronal systems simulator")
set(CPACK_COMPONENT_TYPES_DESCRIPTION "iqr default Neurons, Synapses, and Modules")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION "C/C++ header files for writing iqr types")

# Put the components into two different groups: "Runtime"
set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set(CPACK_COMPONENT_TYPES_GROUP "Runtime")
set(CPACK_COMPONENT_HEADERS_GROUP "Development")


# It doesn't make sense to install the headers without the libraries
# (because you could never use the headers!), so make the headers component
# depend on the application component.
set(CPACK_COMPONENT_HEADERS_DEPENDS applications)


# Create two installation types with pre-selected components.
# The "Developer" installation has just the library,
# while the "Full" installation has everything.
set(CPACK_ALL_INSTALL_TYPES Full Developer)
set(CPACK_INSTALL_TYPE_FULL_DISPLAY_NAME "Everything")
set(CPACK_COMPONENT_TYPES_INSTALL_TYPES Developer Full)
set(CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer Full)
set(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Full)


INCLUDE(CPack)


