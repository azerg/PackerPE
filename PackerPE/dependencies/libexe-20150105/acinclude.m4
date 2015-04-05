dnl Function to detect if libexe dependencies are available
AC_DEFUN([AX_LIBEXE_CHECK_LOCAL],
 [dnl Check for internationalization functions in libexe/libexe_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

