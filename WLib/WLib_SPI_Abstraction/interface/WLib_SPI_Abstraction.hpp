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
  class Channel_Provider;

  /// <summary>
  /// This interface abstracts the communication on the bus
  /// </summary>
  class Connection_Interface
  {
  public:
    /// <summary>
    /// Transmit and receive function for bus communication
    /// </summary>
    /// <param name="tx">Pointer to the outgoing data</param>
    /// <param name="rx">Pointer for the incoming data</param>
    /// <param name="len">Number of bytes to be sent or received</param>
    /// <details>
    /// Transmit and receive 20 bytes
    /// <code>
    /// std::byte tx[20];
    /// std::byte rx[20];
    /// // ... fill tx with data
    /// con.transceive(tx, rx, 20);
    /// // ... use the received data from rx
    /// </code>
    /// Transmit and receive 20 bytes where the buffers for outgoing and incoming data are shared.
    /// <code>
    /// std::byte tx_rx[20];
    /// // ... fill tx_rx with data
    /// con.transceive(tx_rx, tx_rx, 20);
    /// // ... use the received data from tx_rx
    /// </code>
    /// Sending 20 bytes the received bytes are not of interest and are discarded
    /// <code>
    /// std::byte tx[20];
    /// // ... fill tx with data
    /// con.transceive(tx, nullptr, 20);
    /// </code>
    /// Receive 20 bytes the outgoing data are not of interest
    /// <code>
    /// std::byte rx[20];
    /// con.transceive(nullptr, rx, 20);
    /// // ... use the received data from rx
    /// </code>
    /// Clocks of 20 bytes neither outgoing nor incoming data are of interest
    /// <code>
    /// con.transceive(nullptr, nullptr, 20);
    /// </code>
    /// Send 2 command bytes then send 4 dummy bytes followed by receive 200 data bytes
    /// <code>
    /// std::byte tx[2];
    /// std::byte rx[200];
    /// // ... fill tx with data
    /// con.transceive(tx, nullptr, 2);
    /// con.transceive(nullptr, nullptr, 4);
    /// con.transceive(nullptr, rx, 200);
    /// // ... use the received data from rx
    /// </code>
    /// </details>
    virtual void transceive(std::byte const* tx, std::byte* rx, std::size_t len) = 0;
  };

  /// <summary>
  ///  This interface abstracts the SPI hardware
  /// </summary>
  class HW_Interface
      : public Connection_Interface
      , private Utility::non_copyable_non_moveable_t
  {
  public:
    /// <summary>
    /// Requests a reference to the null_hw
    /// </summary>
    /// <returns>Reference to the null_hw</returns>
    /// <details>
    /// The null_hw is a dummy implementation of the HW_Interface
    /// </details>
    static HW_Interface& get_null_hw();

    /// <summary>
    /// The configuration object for the SPI hardware
    /// </summary>
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

      constexpr Configuration(uint32_t const& max_bautrate, Mode const& spi_mode)
          : m_max_bautrate(max_bautrate)
          , m_spi_mode(spi_mode){};

      constexpr Configuration(uint32_t const& max_bautrate,
                              uint32_t const& min_bautrate,
                              Mode const&     spi_mode)
          : m_max_bautrate(max_bautrate)
          , m_min_bautrate(min_bautrate)
          , m_spi_mode(spi_mode){};

      constexpr Configuration(uint32_t const&  max_bautrate,
                              Mode const&      spi_mode,
                              Bit_Order const& bit_order)
          : m_max_bautrate(max_bautrate)
          , m_spi_mode(spi_mode)
          , m_bit_order(bit_order){};

      constexpr Configuration(uint32_t const&  max_bautrate,
                              uint32_t const&  min_bautrate,
                              Mode const&      spi_mode,
                              Bit_Order const& bit_order)
          : m_max_bautrate(max_bautrate)
          , m_min_bautrate(min_bautrate)
          , m_spi_mode(spi_mode)
          , m_bit_order(bit_order){};

      constexpr uint32_t  get_max_bautrate() const { return this->m_max_bautrate; }
      constexpr uint32_t  get_min_bautrate() const { return this->m_min_bautrate; }
      constexpr Mode      get_spi_mode() const { return this->m_spi_mode; }
      constexpr Bit_Order get_bit_order() const { return this->m_bit_order; }

    private:
      uint32_t  m_max_bautrate = 0;
      uint32_t  m_min_bautrate = 0;
      Mode      m_spi_mode     = Mode::Mode_0;
      Bit_Order m_bit_order    = Bit_Order::MSB_first;
    };

    /// <summary>
    /// Enabels the SPI hardware with the given configuration
    /// </summary>
    /// <param name="cfg">bus configuration</param>
    /// <details>
    /// By enabling the hardware, it is reserved and must be disabled before it can be enabled
    /// again.
    /// </details>
    virtual void enable(Configuration const& cfg) = 0;

    /// <summary>
    /// Returns the currently set baudrate
    /// </summary>
    /// <returns>Bautrate</returns>
    virtual uint32_t get_actual_bautrate() const = 0;

    /// <summary>
    /// Disables the SPI hardware
    /// </summary>
    virtual void disable() = 0;
  };

  /// <summary>
  /// This interface abstracts the selection mechanism for the bus nodes
  /// </summary>
  class ChipSelect_Interface
  {
  public:
    /// <summary>
    /// Requests a reference to the null_chip_select
    /// </summary>
    /// <returns>Reference to the null_chip_select</returns>
    /// <details>
    /// The null_chip_select is a dummy implementation of the ChipSelect_Interface
    /// </details>
    static ChipSelect_Interface& get_null_chip_select();

    /// <summary>
    /// Selects the bus node
    /// </summary>
    virtual void select() = 0;

    /// <summary>
    /// Deselects the bus node
    /// </summary>
    virtual void deselect() = 0;
  };

  namespace Internal
  {
    class unique_chip_select_wrapper_t final
        : public ChipSelect_Interface
        , private Utility::non_copyable_non_moveable_t
    {
    public:
      unique_chip_select_wrapper_t(ChipSelect_Interface& chip_select)
          : m_chip_select(chip_select)
      {
      }

      ~unique_chip_select_wrapper_t() = default;

      void select() override
      {
        if (this->m_sel_count != 0)
          mutible_selection_error_handler();

        ++this->m_sel_count;
        this->m_chip_select.select();
      };
      void deselect() override
      {
        --this->m_sel_count;
        this->m_chip_select.deselect();
      };

      ChipSelect_Interface& get_native_handle() { return this->m_chip_select; }

    private:
      void mutible_selection_error_handler();

      ChipSelect_Interface& m_chip_select;
      uint32_t              m_sel_count = 0;
    };
  }    // namespace Internal

  class Channel_Provider: private Utility::non_copyable_non_moveable_t
  {
    class Channel_Handle final: private Utility::non_copyable_non_moveable_t
    {
      class Connection_Handle final
          : public Connection_Interface
          , private Utility::non_copyable_non_moveable_t
      {
      public:
        Connection_Handle(Channel_Handle& channel)
            : m_chip_sel(channel.m_chip_select)
            , m_spi(*channel.m_spi_hw)
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

      class Connection_Handle_HW final
          : public Connection_Interface
          , private Utility::non_copyable_non_moveable_t
      {
      public:
        Connection_Handle_HW(Channel_Handle&& channel)
            : m_chip_sel(channel.m_chip_select.get_native_handle())
            , m_spi(*channel.m_spi_hw)
        {
          channel.m_chip_select.select();
          channel.m_spi_hw = &HW_Interface::get_null_hw();
        }
        ~Connection_Handle_HW()
        {
          this->m_chip_sel.deselect();
          this->m_spi.disable();
        }

        void transceive(std::byte const* tx, std::byte* rx, std::size_t len) override
        {
          return this->m_spi.transceive(tx, rx, len);
        }

      private:
        ChipSelect_Interface& m_chip_sel;
        HW_Interface&         m_spi;
      };

    public:
      Channel_Handle(ChipSelect_Interface&              chip_sel,
                     HW_Interface&                      spi,
                     HW_Interface::Configuration const& cfg)
          : m_chip_select(chip_sel)
          , m_spi_hw(&spi)
      {
        this->m_spi_hw->enable(cfg);
      }

      ~Channel_Handle() { this->m_spi_hw->disable(); }

      Connection_Handle select_chip() & { return Connection_Handle(*this); }

      Connection_Handle_HW select_chip() && { return Connection_Handle_HW(std::move(*this)); }

      uint32_t get_actual_bautrate() const { return this->m_spi_hw->get_actual_bautrate(); }

    private:
      Internal::unique_chip_select_wrapper_t m_chip_select;
      HW_Interface*                          m_spi_hw;
    };

  public:
    using channel_handle_t = Channel_Handle;

    Channel_Provider(ChipSelect_Interface& chip_sel, HW_Interface& spi)
        : m_chip_select(chip_sel)
        , m_spi_hw(spi)
    {
    }

    channel_handle_t request_channel(HW_Interface::Configuration const& cfg)
    {
      return channel_handle_t(this->m_chip_select, this->m_spi_hw, cfg);
    }

  private:
    ChipSelect_Interface& m_chip_select;
    HW_Interface&         m_spi_hw;
  };
}    // namespace WLib::SPI

#endif
