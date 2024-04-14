#include "doc.hpp"

#include <format>

using namespace std::literals;

// ===========
// === LUA ===
// ===========

[[nodiscard]] inline auto lua_tolstring(lua_State *L, int idx) noexcept -> std::string_view
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
    return std::format_to(out, "{:?}", lua_tolstring(L, idx));
  case LUA_TTABLE:
  {
    idx += idx < 0 ? lua_gettop(L) + 1 : 0;
    out = std::format_to(out, "{{ ");
    for (lua_pushnil(L); lua_next(L, idx); lua_pop(L, 1))
    {
      if (lua_type(L, -2) == LUA_TSTRING)
      {
        out = std::format_to(out, "{}", lua_tolstring(L, -2));
        out = std::format_to(out, " = ");
      }
      out = lua_tocode(L, -1, out);
      out = std::format_to(out, ", ");
    }
    out = std::format_to(out, "}}");
    return out;
  }
  default:
    return std::format_to(out, "nil");
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
  lua_createtable(L, 0, 2);

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

// ===============
// === WINDOWS ===
// ===============

LRESULT CALLBACK doc_icon_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  case WM_DESTROY:
  {
    PostQuitMessage(0);
    return 0;
  }
  case WM_PAINT:
  {
    PAINTSTRUCT ps{};
    auto hdc = BeginPaint(hwnd, &ps);
    /* Paint Here */
    EndPaint(hwnd, &ps);
    return 0;
  }
  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}

auto doc_icon_window_register_class()
{
  WNDCLASSEXW const wc{
      .cbSize /*       */ = sizeof(WNDCLASSEXW),
      .style /*        */ = CS_VREDRAW bitor CS_HREDRAW,
      .lpfnWndProc /*  */ = doc_icon_window_procedure,
      .cbClsExtra /*   */ = 0,
      .cbWndExtra /*   */ = 0,
      .hInstance /*    */ = GetModuleHandle(nullptr),
      .hIcon /*        */ = nullptr,
      .hCursor /*      */ = LoadCursor(nullptr, IDC_HAND),
      .hbrBackground /**/ = nullptr,
      .lpszMenuName /* */ = nullptr,
      .lpszClassName /**/ = L"doc::icon",
      .hIconSm /*      */ = nullptr,
  };
  return RegisterClassExW(&wc);
}

void doc::icon::load(HWND parent_window)
{
  if (!hwnd)
    hwnd = {
        CreateWindowExW(
            0,
            L"doc::icon",
            0,
            WS_CHILDWINDOW,
            0,
            0,
            0,
            0,
            parent_window,
            0,
            GetModuleHandleW(0),
            0),
        DestroyWindow};
}

void doc::icon::update()
{
}

LRESULT CALLBACK doc_icon_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  case WM_DESTROY:
  {
    PostQuitMessage(0);
    return 0;
  }
  case WM_PAINT:
  {
    PAINTSTRUCT ps{};
    auto hdc = BeginPaint(hwnd, &ps);
    /* Paint Here */
    EndPaint(hwnd, &ps);
    return 0;
  }
  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}

auto doc_doc_window_register_class()
{
  WNDCLASSEXW wc{
      .cbSize /*       */ = sizeof(wc),
      .style /*        */ = WS_EX_OVERLAPPEDWINDOW,
      .lpfnWndProc /*  */ = doc_icon_window_procedure,
      .cbClsExtra /*   */ = 0,
      .cbWndExtra /*   */ = 0,
      .hInstance /*    */ = GetModuleHandle(nullptr),
      .hIcon /*        */ = nullptr,
      .hCursor /*      */ = LoadCursor(nullptr, IDC_HAND),
      .hbrBackground /**/ = nullptr,
      .lpszMenuName /* */ = nullptr,
      .lpszClassName /**/ = L"doc::doc",
      .hIconSm /*      */ = nullptr,
  };
  return RegisterClassExW(&wc);
}

void doc::doc::load(HWND parent_window)
{
  if (!hwnd)
    hwnd = {
        CreateWindowExW(
            0,
            L"doc::doc",
            0,
            WS_EX_TOOLWINDOW,
            0,
            0,
            0,
            0,
            parent_window,
            0,
            GetModuleHandleW(0),
            0),
        DestroyWindow};
}

void doc::doc::update()
{
}
