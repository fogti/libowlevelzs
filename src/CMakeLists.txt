set(CMAKE_C_FLAGS "-fno-plt -fno-exceptions -fno-unwind-tables")
set(CMAKE_CXX_FLAGS "-fno-plt -fno-rtti -fno-exceptions -fno-unwind-tables -rdynamic")

add_library(lowlevelzs SHARED ctchk.cxx hash.c lowlevel.c MMAPguard.cxx
  dtors4plugins.c plg.c plugins/file.c plugins/spawn.c
  string/csarray.cxx string/replace.cxx string/xcpy.c)

set_target_properties(lowlevelzs PROPERTIES VERSION "0.0.0" SOVERSION 0 OUTPUT_NAME "owlevelzs")
target_include_directories(lowlevelzs INTERFACE $<INSTALL_INTERFACE:include/zs>)
target_link_libraries(lowlevelzs ${CMAKE_DL_LIBS})

function(src_compile_flags flag)
  set_property(SOURCE ${ARGN} APPEND_STRING PROPERTY COMPILE_FLAGS " ${flag}")
endfunction()

src_compile_flags("-fexceptions -funwind-tables" string/replace.cxx)

# === TEST EXECUTABLES === (check if plugin system works)
add_executable(plgtest-echo test/echo.c)
target_link_libraries(plgtest-echo lowlevelzs)

add_executable(plgtest-xx test/plgxx.cxx)
target_link_libraries(plgtest-xx lowlevelzs)

# === INSTALL SECTION ===
install(TARGETS lowlevelzs DESTINATION "${INSTALL_LIB_DIR}" EXPORT LowlevelZSTargets)
install(EXPORT LowlevelZSTargets DESTINATION "${INSTALL_CMAKE_DIR}" NAMESPACE LowlevelZS:: COMPONENT dev)

install(FILES cxa_noexcept.h hash.hpp mman.h MMAPguard.hpp zsig.h
    dtors4plugins.h plg.h plg4plugins.h
    string/csarray.hpp string/replace.hpp string/xcpy.h
  DESTINATION "${INSTALL_INCLUDE_DIR}")
