#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void greyscale(uint8_t const volatile *const frame_in, uint32_t const bytes_per_pixel, uint32_t const pixels_in_block, uint8_t volatile *const frame_out)
{
  // frame_in & frame_out can be the same
  // it's therefore crucial to have x & y incrementing
  for (uint32_t x = 0; x < pixels_in_block; x++)
  {
    uint32_t s = 0;
    for (uint32_t b = 0; b < bytes_per_pixel; b++)
    {
      s += frame_in[x * bytes_per_pixel + b];
    }
    frame_out[x] = s / bytes_per_pixel;
  }
}
