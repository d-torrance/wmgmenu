#define MATEMENU_I_KNOW_THIS_IS_UNSTABLE
#include <matemenu-tree.h>
#define GMenuTree          MateMenuTree
#define GMenuTreeEntry     MateMenuTreeEntry
#define GMenuTreeDirectory MateMenuTreeDirectory
#define GMenuTreeAlias     MateMenuTreeAlias
#define GMenuTreeIter      MateMenuTreeIter
#define GMenuTreeItemType  MateMenuTreeItemType
#define GMenuTreeFlags     MateMenuTreeFlags
#define gmenu_tree_new                matemenu_tree_new
#define gmenu_tree_new_for_path       matemenu_tree_new_for_path
#define gmenu_tree_load_sync          matemenu_tree_load_sync
#define gmenu_tree_get_root_directory matemenu_tree_get_root_directory
#define gmenu_tree_entry_get_app_info matemenu_tree_entry_get_app_info
#define gmenu_tree_directory_get_name matemenu_tree_directory_get_name
#define gmenu_tree_directory_iter     matemenu_tree_directory_iter
#define gmenu_tree_alias_get_aliased_item_type \
	matemenu_tree_alias_get_aliased_item_type
#define gmenu_tree_alias_get_aliased_entry     \
	matemenu_tree_alias_get_aliased_entry
#define gmenu_tree_alias_get_aliased_directory \
	matemenu_tree_alias_get_aliased_directory
#define gmenu_tree_iter_next          matemenu_tree_iter_next
#define gmenu_tree_iter_get_entry     matemenu_tree_iter_get_entry
#define gmenu_tree_iter_get_directory matemenu_tree_iter_get_directory
#define gmenu_tree_iter_get_alias     matemenu_tree_iter_get_alias
#define gmenu_tree_iter_unref         matemenu_tree_iter_unref
#define gmenu_tree_item_unref         matemenu_tree_item_unref
#define GMENU_TREE_FLAGS_NONE     MATEMENU_TREE_FLAGS_NONE
#define GMENU_TREE_ITEM_INVALID   MATEMENU_TREE_ITEM_INVALID
#define GMENU_TREE_ITEM_ENTRY     MATEMENU_TREE_ITEM_ENTRY
#define GMENU_TREE_ITEM_DIRECTORY MATEMENU_TREE_ITEM_DIRECTORY
#define GMENU_TREE_ITEM_ALIAS     MATEMENU_TREE_ITEM_ALIAS
