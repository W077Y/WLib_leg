#pragma once
#ifndef WLIB_CRC_INTERFACE_HPP
#define WLIB_CRC_INTERFACE_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace WLib::CRC
{
  template <typename T> class CRC_Interface
  {
  public:
    using used_type = typename std::remove_cv_t<T>;

    virtual ~CRC_Interface()               = default;
    virtual void      reset() noexcept     = 0;
    virtual used_type get() const noexcept = 0;

    virtual used_type operator()(std::byte const* beg, std::byte const* end) noexcept = 0;
    virtual used_type operator()(std::byte const* beg, std::size_t const& len) noexcept
    {
      return (*this)(beg, beg + len);
    }
  };
}    // namespace WLib::CRC
#endif    // !WLIB_CRC_INTERFACE_HPP
