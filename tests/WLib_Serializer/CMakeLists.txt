cmake_minimum_required (VERSION 3.15)


IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Serializer-Test")
ENDIF()

add_executable(TEST_WLib_Serializer)

target_sources(TEST_WLib_Serializer
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_main.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_serializer.cpp"
)

if(MSVC)
  target_compile_options(TEST_WLib_Serializer PRIVATE /W4 /WX)
else()
  target_compile_options(TEST_WLib_Serializer PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


target_link_libraries(TEST_WLib_Serializer PRIVATE UT_CATCH)
target_link_libraries(TEST_WLib_Serializer PRIVATE WLib_Serializer)
add_test(TEST_WLib_Serializer TEST_WLib_Serializer)
