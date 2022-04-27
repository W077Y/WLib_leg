#include <WLib_Driver_MB85RS64V.hpp>
#include <cstring>

namespace WLib::Driver::FRAM
{
  namespace OP_CODE
  {
    [[maybe_unused]] static constexpr std::byte WREN = static_cast<std::byte>(0b0000'0110);    // Set Write Enable Latch
    [[maybe_unused]] static constexpr std::byte WRDI = static_cast<std::byte>(0b0000'0100);    // Reset Write Enable Latch
    [[maybe_unused]] static constexpr std::byte RDSR  = static_cast<std::byte>(0b0000'0101);    // Read Status Register
    [[maybe_unused]] static constexpr std::byte WRSR  = static_cast<std::byte>(0b0000'0001);    // Write Status Register
    [[maybe_unused]] static constexpr std::byte READ  = static_cast<std::byte>(0b0000'0011);    // Read Memory Code
    [[maybe_unused]] static constexpr std::byte WRITE = static_cast<std::byte>(0b0000'0010);    //  Write Memory Code
    [[maybe_unused]] static constexpr std::byte RDID  = static_cast<std::byte>(0b1001'1111);    // Read Device ID
  }                                                                                             // namespace OP_CODE

  using spi_cfg_t               = WLib::SPI::Configuration;
  static constexpr auto spi_cfg = spi_cfg_t{spi_cfg_t::Clock_Range(1, 20'000'000), spi_cfg_t::Mode::Mode_0};

  namespace comm_func
  {
    using channel_handle_t = WLib::SPI::Channel_Provider::channel_handle_t;

    void read(channel_handle_t& channel, uint16_t const& add, std::byte* ptr, std::size_t const& len)
    {
      std::byte tx[1 + 2] = {};
      tx[0]               = OP_CODE::READ;
      tx[1]               = static_cast<std::byte>((add >> 8) & 0b0001'1111);
      tx[2]               = static_cast<std::byte>((add >> 0) & 0b1111'1111);
      auto&& sel          = channel.select_chip();
      sel.transceive(tx, nullptr, 3);
      sel.transceive(nullptr, ptr, len);
    }

    uint8_t read_status_reg(channel_handle_t& channel)
    {
      std::byte txrx[2] = {};
      txrx[0]           = OP_CODE::RDSR;
      channel.select_chip().transceive(txrx, txrx, 2);
      return static_cast<uint8_t>(txrx[1]);
    }

    void enable_write(channel_handle_t& channel)
    {
      std::byte tx[1] = {};
      tx[0]           = OP_CODE::WREN;
      channel.select_chip().transceive(tx, nullptr, 1);
    }

    void write(channel_handle_t& channel, uint16_t const& add, std::byte const* ptr, std::size_t const& len)
    {
      std::byte tx[1 + 2] = {};
      tx[0]               = OP_CODE::WRITE;
      tx[1]               = static_cast<std::byte>((add >> 8) & 0b0001'1111);
      tx[2]               = static_cast<std::byte>((add >> 0) & 0b1111'1111);
      auto&& sel          = channel.select_chip();
      sel.transceive(tx, nullptr, 3);
      sel.transceive(ptr, nullptr, len);
    }

  }    // namespace comm_func

  MB85RS64V::MB85RS64V(WLib::SPI::Channel_Provider& device)
    : m_device(device)
  {
    std::byte txrx[5] = {};
    {
      txrx[0] = OP_CODE::RDID;
      this->m_device.request_channel(spi_cfg).select_chip().transceive(txrx, txrx, 5);
    }

    std::memcpy(&this->m_manufacturer_id, &txrx[1], 1);
    std::memcpy(&this->m_continuation_code, &txrx[2], 1);
    uint8_t tmp_1 = 0;
    uint8_t tmp_2 = 0;
    std::memcpy(&tmp_1, &txrx[3], 1);
    std::memcpy(&tmp_2, &txrx[4], 1);
    this->m_product_id = tmp_1 << 8 | tmp_2;
  }

  void MB85RS64V::read(uint32_t const& add, std::byte* ptr, std::size_t const& len)
  {
    auto&& channel = this->m_device.request_channel(spi_cfg);
    comm_func::read(channel, add, ptr, len);
  }

  void MB85RS64V::write(uint32_t const& add, std::byte const* ptr, std::size_t const& len)
  {
    auto&& channel = this->m_device.request_channel(spi_cfg);
    comm_func::enable_write(channel);
    comm_func::write(channel, add, ptr, len);
  }

  uint8_t MB85RS64V::get_manufacturer_id() const { return this->m_manufacturer_id; }

  uint8_t MB85RS64V::get_continuation_code() const { return this->m_continuation_code; }

  uint16_t MB85RS64V::get_product_id() const { return this->m_product_id; }

}    // namespace WLib::Driver::FRAM
