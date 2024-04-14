#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <lua.hpp>

#include <string>
#include <filesystem>
#include <vector>

/*         */ auto lua_tocode(lua_State *L, int idx, std::output_iterator<char> auto out) noexcept -> decltype(out);
[[nodiscard]] auto lua_tocode(lua_State *L, int idx) noexcept -> std::string;
namespace doc
{
  using std::string,
      std::filesystem::path,
      std::vector;

  using shared_HWND = std::shared_ptr<std::remove_pointer_t<HWND>>;

  struct icon
  {
    string title{};
    path icon_path{};

    shared_HWND hwnd{};

    struct update_info;
    void load(HWND parent_window=nullptr);
    void update();

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

    shared_HWND hwnd{};

    void load(HWND parent_window=nullptr);
    void update();

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

  struct icon::update_info
  {
    doc &doc;
  };
} // namespace doc

#include "formatters.hpp"
