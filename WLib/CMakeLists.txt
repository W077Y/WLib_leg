cmake_minimum_required (VERSION 3.15)

MESSAGE_TREEVIEW("WLib Library")

add_subdirectory(WLib_Utility)
add_subdirectory(WLib_CRC)
add_subdirectory(WLib_Serializer)
add_subdirectory(WLib_StateMachine)



add_subdirectory(WLib_SPI_Abstraction)
