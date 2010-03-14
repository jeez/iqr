 

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DLINUX -DNO_DEBUG")

IF(UNIX)
  SET (libSrc
    libraryLoader_unix.cpp
    pathFunctions_unix.cpp
    typeManager.cpp)

  ADD_DEFINITIONS(-DLINUX) 
ENDIF(UNIX)

IF(WIN32)
  SET (libSrc
    libraryLoader_windows.cpp
    pathFunctions_windows.cpp
    typeManager.cpp)

  ADD_DEFINITIONS(-D_WINDOWS) 
ENDIF(WIN32)



SET(CMAKE_INSTALL_PREFIX /usr/lib)
ADD_LIBRARY(IqrHelper SHARED ${libSrc})

IF(UNIX)
  INSTALL(TARGETS IqrHelper
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
  )
ENDIF(UNIX)

IF(WIN32)
  INSTALL(TARGETS IqrHelper
    LIBRARY DESTINATION lib/.. 
    ARCHIVE DESTINATION lib/.. 
    RUNTIME DESTINATION bin/.. COMPONENT applications
  )
ENDIF(WIN32)
