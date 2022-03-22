cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-StateMachine-Example")
ENDIF()

add_executable(EXAMPLE_WLib_StateMachine)
target_sources(EXAMPLE_WLib_StateMachine
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/exa_StateMachine.cpp"
)



if(NOT MSVC)
	target_link_libraries(EXAMPLE_WLib_StateMachine	PRIVATE pthread)
endif()

target_link_libraries(EXAMPLE_WLib_StateMachine	PRIVATE WLib_StateMachine)


add_test(EXAMPLE_WLib_StateMachine EXAMPLE_WLib_StateMachine)
