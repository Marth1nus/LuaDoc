#include <print>
#include <lua.hpp>

int main()
{
  auto L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dostring(L, "print('Hello Lua', a)");
  lua_close(L);
  std::println("Hello C++");
}