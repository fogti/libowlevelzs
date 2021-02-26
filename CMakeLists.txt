cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW)
project(libowlevelzs)

include(GNUInstallDirs)

set(ZSINST_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/zs/ll CACHE PATH "Installation directory for header files")
set(ZSINST_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/LowlevelZS CACHE PATH "Installation directory for CMake module files")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCSourceCompiles)
check_c_source_compiles(
  "int main(void) { return __builtin_expect(0, 1); }" HAVE_BUILTIN_EXPECT)

check_c_source_compiles(
  "static int __attribute__((pure)) zs_st_pure_func(const int x) { return 2 * x; }\nint main(void) { return zs_st_pure_func(0); }"
  HAVE_ATTRIBUTE)

configure_file(config.h.in "${PROJECT_BINARY_DIR}/zs/ll/llzs_config.h")
include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}" "${PROJECT_BINARY_DIR}/zs/ll")
install(FILES "${PROJECT_BINARY_DIR}/zs/ll/llzs_config.h" DESTINATION "${ZSINST_INCLUDE_DIR}")

add_subdirectory(cmake)
add_subdirectory(src)
