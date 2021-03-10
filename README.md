![workflow badge](https://github.com/d-torrance/wmgmenu/actions/workflows/build.yml/badge.svg)

wmgmenu
=======

**wmgmenu** is a small application that uses
[gnome-menus](https://gitlab.gnome.org/GNOME/gnome-menus) to produce a
proplist menu for Window Maker that follows the
[freedesktop.org Desktop Menu Specification](
https://specifications.freedesktop.org/menu-spec/latest/).

Usage
-----

To use wmgmenu, edit `~/GNUstep/Default/WMRootMenu` to include a line
such as:

```
("freedesktop.org menu", OPEN_PLMENU, "|| wmgmenu"),
```

Alternatively, using WPrefs, navigate to "Applications Menu
Definition", drag "Generated PL Menu" to the menu that pops up, enter
"wmgmenu" under "Command", and then click "Save".

Note that `gtk-launch` is used to launch each application from the menu.
