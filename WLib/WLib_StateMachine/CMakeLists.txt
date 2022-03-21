cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-StateMachine")
ENDIF()

add_library(WLib_StateMachine STATIC)

#Interface
target_include_directories(WLib_StateMachine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/interface")
target_sources(WLib_StateMachine
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/interface/WLib_StateMachine.hpp"
)

# Implementation
target_sources(WLib_StateMachine
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/implementation/WLib_StateMachine.cpp"
)



add_subdirectory("./test")
add_subdirectory("./example")