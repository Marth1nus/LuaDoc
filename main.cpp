#include <print>
#include <lua.hpp>
#include "doc.hpp"

int main()
{
  auto L = luaL_newstate();
  luaL_dofile(L, "example-doc.lua");
  doc::doc d{};
  lua_pushglobaltable(L);
  auto src_doc = lua_tocode(L, -1);
  pull(L, -1, d);
  lua_close(L);
  std::println("src doc: {}", src_doc);

  auto cpp_doc = std::format("{}", d);
  std::println("cpp doc: {}", cpp_doc);

  L = luaL_newstate();
  push(L, d);
  auto lua_doc = lua_tocode(L, -1);
  lua_close(L);
  std::println("lua doc: {}", lua_doc);
}