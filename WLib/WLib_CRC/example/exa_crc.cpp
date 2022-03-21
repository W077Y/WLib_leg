#include <WLib_CRC_32.hpp>

int main()
{
  std::byte buf[10] = {/*...*/};

  WLib::CRC::CRC_32 crc;

  [[maybe_unused]] uint32_t crc_value = crc(buf, buf + 10);
  return 0;
}
