cmake_minimum_required (VERSION 3.15)


IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-CRC-Example")
ENDIF()

add_executable(EXAMPLE_WLib_CRC)

target_sources(EXAMPLE_WLib_CRC
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/exa_crc.cpp"
)

target_link_libraries(EXAMPLE_WLib_CRC PRIVATE WLib_CRC)
add_test(EXAMPLE_WLib_CRC EXAMPLE_WLib_CRC)
