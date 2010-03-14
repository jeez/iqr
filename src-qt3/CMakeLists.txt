PROJECT(iqr)

cmake_minimum_required(VERSION 2.6)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeScripts)
INCLUDE(getVersion)

INCLUDE(getLinuxDistro)

STRING(REGEX MATCHALL "[0-9]*\\.[0-9]" IQR_MAJOR "${iqrVersion}" )
MESSAGE( STATUS "MAJOR: " ${IQR_MAJOR} )

STRING(REGEX REPLACE "[0-9]*\\.[0-9]-" "" IQR_REL "${iqrVersion}" )
MESSAGE( STATUS "REL: " ${IQR_REL} )

SET(CMAKE_CXX_FLAGS 
"${CMAKE_CXX_FLAGS} -pipe -W -Wpointer-arith -Wall -Wuninitialized -fexceptions -fpermissive -fno-use-cxa-atexit -g3 -O2"
)                           




IF(NOT CMAKE_OSX_ARCHITECTURES)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -pthread")
ENDIF(NOT CMAKE_OSX_ARCHITECTURES)


ADD_DEFINITIONS(
  -DQT_THREAD_SUPPORT 
  -DQT_NO_DEBUG 
  -DLINUX 
  -D_REENTRANT 
  -DUSE_USER_TYPES
  -DDEBUG_IQRFE_CLSTYPEMANAGER 
  -DDEBUG_MODULEMANAGER 
  -DDEBUG_NEURONMANAGER 
  -DDEBUG_SYNAPSEMANAGER 
)

SET(CMAKE_VERBOSE_MAKEFILE TRUE)



SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt3 REQUIRED)
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
  FE/DataManager/ClsQBaseStateVariableDisplay.cpp 
  FE/DataManager/ClsQDivider.cpp 
  FE/DataManager/ClsQDragHandle.cpp 
  FE/DataManager/ClsQNeuronStateVariableDisplay.cpp 
  FE/DataManager/ClsQSynapseStateVariableDisplay.cpp 
  FE/DiagramManager/ClsBaseQStateArrayView.cpp 
  FE/DiagramManager/ClsBlockDiagram.cpp 
  FE/DiagramManager/ClsCanvasConnection.cpp 
  FE/DiagramManager/ClsCanvasHandle.cpp 
  FE/DiagramManager/ClsCanvasHandleEnd.cpp 
  FE/DiagramManager/ClsCanvasHandleStart.cpp 
  FE/DiagramManager/ClsCanvasNodeAP.cpp 
  FE/DiagramManager/ClsFEDiagramManager.cpp 
  FE/DiagramManager/ClsQCanvas.cpp 
  FE/DiagramManager/ClsQCanvasPanner.cpp 
  FE/DiagramManager/ClsQDiagramCanvasView.cpp 
  FE/DiagramManager/ClsQDiagramView.cpp 
  FE/DiagramManager/ClsQGroupStateManip.cpp 
  FE/DiagramManager/ClsQPanner.cpp 
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



SET(appMOC_HDR
  FE/System/ClsFESystemManager.h 
  FE/Browser/ClsQSystemBrowser.h 
  FE/DataManager/ClsFEConnectionDiagram.h 
  FE/DataManager/ClsFEDataManager.h 
  FE/DataManager/ClsFEDataSampler.h 
  FE/DataManager/ClsFEGroupPlot.h 
  FE/DataManager/ClsFEPlotFramework.h 
  FE/DataManager/ClsFETimePlot.h 
  FE/DataManager/ClsQDivider.h 
  FE/DataManager/ClsQDragHandle.h 
  FE/DataManager/ClsQNeuronStateVariableDisplay.h 
  FE/DataManager/ClsQSynapseStateVariableDisplay.h 
  FE/DiagramManager/ClsBaseQStateArrayView.h 
  FE/DiagramManager/ClsBlockDiagram.h 
  FE/DiagramManager/ClsFEDiagramManager.h 
  FE/DiagramManager/ClsQCanvasPanner.h 
  FE/DiagramManager/ClsQConnection.h 
  FE/DiagramManager/ClsQDiagramCanvasView.h 
  FE/DiagramManager/ClsQDiagramView.h 
  FE/DiagramManager/ClsQGroupStateManip.h 
  FE/DiagramManager/ClsQPanner.h 
  FE/DiagramManager/ClsQSAList.h 
  FE/DiagramManager/ClsQStateArrayViewHex.h 
  FE/DiagramManager/ClsQStateArrayViewRect.h 
  FE/DiagramManager/ClsQStateArrayViewSparse.h 
  FE/DialogManager/ClsFEDialogManager.h 
  FE/IQR/iqrMainWindow.h 
  FE/IQR/ipcserver.h 
  FE/IQR/ipcsocket.h 
  FE/RTInfo/ClsQLogWindow.h 
  FE/Harbor/ClsFEParamRelais.h
)

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
  ${XERCESC_INCLUDE_DIR}
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




INSTALL(FILES  ${PROJECT_SOURCE_DIR}/iqr.version                     DESTINATION lib/iqr)
INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr-logo.gif         DESTINATION lib/iqr)
INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr-logo-lg.gif      DESTINATION lib/iqr)
INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr.desktop          DESTINATION share/applications/)
INSTALL(FILES  ${PROJECT_SOURCE_DIR}/inst_files/iqr.xpm              DESTINATION share/icons/)
INSTALL(FILES  ${PROJECT_SOURCE_DIR}/../doc/iqrUserManual.pdf        DESTINATION share/doc/iqr)
INSTALL(TARGETS iqr RUNTIME DESTINATION bin )


#NOT NEEDED ANYMORE CONFIGURE_FILE(runCheckinstall.sh.in runCheckinstall.sh)


SET_TARGET_PROPERTIES(iqr PROPERTIES VERSION ${iqrVersion})




#INCLUDE(UseRPMTools)
#IF(RPMTools_FOUND)
#   RPMTools_ADD_RPM_TARGETS(iqr)
#ENDIF(RPMTools_FOUND)

SET(CPACK_GENERATOR RPM;DEB)

###NOT USED SET(PACKAGE_RELEASE ${IQR_REL}.${LINUX_DISTRO})
###NOT USED SET(PACKAGE_VERSION ${IQR_MAJOR})


SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "large-scale neuronal systems simulator")
SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_VERSION ${IQR_MAJOR}.${IQR_REL})
###--- SET(CPACK_PACKAGE_VERSION_MAJOR ${IQR_MAJOR})
###--- SET(CPACK_PACKAGE_VERSION_PATCH "${IQR_REL}")

SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.${LINUX_DISTRO}.${CMAKE_SYSTEM_PROCESSOR}")


SET(CPACK_RESOURCE_FILE_README "../../README")

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


SET(CPACK_RPM_PACKAGE_DESCRIPTION_SUMMARY "large-scale neuronal systems simulator")
SET(CPACK_RPM_PACKAGE_GROUP "X11/KDE/Applications")
SET(CPACK_RPM_PACKAGE_LICENSE "GPL")	
###NOT USED SET(CPACK_RPM_PACKAGE_NAME "${PROJECT_NAME}")
###NOT USED SET(CPACK_RPM_PACKAGE_RELEASE "${IQR_REL}.${LINUX_DISTRO}")
SET(CPACK_RPM_PACKAGE_VENDOR "specs@iua")
###NOT USED SET(CPACK_RPM_PACKAGE_VERSION ${IQR_MAJOR})

SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ulysses Bernardet")
SET(CPACK_PACKAGE_CONTACT "bernuly@gmail.com")

INCLUDE(CPack)

	



