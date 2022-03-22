cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("WLib-Examples")
ENDIF()


add_subdirectory(WLib_Utility)
add_subdirectory(WLib_CRC)
add_subdirectory(WLib_Serializer)
add_subdirectory(WLib_StateMachine)
add_subdirectory(WLib_SPI_Abstraction)
