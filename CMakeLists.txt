cmake_minimum_required(VERSION 3.28.0)

project(LuaDoc VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lua STATIC
lua-5.4.6/src/lapi.c
lua-5.4.6/src/lauxlib.c
lua-5.4.6/src/lbaselib.c
lua-5.4.6/src/lcode.c
lua-5.4.6/src/lcorolib.c
lua-5.4.6/src/lctype.c
lua-5.4.6/src/ldblib.c
lua-5.4.6/src/ldebug.c
lua-5.4.6/src/ldo.c
lua-5.4.6/src/ldump.c
lua-5.4.6/src/lfunc.c
lua-5.4.6/src/lgc.c
lua-5.4.6/src/linit.c
lua-5.4.6/src/liolib.c
lua-5.4.6/src/llex.c
lua-5.4.6/src/lmathlib.c
lua-5.4.6/src/lmem.c
lua-5.4.6/src/loadlib.c
lua-5.4.6/src/lobject.c
lua-5.4.6/src/lopcodes.c
lua-5.4.6/src/loslib.c
lua-5.4.6/src/lparser.c
lua-5.4.6/src/lstate.c
lua-5.4.6/src/lstring.c
lua-5.4.6/src/lstrlib.c
lua-5.4.6/src/ltable.c
lua-5.4.6/src/ltablib.c
lua-5.4.6/src/ltm.c
lua-5.4.6/src/lua.c
lua-5.4.6/src/luac.c
lua-5.4.6/src/lundump.c
lua-5.4.6/src/lutf8lib.c
lua-5.4.6/src/lvm.c
lua-5.4.6/src/lzio.c
)
include_directories(lua PUBLIC lua-5.4.6/src)

add_executable(LuaDoc 
  main.cpp
  doc.cpp
)
target_compile_features(LuaDoc PUBLIC cxx_std_23)
target_link_libraries(LuaDoc PRIVATE lua)
