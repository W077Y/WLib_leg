#include <WLib_SPI_Abstraction.hpp>

class cout_chip_select final: public WLib::SPI::ChipSelect_Interface
{
private:
  void select() override { };
  void deselect() override { };
};

class cout_spi_dummy final: public WLib::SPI::HW_Interface
{
private:
  void transceive(std::byte const*, std::byte*, std::size_t len) override
  {
  }

  void enable(device_cfg_t const& cfg) override
  {
  };

  uint32_t get_actual_bautrate() const override { return 0; }

  void disable() override
  {
  }
};

WLib::SPI::ChipSelect_Interface& get_chip_select_PA3()
{
  static cout_chip_select obj;
  return obj;
}

WLib::SPI::HW_Interface& get_spi_1()
{
  static cout_spi_dummy obj;
  return obj;
}

WLib::SPI::Device_Interface& get_eeprom_1()
{
  static WLib::SPI::Device obj{ get_chip_select_PA3(), get_spi_1() };
  return obj;
}


int main()
{
  constexpr WLib::SPI::Device_Configuration cfg{ 123'000'000,
                                                 WLib::SPI::Device_Configuration::Mode::Mode_3 };


  WLib::SPI::Device_Interface& eeprom_1 = get_eeprom_1();

  {
    auto&& ses = eeprom_1.get_session(cfg);
    ses.select_chip().transceive(nullptr, nullptr, 10);
    auto&& chip = ses.select_chip();
    chip.transceive(nullptr, nullptr, 20);
    ses.select_chip().transceive(nullptr, nullptr, 25);
    chip.transceive(nullptr, nullptr, 30);
  }
  return 0;
}
