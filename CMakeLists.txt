cmake_minimum_required(VERSION 3.28.0)

project(LuaDoc VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LuaDoc main.cpp)
target_compile_features(LuaDoc PUBLIC cxx_std_23)

