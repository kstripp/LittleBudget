dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(lb, 1.0.1, gabriel@teuton.org)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src])

dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

dnl Checks for libraries.
AC_ARG_ENABLE(qt,
	AC_HELP_STRING([--enable-qt],
	[Compile with the optional Qt 3 Frontend. (On by default)]),
	[case "${enableval}" in
	     yes) qt=true ;;
	     no) qt=false ;;
	     *) qt=true ;;
	esac],
	[qt=true])

if test x$qt = xtrue; then
   BNV_HAVE_QT
fi

if test x$qt$have_qt = xtrueno ; then
   AC_MSG_ERROR([Qt libraries not found.  To disable GUI use --disable-qt])
fi

dnl Checks for header files.

dnl Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

dnl Checks for library functions.

dnl Configureation options


AM_CONDITIONAL([WANT_QT], test x$qt$have_qt = xtrueyes)

AC_ARG_ENABLE(debug,
AC_HELP_STRING([--enable-debug],
	[Turn on debugging]),
[
	# Strip out compiler optimizations.
	dnl shamelessly stolen from the subversion source.
	# Source:  CollabNet (http://www.Collab.Net/) Subversion source code (/configure.in)
	CFLAGS=["`echo $CFLAGS' ' | sed -e 's/-O[^ ]* //g'`"]
	CXXFLAGS=["`echo $CXXFLAGS' ' | sed -e 's/-O[^ ]* //g'`"]
	CFLAGS="$CFLAGS -O0 -ggdb -Wall -fno-inline -fno-default-inline -fno-implicit-inline-templates"
	CXXFLAGS="$CXXFLAGS -O0 -ggdb -Wall -fno-inline -fno-default-inline -fno-implicit-inline-templates"
])

dnl This is a prototype for making the
dnl src/gui directory qmake-based.
dnl
dnl For the qmake project in src/gui
dnl AC_CONFIG_COMMANDS_PRE([
dnl	cd src/gui
dnl	qmake -o Makefile.in lbgui.pro
dnl	cd ../.. ])

dnl Output files:
AC_CONFIG_FILES([
	Makefile
	example/Makefile
	doc/Makefile
	src/Makefile
	src/test/Makefile
	src/core/Makefile
	src/gui/Makefile
	])
AC_OUTPUT
