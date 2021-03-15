bin_PROGRAMS = wmgmenu
wmgmenu_SOURCES = wmgmenu.c
dist_man_MANS = ${srcdir}/wmgmenu.1

AM_CFLAGS = $(GIO_CFLAGS) $(GMENU_CFLAGS) $(WUTIL_CFLAGS)
LIBS += $(GIO_LIBS) $(GMENU_LIBS) $(WUTIL_LIBS)

EXTRA_DIST = NEWS.md README.md wmgmenu.test

${srcdir}/wmgmenu.1: | wmgmenu
	help2man -N ./$| > $@

TESTS = tests/wmgmenu.test
