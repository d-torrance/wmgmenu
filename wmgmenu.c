#define GMENU_I_KNOW_THIS_IS_UNSTABLE

#include <WINGs/WUtil.h>
#include <gio/gdesktopappinfo.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <gmenu-tree.h>
#include <stdio.h>
#include <stdlib.h>

WMPropList *entry_to_plist(GMenuTreeEntry *entry)
{
	GDesktopAppInfo *info;
	const char *name;
	char *cmd;

	info = gmenu_tree_entry_get_app_info(entry);
	name = g_app_info_get_display_name(G_APP_INFO(info));
	cmd = wstrdup("gtk-launch ");
	wstrappend(cmd, g_app_info_get_id(G_APP_INFO(info)));

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
		} /* TODO - GMENU_TREE_ITEM_ALIAS */
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
