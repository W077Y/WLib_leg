#include <WLib_Driver_MB85RS64V.hpp>
#include <ut_catch.hpp>
#include <vector>

std::byte rdid_resp[5] = {
  static_cast<std::byte>(0xAA), static_cast<std::byte>(0x04), static_cast<std::byte>(0x7F),
  static_cast<std::byte>(0x03), static_cast<std::byte>(0x02),
};

std::byte opcode_rdid  = static_cast<std::byte>(0b1001'1111);
std::byte opcode_read  = static_cast<std::byte>(0b0000'0011);
std::byte opcode_wren  = static_cast<std::byte>(0b0000'0110);
std::byte opcode_write = static_cast<std::byte>(0b0000'0010);

struct spi_slave_mock
{
  void sel()
  {
    this->is_selected  = true;
    this->m_byte_count = 0;
    this->m_cur_res    = nullptr;
  }
  void desel() { this->is_selected = false; }

  std::byte transceive(std::byte const& tx)
  {
    if (!this->is_selected)
      throw "not selected";

    uint32_t cur_idx = this->m_byte_count++;
    if (cur_idx == 0)
    {
      this->m_opcodes.push_back(tx);
      if (tx == opcode_rdid)
      {
        this->m_cur_res            = rdid_resp;
        this->m_res_max_byte_count = std::size(rdid_resp);
      }
    }

    if (cur_idx >= this->m_res_max_byte_count)
      return static_cast<std::byte>(0);

    if (this->m_cur_res == nullptr)
      return static_cast<std::byte>(0);

    return this->m_cur_res[cur_idx];
  }

  bool        is_selected          = false;
  uint32_t    m_byte_count         = 0;
  std::size_t m_res_max_byte_count = 0;
  std::byte*  m_cur_res            = nullptr;

  std::vector<std::byte> m_opcodes;
};

struct cs_mock: public WLib::SPI::ChipSelect_Interface
{
  cs_mock(spi_slave_mock& slave)
      : slave(slave)
  {
  }
  virtual void select() override
  {
    if (this->m_status)
      throw "multi select";
    this->slave.sel();
    this->m_status = true;
  }
  virtual void deselect() override
  {
    if (!this->m_status)
      throw "multi deselect";

    this->slave.desel();
    this->m_status = false;
  }

  bool m_status = false;

  spi_slave_mock& slave;
};

struct hw_mock: public WLib::SPI::HW_Interface
{
  hw_mock(spi_slave_mock& slave)
      : slave(slave)
  {
  }

  virtual void transceive(std::byte const* tx, std::byte* rx, std::size_t len) override
  {
    for (std::size_t i = 0; i < len; i++)
    {
      std::byte outgoing_byte;
      if (tx != nullptr)
        outgoing_byte = tx[i];

      std::byte incomming_byte = this->slave.transceive(outgoing_byte);

      if (rx != nullptr)
        rx[i] = incomming_byte;
    }
  }

  virtual void enable(cfg_t const& cfg) override
  {
    if (this->m_status)
      throw "multi enable";
    this->m_status   = true;
    this->m_bautrate = (cfg.get_max_clock_rate() + cfg.get_min_clock_rate()) / 2;
  }

  virtual uint32_t get_actual_clock_rate() const override { return this->m_bautrate; }

  virtual void disable() override
  {
    if (!this->m_status)
      throw "multi disable";
    this->m_bautrate = 0;
    this->m_status   = false;
  }

  bool            m_status   = false;
  uint32_t        m_bautrate = 0;
  spi_slave_mock& slave;
};

TEST_CASE()
{
  spi_slave_mock chip;
  hw_mock        hw(chip);
  cs_mock        cs(chip);

  WLib::SPI::Channel_Provider channel_pro(cs, hw);

  {
    REQUIRE(chip.m_opcodes.size() == 0);

    WLib::Driver::FRAM::MB85RS64V fram(channel_pro);

    REQUIRE(chip.m_opcodes.size() == 1);
    REQUIRE(chip.m_opcodes[0] == opcode_rdid);

    REQUIRE(fram.get_manufacturer_id() == 0x04);
    REQUIRE(fram.get_continuation_code() == 0x7F);
    REQUIRE(fram.get_product_id() == 0x0302);

    std::byte data[100];
    fram.write(0x0000'0000, data, 100);
    fram.read(0x0000'0000, data, 100);

    REQUIRE(chip.m_opcodes.size() == 4);
    REQUIRE(chip.m_opcodes[1] == opcode_wren);
    REQUIRE(chip.m_opcodes[2] == opcode_write);
    REQUIRE(chip.m_opcodes[3] == opcode_read);
  }
}
