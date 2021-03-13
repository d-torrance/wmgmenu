bin_PROGRAMS = wmgmenu
wmgmenu_SOURCES = wmgmenu.c
dist_man_MANS = wmgmenu.1

AM_CFLAGS = $(GIO_CFLAGS) $(GMENU_CFLAGS) $(WUTIL_CFLAGS)
LIBS += $(GIO_LIBS) $(GMENU_LIBS) $(WUTIL_LIBS)

EXTRA_DIST = NEWS.md README.md

wmgmenu.1: wmgmenu
	help2man -N ./$< > $@
