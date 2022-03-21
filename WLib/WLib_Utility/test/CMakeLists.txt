cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Utility-Test")
ENDIF()

add_executable(WLib_Utility_TEST)

target_sources(WLib_Utility_TEST
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_utility.cpp"
)


if(MSVC)
  target_compile_options(WLib_Utility_TEST PRIVATE /W4 /WX)
else()
  target_compile_options(WLib_Utility_TEST PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


target_link_libraries(WLib_Utility_TEST PRIVATE WLib_Utility)
target_link_libraries(WLib_Utility_TEST PRIVATE UT_CATCH)
add_test(WLib_Utility_TEST WLib_Utility_TEST)