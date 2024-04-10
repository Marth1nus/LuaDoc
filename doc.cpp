#include "doc.hpp"
#include <lua.hpp>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <format>

using namespace std::literals;

inline auto lua_tolstring(lua_State *L, int idx) noexcept -> std::string_view
{
  size_t clen{};
  auto cstr = lua_tolstring(L, idx, &clen);
  return {cstr, clen};
}

inline auto lua_pushlstring(lua_State *L, std::string_view str) noexcept
{
  return lua_pushlstring(L, str.data(), str.size());
}

auto lua_tocode(lua_State *L, int idx, std::output_iterator<char> auto out) noexcept -> decltype(out)
{
  switch (lua_type(L, idx))
  {
  case LUA_TNIL:
    return std::format_to(out, "nil");
  case LUA_TBOOLEAN:
    return std::format_to(out, "{}", !!lua_toboolean(L, idx));
  case LUA_TNUMBER:
    return std::format_to(out, "{}", lua_tonumberx(L, idx, 0));
  case LUA_TSTRING:
  {
    *out++ = '"';
    for (auto c : lua_tolstring(L, idx))
    {
      if ("\"\'\\"sv.contains(c))
        *out++ = '\\';
      *out++ = c;
    }
    *out++ = '"';
    return out;
  }
  case LUA_TTABLE:
  {
    idx += idx < 0 ? lua_gettop(L) + 1 : 0;
    out = std::format_to(out, "{}", "{ "sv);
    for (lua_pushnil(L); lua_next(L, idx); lua_pop(L, 1))
    {
      if (lua_type(L, -2) == LUA_TSTRING)
      {
        out = std::format_to(out, "{}", lua_tolstring(L, -2));
        out = std::format_to(out, "{}", " = "sv);
      }
      out = lua_tocode(L, -1, out);
      out = std::format_to(out, "{}", ", "sv);
    }
    out = std::format_to(out, "{}", "}"sv);
    return out;
  }
  default:
    return std::format_to(out, "{}", "nil"sv);
  }
}

auto lua_tocode(lua_State *L, int idx) noexcept -> std::string
{
  std::string res{};
  lua_tocode(L, idx, std::back_inserter(res));
  return res;
}

bool doc::pull(lua_State *L, int idx, icon &out)
{
  // TODO: Type and fail checking
  if (lua_type(L, idx) != LUA_TTABLE)
    return false;

  lua_getfield(L, idx, "title");
  out.title = lua_tolstring(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, idx, "icon_path");
  out.icon_path = lua_tolstring(L, -1);
  lua_pop(L, 1);

  return true;
}

bool doc::push(lua_State *L, icon const &in)
{
  lua_createtable(L, 0, 1);

  lua_pushlstring(L, in.title);
  lua_setfield(L, -2, "title");

  lua_pushlstring(L, in.icon_path.string());
  lua_setfield(L, -2, "icon_path");

  return true;
}

bool doc::pull(lua_State *L, int idx, doc &out)
{
  // TODO: Type and fail checking
  if (lua_type(L, idx) != LUA_TTABLE)
    return false;

  lua_getfield(L, idx, "icons");
  for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1))
    pull(L, -1, out.icons.emplace_back());
  lua_pop(L, 1);

  return true;
}

bool doc::push(lua_State *L, doc const &in)
{
  lua_createtable(L, 0, 1);

  lua_createtable(L, in.icons.size(), 0);
  for (lua_Integer i = 1; auto &icon : in.icons)
    push(L, icon), lua_seti(L, -2, i), i++;
  lua_setfield(L, -2, "icons");

  return true;
}