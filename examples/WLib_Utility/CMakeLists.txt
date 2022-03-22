cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Utility-Example")
ENDIF()

add_executable(EXAMPLE_WLib_Utility)
target_sources(EXAMPLE_WLib_Utility PRIVATE ${CMAKE_CURRENT_LIST_DIR}/exa_utility.cpp)

target_link_libraries(EXAMPLE_WLib_Utility PRIVATE WLib_Utility)
add_test(EXAMPLE_WLib_Utility EXAMPLE_WLib_Utility)
