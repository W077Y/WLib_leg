#include <WLib_Serializer.hpp>
#include <ut_catch.hpp>

static_assert(WLib::maximal_serialized_size_v<char> == 1, "char size");
static_assert(WLib::maximal_serialized_size_v<signed char> == 1, "char size");
static_assert(WLib::maximal_serialized_size_v<unsigned char> == 1, "char size");
static_assert(WLib::maximal_serialized_size_v<std::byte> == 1, "char size");

static_assert(WLib::maximal_serialized_size_v<short> == 2, "short size");
static_assert(WLib::maximal_serialized_size_v<int> == 4, "int size");
static_assert(WLib::maximal_serialized_size_v<long> == 4 ||
                  WLib::maximal_serialized_size_v<long> == 8,
              "long size");
static_assert(WLib::maximal_serialized_size_v<long long> == 8, "long long size");

static_assert(WLib::maximal_serialized_size_v<unsigned short> == 2, "short size");
static_assert(WLib::maximal_serialized_size_v<unsigned int> == 4, "int size");
static_assert(WLib::maximal_serialized_size_v<unsigned long> == 4 ||
                  WLib::maximal_serialized_size_v<unsigned long> == 8,
              "long size");
static_assert(WLib::maximal_serialized_size_v<unsigned long long> == 8, "long long size");

static_assert(WLib::maximal_serialized_size_v<float> == 4, "float size");
static_assert(WLib::maximal_serialized_size_v<double> == 8, "double size");
static_assert(WLib::maximal_serialized_size_v<long double> == 8 ||
                  WLib ::maximal_serialized_size_v<long double> == 16,
              "long double size");

static_assert(WLib::has_constant_serialized_size_v<char>, "char size");
static_assert(WLib::has_constant_serialized_size_v<signed char>, "char size");
static_assert(WLib::has_constant_serialized_size_v<unsigned char>, "char size");
static_assert(WLib::has_constant_serialized_size_v<std::byte>, "char size");

static_assert(WLib::has_constant_serialized_size_v<short>, "short size");
static_assert(WLib::has_constant_serialized_size_v<int>, "int size");
static_assert(WLib::has_constant_serialized_size_v<long>, "long size");
static_assert(WLib::has_constant_serialized_size_v<long long>, "long long size");

static_assert(WLib::has_constant_serialized_size_v<unsigned short>, "short size");
static_assert(WLib::has_constant_serialized_size_v<unsigned int>, "int size");
static_assert(WLib::has_constant_serialized_size_v<unsigned long>, "long size");
static_assert(WLib::has_constant_serialized_size_v<unsigned long long>, "long long size");

static_assert(WLib::has_constant_serialized_size_v<float>, "float size");
static_assert(WLib::has_constant_serialized_size_v<double>, "double size");
static_assert(WLib::has_constant_serialized_size_v<long double>, "long double size");

