#include <stdio.h>
#include <stdint.h>

void greyscale(uint8_t const volatile **const bytes_in, uint32_t const length, uint32_t const bitsperpixel_in,
               uint8_t volatile *const bytes_out)
{
  uint32_t const bytes = bitsperpixel_in / 8;
  uint32_t s = 0;
  for (uint32_t i = 0; i < length; i++)
  {
    s = bytes_in[0][i] + bytes_in[1][i] + bytes_in[2][i];
    bytes_out[i] = s / bytes;
  }
}
