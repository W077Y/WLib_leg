cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-StateMachine-Test")
ENDIF()

add_executable(WLib_StateMachine_TEST)

target_sources(WLib_StateMachine_TEST	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/tst_state_machine.cpp)


if(MSVC)
  target_compile_options(WLib_StateMachine_TEST PRIVATE /W4 /WX)
else()
  target_compile_options(WLib_StateMachine_TEST PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(WLib_StateMachine_TEST PRIVATE WLib_StateMachine)
target_link_libraries(WLib_StateMachine_TEST PRIVATE UT_CATCH)
add_test(WLib_StateMachine_TEST WLib_StateMachine_TEST)
