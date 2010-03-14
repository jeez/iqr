EXEC_PROGRAM(bash
  ARGS "../distro/scripts/extractLinuxDistroInfo.sh"
  OUTPUT_VARIABLE LINUX_DISTRO
)

MESSAGE( STATUS "Linux Distribution: " ${LINUX_DISTRO} )