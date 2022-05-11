#include <WLib_SPI_Abstraction.hpp>

namespace WLib::SPI_Abstraction::Sync
{
  class mutible_selection_exception_t
  {
  public:
    virtual char const* what() const noexcept { return "multible chip select usage error"; }
  };

  namespace
  {
    class dummy_connection_t final: public Connection_Interface
    {
    public:
      virtual void transceive(std::byte const*, std::byte*, std::size_t) override{};
    };

    class dummy_cs_t final: public ChipSelect_Interface
    {
    public:
      virtual void select() override{};
      virtual void deselect() override{};
    };

    class dummy_hw_t final: public HW_Interface
    {
    public:
      virtual void     enable(Configuration const& cfg) override{};
      virtual uint32_t get_actual_clock_rate() const override { return 0; };
      virtual void     disable() override{};
      virtual void     transceive(std::byte const*, std::byte*, std::size_t) override{};
    };

    dummy_connection_t dummy_connection;
    dummy_cs_t         dummy_cs;
    dummy_hw_t         dummy_hw;
  }    // namespace

  namespace Internal
  {
    unique_chip_select_wrapper_t::unique_chip_select_wrapper_t(ChipSelect_Interface& chip_select)
      : m_chip_select(chip_select)
    {
    }

    void unique_chip_select_wrapper_t::select()
    {
      if (this->m_sel_count != 0)
        mutible_selection_error_handler();

      ++this->m_sel_count;
      this->m_chip_select.select();
    }

    void unique_chip_select_wrapper_t::deselect()
    {
      --this->m_sel_count;
      this->m_chip_select.deselect();
    }

    ChipSelect_Interface& unique_chip_select_wrapper_t::get_native_handle() { return this->m_chip_select; }

    void unique_chip_select_wrapper_t::mutible_selection_error_handler() { throw mutible_selection_exception_t(); }
  }    // namespace Internal

  Connection_Interface& Connection_Interface::get_dummy() { return dummy_connection; }
  HW_Interface&         HW_Interface::get_dummy() { return dummy_hw; }
  ChipSelect_Interface& ChipSelect_Interface::get_dummy() { return dummy_cs; }

  Channel_Handle::Channel_Handle(ChipSelect_Interface& chip_sel, HW_Interface& spi, Configuration const& cfg)
    : m_chip_select(chip_sel)
    , m_spi_hw(&spi)
  {
    this->m_spi_hw->enable(cfg);
  }

  Channel_Handle::~Channel_Handle() { this->m_spi_hw->disable(); }

  Channel_Handle::connection_handle_t Channel_Handle::select_chip() &
  {
    return Channel_Handle::connection_handle_t(*this);
  }

  Channel_Handle::connection_handle_t Channel_Handle::select_chip() &&
  {
    return Channel_Handle::connection_handle_t(std::move(*this));
  }

  uint32_t Channel_Handle::get_actual_clock_rate() const { return this->m_spi_hw->get_actual_clock_rate(); }

  Connection_Handle::Connection_Handle(Channel_Handle& channel)
    : m_cs(channel.m_chip_select)
    , m_hw(HW_Interface::get_dummy())
    , m_con(*channel.m_spi_hw)
  {
    channel.m_chip_select.select();
  }

  Connection_Handle::Connection_Handle(Channel_Handle&& channel)
    : m_cs(channel.m_chip_select.get_native_handle())
    , m_hw(*channel.m_spi_hw)
    , m_con(*channel.m_spi_hw)
  {
    channel.m_chip_select.select();
    channel.m_spi_hw = &HW_Interface::get_dummy();
  }

  Connection_Handle::~Connection_Handle()
  {
    this->m_cs.deselect();
    this->m_hw.disable();
  }

  void Connection_Handle::transceive(std::byte const* tx, std::byte* rx, std::size_t len)
  {
    return this->m_con.transceive(tx, rx, len);
  }

  Channel_Provider::Channel_Provider(ChipSelect_Interface& chip_sel, HW_Interface& spi)
    : m_chip_select(chip_sel)
    , m_spi_hw(spi)
  {
  }

  Channel_Provider::channel_handle_t Channel_Provider::request_channel(Configuration const& cfg)
  {
    return Channel_Provider::channel_handle_t(this->m_chip_select, this->m_spi_hw, cfg);
  }
}    // namespace WLib::SPI_Abstraction::Sync
