set(CMAKE_C_FLAGS "-fno-plt -fno-exceptions -fno-unwind-tables -rdynamic")
set(CMAKE_CXX_FLAGS "-fno-plt -fno-rtti -fno-exceptions -fno-unwind-tables -rdynamic")

add_library(lowlevelzs SHARED
  ctchk.cxx escape.cxx hash.c hexcode.c lowlevel.c MMAPguard.cxx Parser.cxx random.cxx
  io/fd.c io/file.cxx
  lexer/InputSlice.cxx lexer/Lexer.cxx
  string/replace.cxx string/rest.cxx string/split.cxx string/xcpy.c
)

set_target_properties(lowlevelzs PROPERTIES VERSION "1.0.0" SOVERSION 1 OUTPUT_NAME "owlevelzs")
target_compile_options(lowlevelzs PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
target_include_directories(lowlevelzs INTERFACE $<INSTALL_INTERFACE:include/zs>)
target_link_libraries(lowlevelzs ${CMAKE_DL_LIBS})

function(src_compile_flags flag)
  set_property(SOURCE ${ARGN} APPEND_STRING PROPERTY COMPILE_FLAGS " ${flag}")
endfunction()

src_compile_flags("-fexceptions -funwind-tables"
  Parser.cxx io/file.cxx lexer/Lexer.cxx
  string/csarray.cxx string/replace.cxx
)

# === INSTALL SECTION ===
install(TARGETS lowlevelzs DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT LowlevelZSTargets)
install(EXPORT LowlevelZSTargets DESTINATION "${ZSINST_CMAKE_DIR}" NAMESPACE LowlevelZS:: COMPONENT dev)

install(FILES
  cxa_noexcept.h escape.h hash.hpp hexcode.h memut.hpp mman.h
  MMAPguard.hpp Parser.hpp random.hpp reverse.hpp zsig.h
  DESTINATION "${ZSINST_INCLUDE_DIR}")

add_subdirectory(io)
add_subdirectory(lexer)
add_subdirectory(string)
