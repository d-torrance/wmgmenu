AC_INIT([wmgmenu], [0.1])
AC_CONFIG_SRCDIR([wmgmenu.c])
AM_INIT_AUTOMAKE([foreign serial-tests])
AC_PROG_CC
PKG_CHECK_MODULES([GIO], [gio-unix-2.0])
AC_ARG_WITH([cinnamon-menus],
	[AS_HELP_STRING([--with-cinnamon-menus],
	  [compile using cinnamon-menus instead of gnome-menus])],
	[],
	[with_cinnamon_menus=no])
AC_ARG_WITH([mate-menus],
	[AS_HELP_STRING([--with-mate-menus],
	  [compile using mate-menus instead of gnome-menus])],
	[],
	[with_mate_menus=no])
num_libs=0
for menu_lib in $with_cinnamon_menus $with_mate_menus
do
	AS_IF([test "x$menu_lib" != xno], [num_libs=$((num_libs + 1))])
done
AS_IF([test $num_libs -gt 1],
      [AC_MSG_ERROR([you must select at most one alternative to gnome-menus])])
AS_IF([test $num_libs -eq 0],
      [PKG_CHECK_MODULES([GMENU], [libgnome-menu-3.0])]
      [menu_library=\"gnome-menus\"])
AS_IF([test "x$with_cinnamon_menus" != xno],
      [PKG_CHECK_MODULES([GMENU], [libcinnamon-menu-3.0])]
      [menu_library=\"cinnamon-menus\"])
AS_IF([test "x$with_mate_menus" != xno],
      [PKG_CHECK_MODULES([GMENU], [libmate-menu])]
      [menu_library=\"mate-menus\"]
      [AC_DEFINE([USE_MATE_MENUS], [1], [Define if using mate-menus])])
AC_DEFINE_UNQUOTED([MENU_LIBRARY], [$menu_library],
			  [Menu library used to compile wmgmenu])
PKG_CHECK_MODULES([WUTIL], [WUtil])
AC_CONFIG_FILES([Makefile tests/wmgmenu.test],
		[chmod +x tests/wmgmenu.test])
AC_OUTPUT
