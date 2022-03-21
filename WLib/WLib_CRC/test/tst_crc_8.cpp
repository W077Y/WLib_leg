#include <WLib_CRC_8.hpp>
#include <ut_catch.hpp>

TEST_CASE()
{
  char tst_str[] = {
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  };

  uint8_t crc = WLib::CRC::CRC_8()(reinterpret_cast<std::byte const*>(tst_str), std::size(tst_str));
  REQUIRE((crc) == 0xF4);

  WLib::CRC::CRC_8 crc_obj;
  crc_obj(reinterpret_cast<std::byte const*>(tst_str) + 0, 1);
  crc_obj(reinterpret_cast<std::byte const*>(tst_str) + 1, 2);
  crc_obj(reinterpret_cast<std::byte const*>(tst_str) + 3, 6);
  REQUIRE(crc_obj.get() == 0xF4);

  

  crc_obj.reset();
  REQUIRE(crc_obj.get() == 0x00);
}

TEST_CASE()
{
  char tst_str[] = "This is a test string";
  
  uint8_t crc =
      WLib::CRC::CRC_8()(reinterpret_cast<std::byte const*>(tst_str), std::size(tst_str) - 1);
  REQUIRE((crc) == 0xDF);
}
