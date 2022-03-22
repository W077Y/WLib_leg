cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-StateMachine-Test")
ENDIF()

add_executable(TEST_WLib_StateMachine)

target_sources(TEST_WLib_StateMachine
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_main.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_state_machine.cpp"
)


if(MSVC)
  target_compile_options(TEST_WLib_StateMachine PRIVATE /W4 /WX)
else()
  target_compile_options(TEST_WLib_StateMachine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(TEST_WLib_StateMachine PRIVATE WLib_StateMachine)
target_link_libraries(TEST_WLib_StateMachine PRIVATE UT_CATCH)
add_test(TEST_WLib_StateMachine TEST_WLib_StateMachine)
