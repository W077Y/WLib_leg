#include <WLib_Serializer.hpp>
#include <iostream>
#include <string>

int main()
{
  std::byte byte_buffer[100] = {};

  auto gen_value = [](unsigned int i) -> short
  {
    return (static_cast<unsigned short>(i & 0xFFFF) - 10) * 3;
  };

  {
    WLib::byte_sink_ext_buffer in(byte_buffer);
    for (unsigned int i = 0; i < 10; i++)
      WLib::serialize(in, gen_value(i));
  }

  {
    WLib::byte_source_ptr out(byte_buffer);
    for (unsigned int i = 0; i < 10; i++)
      if (WLib::deserialize<short>(out) != gen_value(i))
        return i;
  }

  return 0;
}