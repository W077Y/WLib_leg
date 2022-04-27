#pragma once
#ifndef WLIB_SPI_ABSTRACTION_HPP
#define WLIB_SPI_ABSTRACTION_HPP

#include <WLib_Utility.hpp>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace WLib::SPI
{
  class Connection_Interface;
  class HW_Interface;
  class ChipSelect_Interface;

  class Configuration;
  class Channel_Provider;
  class Channel_Handle;
  class Connection_Handle;

  class Configuration
  {
  public:
    enum class Mode
    {
      Mode_0        = 0,
      Mode_1        = 1,
      Mode_2        = 2,
      Mode_3        = 3,
      CPOL_0_CPHA_0 = Mode_0,
      CPOL_0_CPHA_1 = Mode_1,
      CPOL_1_CPHA_0 = Mode_2,
      CPOL_1_CPHA_1 = Mode_3,
    };

    enum class Bit_Order
    {
      MSB_first = 0,
      LSB_first = 1,
    };

    class Clock_Range
    {
    public:
      explicit constexpr Clock_Range(uint32_t const& from, uint32_t const& to)
        : m_min(std::min(from, to))
        , m_max(std::max(from, to))
      {
      }

      explicit constexpr Clock_Range(uint32_t const& up_to)
        : m_max(up_to)
      {
      }

      constexpr uint32_t get_min() const
      {
        return this->m_min;
      }
      constexpr uint32_t get_max() const
      {
        return this->m_max;
      }

    private:
      uint32_t m_min = 0;
      uint32_t m_max = 0;
    };

    constexpr Configuration(Clock_Range const& clock_speed, Mode const& spi_mode, Bit_Order const& bit_order)
      : m_clock_speed_range(clock_speed)
      , m_spi_mode(spi_mode)
      , m_bit_order(bit_order){};

    constexpr Configuration(Clock_Range const& clock_speed, Mode const& spi_mode)
      : m_clock_speed_range(clock_speed)
      , m_spi_mode(spi_mode){};

    constexpr uint32_t get_min_clock_rate() const
    {
      return this->m_clock_speed_range.get_min();
    }
    constexpr uint32_t get_max_clock_rate() const
    {
      return this->m_clock_speed_range.get_max();
    }
    constexpr Mode get_spi_mode() const
    {
      return this->m_spi_mode;
    }
    constexpr Bit_Order get_bit_order() const
    {
      return this->m_bit_order;
    }

  private:
    Clock_Range m_clock_speed_range = Clock_Range(0);
    Mode        m_spi_mode          = Mode::Mode_0;
    Bit_Order   m_bit_order         = Bit_Order::MSB_first;
  };

  class Connection_Interface
  {
  public:
    static Connection_Interface& get_null();

    virtual void transceive(std::byte const* tx, std::byte* rx, std::size_t len) = 0;
  };

  class HW_Interface: public Connection_Interface
  {
  public:
    using cfg_t = Configuration;

    static HW_Interface& get_null();

    virtual void     enable(cfg_t const& cfg)      = 0;
    virtual uint32_t get_actual_clock_rate() const = 0;
    virtual void     disable()                     = 0;
  };

  class ChipSelect_Interface
  {
  public:
    static ChipSelect_Interface& get_null();

    virtual void select()   = 0;
    virtual void deselect() = 0;
  };

  namespace Internal
  {
    class unique_chip_select_wrapper_t final
      : private Utility::non_copyable_non_moveable_t
      , public ChipSelect_Interface
    {
    public:
      unique_chip_select_wrapper_t(ChipSelect_Interface& chip_select);

      void select() override;
      void deselect() override;

      ChipSelect_Interface& get_native_handle();

    private:
      void mutible_selection_error_handler();

      ChipSelect_Interface& m_chip_select;
      uint32_t              m_sel_count = 0;
    };
  }    // namespace Internal

  class Channel_Provider: private Utility::non_copyable_non_moveable_t
  {
  public:
    using channel_handle_t = Channel_Handle;

    Channel_Provider(ChipSelect_Interface& chip_sel, HW_Interface& spi);

    channel_handle_t request_channel(Configuration const& cfg);

  private:
    ChipSelect_Interface& m_chip_select;
    HW_Interface&         m_spi_hw;
  };

  class Channel_Handle final: private Utility::non_copyable_non_moveable_t
  {
  public:
    using connection_handle_t = Connection_Handle;

    Channel_Handle(ChipSelect_Interface& chip_sel, HW_Interface& spi, Configuration const& cfg);
    ~Channel_Handle();

    connection_handle_t select_chip() &;
    connection_handle_t select_chip() &&;
    uint32_t            get_actual_clock_rate() const;

  private:
    friend connection_handle_t;

    Internal::unique_chip_select_wrapper_t m_chip_select;
    HW_Interface*                          m_spi_hw;
  };

  class Connection_Handle final
    : private Utility::non_copyable_non_moveable_t
    , public Connection_Interface
  {
  public:
    Connection_Handle(Channel_Handle& channel);
    Connection_Handle(Channel_Handle&& channel);
    ~Connection_Handle();

    void transceive(std::byte const* tx, std::byte* rx, std::size_t len) override;

  private:
    ChipSelect_Interface& m_cs;
    HW_Interface&         m_hw;
    Connection_Interface& m_con;
  };

}    // namespace WLib::SPI

#endif
