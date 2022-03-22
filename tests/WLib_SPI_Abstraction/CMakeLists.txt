cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-SPI-Abstraction-Test")
ENDIF()

add_executable(TEST_WLib_SPI_Abstraction)

target_sources(TEST_WLib_SPI_Abstraction
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_main.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_spi_abstraction.cpp"
)


if(MSVC)
  target_compile_options(TEST_WLib_SPI_Abstraction PRIVATE /W4 /WX)
else()
  target_compile_options(TEST_WLib_SPI_Abstraction PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(TEST_WLib_SPI_Abstraction PRIVATE WLib_SPI_Abstraction)
target_link_libraries(TEST_WLib_SPI_Abstraction PRIVATE UT_CATCH)
add_test(TEST_WLib_SPI_Abstraction TEST_WLib_SPI_Abstraction)
