cmake_minimum_required (VERSION 3.15)


IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-SPI-Abstraction-Example")
ENDIF()

add_executable(WLib_SPI_Abstraction_EXAMPLE)

target_sources(WLib_SPI_Abstraction_EXAMPLE	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/exa_spi_abstraction.cpp)


target_link_libraries(WLib_SPI_Abstraction_EXAMPLE PRIVATE WLib_SPI_Abstraction)
add_test(WLib_SPI_Abstraction_EXAMPLE WLib_SPI_Abstraction_EXAMPLE)
