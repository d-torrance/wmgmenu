[![license badge](https://img.shields.io/github/license/d-torrance/wmgmenu)](
https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![workflow badge](https://github.com/d-torrance/wmgmenu/actions/workflows/build.yml/badge.svg)](
https://github.com/d-torrance/wmgmenu/actions)
[![release badge](https://img.shields.io/github/v/release/d-torrance/wmgmenu)](
https://github.com/d-torrance/wmgmenu/releases)

wmgmenu
=======

**wmgmenu** is a small application that uses
[gnome-menus](https://gitlab.gnome.org/GNOME/gnome-menus) (or one of its various forks) to produce a proplist menu for [Window Maker](
https://www.windowmaker.org/) that follows the
[freedesktop.org Desktop Menu Specification](
https://specifications.freedesktop.org/menu-spec/latest/).

Usage
-----

To use wmgmenu, edit `~/GNUstep/Default/WMRootMenu` to include a line
such as:

```
(Applications, OPEN_PLMENU, "|| wmgmenu"),
```

Alternatively, using WPrefs, navigate to "Applications Menu
Definition", drag "Generated PL Menu" to the menu that pops up, enter
"wmgmenu" under "Command", and then click "Save".

Note that `gtk-launch` is used to launch each application from the menu.

By default, wmgmenu will load `${XDG_MENU_PREFIX}applications.menu`.
If you would like to load another file, then you may specify it
with the `-f` or `--filename` commandline arguments, e.g.,

```
wmgmenu -f /etc/xdg/menus/mate-settings.menu
```

If the file exists in the `menus` subdirectory of the user or system
configuration directories (by default, `~/.config` or `/etc/xdg` and
possibly given by the `XDG_CONFIG_HOME` and `XDG_CONFIG_DIRS`
environment variables -- see the [XDG Base Directory Specification](
https://specifications.freedesktop.org/basedir-spec/latest/) for more
information), then a full path need not be given.  For example:

```
wmgmenu -f mate-settings.menu
```

There are also a number of commandline options that set flags used
by gnome-menus to determine how it decides which entries to include in
the menu and in what order.

* `-x`, `--include-excluded`: Include excluded entries.
* `-n`, `--include-nodisplay`: Include entries marked 'NoDisplay'.
* `-u`, `--include-unallocated`: Include unallocated entries.
* `-e`, `--show-empty`: Show empty directories.
* `-s`, `--sort-display-name`: By default, gnome-menus determines
  how to sort an entry based on the `Name` given in its `.desktop`
  file.  With this flag set, it uses [`X-GNOME-FullName`](
  https://wiki.gnome.org/Initiatives/GnomeGoals/CorrectDesktopFiles)
  instead, falling back to `Name` if this key doesn't exist.  Note
  that `X-GNOME-FullName` is not very common, and setting this flag
  will likely have little to no effect.

Installing
----------
wmgmenu is available in Ubuntu using a PPA:

```
sudo add-apt-repository ppa:profzoom/dockapps
sudo apt install wmgmenu
```

Building
--------

*Prerequisites:*

* [GLib](https://wiki.gnome.org/Projects/GLib)
* [gnome-menus](https://gitlab.gnome.org/GNOME/gnome-menus)
* [WUtil](https://www.windowmaker.org/)

If building from a tarball from the [releases page](
https://github.com/d-torrance/wmgmenu/releases), then unpack it, change
to the corresponding directory, and run:

```
./configure
make
sudo make install
```

If building from a git repository, first run

```
./autogen.sh
```

and then proceed as above.

Alternatively, wmgmenu can be built using [cinnamon-menus](
https://github.com/linuxmint/cinnamon-menus) or [mate-menus](
https://github.com/mate-desktop/mate-menus) instead of gnome-menus.  To do
this, start with

```
./configure --with-cinnamon-menus
```

or

```
./configure --with-mate-menus
```

and then proceed as above.

Test suite
----------

The test suite uses [Bats](https://github.com/bats-core/bats-core).
It expects to find some menu files on the system:

* a default menu file (`${XDG_MENU_PREFIX}applications.menu`),
* `/etc/xdg/menus/gnome-applications.menu`, for testing the `-f`
  commandline option, and
* `e-applications.menu` (the [Enlightment](https://www.enlightenment.org/)
  menu), for testing `XDG_MENU_PREFIX` support with a non-default value.

To run the test suite:

```
make check
```

Bug reports
-----------
Found a bug?  Open an issue at https://github.com/d-torrance/wmgmenu.
