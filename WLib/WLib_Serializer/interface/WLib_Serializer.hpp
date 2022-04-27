#pragma once
#ifndef WLIB_SERIALIZER_HPP
#define WLIB_SERIALIZER_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace WLib
{
  enum class ByteOrder
  {
    little_endian,
    big_endian,
    native = little_endian,
  };

  class byte_sink_t
  {
  };

  template <typename T> constexpr bool is_byte_sink_v = std::is_base_of_v<byte_sink_t, T>;

  class byte_source_t
  {
  };

  template <typename T> constexpr bool is_byte_source_v = std::is_base_of_v<byte_source_t, T>;

  class byte_sink_ptr: public byte_sink_t
  {
  public:
    constexpr byte_sink_ptr(std::byte* ptr)
      : m_pos(ptr)
    {
    }

    constexpr void operator()(std::byte const& val) { *this->m_pos++ = val; }

  private:
    std::byte* m_pos;
  };

  class byte_source_ptr: public byte_source_t
  {
  public:
    constexpr byte_source_ptr(std::byte const* ptr)
      : m_pos(ptr)
    {
    }

    constexpr std::byte operator()() { return *this->m_pos++; }

  private:
    std::byte const* m_pos;
  };

  class byte_sink_ext_buffer: public byte_sink_t
  {
  public:
    constexpr byte_sink_ext_buffer(std::byte* begin, std::byte const* end)
      : m_beg(begin)
      , m_pos(begin)
      , m_end(end)
    {
    }

    template <std::size_t N>
    constexpr byte_sink_ext_buffer(std::byte (&buffer)[N])
      : m_beg(buffer)
      , m_pos(buffer)
      , m_end(buffer + N)
    {
    }

    constexpr void operator()(std::byte const& val)
    {
      if (this->m_pos == this->m_end)
        return;
      *this->m_pos++ = val;
    }

    constexpr std::byte const* get_begin() const { return this->m_beg; }
    constexpr std::byte const* get_pos() const { return this->m_pos; }
    constexpr std::byte const* get_end() const { return this->m_end; }
    constexpr std::size_t      get_index() const { return this->m_pos - this->m_beg; }

    constexpr void set_index(std::size_t const& idx)
    {
      this->m_pos = this->m_beg + idx;
      this->range_pos_ptr();
    }

    constexpr void skip_bytes(std::size_t const& number_of_bytes)
    {
      this->m_pos += number_of_bytes;
      this->range_pos_ptr();
    }

  private:
    constexpr void range_pos_ptr()
    {
      if (this->m_pos > this->m_end)
        this->m_pos -= this->m_pos - this->m_end;
    }

    std::byte*       m_beg;
    std::byte*       m_pos;
    std::byte const* m_end;
  };

  class byte_source_ext_buffer: public byte_source_t
  {
  public:
    constexpr byte_source_ext_buffer(std::byte const* begin, std::byte const* end)
      : m_beg(begin)
      , m_pos(begin)
      , m_end(end)
    {
    }

    template <std::size_t N>
    constexpr byte_source_ext_buffer(std::byte const (&buffer)[N])
      : m_beg(buffer)
      , m_pos(buffer)
      , m_end(buffer + N)
    {
    }

    constexpr std::byte operator()()
    {
      if (this->m_pos == this->m_end)
        return static_cast<std::byte>(0);
      return *this->m_pos++;
    }

    constexpr std::byte const* get_begin() const { return this->m_beg; }
    constexpr std::byte const* get_pos() const { return this->m_pos; }
    constexpr std::byte const* get_end() const { return this->m_end; }
    constexpr std::size_t      get_index() const { return this->m_pos - this->m_beg; }

    constexpr void set_index(std::size_t const& idx)
    {
      this->m_pos = this->m_beg + idx;
      this->range_pos_ptr();
    }

    constexpr void skip_bytes(std::size_t const& number_of_bytes)
    {
      this->m_pos += number_of_bytes;
      this->range_pos_ptr();
    }

  private:
    constexpr void range_pos_ptr()
    {
      if (this->m_pos > this->m_end)
        this->m_pos = this->m_end;
    }

    std::byte const* m_beg;
    std::byte const* m_pos;
    std::byte const* m_end;
  };

  namespace Internal
  {
    struct not_seriailizeable_type
    {
      static constexpr bool is_serializable = false;
    };

    template <typename T, std::size_t min_size, std::size_t max_size> struct template_serializeable_type
    {
      using type_t                                              = std::remove_cv_t<T>;
      static constexpr bool        is_serializable              = true;
      static constexpr std::size_t minimal_serialized_size      = min_size;
      static constexpr std::size_t maximal_serialized_size      = max_size;
      static constexpr bool        has_constant_serialized_size = minimal_serialized_size == maximal_serialized_size;
    };

    template <typename T> struct native_serializeable_type: public template_serializeable_type<T, sizeof(T), sizeof(T)>
    {
      using typename template_serializeable_type<T, sizeof(T), sizeof(T)>::type_t;
      using template_serializeable_type<T, sizeof(T), sizeof(T)>::maximal_serialized_size;

      template <typename byte_sink_t>
      static constexpr void ser(byte_sink_t& sink, type_t const& value, ByteOrder const& byte_order = ByteOrder::native)
      {
        if (byte_order == ByteOrder::native)
        {
          for (std::size_t i = 0; i < maximal_serialized_size;)
          {
            sink(reinterpret_cast<std::byte const*>(&value)[i++]);
          }
        }
        else
        {
          for (std::size_t i = maximal_serialized_size; i > 0;)
          {
            sink(reinterpret_cast<std::byte const*>(&value)[--i]);
          }
        }
      }

      template <typename byte_source_t>
      static constexpr type_t deser(byte_source_t& source, ByteOrder const& byte_order = ByteOrder::native)
      {
        type_t ret;
        if (byte_order == ByteOrder::native)
        {
          for (std::size_t i = 0; i < maximal_serialized_size;)
          {
            reinterpret_cast<std::byte*>(&ret)[i++] = source();
          }
        }
        else
        {
          for (std::size_t i = maximal_serialized_size; i > 0;)
          {
            reinterpret_cast<std::byte*>(&ret)[--i] = source();
          }
        }
        return ret;
      }
    };

    template <typename T> constexpr bool is_native_serializable_v = false;

    template <> constexpr bool is_native_serializable_v<std::byte> = true;
    template <> constexpr bool is_native_serializable_v<char>      = true;
    template <> constexpr bool is_native_serializable_v<char16_t>  = true;
    template <> constexpr bool is_native_serializable_v<char32_t>  = true;

    template <> constexpr bool is_native_serializable_v<signed char>      = true;
    template <> constexpr bool is_native_serializable_v<signed short>     = true;
    template <> constexpr bool is_native_serializable_v<signed int>       = true;
    template <> constexpr bool is_native_serializable_v<signed long>      = true;
    template <> constexpr bool is_native_serializable_v<signed long long> = true;

    template <> constexpr bool is_native_serializable_v<unsigned char>      = true;
    template <> constexpr bool is_native_serializable_v<unsigned short>     = true;
    template <> constexpr bool is_native_serializable_v<unsigned int>       = true;
    template <> constexpr bool is_native_serializable_v<unsigned long>      = true;
    template <> constexpr bool is_native_serializable_v<unsigned long long> = true;

    template <> constexpr bool is_native_serializable_v<float>       = true;
    template <> constexpr bool is_native_serializable_v<double>      = true;
    template <> constexpr bool is_native_serializable_v<long double> = true;

    constexpr std::size_t sum(std::initializer_list<std::size_t> const V)
    {
      std::size_t ret = 0;
      for (std::size_t item : V)
        ret += item;
      return ret;
    }
  }    // namespace Internal

  template <typename T>
  struct serializer_traits
    : public std::conditional_t<Internal::is_native_serializable_v<T>, Internal::native_serializeable_type<T>, Internal::not_seriailizeable_type>
  {
  };

  template <typename T> constexpr bool is_serializeable_v = serializer_traits<T>::is_serializable;

  template <typename T>
  constexpr bool has_constant_serialized_size_v = serializer_traits<T>::has_constant_serialized_size;

  template <typename... Ts>
  constexpr std::size_t minimal_serialized_size_v = Internal::sum({serializer_traits<Ts>::minimal_serialized_size...});

  template <typename... Ts>
  constexpr std::size_t maximal_serialized_size_v = Internal::sum({serializer_traits<Ts>::maximal_serialized_size...});

  template <typename T, typename sink_t>
  constexpr std::enable_if_t<is_serializeable_v<T> && is_byte_sink_v<sink_t>, void> serialize(
    sink_t& sink, T const& value, ByteOrder const& byte_order = ByteOrder::native)
  {
    return serializer_traits<T>::ser(sink, value, byte_order);
  }

  template <typename T, typename source_t>
  constexpr std::enable_if_t<is_serializeable_v<T> && is_byte_source_v<source_t>, T> deserialize(source_t& source,
                                                                                                 ByteOrder const& byte_order = ByteOrder::native)
  {
    return serializer_traits<T>::deser(source, byte_order);
  }

}    // namespace WLib

#endif    // !WLIB_STATEMACHINE
