cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Utility-Example")
ENDIF()

add_executable(WLib_Utility_EXAMPLE)
target_sources(WLib_Utility_EXAMPLE PRIVATE ${CMAKE_CURRENT_LIST_DIR}/exa_utility.cpp)

target_link_libraries(WLib_Utility_EXAMPLE PRIVATE WLib_Utility)
add_test(WLib_Utility_EXAMPLE WLib_Utility_EXAMPLE)
