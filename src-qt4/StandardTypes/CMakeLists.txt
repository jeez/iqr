SET(CMAKE_INSTALL_PREFIX /opt/iqr)
SET(CMAKE_VERBOSE_MAKEFILE TRUE)

ADD_SUBDIRECTORY(Neurons)

ADD_SUBDIRECTORY(Synapses)

IF(NOT WIN32)
  ADD_SUBDIRECTORY(Modules/SonyPanTilt/)
  ADD_SUBDIRECTORY(Modules/Robots/LegoMindStorm/)
  ADD_SUBDIRECTORY(Modules/Robots/KRobots/EPuckAdvanced/)
  ADD_SUBDIRECTORY(Modules/Robots/KRobots/EPuck_qextserialport/)
  ADD_SUBDIRECTORY(Modules/openCVmpeg)
  ADD_SUBDIRECTORY(Modules/Video/openCVvideo)

  IF(NOT APPLE) # most hardware related modules do not work wit Darwin...
    ADD_SUBDIRECTORY(Modules/Video/video)
    ADD_SUBDIRECTORY(Modules/Joystick/)
    ADD_SUBDIRECTORY(Modules/Robots/KRobots/Khepera/)
  ENDIF(NOT APPLE)
ENDIF(NOT WIN32)

IF(WIN32)
  ADD_SUBDIRECTORY(Modules/Video/openCVvideo)
  ADD_SUBDIRECTORY(Modules/Joystick/)
  ADD_SUBDIRECTORY(Modules/openCVmpeg)
  ADD_SUBDIRECTORY(Modules/Robots/KRobots/EPuck_qextserialport/)
ENDIF(WIN32)
