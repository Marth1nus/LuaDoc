#include "formatters.hpp"

auto std::formatter<doc::icon>::format(doc::icon const &val, std::format_context &ctx) const -> decltype(ctx.out())
{
  auto str = std::format(
      "{{ "
      "title = \"{}\", "
      "icon_path = \"{}\", "
      "}}",
      val.title,
      val.icon_path.string());
  return std::formatter<std::string_view>::format(str, ctx);
}

auto std::formatter<doc::doc>::format(doc::doc const &val, std::format_context &ctx) const -> decltype(ctx.out())
{
  std::string str{};
  auto out = std::back_inserter(str);

  out = std::format_to(out, "{{ ");

  out = std::format_to(out, "icons = {{ ");
  for (auto &icon : val.icons)
    out = std::format_to(out, "{}, ", icon);
  out = std::format_to(out, "}}, ");

  out = std::format_to(out, "}}");

  return std::formatter<std::string_view>::format(str, ctx);
}
