cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib_CRC-Test")
ENDIF()


add_executable(WLib_CRC_TEST)

target_sources(WLib_CRC_TEST
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_8.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_16_ccitt_false.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_32.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_64_go_iso.cpp"
)


if(MSVC)
  target_compile_options(WLib_CRC_TEST PRIVATE /W4 /WX)
else()
  target_compile_options(WLib_CRC_TEST PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(WLib_CRC_TEST PRIVATE WLib_CRC)
target_link_libraries(WLib_CRC_TEST PRIVATE UT_CATCH)
add_test(WLib_CRC_TEST WLib_CRC_TEST)
