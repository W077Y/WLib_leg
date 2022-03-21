#pragma once
#ifndef WLIB_CRC_32_HPP
#define WLIB_CRC_32_HPP

#include <WLib_CRC_Interface.hpp>
#include <cstddef>
#include <cstdint>

namespace WLib::CRC
{
  class CRC_32 final: public CRC_Interface<uint32_t>
  {
  public:
    using base_t = CRC_Interface<uint32_t>;

    virtual void      reset() noexcept override;
    virtual used_type get() const noexcept override;

    using base_t::operator();

    virtual used_type operator()(std::byte const* beg, std::byte const* end) noexcept override;

  private:
    static constexpr used_type init_value = 0xFFFF'FFFF;
    used_type                  m_crc      = init_value;
  };
}    // namespace WLib::CRC
#endif
