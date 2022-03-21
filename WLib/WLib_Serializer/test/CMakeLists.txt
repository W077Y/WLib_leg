cmake_minimum_required (VERSION 3.15)


IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Serializer-Test")
ENDIF()

add_executable(WLib_Serializer_TEST)

target_sources(WLib_Serializer_TEST PRIVATE ${CMAKE_CURRENT_LIST_DIR}/tst_serializer.cpp)

if(MSVC)
  target_compile_options(WLib_Serializer_TEST PRIVATE /W4 /WX)
else()
  target_compile_options(WLib_Serializer_TEST PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


target_link_libraries(WLib_Serializer_TEST PRIVATE WLib_Serializer)
target_link_libraries(WLib_Serializer_TEST PRIVATE UT_CATCH)
add_test(WLib_Serializer_TEST WLib_Serializer_TEST)
