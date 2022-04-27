#include <WLib_CRC_64_go_iso.hpp>
#include <ut_catch.hpp>

TEST_CASE("tst_crc64_go_iso")
{
  char tst_str[] = {
    0x31,
    0x32,
    0x33,
    0x34,
    0x35,
    0x36,
    0x37,
    0x38,
    0x39,
  };

  uint64_t crc = WLib::CRC::CRC_64_go_iso()(reinterpret_cast<std::byte const*>(tst_str), std::size(tst_str));
  REQUIRE((crc) == 0xB90956C775A41001);

  WLib::CRC::CRC_64_go_iso crc_obj;
  crc_obj(reinterpret_cast<std::byte const*>(tst_str) + 0, 1);
  crc_obj(reinterpret_cast<std::byte const*>(tst_str) + 1, 2);
  crc_obj(reinterpret_cast<std::byte const*>(tst_str) + 3, 6);
  REQUIRE(crc_obj.get() == 0xB90956C775A41001);

  crc_obj.reset();
  REQUIRE(crc_obj.get() == 0x0000'0000'0000'0000);
}

TEST_CASE("tst_crc64_go_iso string")
{
  char tst_str[] = "This is a test string";

  uint64_t crc = WLib::CRC::CRC_64_go_iso()(reinterpret_cast<std::byte const*>(tst_str), std::size(tst_str) - 1);
  REQUIRE((crc) == 0x92737A60F502C7B5);
}
