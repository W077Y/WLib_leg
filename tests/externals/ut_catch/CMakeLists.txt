cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("Catch2")
ENDIF()

add_library(UT_CATCH STATIC)

target_include_directories(UT_CATCH PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_sources(UT_CATCH 
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/inc/catch.hpp"
)

target_sources(UT_CATCH
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/inc/ut_catch.hpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/ut_catch.cpp"
)


