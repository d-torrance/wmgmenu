/* wmgmenu - Window Maker menu using gnome-menus
 * Copyright (C) 2021 Doug Torrance
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <WINGs/WUtil.h>          /* for WMAddToPLArray, WMCreatePLString */
#include <gio/gdesktopappinfo.h>  /* for GDesktopAppInfo */
#include <gio/gio.h>              /* for g_app_info_get_display_name, ... */
#include <glib-object.h>          /* for g_object_unref */
#include <glib.h>                 /* for GError */
#ifdef USE_MATE_MENUS
# include "gnome-to-mate.h"
#else
# define GMENU_I_KNOW_THIS_IS_UNSTABLE
# include <gmenu-tree.h>           /* for gmenu_tree_item_unref, ... */
#endif
#include <stdio.h>                /* for NULL, printf */
#include <stdlib.h>               /* for exit, EXIT_FAILURE */

WMPropList *entry_to_plist(GMenuTreeEntry *entry)
{
	GDesktopAppInfo *info;
	const char *name;
	char *cmd;

	info = gmenu_tree_entry_get_app_info(entry);
	name = g_app_info_get_display_name(G_APP_INFO(info));
	cmd = wstrdup("gtk-launch ");
	cmd = wstrappend(cmd, g_app_info_get_id(G_APP_INFO(info)));

	return WMCreatePLArray(
		WMCreatePLString(name),
		WMCreatePLString("EXEC"),
		WMCreatePLString(cmd),
		NULL);
}

WMPropList *treedir_to_plist(GMenuTreeDirectory *directory)
{
	GMenuTreeIter *iter;
	GMenuTreeItemType type;
	WMPropList *plist;

	plist = WMCreatePLArray(
		WMCreatePLString(gmenu_tree_directory_get_name(directory)),
		NULL);

	iter = gmenu_tree_directory_iter(directory);
	for (type = gmenu_tree_iter_next(iter);
	     type != GMENU_TREE_ITEM_INVALID;
	     type = gmenu_tree_iter_next(iter)) {
		switch (type) {
		case GMENU_TREE_ITEM_ENTRY: {
			GMenuTreeEntry *entry;

			entry = gmenu_tree_iter_get_entry(iter);
			WMAddToPLArray(plist, entry_to_plist(entry));
			gmenu_tree_item_unref(entry);
			break;
		}
		case GMENU_TREE_ITEM_DIRECTORY: {
			GMenuTreeDirectory *subdir;

			subdir = gmenu_tree_iter_get_directory(iter);
			WMAddToPLArray(plist, treedir_to_plist(subdir));
			gmenu_tree_item_unref(subdir);
			break;
		}
		case GMENU_TREE_ITEM_ALIAS: {
			GMenuTreeAlias *alias;
			GMenuTreeItemType alias_type;

			alias = gmenu_tree_iter_get_alias(iter);
			alias_type =
				gmenu_tree_alias_get_aliased_item_type(alias);

			switch (alias_type) {
			case GMENU_TREE_ITEM_ENTRY: {
				GMenuTreeEntry *entry;

				entry = gmenu_tree_alias_get_aliased_entry(
					alias);
				WMAddToPLArray(plist, entry_to_plist(entry));
				gmenu_tree_item_unref(entry);
				break;
			}
			case GMENU_TREE_ITEM_DIRECTORY: {
				GMenuTreeDirectory *subdir;

				subdir = gmenu_tree_alias_get_aliased_directory(
					alias);
				WMAddToPLArray(plist, treedir_to_plist(subdir));
				gmenu_tree_item_unref(subdir);
				break;
			}
			default:
				break;
			}
			gmenu_tree_item_unref(alias);
			break;
		}
		default:
			break;
		}
	}

	return plist;
}

GMenuTree *gmenu_tree_new_maybe_for_path(const char *filename,
					 GMenuTreeFlags flags)
{
	char *basename;
	GMenuTree *tree;

	basename = g_path_get_basename(filename);
	if (g_strcmp0(filename, basename) == 0)
		tree = gmenu_tree_new(filename, flags);
	else
		tree = gmenu_tree_new_for_path(filename, flags);

	wfree(basename);
	return tree;
}

