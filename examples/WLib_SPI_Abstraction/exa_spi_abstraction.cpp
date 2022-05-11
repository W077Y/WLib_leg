#include <WLib_SPI_Abstraction.hpp>
#include <iostream>

namespace spi_abst = WLib::SPI_Abstraction::Sync;

class cout_chip_select final: public spi_abst::ChipSelect_Interface
{
public:
  cout_chip_select(const char* name)
    : m_name(name)
  {
  }

private:
  void select() override { std::cout << "  " << this->m_name << " selected" << std::endl; };
  void deselect() override { std::cout << "  " << this->m_name << " deselected" << std::endl; };

  const char* m_name;
};

class cout_spi_dummy final: public spi_abst::HW_Interface
{
public:
  cout_spi_dummy(const char* name)
    : m_name(name)
  {
  }

private:
  void transceive(std::byte const*, std::byte*, std::size_t len) override
  {
    std::cout << "    " << this->m_name << " transceive " << len << " bytes" << std::endl;
  }

  void enable(cfg_t const& cfg) override
  {
    std::cout << this->m_name << " enabled with bautrate " << cfg.get_max_clock_rate() << std::endl;
  };

  uint32_t get_actual_clock_rate() const override { return 0; }

  void disable() override { std::cout << this->m_name << " disabled" << std::endl; }

  const char* m_name;
};

spi_abst::ChipSelect_Interface& get_chip_select_eeprom_1()
{
  static cout_chip_select obj("EEPROM_1");
  return obj;
}

spi_abst::HW_Interface& get_spi_1()
{
  static cout_spi_dummy obj("SPI_1");
  return obj;
}

spi_abst::Channel_Provider& get_eeprom_1()
{
  static spi_abst::Channel_Provider obj{get_chip_select_eeprom_1(), get_spi_1()};
  return obj;
}

int main()
{
  constexpr spi_abst::Configuration cfg{spi_abst::Configuration::Clock_Range(123'000'000), spi_abst::Configuration::Mode::Mode_3};

  {
    auto&& channel = get_eeprom_1().request_channel(cfg);
    channel.select_chip().transceive(nullptr, nullptr, 10);
    {
      auto&& connection = channel.select_chip();
      connection.transceive(nullptr, nullptr, 20);
      connection.transceive(nullptr, nullptr, 30);
    }
  }
  {
    auto&& connection = get_eeprom_1().request_channel(cfg).select_chip();
    connection.transceive(nullptr, nullptr, 12);
    connection.transceive(nullptr, nullptr, 21);
  }
  return 0;
}
