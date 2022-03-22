cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Utility")
ENDIF()


add_library(WLib_Utility STATIC)

# Interface
target_include_directories(WLib_Utility PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/interface")
target_sources(WLib_Utility
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/interface/WLib_Utility.hpp"
)


# Implementation
target_sources(WLib_Utility
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/implementation/WLib_Utility.cpp"
)


target_compile_features(WLib_Utility PUBLIC cxx_std_17)
