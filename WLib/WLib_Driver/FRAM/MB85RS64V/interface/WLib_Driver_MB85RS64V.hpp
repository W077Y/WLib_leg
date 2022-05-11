#pragma once
#ifndef WLIB_DRIVER_MB85RS64V_HPP
#define WLIB_DRIVER_MB85RS64V_HPP

#include <WLib_SPI_Abstraction.hpp>

namespace WLib::Driver::FRAM
{
  class MB85RS64V
  {
  public:
    using channel_provider_t = WLib::SPI_Abstraction::Sync::Channel_Provider;

    MB85RS64V(channel_provider_t& device);

    void read(uint32_t const& add, std::byte* ptr, std::size_t const& len);
    void write(uint32_t const& add, std::byte const* ptr, std::size_t const& len);

    uint8_t  get_manufacturer_id() const;
    uint8_t  get_continuation_code() const;
    uint16_t get_product_id() const;

  private:
    channel_provider_t& m_device;
    uint8_t             m_manufacturer_id   = 0;
    uint8_t             m_continuation_code = 0;
    uint16_t            m_product_id        = 0;
  };
}    // namespace WLib::Driver::FRAM
#endif    // !WLIB_DRIVER_MB85RS64V_HPP
