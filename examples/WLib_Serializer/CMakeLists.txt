cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Serializer-Example")
ENDIF()

add_executable(EXAMPLE_WLib_Serializer)
target_sources(EXAMPLE_WLib_Serializer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/exa_serializer.cpp)


target_link_libraries(EXAMPLE_WLib_Serializer PRIVATE WLib_Serializer)

add_test(EXAMPLE_WLib_Serializer EXAMPLE_WLib_Serializer)
