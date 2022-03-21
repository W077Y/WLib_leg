cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-CRC")
ENDIF()



add_library(WLib_CRC STATIC)

# Interface
target_include_directories(WLib_CRC PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/interface")
target_sources(WLib_CRC
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/interface/WLib_CRC_Interface.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/interface/WLib_CRC_8.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/interface/WLib_CRC_16_ccitt_false.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/interface/WLib_CRC_32.hpp"
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/interface/WLib_CRC_64_go_iso.hpp"
)

# Implementation
target_sources(WLib_CRC
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/implementation/crc_8.hpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/implementation/crc_8.cpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/implementation/crc_16_ccitt_false.hpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/implementation/crc_16_ccitt_false.cpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/implementation/crc_32.hpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/implementation/crc_32.cpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/implementation/crc_64_go_iso.hpp"
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/implementation/crc_64_go_iso.cpp"
)

 add_subdirectory("./test")
 add_subdirectory("./example")