int main(int argc, char **argv)
{
	GError *error;
	GMenuTree *tree;
	GMenuTreeDirectory *root;
	GMenuTreeFlags flags;

	static char *filename = NULL;
	static gboolean version = FALSE, path = FALSE, library = FALSE,
		excluded = FALSE, nodisplay = FALSE, unallocated = FALSE,
		empty = FALSE, sort = FALSE;
	GOptionContext *context;
	static GOptionEntry entries[] = {
		{"version", 'v', 0, G_OPTION_ARG_NONE, &version,
		 "Print version number", NULL},
		{"filename", 'f', 0, G_OPTION_ARG_FILENAME, &filename,
		 "Specify menu file", NULL},
		{"path", 'p', 0, G_OPTION_ARG_NONE, &path,
		 "Print path to menu file", NULL},
		{"library", 'l', 0, G_OPTION_ARG_NONE, &library,
		 "Print menu library that wmgmenu is using", NULL},
		{"include-excluded", 'x', 0, G_OPTION_ARG_NONE, &excluded,
		 "Include excluded entries", NULL},
		{"include-nodisplay", 'n', 0, G_OPTION_ARG_NONE, &nodisplay,
		 "Include entries marked 'NoDisplay'", NULL},
		{"include-unallocated", 'u', 0, G_OPTION_ARG_NONE, &unallocated,
		 "Include unallocated entries", NULL},
		{"show-empty", 'e', 0, G_OPTION_ARG_NONE, &empty,
		 "Show empty directories", NULL},
		{"sort-display-name", 's', 0, G_OPTION_ARG_NONE, &sort,
		 "Sort by display name", NULL}
	};

	context = g_option_context_new("");
	g_option_context_set_description(
		context,
		"Window Maker menu that follows the freedesktop.org Desktop "
		"Menu Specification");
	g_option_context_add_main_entries(context, entries, NULL);

	error = NULL;
	if (!g_option_context_parse(context, &argc, &argv, &error)) {
		werror("option parsing failed: %s\n", error->message);
		exit(EXIT_FAILURE);
	}

	if (version) {
		printf(PACKAGE_STRING"\n");
		exit(EXIT_SUCCESS);
	}

	if (library) {
		printf(MENU_LIBRARY"\n");
		exit(EXIT_SUCCESS);
	}

	if (!filename) {
		/* the default menu filename varies depending on the library:
		   - cinnamon-menus and the debian package of gnome-menus
		     prepend "gnome-" if XDG_MENU_PREFIX isn't defined, so
		     we do that ahead of time just in case a non-debian
		     gnome-menus is being used.  if XDG_MENU_PREFIX is defined,
		     then we just leave it as applications.menu and let the
		     library add the prefix for us
		   - mate-menus is different and expects "mate-" in front
		     in both cases.  if XDG_MENU_PREFIX is defined, then
		     the library will swap the prefix for us. */
#ifdef USE_MATE_MENUS
		filename = wstrdup("mate-applications.menu");
#else
		if (!getenv("XDG_MENU_PREFIX"))
			filename = wstrdup("gnome-");
		else
			filename = wstrdup("");
		filename = wstrappend(filename, "applications.menu");
#endif
	}

	flags = GMENU_TREE_FLAGS_NONE;
	if (excluded)
		flags |= GMENU_TREE_FLAGS_INCLUDE_EXCLUDED;
	if (nodisplay)
		flags |= GMENU_TREE_FLAGS_INCLUDE_NODISPLAY;
	if (unallocated)
		flags |= GMENU_TREE_FLAGS_INCLUDE_UNALLOCATED;
	if (empty)
		flags |= GMENU_TREE_FLAGS_SHOW_EMPTY;
	if (sort)
		flags |= GMENU_TREE_FLAGS_SORT_DISPLAY_NAME;

	tree = gmenu_tree_new_maybe_for_path(filename, flags);
	wfree(filename);

	error = NULL;
	if (!gmenu_tree_load_sync(tree, &error)) {
		werror("%s", error->message);
		exit(EXIT_FAILURE);
	}

	if (path) {
		printf("%s\n", gmenu_tree_get_canonical_menu_path(tree));
		exit(EXIT_SUCCESS);
	}

	root = gmenu_tree_get_root_directory(tree);
	if (root) {
		printf("%s\n",
		       WMGetPropListDescription(treedir_to_plist(root), TRUE));
		gmenu_tree_item_unref(root);
	}

	g_object_unref(tree);

	return 0;
}
