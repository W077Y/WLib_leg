#include <WLib_SPI_Abstraction.hpp>
#include <iostream>
#include <ut_catch.hpp>

class bool_chip_select final: public WLib::SPI::ChipSelect_Interface
{
public:
  bool_chip_select(bool& cs)
      : m_flag(cs)
  {
  }

private:
  void select() override { this->m_flag = true; };
  void deselect() override { this->m_flag = false; };

  bool& m_flag;
};

class count_spi_dummy final: public WLib::SPI::HW_Interface
{
public:
  count_spi_dummy(std::size_t& count, bool& en)
      : m_count(count)
      , m_en(en)
  {
  }

private:
  void transceive(std::byte const*, std::byte*, std::size_t len) override { this->m_count += len; }

  void enable(Configuration const& cfg) override
  {
    this->m_en       = true;
    this->m_bautrate = cfg.get_max_bautrate();
  };

  uint32_t get_actual_bautrate() const override { return this->m_bautrate; }

  void disable() override
  {
    this->m_en       = false;
    this->m_bautrate = 0;
  }

  std::size_t& m_count;
  bool&        m_en;
  uint32_t     m_bautrate = 0;
};

TEST_CASE()
{
  bool        flag_spi   = false;
  bool        flag_cs    = false;
  std::size_t byte_count = 0;

  count_spi_dummy   spi_obj(byte_count, flag_spi);
  bool_chip_select  cs_obj(flag_cs);
  WLib::SPI::Channel_Provider dev_obj(cs_obj, spi_obj);

  WLib::SPI::Channel_Provider_Interface& dev_i = dev_obj;

  constexpr WLib::SPI::HW_Interface::Configuration cfg{
    123'000'000,
    WLib::SPI::HW_Interface::Configuration::Mode::Mode_3,
  };

  REQUIRE(!flag_spi);
  REQUIRE(!flag_cs);
  REQUIRE(byte_count == 0);

  {
    auto&& session = dev_i.request_channel(cfg);
    REQUIRE(flag_spi);
    REQUIRE(!flag_cs);
    REQUIRE(byte_count == 0);
    REQUIRE(session.get_actual_bautrate() <= cfg.get_max_bautrate());
  }

  REQUIRE(!flag_spi);
  REQUIRE(!flag_cs);
  REQUIRE(byte_count == 0);

  {
    auto&& session = dev_i.request_channel(cfg);
    session.select_chip().transceive(nullptr, nullptr, 10);

    REQUIRE(flag_spi);
    REQUIRE(!flag_cs);
    REQUIRE(byte_count == 10);

    auto&& chip = session.select_chip();
    chip.transceive(nullptr, nullptr, 20);

    REQUIRE(flag_spi);
    REQUIRE(flag_cs);
    REQUIRE(byte_count == 30);

    session.select_chip().transceive(nullptr, nullptr, 25);

    REQUIRE(flag_spi);
    REQUIRE(flag_cs);
    REQUIRE(byte_count == 55);

    chip.transceive(nullptr, nullptr, 30);

    REQUIRE(flag_spi);
    REQUIRE(flag_cs);
    REQUIRE(byte_count == 85);
  }
  REQUIRE(!flag_spi);
  REQUIRE(!flag_cs);
  REQUIRE(byte_count == 85);
}

TEST_CASE()
{
  bool cs_flag = false;

  bool_chip_select                                     cs(cs_flag);
  WLib::SPI::Internal::recursive_chip_select_wrapper_t w_cs(cs);

  REQUIRE(!cs_flag);

  w_cs.select();
  REQUIRE(cs_flag);
  
  w_cs.deselect();
  REQUIRE(!cs_flag);

  w_cs.select();
  REQUIRE(cs_flag);

  w_cs.select();
  REQUIRE(cs_flag);

  w_cs.select();
  REQUIRE(cs_flag);

  w_cs.select();
  REQUIRE(cs_flag);

  w_cs.select();
  REQUIRE(cs_flag);

  w_cs.deselect();
  REQUIRE(cs_flag);

  w_cs.deselect();
  REQUIRE(cs_flag);

  w_cs.deselect();
  REQUIRE(cs_flag);
  
  w_cs.deselect();
  REQUIRE(cs_flag);

  w_cs.deselect();
  REQUIRE(!cs_flag);
}
