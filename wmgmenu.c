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

#define GMENU_I_KNOW_THIS_IS_UNSTABLE

#include <WINGs/WUtil.h>          /* for WMAddToPLArray, WMCreatePLString */
#include <X11/Xlib.h>             /* for True */
#include <gio/gdesktopappinfo.h>  /* for GDesktopAppInfo */
#include <gio/gio.h>              /* for g_app_info_get_display_name, ... */
#include <glib-object.h>          /* for g_object_unref */
#include <glib.h>                 /* for GError */
#include <gmenu-tree.h>           /* for gmenu_tree_item_unref, ... */
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
		if (type == GMENU_TREE_ITEM_ENTRY) {
			GMenuTreeEntry *entry;

			entry = gmenu_tree_iter_get_entry(iter);
			WMAddToPLArray(plist, entry_to_plist(entry));
			gmenu_tree_item_unref(entry);
		} else if (type == GMENU_TREE_ITEM_DIRECTORY) {
			GMenuTreeDirectory *subdir;

			subdir = gmenu_tree_iter_get_directory(iter);
			WMAddToPLArray(plist, treedir_to_plist(subdir));
			gmenu_tree_item_unref(subdir);
		} else if (type == GMENU_TREE_ITEM_ALIAS) {
			GMenuTreeAlias *alias;
			GMenuTreeItemType alias_type;

			alias = gmenu_tree_iter_get_alias(iter);
			alias_type =
				gmenu_tree_alias_get_aliased_item_type(alias);
			if (alias_type == GMENU_TREE_ITEM_ENTRY) {
				GMenuTreeEntry *entry;

				entry = gmenu_tree_alias_get_aliased_entry(
					alias);
				WMAddToPLArray(plist, entry_to_plist(entry));
				gmenu_tree_item_unref(entry);
			} else if (alias_type == GMENU_TREE_ITEM_DIRECTORY) {
				GMenuTreeDirectory *subdir;

				subdir = gmenu_tree_alias_get_aliased_directory(
					alias);
				WMAddToPLArray(plist, treedir_to_plist(subdir));
				gmenu_tree_item_unref(subdir);
			}
			gmenu_tree_item_unref(alias);
		}
	}

	return plist;
}

int main(int argc, char **argv)
{
	GError *error;
	GMenuTree *tree;
	GMenuTreeDirectory *root;

	tree = gmenu_tree_new("applications.menu", GMENU_TREE_FLAGS_NONE);

	error = NULL;
	if (!gmenu_tree_load_sync(tree, &error)) {
		werror("%s", error->message);
		exit(EXIT_FAILURE);
	}

	root = gmenu_tree_get_root_directory(tree);
	if (root) {
		printf("%s\n", WMGetPropListDescription(treedir_to_plist(root),
							True));
		gmenu_tree_item_unref(root);
	}

	g_object_unref(tree);

	return 0;
}
