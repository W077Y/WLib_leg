cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib_CRC-Test")
ENDIF()


add_executable(TEST_WLib_CRC)

target_sources(TEST_WLib_CRC
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_main.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_8.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_16_ccitt_false.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_32.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_crc_64_go_iso.cpp"
)


if(MSVC)
  target_compile_options(TEST_WLib_CRC PRIVATE /W4 /WX)
else()
  target_compile_options(TEST_WLib_CRC PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(TEST_WLib_CRC PRIVATE WLib_CRC)
target_link_libraries(TEST_WLib_CRC PRIVATE UT_CATCH)
add_test(TEST_WLib_CRC TEST_WLib_CRC)
