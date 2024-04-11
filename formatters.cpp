#include "formatters.hpp"

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
  auto out = ctx.out();

  out = std::format_to(out, "{{ ");

  out = std::format_to(out, "icons = {{ ");
  for (auto &icon : val.icons)
    out = std::format_to(out, "{}, ", icon);
  out = std::format_to(out, "}}, ");

  out = std::format_to(out, "}}");

  return out;
}
