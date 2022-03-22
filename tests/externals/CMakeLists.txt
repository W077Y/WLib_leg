cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("externals")
ENDIF()

add_subdirectory(ut_catch)


