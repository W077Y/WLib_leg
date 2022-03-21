cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Serializer-Example")
ENDIF()

add_executable(WLib_Serializer_EXAMPLE)
target_sources(WLib_Serializer_EXAMPLE PRIVATE ${CMAKE_CURRENT_LIST_DIR}/exa_serializer.cpp)


target_link_libraries(WLib_Serializer_EXAMPLE PRIVATE WLib_Serializer)

add_test(WLib_Serializer_EXAMPLE WLib_Serializer_EXAMPLE)