TEST_CASE()
{

  std::byte           tmp[100];
  WLib::byte_sink_ptr cur(tmp);

  WLib::serialize(cur, static_cast<char>(0xAA), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<signed char>(0xBB), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<unsigned char>(0xCC), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<std::byte>(0xDD), WLib::ByteOrder::little_endian);

  WLib::serialize(cur, static_cast<short>(0xEEFF), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<int>(0xAABB'CCDD), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<long>(0xEEFF'AABB), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<long long>(0xCCDD'EEFF'AABB'CCDD),
                  WLib::ByteOrder::little_endian);

  WLib::serialize(cur, static_cast<unsigned short>(0xEEFF), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<unsigned int>(0xAABB'CCDD), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<unsigned long>(0xEEFF'AABB), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<unsigned long long>(0xCCDD'EEFF'AABB'CCDD),
                  WLib::ByteOrder::little_endian);

  WLib::serialize(cur, static_cast<float>(3.14159f), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<double>(31.4159), WLib::ByteOrder::little_endian);
  WLib::serialize(cur, static_cast<long double>(314.159), WLib::ByteOrder::little_endian);

  if constexpr (WLib::maximal_serialized_size_v<long> == 4)
  {
    int const ref[] = {
      0xAA, 0xBB, 0xCC, 0xDD, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0xBB, 0xAA, 0xFF, 0xEE,
      0xDD, 0xCC, 0xBB, 0xAA, 0xFF, 0xEE, 0xDD, 0xCC, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA,
      0xBB, 0xAA, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0xFF, 0xEE, 0xDD, 0xCC,
    };

    for (std::size_t i = 0; i < std::size(ref); ++i)
    {
      REQUIRE(tmp[i] == static_cast<std::byte>(ref[i]));
    }
  }
  if constexpr (WLib::maximal_serialized_size_v<long> == 8)
  {
    int const ref[] = {
      0xAA, 0xBB, 0xCC, 0xDD, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0xBB, 0xAA,
      0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0xDD, 0xCC, 0xBB, 0xAA, 0xFF, 0xEE,
      0xDD, 0xCC, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0xBB, 0xAA, 0xFF, 0xEE,
      0x00, 0x00, 0x00, 0x00, 0xDD, 0xCC, 0xBB, 0xAA, 0xFF, 0xEE, 0xDD, 0xCC,
    };

    for (std::size_t i = 0; i < std::size(ref); ++i)
    {
      REQUIRE(tmp[i] == static_cast<std::byte>(ref[i]));
    }
  }

  WLib::byte_source_ptr pos(tmp);
  REQUIRE(WLib::deserialize<char>(pos, WLib::ByteOrder::little_endian) == static_cast<char>(0xAA));
  REQUIRE(WLib::deserialize<signed char>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<signed char>(0xBB));
  REQUIRE(WLib::deserialize<unsigned char>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<unsigned char>(0xCC));
  REQUIRE(WLib::deserialize<std::byte>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<std::byte>(0xDD));

  REQUIRE(WLib::deserialize<short>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<short>(0xEEFF));
  REQUIRE(WLib::deserialize<int>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<int>(0xAABB'CCDD));
  REQUIRE(WLib::deserialize<long>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<long>(0xEEFF'AABB));
  REQUIRE(WLib::deserialize<long long>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<long long>(0xCCDD'EEFF'AABB'CCDD));

  REQUIRE(WLib::deserialize<unsigned short>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<unsigned short>(0xEEFF));
  REQUIRE(WLib::deserialize<unsigned int>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<unsigned int>(0xAABB'CCDD));
  REQUIRE(WLib::deserialize<unsigned long>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<unsigned long>(0xEEFF'AABB));
  REQUIRE(WLib::deserialize<unsigned long long>(pos, WLib::ByteOrder::little_endian) ==
          static_cast<unsigned long long>(0xCCDD'EEFF'AABB'CCDD));

  REQUIRE(WLib::deserialize<float>(pos, WLib::ByteOrder::little_endian) == 3.14159f);
  REQUIRE(WLib::deserialize<double>(pos, WLib::ByteOrder::little_endian) == 31.4159);
  REQUIRE(WLib::deserialize<long double>(pos, WLib::ByteOrder::little_endian) == 314.159);
}

TEST_CASE()
{
  std::byte tmp[100];

  WLib::byte_sink_ptr cur(tmp);
  WLib::serialize(cur, static_cast<char>(0xAA), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<signed char>(0xBB), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<unsigned char>(0xCC), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<std::byte>(0xDD), WLib::ByteOrder::big_endian);

  WLib::serialize(cur, static_cast<short>(0xEEFF), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<int>(0xAABB'CCDD), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<long>(0xEEFF'AABB), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<long long>(0xCCDD'EEFF'AABB'CCDD), WLib::ByteOrder::big_endian);

  WLib::serialize(cur, static_cast<unsigned short>(0xEEFF), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<unsigned int>(0xAABB'CCDD), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<unsigned long>(0xEEFF'AABB), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<unsigned long long>(0xCCDD'EEFF'AABB'CCDD),
                  WLib::ByteOrder::big_endian);

  WLib::serialize(cur, static_cast<float>(3.14159f), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<double>(31.4159), WLib::ByteOrder::big_endian);
  WLib::serialize(cur, static_cast<long double>(314.159), WLib::ByteOrder::big_endian);

  if constexpr (WLib::maximal_serialized_size_v<long> == 4)
  {
    int const ref[] = {
      0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB,
      0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD,
      0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD,
    };

    for (std::size_t i = 0; i < std::size(ref); ++i)
    {
      REQUIRE(tmp[i] == static_cast<std::byte>(ref[i]));
    }
  }
  if constexpr (WLib::maximal_serialized_size_v<long> == 8)
  {
    int const ref[] = {
      0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0x00, 0x00,
      0x00, 0x00, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB,
      0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0x00, 0x00, 0x00, 0x00,
      0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD,
    };

    for (std::size_t i = 0; i < std::size(ref); ++i)
    {
      REQUIRE(tmp[i] == static_cast<std::byte>(ref[i]));
    }
  }

  WLib::byte_source_ptr pos(tmp);
  REQUIRE(WLib::deserialize<char>(pos, WLib::ByteOrder::big_endian) == static_cast<char>(0xAA));
  REQUIRE(WLib::deserialize<signed char>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<signed char>(0xBB));
  REQUIRE(WLib::deserialize<unsigned char>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<unsigned char>(0xCC));
  REQUIRE(WLib::deserialize<std::byte>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<std::byte>(0xDD));

  REQUIRE(WLib::deserialize<short>(pos, WLib::ByteOrder::big_endian) == static_cast<short>(0xEEFF));
  REQUIRE(WLib::deserialize<int>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<int>(0xAABB'CCDD));
  REQUIRE(WLib::deserialize<long>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<long>(0xEEFF'AABB));
  REQUIRE(WLib::deserialize<long long>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<long long>(0xCCDD'EEFF'AABB'CCDD));

  REQUIRE(WLib::deserialize<unsigned short>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<unsigned short>(0xEEFF));
  REQUIRE(WLib::deserialize<unsigned int>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<unsigned int>(0xAABB'CCDD));
  REQUIRE(WLib::deserialize<unsigned long>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<unsigned long>(0xEEFF'AABB));
  REQUIRE(WLib::deserialize<unsigned long long>(pos, WLib::ByteOrder::big_endian) ==
          static_cast<unsigned long long>(0xCCDD'EEFF'AABB'CCDD));

  REQUIRE(WLib::deserialize<float>(pos, WLib::ByteOrder::big_endian) == 3.14159f);
  REQUIRE(WLib::deserialize<double>(pos, WLib::ByteOrder::big_endian) == 31.4159);
  REQUIRE(WLib::deserialize<long double>(pos, WLib::ByteOrder::big_endian) == 314.159);
}

namespace
{
  class tst_struct
  {
  public:
    void set_a(char val) { this->a = val; };
    void set_b(int val) { this->b = val; };
    void set_c(char val) { this->c = val; };

    char get_a() const { return this->a; };
    int  get_b() const { return this->b; };
    char get_c() const { return this->c; };

  private:
    friend WLib::serializer_traits<tst_struct>;
    char a = 0;
    int  b = 0;
    char c = 0;
  };
}    // namespace

namespace WLib
{
  template <>
  struct serializer_traits<tst_struct>
      : public Internal::template_serializeable_type<tst_struct,
                                                     minimal_serialized_size_v<char, int, char>,
                                                     maximal_serialized_size_v<char, int, char>>
  {
    template <typename iter_t>
    static constexpr void
    ser(iter_t& iter, type_t const& value, ByteOrder const& byte_order = ByteOrder::native)
    {
      serialize(iter, value.a, byte_order);
      serialize(iter, value.b, byte_order);
      serialize(iter, value.c, byte_order);
    }

    template <typename iter_t>
    static constexpr type_t deser(iter_t& iter, ByteOrder const& byte_order = ByteOrder::native)
    {
      type_t ret;
      ret.a = deserialize<char>(iter, byte_order);
      ret.b = deserialize<int>(iter, byte_order);
      ret.c = deserialize<char>(iter, byte_order);
      return ret;
    }
  };
}    // namespace WLib

TEST_CASE()
{
  std::byte  tmp[100];
  tst_struct in;
  in.set_a(static_cast<char>(0xAA));
  in.set_b(0xBB);
  in.set_c(static_cast<char>(0xCC));

  WLib::byte_sink_ptr cur(tmp);
  WLib::serialize(cur, in);

  WLib::byte_source_ptr pos(tmp);
  auto&&                out = WLib::deserialize<tst_struct>(pos);

  REQUIRE(in.get_a() == out.get_a());
  REQUIRE(in.get_b() == out.get_b());
  REQUIRE(in.get_c() == out.get_c());
}

TEST_CASE()
{
  tst_struct in;
  in.set_a(static_cast<char>(0xAA));
  in.set_b(0xBB);
  in.set_c(static_cast<char>(0xCC));

  std::byte                  tmp[100];
  WLib::byte_sink_ext_buffer cur(tmp);
  WLib::serialize(cur, in);

  REQUIRE(cur.get_begin() == tmp);
  REQUIRE(cur.get_pos() == tmp + 6);
  REQUIRE(cur.get_end() == tmp + 100);

  REQUIRE(cur.get_index() == 6);
  cur.skip_bytes(3);
  REQUIRE(cur.get_index() == 9);
  cur.set_index(12);
  REQUIRE(cur.get_index() == 12);

  WLib::serialize(cur, static_cast<uint64_t>(0x0123'4567'89AB'CDEF));

  WLib::byte_source_ext_buffer pos(tmp);
  auto&&                       out = WLib::deserialize<tst_struct>(pos);

  REQUIRE(pos.get_begin() == tmp);
  REQUIRE(pos.get_pos() == tmp + 6);
  REQUIRE(pos.get_end() == tmp + 100);

  REQUIRE(pos.get_index() == 6);
  pos.skip_bytes(3);
  REQUIRE(pos.get_index() == 9);
  pos.set_index(12);
  REQUIRE(pos.get_index() == 12);

  REQUIRE(WLib::deserialize<uint64_t>(pos) == static_cast<uint64_t>(0x0123'4567'89AB'CDEF));

  REQUIRE(in.get_a() == out.get_a());
  REQUIRE(in.get_b() == out.get_b());
  REQUIRE(in.get_c() == out.get_c());
}
