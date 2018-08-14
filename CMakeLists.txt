cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW)
project(libowlevelzs)

set(INSTALL_LIB_DIR     lib               CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include/zs/ll     CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR lib${LIB_SUFFIX}/cmake/LowlevelZS CACHE PATH "Installation directory for CMake module files")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCSourceCompiles)
check_c_source_compiles(
  "int main(void) { return __builtin_expect(0, 1); }" HAVE_BUILTIN_EXPECT)

check_c_source_compiles(
  "static int __attribute__((pure)) zs_st_pure_func(const int x) { return 2 * x; }\nint main(void) { return zs_st_pure_func(0); }"
  HAVE_ATTRIBUTE)

configure_file(config.h.in "${PROJECT_BINARY_DIR}/llzs_config.h")
include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}")
install(FILES "${PROJECT_BINARY_DIR}/llzs_config.h" DESTINATION "${INSTALL_INCLUDE_DIR}")

add_subdirectory(cmake)
add_subdirectory(src)
