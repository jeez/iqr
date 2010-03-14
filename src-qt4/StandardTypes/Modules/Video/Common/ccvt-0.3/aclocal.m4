dnl Check for Intel architecture
AC_DEFUN(AC_CHECK_INTELCPU,
[
  AC_CACHE_CHECK(for Intel processor(s), ac_cv_flag_intelcpu, [
  if [ uname -m | grep -q "i.86" ]; then
    ac_cv_flag_intelcpu=yes
  else
    ac_cv_flag_intelcpu=no
  fi
  ])
  
  if test $ac_cv_flag_intelcpu = "yes"; then
    AC_DEFINE(HAVE_INTELCPU)
    AFLAGS="-DHAVE_INTELCPU $AFLAGS"
  fi
])


AC_DEFUN(AC_CHECK_MMX,
[
  AC_MSG_CHECKING(if processor supports MMX instructions);
  AC_ARG_ENABLE(mmx, 
    [  --disable-mmx           disable use of MMX instructions [default=enabled]],
    [
      if test $enableval = "no"; then
        ac_cv_flag_mmx="no"
      else
        ac_cv_flag_mmx="yes"
      fi
    ], [ac_cv_flag_mmx="yes"])

  if test $ac_cv_flag_intelcpu = "yes"; then
    if test $ac_cv_flag_mmx = "yes" ; then
      if grep "^flags.* mmx" /proc/cpuinfo > /dev/null; then
        ac_cv_flag_mmx=yes
      else
        ac_cv_flag_mmx=no
      fi
    fi
  fi

  if test $ac_cv_flag_mmx = "yes"; then
    AC_DEFINE(HAVE_MMX)
    AFLAGS="-DHAVE_MMX $AFLAGS"
  fi
  
  AC_MSG_RESULT($ac_cv_flag_mmx)
])

