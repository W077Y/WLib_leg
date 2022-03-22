cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-SPI-Abstraction")
ENDIF()

add_library(WLib_SPI_Abstraction STATIC)


# Interface
target_include_directories(WLib_SPI_Abstraction PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/interface")
target_sources(WLib_SPI_Abstraction
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/interface/WLib_SPI_Abstraction.hpp"
)

# Implementation
target_sources(WLib_SPI_Abstraction
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/implementation/WLib_SPI_Abstraction.cpp"
)

target_compile_features(WLib_SPI_Abstraction PUBLIC cxx_std_17)

target_link_libraries(WLib_SPI_Abstraction PUBLIC WLib_Utility)

