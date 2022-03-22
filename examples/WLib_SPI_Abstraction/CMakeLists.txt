cmake_minimum_required (VERSION 3.15)


IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-SPI-Abstraction-Example")
ENDIF()

add_executable(EXAMPLE_WLib_SPI_Abstraction)

target_sources(EXAMPLE_WLib_SPI_Abstraction	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/exa_spi_abstraction.cpp)


target_link_libraries(EXAMPLE_WLib_SPI_Abstraction PRIVATE WLib_SPI_Abstraction)
add_test(EXAMPLE_WLib_SPI_Abstraction EXAMPLE_WLib_SPI_Abstraction)
