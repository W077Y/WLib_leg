#pragma once
#ifndef WLIB_CRC_64_GO_ISO_HPP
#define WLIB_CRC_64_GO_ISO_HPP

#include <WLib_CRC_Interface.hpp>
#include <cstddef>
#include <cstdint>

namespace WLib::CRC
{
  class CRC_64_go_iso final: public CRC_Interface<uint64_t>
  {
  public:
    using base_t = CRC_Interface<uint64_t>;

    virtual void      reset() noexcept override;
    virtual used_type get() const noexcept override;

    using base_t::operator();

    virtual used_type operator()(std::byte const* beg, std::byte const* end) noexcept override;

  private:
    static constexpr used_type init_value = 0xFFFF'FFFF'FFFF'FFFF;
    used_type                  m_crc      = init_value;
  };
}    // namespace WLib::CRC
#endif
