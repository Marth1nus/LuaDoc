#pragma once

#include <string>
#include <filesystem>
#include <vector>

struct HWND__;
typedef HWND__ *HWND;

struct lua_State;
auto lua_tocode(lua_State *L, int idx, std::output_iterator<char> auto out) noexcept -> decltype(out);
auto lua_tocode(lua_State *L, int idx) noexcept -> std::string;
namespace doc
{
  using std::string,
      std::filesystem::path,
      std::vector;

  struct icon
  {
    string title{};
    path icon_path{};

    HWND hwnd{};

    /// @brief Copy the value at idx on the lua stack into an icon object
    /// @param L
    /// @param idx
    /// @param output
    /// @return
    friend bool pull(lua_State *L, int idx, icon &output);

    /// @brief Copy an icon object top the top of the lua stack
    /// @param L
    /// @param input
    /// @return
    friend bool push(lua_State *L, icon const &input);
  };

  struct doc
  {
    vector<icon> icons{};

    HWND hwnd{};

    /// @brief Copy the value at idx on the lua stack into a doc object
    /// @param L
    /// @param idx
    /// @param output
    /// @return
    friend bool pull(lua_State *L, int idx, doc &output);

    /// @brief Copy a doc object top the top of the lua stack
    /// @param L
    /// @param input
    /// @return
    friend bool push(lua_State *L, doc const &input);
  };
} // namespace doc

#include "formatters.hpp"
