cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-SPI-Abstraction")
ENDIF()

add_library(WLib_DRIVER_MB85RS64V STATIC)


# Interface
target_include_directories(WLib_DRIVER_MB85RS64V PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/interface")
target_sources(WLib_DRIVER_MB85RS64V
	PUBLIC	"${CMAKE_CURRENT_LIST_DIR}/interface/WLib_Driver_MB85RS64V.hpp"
)

# Implementation
target_sources(WLib_DRIVER_MB85RS64V
	PRIVATE	"${CMAKE_CURRENT_LIST_DIR}/interface/WLib_Driver_MB85RS64V.hpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/implementation/WLib_Driver_MB85RS64V.cpp"
)

target_compile_features(WLib_DRIVER_MB85RS64V PUBLIC cxx_std_17)

target_link_libraries(WLib_DRIVER_MB85RS64V PUBLIC WLib_SPI_Abstraction)

