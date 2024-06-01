#include <stdio.h>
#include <stdint.h>

void greyscale(uint8_t const volatile *const line_in, uint32_t const length, uint32_t const y, uint32_t const bitsperpixel_in,
               uint8_t volatile *const line_out)
{
  uint32_t const bytes = bitsperpixel_in / 8;
  for (uint32_t i = 0; i < length; i++)
  {
    uint32_t s = 0;
    for (uint32_t b = 0; b < bytes; b++)
    {
      s += line_in[i * bytes + b];
    }
    line_out[i] = s / bytes;
  }
}
