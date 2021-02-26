cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW)
project(libowlevelzs)

set(INSTALL_LIB_DIR     lib${LIB_SUFFIX}  CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include/zs/ll     CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR ${INSTALL_LIB_DIR}/cmake/LowlevelZS CACHE PATH "Installation directory for CMake module files")

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

message(STATUS "Looking for include file string_view")

try_run(LLZS_SV_RUN_RES LLZS_SV_COMP_RES "${PROJECT_BINARY_DIR}/llzs_svt" "${PROJECT_SOURCE_DIR}/test_sv.cxx"
  CXX_STANDARD 17 COMPILE_OUTPUT_VARIABLE LLZS_SV_COMP_OUT RUN_OUTPUT_VARIABLE LLZS_RES_STRING_VIEW)

if(NOT LLZS_SV_RUN_RES)
  message(STATUS "Looking for include file string_view - found")
else()
  message(FATAL_ERROR "Looking for include file string_view - not found - this is unsupported")
endif()

configure_file(config.h.in "${PROJECT_BINARY_DIR}/zs/ll/llzs_config.h")
include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}")
install(FILES "${PROJECT_BINARY_DIR}/zs/ll/llzs_config.h" DESTINATION "${INSTALL_INCLUDE_DIR}")
file(COPY "${PROJECT_BINARY_DIR}/zs/ll/llzs_config.h" DESTINATION "${PROJECT_BINARY_DIR}")

add_subdirectory(cmake)
add_subdirectory(src)
