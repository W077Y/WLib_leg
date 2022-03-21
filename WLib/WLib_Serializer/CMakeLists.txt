cmake_minimum_required (VERSION 3.15)


IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Serializer")
ENDIF()

add_library(WLib_Serializer STATIC)

# Interface
target_include_directories(WLib_Serializer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/interface")
target_sources(WLib_Serializer
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/interface/WLib_Serializer.hpp"
)

# Implementation
target_sources(WLib_Serializer
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/implementation/WLib_Serializer.cpp"
)



 add_subdirectory("./test")
 add_subdirectory("./example")
