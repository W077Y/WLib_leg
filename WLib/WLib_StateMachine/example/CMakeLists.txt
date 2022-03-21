cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-StateMachine-Example")
ENDIF()

add_executable(WLib_StateMachine_EXAMPLE)
target_sources(WLib_StateMachine_EXAMPLE
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/exa_StateMachine.cpp"
)



if(NOT MSVC)
	target_link_libraries(WLib_StateMachine_EXAMPLE	PRIVATE pthread)
endif()

target_link_libraries(WLib_StateMachine_EXAMPLE	PRIVATE WLib_StateMachine)


add_test(WLib_StateMachine_EXAMPLE WLib_StateMachine_EXAMPLE)
