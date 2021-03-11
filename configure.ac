AC_INIT([wmgmenu], [0.1])
AC_CONFIG_SRCDIR([wmgmenu.c])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC
PKG_CHECK_MODULES([GIO], [gio-unix-2.0])
AC_ARG_WITH([cinnamon-menus],
	[AS_HELP_STRING([--with-cinnamon-menus],
	  [compile using cinnamon-menus instead of gnome-menus])],
	[],
	[with_cinnamon_menus=no])
AS_IF([test "x$with_cinnamon_menus" != xno],
	[PKG_CHECK_MODULES([GMENU], [libcinnamon-menu-3.0])],
	[PKG_CHECK_MODULES([GMENU], [libgnome-menu-3.0])])
PKG_CHECK_MODULES([WUTIL], [WUtil])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
