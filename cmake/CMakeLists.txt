configure_file(FindLowlevelZS.cmake.in "${PROJECT_BINARY_DIR}/FindLowlevelZS.cmake" @ONLY)
install(FILES LowlevelZSConfig.cmake "${PROJECT_BINARY_DIR}/FindLowlevelZS.cmake" DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
