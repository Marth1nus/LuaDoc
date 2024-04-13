#include "formatters.hpp"

template <class T>
struct std::formatter<std::vector<T>> : std::formatter<std::string_view>
{
  auto format(std::vector<T> const &vals, std::format_context &ctx) const -> decltype(ctx.out())
  {
    using namespace std::literals;
    auto out = ctx.out();
    out = std::format_to(out, "{}", open);
    if (trailing_sep)
      for (auto &val : vals)
        out = std::format_to(out, "{}{}", val, sep);
    else
      for (auto first = true; auto &val : vals)
        out = std::format_to(out, "{}{}", first ? (first = false, ""sv) : sep, val);
    out = std::format_to(out, "{}", close);
    return out;
  }
  std::string_view open = "{ ", sep = ", ", close = "}";
  bool trailing_sep = true;
};

auto std::formatter<doc::icon>::format(doc::icon const &val, std::format_context &ctx) const -> decltype(ctx.out())
{
  return std::format_to(
      ctx.out(),
      "{{ "
      "title = {:?}, "
      "icon_path = {:?}, "
      "}}",
      val.title,
      val.icon_path.string());
}

auto std::formatter<doc::doc>::format(doc::doc const &val, std::format_context &ctx) const -> decltype(ctx.out())
{
  return std::format_to(
      ctx.out(),
      "{{ "
      "icons = {}, "
      "}}",
      val.icons);
}
