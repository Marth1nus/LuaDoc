#include <string>
#include <filesystem>
#include <vector>

struct lua_State;
auto lua_tocode(lua_State *L, int idx, std::output_iterator<char> auto out) noexcept -> decltype(out);
auto lua_tocode(lua_State *L, int idx) noexcept -> std::string;
namespace doc
{
  struct icon
  {
    std::string title{};
    std::filesystem::path icon_path{};

    friend bool pull(lua_State *L, int idx, icon &output);
    friend bool push(lua_State *L, icon const &input);
  };

  struct doc
  {
    std::vector<icon> icons{};

    friend bool pull(lua_State *L, int idx, doc &output);
    friend bool push(lua_State *L, doc const &input);
  };
} // namespace doc
