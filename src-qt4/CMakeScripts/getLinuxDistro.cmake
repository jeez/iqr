EXEC_PROGRAM(bash
  ARGS "extractLinuxDistroInfo.sh"
  OUTPUT_VARIABLE LINUX_DISTRO
)

MESSAGE( STATUS "Linux Distribution: " ${LINUX_DISTRO} )
