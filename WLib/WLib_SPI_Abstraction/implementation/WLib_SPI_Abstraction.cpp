#include <WLib_SPI_Abstraction.hpp>

namespace WLib::SPI
{
  class mutible_selection_exception_t
  {
  public:
    virtual char const* what() const noexcept { return "multible chip select usage error"; }
  };

  namespace
  {
    class null_cs_t: public ChipSelect_Interface
    {
    public:
      virtual void select() override{};
      virtual void deselect() override{};
    };

    class null_hw_t: public HW_Interface
    {
    public:
      virtual void     enable(Configuration const& cfg){};
      virtual uint32_t get_actual_bautrate() const { return 0; };
      virtual void     disable(){};
      virtual void     transceive(std::byte const*, std::byte*, std::size_t){};
    };
  }    // namespace

  void Internal::unique_chip_select_wrapper_t::mutible_selection_error_handler()
  {
    throw mutible_selection_exception_t();
  }

  HW_Interface& HW_Interface::get_null_hw()
  {
    static null_hw_t obj;
    return obj;
  }

  ChipSelect_Interface& ChipSelect_Interface::get_null_chip_select()
  {
    static null_cs_t obj;
    return obj;
  }
}    // namespace WLib::SPI
