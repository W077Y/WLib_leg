cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Utility-Test")
ENDIF()

add_executable(TEST_WLib_Utility)

target_sources(TEST_WLib_Utility
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_main.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_utility.cpp"
)


if(MSVC)
  target_compile_options(TEST_WLib_Utility PRIVATE /W4 /WX)
else()
  target_compile_options(TEST_WLib_Utility PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


target_link_libraries(TEST_WLib_Utility PRIVATE WLib_Utility)
target_link_libraries(TEST_WLib_Utility PRIVATE UT_CATCH)
add_test(TEST_WLib_Utility TEST_WLib_Utility)