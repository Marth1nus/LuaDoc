#pragma once

#include <format>
#include <ranges>

#include "doc.hpp"

template <>
struct std::formatter<doc::icon> : std::formatter<std::string_view>
{
  auto format(doc::icon const &val, std::format_context &ctx) const -> decltype(ctx.out());
};

template <>
struct std::formatter<doc::doc> : std::formatter<std::string_view>
{
  auto format(doc::doc const &val, std::format_context &ctx) const -> decltype(ctx.out());
};