cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib_Driver_MB85RS64V-Test")
ENDIF()


add_executable(TEST_WLib_DRIVER_MB85RS64V)

target_sources(TEST_WLib_DRIVER_MB85RS64V
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_main.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_Driver_MB85RS64V.cpp"
)


if(MSVC)
  target_compile_options(TEST_WLib_DRIVER_MB85RS64V PRIVATE /W4 /WX)
else()
  target_compile_options(TEST_WLib_DRIVER_MB85RS64V PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(TEST_WLib_DRIVER_MB85RS64V PRIVATE WLib_DRIVER_MB85RS64V)
target_link_libraries(TEST_WLib_DRIVER_MB85RS64V PRIVATE UT_CATCH)
add_test(TEST_WLib_DRIVER_MB85RS64V TEST_WLib_DRIVER_MB85RS64V)
