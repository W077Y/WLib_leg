cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("FRAM")
ENDIF()

add_subdirectory("./MB85RS64V")
