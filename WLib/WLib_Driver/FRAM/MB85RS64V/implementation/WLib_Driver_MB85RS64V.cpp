#include <WLib_Driver_MB85RS64V.hpp>
#include <cstring>

namespace WLib::Driver::FRAM
{
  namespace OP_CODE
  {
    [[maybe_unused]] static constexpr std::byte WREN =
        static_cast<std::byte>(0b0000'0110);    // Set Write Enable Latch
    [[maybe_unused]] static constexpr std::byte WRDI =
        static_cast<std::byte>(0b0000'0100);    // Reset Write Enable Latch
    [[maybe_unused]] static constexpr std::byte RDSR =
        static_cast<std::byte>(0b0000'0101);    // Read Status Register
    [[maybe_unused]] static constexpr std::byte WRSR =
        static_cast<std::byte>(0b0000'0001);    // Write Status Register
    [[maybe_unused]] static constexpr std::byte READ =
        static_cast<std::byte>(0b0000'0011);    // Read Memory Code
    [[maybe_unused]] static constexpr std::byte WRITE =
        static_cast<std::byte>(0b0000'0010);    //  Write Memory Code
    [[maybe_unused]] static constexpr std::byte RDID =
        static_cast<std::byte>(0b1001'1111);    // Read Device ID
  }                                             // namespace OP_CODE

  using SPI_cfg                 = WLib::SPI::HW_Interface::Configuration;
  static constexpr auto spi_cfg = SPI_cfg{ 20'000'000, SPI_cfg::Mode::Mode_0 };

  namespace comm_func
  {
    void read(WLib::SPI::Channel_Provider::channel_handle_t& channel,
              uint16_t const&                                add,
              std::byte*                                     ptr,
              std::size_t const&                             len)
    {
      std::byte tx[1 + 2] = {};
      tx[0]               = OP_CODE::READ;
      tx[1]               = static_cast<std::byte>((add >> 8) & 0b0001'1111);
      tx[2]               = static_cast<std::byte>((add >> 0) & 0b1111'1111);
      auto&& sel          = channel.select_chip();
      sel.transceive(tx, nullptr, 3);
      sel.transceive(nullptr, ptr, len);
    }

    uint8_t read_status_reg(WLib::SPI::Channel_Provider::channel_handle_t& channel)
    {
      std::byte tx[2] = {};
      std::byte rx[2] = {};

      tx[0] = OP_CODE::RDSR;
      channel.select_chip().transceive(tx, rx, 2);
      return static_cast<uint8_t>(rx[1]);
    }

    void enable_write(WLib::SPI::Channel_Provider::channel_handle_t& channel)
    {
      std::byte tx[1] = {};
      tx[0]           = OP_CODE::WREN;
      channel.select_chip().transceive(tx, nullptr, 1);
    }

    void write(WLib::SPI::Channel_Provider::channel_handle_t& channel,
               uint16_t const&                                add,
               std::byte const*                               ptr,
               std::size_t const&                             len)
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
    std::byte tx[5] = {};
    std::byte rx[5] = {};

    {
      tx[0] = OP_CODE::RDID;
      this->m_device.request_channel(spi_cfg).select_chip().transceive(tx, rx, 5);
    }

    std::memcpy(&this->m_manufacturer_id, &rx[1], 1);
    std::memcpy(&this->m_continuation_code, &rx[2], 1);
    uint8_t tmp_1 = 0;
    uint8_t tmp_2 = 0;
    std::memcpy(&tmp_1, &rx[3], 1);
    std::memcpy(&tmp_2, &rx[4], 1);
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
