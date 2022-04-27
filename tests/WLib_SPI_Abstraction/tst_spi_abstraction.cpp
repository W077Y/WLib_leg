#include <WLib_SPI_Abstraction.hpp>
#include <iostream>
#include <ut_catch.hpp>

class bool_chip_select final: public WLib::SPI::ChipSelect_Interface
{
public:
  bool_chip_select(int32_t& cs)
    : m_count(cs)
  {
  }

private:
  void select() override
  {
    this->m_count++;
  };
  void deselect() override
  {
    this->m_count--;
  };

  int32_t& m_count;
};

class count_spi_dummy final: public WLib::SPI::HW_Interface
{
public:
  count_spi_dummy(std::size_t& count, int32_t& en)
    : m_count(count)
    , m_en(en)
  {
  }

private:
  void transceive(std::byte const*, std::byte*, std::size_t len) override
  {
    this->m_count += len;
  }

  void enable(cfg_t const& cfg) override
  {
    this->m_en++;
    this->m_bautrate = cfg.get_max_clock_rate();
  };

  uint32_t get_actual_clock_rate() const override
  {
    return this->m_bautrate;
  }

  void disable() override
  {
    this->m_en--;
    this->m_bautrate = 0;
  }

  std::size_t& m_count;
  int32_t&     m_en;
  uint32_t     m_bautrate = 0;
};

TEST_CASE()
{
  int32_t     spi_en_count = 0;
  int32_t     cs_sel_count = 0;
  std::size_t byte_count   = 0;

  count_spi_dummy             spi_obj(byte_count, spi_en_count);
  bool_chip_select            cs_obj(cs_sel_count);
  WLib::SPI::Channel_Provider dev_obj(cs_obj, spi_obj);

  WLib::SPI::Channel_Provider& dev_i = dev_obj;

  constexpr WLib::SPI::Configuration cfg{
    WLib::SPI::Configuration::Clock_Range(123'000'000),
    WLib::SPI::Configuration::Mode::Mode_3,
  };

  REQUIRE(spi_en_count == 0);
  REQUIRE(cs_sel_count == 0);
  REQUIRE(byte_count == 0);

  {
    auto&& channel = dev_i.request_channel(cfg);
    REQUIRE(spi_en_count == 1);
    REQUIRE(cs_sel_count == 0);
    REQUIRE(byte_count == 0);
    REQUIRE(channel.get_actual_clock_rate() <= cfg.get_max_clock_rate());
  }

  REQUIRE(spi_en_count == 0);
  REQUIRE(cs_sel_count == 0);
  REQUIRE(byte_count == 0);

  {
    auto&& channel = dev_i.request_channel(cfg);
    channel.select_chip().transceive(nullptr, nullptr, 10);

    REQUIRE(spi_en_count == 1);
    REQUIRE(cs_sel_count == 0);
    REQUIRE(byte_count == 10);

    auto&& chip = channel.select_chip();
    chip.transceive(nullptr, nullptr, 20);

    REQUIRE(spi_en_count == 1);
    REQUIRE(cs_sel_count == 1);
    REQUIRE(byte_count == 30);

    CHECK_THROWS(channel.select_chip().transceive(nullptr, nullptr, 25));
    chip.transceive(nullptr, nullptr, 25);

    REQUIRE(spi_en_count == 1);
    REQUIRE(cs_sel_count == 1);
    REQUIRE(byte_count == 55);

    chip.transceive(nullptr, nullptr, 30);

    REQUIRE(spi_en_count == 1);
    REQUIRE(cs_sel_count == 1);
    REQUIRE(byte_count == 85);
  }
  REQUIRE(spi_en_count == 0);
  REQUIRE(cs_sel_count == 0);
  REQUIRE(byte_count == 85);
  {
    auto&& chip = dev_i.request_channel(cfg).select_chip();
    chip.transceive(nullptr, nullptr, 5);
    chip.transceive(nullptr, nullptr, 10);
    chip.transceive(nullptr, nullptr, 5);
  }
  REQUIRE(spi_en_count == 0);
  REQUIRE(cs_sel_count == 0);
  REQUIRE(byte_count == 105);
}

TEST_CASE()
{
  int32_t cs_flag = 0;

  bool_chip_select                                  cs(cs_flag);
  WLib::SPI::Internal::unique_chip_select_wrapper_t w_cs(cs);

  REQUIRE(cs_flag == 0);

  w_cs.select();
  REQUIRE(cs_flag == 1);

  w_cs.deselect();
  REQUIRE(cs_flag == 0);

  w_cs.select();
  REQUIRE(cs_flag == 1);

  CHECK_THROWS(w_cs.select());

  w_cs.deselect();
  REQUIRE(cs_flag == 0);
}
