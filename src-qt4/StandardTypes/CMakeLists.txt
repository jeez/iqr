SET(CMAKE_INSTALL_PREFIX /opt/iqr)
SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_SUBDIRECTORY(Neurons)

ADD_SUBDIRECTORY(Synapses)

IF(NOT WIN32)
  ADD_SUBDIRECTORY(Modules/SonyPanTilt/)
  ADD_SUBDIRECTORY(Modules/Robots/LegoMindStorm/)
  IF(NOT CMAKE_OSX_ARCHITECTURES) # most hardware related modules do not work wit Darwin...
    ADD_SUBDIRECTORY(Modules/Joystick/)
    ADD_SUBDIRECTORY(Modules/Robots/KRobots/Khepera/)
#    ADD_SUBDIRECTORY(Modules/Robots/KRobots/EPuck/)
    ADD_SUBDIRECTORY(Modules/Robots/KRobots/EPuck_qextserialport/)

#    ADD_SUBDIRECTORY(Modules/Robots/KRobots/KoalaW/)

    ADD_SUBDIRECTORY(Modules/Video/video)
    ADD_SUBDIRECTORY(Modules/openCVmpeg)

#    ADD_SUBDIRECTORY(Modules/Video/bttvVideo)
#    ADD_SUBDIRECTORY(Modules/Video/bttvVideoColor)
#    ADD_SUBDIRECTORY(Modules/Video/USBVideo)
#    ADD_SUBDIRECTORY(Modules/Video/USBVideoColor)
#    ADD_SUBDIRECTORY(Modules/Video/USBVideoOV511-new)
#    ADD_SUBDIRECTORY(Modules/Video/USBVideoPT)
#    ADD_SUBDIRECTORY(Modules/Video/USBVideoPTColor)
#    ADD_SUBDIRECTORY(Modules/Video/USBVideoQC)
#    ADD_SUBDIRECTORY(Modules/Video/USBVideoQCColor)
#    ADD_SUBDIRECTORY(Modules/Video/v4l2video)
#    ADD_SUBDIRECTORY(Modules/Video/video)

  ENDIF(NOT CMAKE_OSX_ARCHITECTURES)
ENDIF(NOT WIN32)


IF(WIN32)
  ADD_SUBDIRECTORY(Modules/Video/openCVvideo)
  ADD_SUBDIRECTORY(Modules/Joystick/)
  ADD_SUBDIRECTORY(Modules/openCVmpeg)
  ADD_SUBDIRECTORY(Modules/Robots/KRobots/EPuck_qextserialport/)
ENDIF(WIN32)
