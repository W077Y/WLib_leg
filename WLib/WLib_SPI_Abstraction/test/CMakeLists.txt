cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-SPI-Abstraction-Test")
ENDIF()

add_executable(WLib_SPI_Abstraction_TEST)

target_sources(WLib_SPI_Abstraction_TEST
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_spi_abstraction.cpp"
)


if(MSVC)
  target_compile_options(WLib_SPI_Abstraction_TEST PRIVATE /W4 /WX)
else()
  target_compile_options(WLib_SPI_Abstraction_TEST PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(WLib_SPI_Abstraction_TEST PRIVATE WLib_SPI_Abstraction)
target_link_libraries(WLib_SPI_Abstraction_TEST PRIVATE UT_CATCH)
add_test(WLib_SPI_Abstraction_TEST WLib_SPI_Abstraction_TEST)
