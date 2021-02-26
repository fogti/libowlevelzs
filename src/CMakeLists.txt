set(CMAKE_C_FLAGS "-fno-plt -fno-exceptions -fno-unwind-tables -rdynamic")
set(CMAKE_CXX_FLAGS "-fno-plt -fno-rtti -fno-exceptions -fno-unwind-tables -rdynamic")

add_library(lowlevelzs SHARED MMAPguard.cxx ctchk.cxx hash.c lowlevel.c random.cxx
  string/replace.cxx string/rest.cxx string/xcpy.c
  io/fd.c io/file.cxx)

set_target_properties(lowlevelzs PROPERTIES VERSION "0.0.0" SOVERSION 0 OUTPUT_NAME "owlevelzs")
target_include_directories(lowlevelzs INTERFACE $<INSTALL_INTERFACE:include/zs>)
target_link_libraries(lowlevelzs ${CMAKE_DL_LIBS})

function(src_compile_flags flag)
  set_property(SOURCE ${ARGN} APPEND_STRING PROPERTY COMPILE_FLAGS " ${flag}")
endfunction()

src_compile_flags("-fexceptions -funwind-tables"
  string/csarray.cxx string/replace.cxx
  io/file.cxx)

# === INSTALL SECTION ===
install(TARGETS lowlevelzs DESTINATION "${INSTALL_LIB_DIR}" EXPORT LowlevelZSTargets)
install(EXPORT LowlevelZSTargets DESTINATION "${INSTALL_CMAKE_DIR}" NAMESPACE LowlevelZS:: COMPONENT dev)

install(FILES MMAPguard.hpp cxa_noexcept.h hash.hpp memut.hpp mman.h random.hpp reverse.hpp zsig.h
  DESTINATION "${INSTALL_INCLUDE_DIR}")

add_subdirectory(io)
add_subdirectory(string)
