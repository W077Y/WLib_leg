#pragma once
#ifndef WLIB_SPI_ABSTRACTION_HPP
#define WLIB_SPI_ABSTRACTION_HPP

#include <WLib_Utility.hpp>
#include <cstddef>
#include <cstdint>

namespace WLib::SPI
{
  class Connection_Interface
  {
  public:
    virtual void transceive(std::byte const* tx, std::byte* rx, std::size_t len) = 0;
  };

  class HW_Interface
      : public Connection_Interface
      , private Utility::non_copyable_non_moveable_t
  {
  public:
    class Configuration
    {
    public:
      enum class Mode
      {
        Mode_0,
        Mode_1,
        Mode_2,
        Mode_3,
        CPOL_0_CPHA_0 = Mode_0,
        CPOL_0_CPHA_1 = Mode_1,
        CPOL_1_CPHA_0 = Mode_2,
        CPOL_1_CPHA_1 = Mode_3,
      };

      enum class Bit_Order
      {
        MSB_first,
        LSB_first,
      };

      constexpr Configuration(uint32_t const& max_bautrate, Mode const& spi_mode)
          : m_max_bautrate(max_bautrate)
          , m_spi_mode(spi_mode){};

      constexpr Configuration(uint32_t const&  max_bautrate,
                              Mode const&      spi_mode,
                              Bit_Order const& bit_order)
          : m_max_bautrate(max_bautrate)
          , m_spi_mode(spi_mode)
          , m_bit_order(bit_order){};

      constexpr uint32_t  get_max_bautrate() const { return this->m_max_bautrate; }
      constexpr Mode      get_spi_mode() const { return this->m_spi_mode; }
      constexpr Bit_Order get_bit_order() const { return this->m_bit_order; }

    private:
      uint32_t  m_max_bautrate = 0;
      Mode      m_spi_mode     = Mode::Mode_0;
      Bit_Order m_bit_order    = Bit_Order::MSB_first;
    };

    virtual void     enable(Configuration const& cfg) = 0;
    virtual uint32_t get_actual_bautrate() const      = 0;
    virtual void     disable()                        = 0;
  };

  class ChipSelect_Interface
  {
  public:
    virtual void select()   = 0;
    virtual void deselect() = 0;
  };

  namespace Internal
  {
    class recursive_chip_select_wrapper_t final
        : public ChipSelect_Interface
        , private Utility::non_copyable_non_moveable_t
    {
    public:
      recursive_chip_select_wrapper_t(ChipSelect_Interface& chip_select)
          : m_chip_select(chip_select)
      {
      }

      ~recursive_chip_select_wrapper_t() = default;

      void select() override
      {
        if (this->m_sel_count++ == 0)
          this->m_chip_select.select();
      };
      void deselect() override
      {
        if (--this->m_sel_count == 0)
          this->m_chip_select.deselect();
      };

    private:
      ChipSelect_Interface& m_chip_select;
      uint32_t              m_sel_count = 0;
    };
  }    // namespace Internal

  class Channel_Provider_Interface: private Utility::non_copyable_non_moveable_t
  {
    class Channel_Handle final: private Utility::non_copyable_non_moveable_t
    {
      class Connection_Handle final
          : public Connection_Interface
          , private Utility::non_copyable_non_moveable_t
      {
      public:
        Connection_Handle(ChipSelect_Interface& chip_sel, Connection_Interface& spi_hw)
            : m_chip_sel(chip_sel)
            , m_spi(spi_hw)
        {
          this->m_chip_sel.select();
        }
        ~Connection_Handle() { this->m_chip_sel.deselect(); }

        void transceive(std::byte const* tx, std::byte* rx, std::size_t len) override
        {
          return this->m_spi.transceive(tx, rx, len);
        }

      private:
        ChipSelect_Interface& m_chip_sel;
        Connection_Interface& m_spi;
      };

    public:
      using connection_handle_t = Connection_Handle;

      Channel_Handle(ChipSelect_Interface&              chip_sel,
                     HW_Interface&                      spi,
                     HW_Interface::Configuration const& cfg)
          : m_chip_select(chip_sel)
          , m_spi_hw(spi)
      {
        this->m_spi_hw.enable(cfg);
      }

      ~Channel_Handle() { this->m_spi_hw.disable(); }

      connection_handle_t select_chip() &
      {
        return connection_handle_t(this->m_chip_select, this->m_spi_hw);
      }

      uint32_t get_actual_bautrate() const { return this->m_spi_hw.get_actual_bautrate(); }

    private:
      Internal::recursive_chip_select_wrapper_t m_chip_select;
      HW_Interface&                             m_spi_hw;
    };

  public:
    using channel_handle_t = Channel_Handle;

    virtual channel_handle_t request_channel(HW_Interface::Configuration const&) & = 0;

  private:
  };

  class Channel_Provider final: public Channel_Provider_Interface
  {
  public:
    Channel_Provider(ChipSelect_Interface& chip_sel, HW_Interface& spi)
        : m_chip_select(chip_sel)
        , m_spi_hw(spi)
    {
    }

    channel_handle_t request_channel(HW_Interface::Configuration const& cfg) & override
    {
      return channel_handle_t(this->m_chip_select, this->m_spi_hw, cfg);
    }

  private:
    ChipSelect_Interface& m_chip_select;
    HW_Interface&         m_spi_hw;
  };
}    // namespace WLib::SPI

#endif
