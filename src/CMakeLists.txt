cmake_minimum_required(VERSION 3.7)

set(CMAKE_C_FLAGS "-fno-plt -fno-exceptions -fno-unwind-tables")
set(CMAKE_CXX_FLAGS "-fno-plt -fno-rtti -fno-exceptions -fno-unwind-tables -rdynamic")
#set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--gc-sections")

add_library(owlevelzs SHARED lowlevel.c MMAPguard.cxx
  dtors4plugins.c plg.c plugins/file.c plugins/spawn.c
  string/csarray.cxx string/replace.cxx string/xcpy.c)
set_target_properties(owlevelzs PROPERTIES VERSION "0.0.0" SOVERSION 0)
target_link_libraries(owlevelzs ${CMAKE_DL_LIBS})

add_executable(plgtest-echo test/echo.c)
target_link_libraries(plgtest-echo owlevelzs)

# === INSTALL SECTION ===
install(TARGETS owlevelzs DESTINATION "${INSTALL_LIB_DIR}")
install(FILES cxa_noexcept.h mman.h MMAPguard.hpp zsig.h
    dtors4plugins.h plg.h plg4plugins.h
    string/csarray.hpp string/replace.hpp string/xcpy.h
  DESTINATION "${INSTALL_INCLUDE_DIR}")
