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

  std::string cpp_doc{};
  auto out = std::back_inserter(cpp_doc);
  out = std::format_to(out, "{{ icons = {{ ");
  for (auto &icon : d.icons)
    out = std::format_to(out, "{{ title = \"{}\", icon_path = \"{}\", }}, ", icon.title, icon.icon_path.string());
  out = std::format_to(out, "}}, }} ");
  std::println("cpp doc: {}", cpp_doc);

  L = luaL_newstate();
  push(L, d);
  auto lua_doc = lua_tocode(L, -1);
  lua_close(L);
  std::println("lua doc: {}", lua_doc);
}