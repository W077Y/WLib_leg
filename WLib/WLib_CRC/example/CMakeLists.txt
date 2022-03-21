cmake_minimum_required (VERSION 3.15)


IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-CRC-Example")
ENDIF()

add_executable(WLib_CRC_EXAMPLE)

target_sources(WLib_CRC_EXAMPLE
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/exa_crc.cpp"
)

target_link_libraries(WLib_CRC_EXAMPLE PRIVATE WLib_CRC)
add_test(WLib_CRC_EXAMPLE WLib_CRC_EXAMPLE)
