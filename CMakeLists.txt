cmake_minimum_required (VERSION 3.15)


FUNCTION(MESSAGE_TREEVIEW VAL)
IF (NOT DEFINED sub_dir_tree_val) 
	message(STATUS "${VAL}")
	SET(sub_dir_tree_val "+-> " PARENT_SCOPE)
ELSE()
	message(STATUS "${sub_dir_tree_val}${VAL}")
	SET(sub_dir_tree_val "|   ${sub_dir_tree_val}" PARENT_SCOPE)
ENDIF()
ENDFUNCTION()



MESSAGE_TREEVIEW("WLib Project")
project(WLib LANGUAGES CXX)

enable_testing()
add_subdirectory("./WLib")
add_subdirectory("./examples")
add_subdirectory("./tests")




