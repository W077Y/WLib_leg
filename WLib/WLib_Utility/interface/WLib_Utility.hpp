#pragma once
#ifndef WLIB_UTILITY_HPP
#define WLIB_UTILITY_HPP

#include <cstddef>
#include <cstdint>

namespace WLib::Utility
{
  struct non_copyable_non_moveable_t
  {
    non_copyable_non_moveable_t()                                   = default;
    non_copyable_non_moveable_t(non_copyable_non_moveable_t const&) = delete;
    non_copyable_non_moveable_t(non_copyable_non_moveable_t&&)      = delete;
  };
}    // namespace WLib::Utility
#endif