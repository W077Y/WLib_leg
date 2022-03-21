#include <iostream>
#include <WLib_SPI_Abstraction.hpp>
#include <ut_catch.hpp>

class cout_chip_select final: public WLib::SPI::ChipSelect_Interface
{
public:
  cout_chip_select(std::string&& name)
      : m_name(name)
  {
  }

private:
  void select() override { std::cout << " select   --> " << this->m_name << std::endl; };
  void deselect() override { std::cout << " deselect  --> " << this->m_name << std::endl; };

  std::string m_name = "";
};

class cout_spi_dummy final: public WLib::SPI::HW_Interface
{
public:
  cout_spi_dummy(std::string&& name)
      : m_name(name)
  {
  }

private:
  void transceive(std::byte const*, std::byte*, std::size_t len) override
  {
    std::cout << "  transceive  --> " << this->m_name << " " << len << " bytes" << std::endl;
  }

  void enable(device_cfg_t const& cfg) override
  {
    std::cout << "enable  --> " << this->m_name << " with max bautrate" << cfg.get_max_bautrate()
              << std::endl;
    this->m_bautrate = cfg.get_max_bautrate() - 1;
  };

  uint32_t get_actual_bautrate() const override { return this->m_bautrate; }

  void disable() override
  {
    std::cout << "disable   --> " << this->m_name << std::endl;
    this->m_bautrate = 0;
  }

  std::string m_name     = "";
  uint32_t    m_bautrate = 0;
};

WLib::SPI::ChipSelect_Interface& get_chip_select_PA3()
{
  static cout_chip_select obj("CS PA3 (eeprom 1)");
  return obj;
}

WLib::SPI::HW_Interface& get_spi_1()
{
  static cout_spi_dummy obj("SPI 1");
  return obj;
}

WLib::SPI::Device_Interface& get_eeprom_1()
{
  static WLib::SPI::Device obj{ get_chip_select_PA3(), get_spi_1() };
  return obj;
}


TEST_CASE()
{
  WLib::SPI::Device_Interface& eeprom_1 = get_eeprom_1();
  constexpr WLib::SPI::Device_Configuration cfg{ 123'000'000,
                                                 WLib::SPI::Device_Configuration::Mode::Mode_3 };

  REQUIRE(eeprom_1.get_session(cfg).get_actual_bautrate() <= 123'000'000);
  {
    auto&& ses = eeprom_1.get_session(cfg);
    ses.select_chip().transceive(nullptr, nullptr, 10);
    auto&& chip = ses.select_chip();
    chip.transceive(nullptr, nullptr, 20);
    ses.select_chip().transceive(nullptr, nullptr, 25);
    chip.transceive(nullptr, nullptr, 30);
  }
}
